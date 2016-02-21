#ifndef xAODAnaHelpers_IParticleHistsAlgo_H
#define xAODAnaHelpers_IParticleHistsAlgo_H

#include <SampleHandler/MetaObject.h>
#include <SampleHandler/MetaFields.h>

// algorithm wrapper
#include <xAODAnaHelpers/Algorithm.h>
#include <xAODAnaHelpers/IParticleHists.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>


class IParticleHistsAlgo : public xAH::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // configuration variables
  std::string m_inContainerName;
  std::string m_detailStr;
  std::string m_inputAlgo;
  /** Histogram name prefix when using IParticleHistsAlgo directly */
  std::string m_histPrefix;
  /** Histogram xaxis title when using IParticleHistsAlgo directly */
  std::string m_histTitle;

private:
  std::map< std::string, IParticleHists* > m_plots; //!

  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  // this is a standard constructor
  IParticleHistsAlgo (std::string className = "IParticleHistsAlgo");

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  /**
      @brief Calls execute<IParticleContainer>
   */
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  /**
      @brief Fill histograms with particles in a container
      @rst
          Tempalated (container type) function that loops over all systematics (or nominal only)
	  and fills the corresponding histogram objects.

	  The event weight, in case of Monte Carlo samples, is
	   mcEventWeight*crossSection*filterEfficiency*kfactor
	  where the sample-weights are taken from SampleHandler and set to 1 by default.
      @endrst
  */
  template<class HIST_T, class CONT_T> EL::StatusCode execute ()
  {
    const xAOD::EventInfo* eventInfo(nullptr);
    RETURN_CHECK("IParticleHistsAlgo::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

    float eventWeight(1);
    if( eventInfo->isAvailable< float >( "mcEventWeight" ) ) {
      eventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
    }
    // if(isMC())
    //   {
    // 	double xs     =wk()->metaData()->castDouble(SH::MetaFields::crossSection    ,1);
    // 	double eff    =wk()->metaData()->castDouble(SH::MetaFields::filterEfficiency,1);
    // 	double kfac   =wk()->metaData()->castDouble(SH::MetaFields::kfactor         ,1);
    // 	eventWeight *= xs * eff * kfac;
    //   }

    // this will hold the collection processed
    const CONT_T* inParticles = 0;

    // if input comes from xAOD, or just running one collection,
    // then get the one collection and be done with it
    if( m_inputAlgo.empty() ) {
      RETURN_CHECK("IParticleHistsAlgo::execute()", HelperFunctions::retrieve(inParticles, m_inContainerName, m_event, m_store, m_verbose) ,("Failed to get "+m_inContainerName).c_str());

      // pass the photon collection
      RETURN_CHECK("IParticleHistsAlgo::execute()", static_cast<HIST_T*>(m_plots[""])->execute( inParticles, eventWeight ), "");
    }
    else { // get the list of systematics to run over

      // get vector of string giving the names
      std::vector<std::string>* systNames(nullptr);
      RETURN_CHECK("IParticleHistsAlgo::execute()", HelperFunctions::retrieve(systNames, m_inputAlgo, 0, m_store, m_verbose) ,"");

      // loop over systematics
      for( auto systName : *systNames ) {
	RETURN_CHECK("IParticleHistsAlgo::execute()", HelperFunctions::retrieve(inParticles, m_inContainerName+systName, m_event, m_store, m_verbose) ,"");
	if( m_plots.find( systName ) == m_plots.end() ) { this->AddHists( systName ); }
	RETURN_CHECK("IParticleHistsAlgo::execute()", static_cast<HIST_T*>(m_plots[systName])->execute( inParticles, eventWeight ), "");
      }
    }

    return EL::StatusCode::SUCCESS;
  }

  // these are the functions not inherited from Algorithm
  /**
      @brief Calls AddHists<IParticleHists>
      @param name Name of the systematic
   */
  virtual EL::StatusCode AddHists( std::string name);

  /**
      @brief Create histograms
      @param name Name of the systematic

      @rst
          Tempalated (histogram colllection class) function that creates all necessary histogram
	  objects for a given systematic. The class chosen for HIST_T template must inherit from
	  IParticleHists.
      @endrst
  */
  template<class HIST_T> EL::StatusCode AddHists( std::string name ) {
    std::string fullname(m_name);
    fullname += name; // add systematic
    HIST_T* particleHists = new HIST_T( fullname, m_detailStr ); // add systematic
    particleHists->m_debug = m_debug;
    RETURN_CHECK((m_name+"::AddHists").c_str(), particleHists->initialize(), "");
    particleHists->record( wk() );
    m_plots[name] = particleHists;

    return EL::StatusCode::SUCCESS;
  }

  /// @cond
  // this is needed to distribute the algorithm to the workers
  ClassDef(IParticleHistsAlgo, 1);
  /// @endcond

};

#endif
