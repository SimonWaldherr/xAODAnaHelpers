#ifndef xAODAnaHelpers_TrackHists_H
#define xAODAnaHelpers_TrackHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include <xAODTracking/TrackParticleContainer.h>
#include <xAODTracking/Vertex.h>

class TrackHists : public HistogramManager
{
  public:
    TrackHists(std::string name, std::string detailStr );
    ~TrackHists();

    StatusCode initialize();
    StatusCode execute( const xAOD::TrackParticleContainer* tracks,  const xAOD::Vertex *pvx, float eventWeight );
    StatusCode execute( const xAOD::TrackParticle* track,            const xAOD::Vertex *pvx, float eventWeight );
    using HistogramManager::book; // make other overloaded versions of book() to show up in subclass
    using HistogramManager::execute; // overload

  protected:
    // bools to control which histograms are filled
    bool m_fillIPDetails; //!
    bool m_fillHitCounts; //!
    bool m_fillChi2Details; //!
    bool m_fillTPErrors; //!
    bool m_fillDebugging; //!

  private:
    // Histograms
    TH1F* m_trk_n; //!
    TH1F* m_trk_Pt; //!
    TH1F* m_trk_Pt_l; //!
    TH1F* m_trk_Eta; //!
    TH1F* m_trk_Phi; //!
    TH1F* m_trk_d0; //!
    TH1F* m_trk_z0; //!
    TH1F* m_trk_chi2Prob; //!
    TH1F* m_trk_charge; //!
    TH1F* m_trk_d0_l; //!
    TH1F* m_trk_d0Err; //!
    TH1F* m_trk_d0Sig; //!
    TH1F* m_trk_z0Err; //!
    TH1F* m_trk_z0_l; //!
    TH1F* m_trk_z0Sig; //!
    TH1F* m_trk_z0sinT; //!
    TH1F* m_trk_z0sinT_l; //!
    TH1F* m_trk_z0SigsinT; //!
    TH1F* m_trk_chi2Prob_l; //!
    TH1F* m_trk_chi2Prob_s; //!
    TH1F* m_trk_chi2Prob_ss; //!
    TH1F* m_trk_chi2ndof; //!
    TH1F* m_trk_chi2ndof_l; //!
    TH1F* m_trk_nSi; //!
    TH1F* m_trk_nSiAndDead; //!
    TH1F* m_trk_nSiDead; //!
    TH1F* m_trk_nSCT; //!
    TH1F* m_trk_nPix; //!
    TH1F* m_trk_nPixHoles; //!
    TH1F* m_trk_nPixDead; //!
    TH1F* m_trk_nTRT; //!
    // TH1F* m_trk_nTRTHoles; //!
    // TH1F* m_trk_nTRTDead; //!
    TH1F* m_trk_nBL; //!
    TH1F* m_trk_phiErr; //!
    TH1F* m_trk_thetaErr; //!
    TH1F* m_trk_qOpErr; //!
    TH1F* m_trk_mc_prob; //!
    TH1F* m_trk_mc_barcode; //!
    TH1F* m_trk_mc_barcode_s; //!
    TH1F* m_trk_eta_vl; //!
    TH1F* m_trk_z0_vl; //!
    TH1F* m_trk_z0_m; //!
    TH1F* m_trk_z0_m_raw; //!
    TH1F* m_trk_d0_vl; //!
    TH1F* m_trk_pt_ss; //!
    TH1F* m_trk_phiManyBins; //!

};


#endif
