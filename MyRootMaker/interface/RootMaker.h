// second line of test comments here
#ifndef RootMaker_h
#define RootMaker_h
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <algorithm>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"

//#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
//#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
//#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
//#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
//#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "CondFormats/L1TObjects/interface/L1GtPrescaleFactors.h"
#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsTechTrigRcd.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "DataFormats/TrackReco/interface/DeDxData.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METFwd.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetCollection.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "RecoEgamma/EgammaTools/interface/ConversionInfo.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <Math/Functions.h>
#include <Math/SVector.h>
#include <Math/SMatrix.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/GeometrySurface/interface/SimpleCylinderBounds.h"
#include "DataFormats/GeometrySurface/interface/SimpleDiskBounds.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "DataFormats/GeometrySurface/interface/BoundCylinder.h"
#include "DataFormats/GeometrySurface/interface/BoundDisk.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/MaterialEffects/interface/PropagatorWithMaterial.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"

#include "DataFormats/ParticleFlowCandidate/interface/IsolatedPFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/IsolatedPFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateEGammaExtra.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateEGammaExtraFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateElectronExtra.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateElectronExtraFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidatePhotonExtra.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidatePhotonExtraFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PileUpPFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PileUpPFCandidateFwd.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"


#include "EgammaAnalysis/ElectronTools/src/PFIsolationEstimator.cc"
#include "EgammaAnalysis/ElectronTools/src/SuperClusterHelper.cc"
#include "PFIsolation/SuperClusterFootprintRemoval/interface/SuperClusterFootprintRemoval.h"

#include "TGeoPara.h"

using namespace std;
using namespace reco;
using namespace pat;

#define M_trackmaxcount 500
#define M_superclustermaxcount 1000
#define M_superclustermembermaxcount 1000
#define M_superclusterhitmaxcount 5000
#define M_primvertexmaxcount 500
#define M_muonmaxcount 500
#define M_taumaxcount 500
#define M_electronmaxcount 500
#define M_photonmaxcount 500
#define M_conversionmaxcount 500
#define M_jetmaxcount 500
#define M_musecverticesmaxcount 500
#define M_secverticesmaxcount 1000
#define M_genallparticlesmaxcount 10000
#define M_genparticlesmaxcount 500
#define M_genjetmaxcount 500
#define M_genmotherdaughtermaxcount 100000
#define M_btagmax 3


class RootMaker : public edm::EDAnalyzer {
  public:
    explicit RootMaker(const edm::ParameterSet &iConfig);
    ~RootMaker();
  private:
    virtual void beginJob();
    virtual void endJob();

    const PFCandidate &removeRef(const PFCandidatePtr &pfRef);
    template<typename Collection, typename Function>
    std::vector<double> extract(const Collection &cands, Function func);

    virtual void beginRun(const edm::Run &iRun, const edm::EventSetup &iSetup);
    void TriggerIndexSelection(vector<string> configstring, vector<pair<unsigned, int> > &triggers, string &allnames);
    virtual void beginLuminosityBlock(const edm::LuminosityBlock &iLumiBlock, const edm::EventSetup &iSetup);
    virtual void endLuminosityBlock(const edm::LuminosityBlock &iLumiBlock, const edm::EventSetup &iSetup);
    virtual void analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup);

    // tokens
    edm::EDGetTokenT<L1GlobalTriggerReadoutRecord> l1TriggerToken_;

    edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
    edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjects_;
    edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;

    edm::EDGetTokenT<edm::ValueMap<DeDxData>> dharmonicToken_;
    edm::EDGetTokenT<reco::VertexCollection> verticesToken_;

    edm::EDGetTokenT<reco::ConversionCollection> conversionsToken_;
    edm::EDGetTokenT<pat::JetCollection> ak4pfchsJetsToken_;
    edm::EDGetTokenT<pat::JetCollection> ak4pfchsJetsPuppiToken_;
    edm::EDGetTokenT<reco::GenParticleCollection> genSimParticlesToken_;
    edm::EDGetTokenT<reco::GenParticleCollection> genParticlesToken_;
    edm::EDGetTokenT<reco::GenJetCollection> genJetsToken_;
    edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;
    edm::EDGetTokenT<reco::SuperClusterCollection> superClustersToken_;
    edm::EDGetTokenT<GenEventInfoProduct> genInfoToken_;
    edm::EDGetTokenT<std::vector<PileupSummaryInfo>> puInfoToken_;
    edm::EDGetTokenT<reco::CaloClusterCollection> ebeeClustersToken_;
    edm::EDGetTokenT<reco::CaloClusterCollection> esClustersToken_;
    edm::EDGetTokenT<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit>>> ebRecHitsToken_;
    edm::EDGetTokenT<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit>>> eeRecHitsToken_;
    edm::EDGetTokenT<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit>>> esRecHitsToken_;
    // AOD tokens
    edm::EDGetTokenT<reco::TrackCollection> recoTracksToken_;
    edm::EDGetTokenT<reco::MuonCollection> recoMuonsToken_;
    edm::EDGetTokenT<reco::GsfElectronCollection> recoElectronsToken_;
    edm::EDGetTokenT<reco::PhotonCollection> recoPhotonsToken_;
    edm::EDGetTokenT<reco::PFTauCollection> recoTausToken_;
    edm::EDGetTokenT<pat::JetCollection> tauJetsToken_;
    edm::EDGetTokenT<pat::JetCollection> ak4caloJetsToken_;
    edm::EDGetTokenT<pat::JetCollection> ak4jptJetsToken_;
    edm::EDGetTokenT<reco::PFJetCollection> ak4pfJetsToken_;
    edm::EDGetTokenT<reco::PFCandidateCollection> recoPFCandsToken_;
    edm::EDGetTokenT<reco::PFMETCollection> recoMetToken_;
    edm::EDGetTokenT<reco::PFMETCollection> recoMetT1Token_;
    edm::EDGetTokenT<reco::PFMETCollection> recoMetT1T0Token_;
    // miniAOD tokens
    edm::EDGetTokenT<pat::PackedCandidateCollection> lostTracksToken_;
    edm::EDGetTokenT<reco::TrackCollection> unpackedTracksToken_;
    edm::EDGetTokenT<pat::MuonCollection> patMuonsToken_;
    edm::EDGetTokenT<pat::ElectronCollection> patElectronsToken_;
    edm::EDGetTokenT<pat::PhotonCollection> patPhotonsToken_;
    edm::EDGetTokenT<pat::TauCollection> patTausToken_;
    edm::EDGetTokenT<pat::PackedCandidateCollection> packedPFCandsToken_;
    //edm::EDGetTokenT<pat::METCollection> patMVAMetToken_;
    edm::InputTag patMVAMetEMTToken_;
    edm::InputTag patMVAMetEMToken_;
    edm::InputTag patMVAMetETToken_;
    edm::InputTag patMVAMetMTToken_;
    edm::InputTag patMVAMetTTToken_;
    edm::InputTag newMetLabel_;
    edm::EDGetTokenT<pat::METCollection> patMetToken_;
    edm::EDGetTokenT<pat::METCollection> patMetPuppiToken_;
    edm::EDGetTokenT<double> rhoToken_;
    edm::EDGetTokenT<vector<reco::GsfElectronCore>> gedGsfElectronCoresToken_;
    // ID decisions objects
    edm::EDGetTokenT<edm::ValueMap<bool> > eleVetoIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > eleLooseIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > eleMediumIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > eleTightIdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > eleHeepV60IdMapToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> > eleMVAIdMap_wp80Token_;
    edm::EDGetTokenT<edm::ValueMap<bool> > eleMVAIdMap_wp90Token_;

    edm::EDGetTokenT<vector<reco::PhotonCore>> gedPhotonCoresToken_;

    edm::Handle<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > ebRecHits;
    edm::Handle<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > eeRecHits;
    edm::Handle<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > esRecHits;
    edm::Handle<VertexCollection> Vertices;
    edm::Handle<edm::ValueMap<DeDxData> > dEdxharmonic2;
    edm::Handle<L1GlobalTriggerReadoutRecord> L1trigger;

    bool AddTracks(const edm::Event &iEvent);
    bool AddElectrons(const edm::Event &iEvent);
    bool AddPatElectrons(const edm::Event &iEvent);
    bool AddMuons(const edm::Event &iEvent);
    bool AddPatMuons(const edm::Event &iEvent);
    bool AddPhotons(const edm::Event &iEvent, const edm::EventSetup &iSetup);
    bool AddPatPhotons(const edm::Event &iEvent, const edm::EventSetup &iSetup);
    bool AddAllConversions(const edm::Event &iEvent);
    bool AddTaus(const edm::Event &iEvent);
    bool AddPatTaus(const edm::Event &iEvent);
    bool AddAK4CaloJets(const edm::Event &iEvent, const edm::EventSetup &iSetup);
    bool AddAK4JPTJets(const edm::Event &iEvent, const edm::EventSetup &iSetup);
    bool AddAK4PFJets(const edm::Event &iEvent, const edm::EventSetup &iSetup);
    bool AddAK4PFCHSJets(const edm::Event &iEvent, const edm::EventSetup &iSetup);
    bool AddAK4PFCHSPuppiJets(const edm::Event &iEvent, const edm::EventSetup &iSetup);
    bool AddVertices(const edm::Event &iEvent);
    bool AddMuVertices(const edm::Event &iEvent);
    bool AddConvPhotons(const edm::Event &iEvent);
    bool AddCaloPhotons(const edm::Event &iEvent);
    bool foundCompatibleInnerHits(const reco::HitPattern &hitPatA, const reco::HitPattern &hitPatB);

    struct JetShape {
        float chargeda;
        float chargedb;
        float neutrala;
        float neutralb;
        float alla;
        float allb;
        float chargedfractionmv;
    };
    JetShape getJetShape(const PFJet &jet);
    JetShape getJetShape(const pat::Jet &jet);
    JetShape getSlimmedJetShape(const pat::Jet &jet);
    UInt_t GenParticleInfo(const GenParticle *particle);
    UInt_t GetTrigger(const LeafCandidate &particle, vector<pair<unsigned, int> > &triggers);
    UInt_t FindGenParticle(const Candidate *particle);
    Int_t HasAnyMother(const GenParticle *particle, int id);
    pair<Int_t, Int_t> HasAnyMother(const GenParticle *particle, vector<int> ids);
    math::XYZPoint PositionOnECalSurface(reco::TransientTrack &);
    Int_t getSuperClusterPh(const SuperClusterRef &A);
    Int_t getSuperClusterEl(const SuperClusterRef &A);
    Int_t getPrimVertex(const pat::PackedCandidate *con);
    Int_t getPrimVertex(const Track &trk);
    //Int_t getSuperCluster(const Candidate& A);

    TTree *tree;
    TTree *lumitree;
    TTree *runtree;
    TH1D *drhist;

    //Configuration
    bool cisMiniAOD;
    bool cisMC;
    bool cdebug;
    bool cgen;
    bool cgenallparticles;
    bool cgenak4jets;
    bool ctrigger;
    bool cbeamspot;
    bool crectrack;
    bool crecprimvertex;
    bool crecsupercluster;
    double crecsuperclusterFilterPtMin;
    double crecsuperclusterFilterEtaMax;
    bool crecsuperclustermember;
    bool crecsuperclusterhit;
    bool crecmuon;
    bool crectau;
    bool crecelectron;
    bool crecphoton;
    bool crecallconversion;
    bool crecak4calojet;
    bool crecak4jptjet;
    bool crecak4pfjet;
    bool crecak4pfchsjet;
    bool crecak4pfchspuppijet;
    bool crecjettrigger;
    bool crecpfmet;
    bool crecsecvertices;
    bool crecmusecvertices;
    vector<string> cHLTriggerNamesSelection;
    string cTriggerProcess;

    double cMuPtMin;
    double cMuTrackIso;
    double cMuEtaMax;
    vector<string> cMuHLTriggerMatching;
    int cMuNum;
    double cElPtMin;
    double cElTrackIso;
    double cElEtaMax;
    vector<string> cElHLTriggerMatching;
    int cElNum;
    double cElFilterPtMin;
    double cElFilterEtaMax;
    double cTauPtMin;
    double cTauEtaMax;
    vector<string> cTauHLTriggerMatching;
    vector<string> cTauDiscriminators;
    int cTauNum;
    double cTrackFilterPtMin;
    double cTrackPtMin;
    double cTrackEtaMax;
    int cTrackNum;
    double cPhotonPtMin;
    double cPhotonEtaMax;
    vector<string> cPhotonHLTriggerMatching;
    int cPhotonNum;
    double cPhotonFilterPtMin;
    double cPhotonFilterEtaMax;
    double cAK4CaloFilterPtMin;
    double cAK4CaloPtMin;
    double cAK4CaloEtaMax;
    int cAK4CaloNum;
    double cAK4JPTFilterPtMin;
    double cAK4JPTPtMin;
    double cAK4JPTEtaMax;
    int cAK4JPTNum;
    double cAK4PFCHSFilterPtMin;
    double cAK4PFCHSPtMin;
    double cAK4PFCHSEtaMax;
    int cAK4PFCHSNum;
    double cAK4PFCHSPuppiFilterPtMin;
    double cAK4PFCHSPuppiPtMin;
    double cAK4PFCHSPuppiEtaMax;
    int cAK4PFCHSPuppiNum;
    double cAK4PFFilterPtMin;
    double cAK4PFPtMin;
    double cAK4PFEtaMax;
    int cAK4PFNum;
    string cJetCorrection;
    vector<string> cJetHLTriggerMatching;

    double cMassMuMuMin;
    double cMassMuMuMax;
    vector<TLorentzVector> MuVector;
    vector<TLorentzVector> TrackVector;

    double cVertexTRKChi2;
    int cVertexTRKHitsMin;
    double cVertexChi2;
    double cVertexSig2D;
    double cKaonMassWindow;
    double cLambdaMassWindow;

    //Variables
    edm::ESHandle<TransientTrackBuilder> TTrackBuilder;
    edm::ESHandle<MagneticField> magneticField;
    Cylinder::ConstCylinderPointer ecalBarrel;
    Plane::ConstPlanePointer ecalNegativeEtaEndcap;
    Plane::ConstPlanePointer ecalPositiveEtaEndcap;
    PropagatorWithMaterial *propagatorWithMaterial;

    HLTConfigProvider HLTConfiguration;
    edm::Handle<edm::TriggerResults> HLTrigger;
    edm::Handle<trigger::TriggerEvent> HLTriggerEvent;
    //edm::Handle<l1extra::L1MuonParticleCollection> L1Muons;
    //edm::Handle<l1extra::L1EmParticleCollection> L1Electrons;
    //edm::Handle<l1extra::L1EmParticleCollection> L1ElectronsIso;
    vector<pair<unsigned, int> > muontriggers;
    vector<pair<unsigned, int> > electrontriggers;
    vector<pair<unsigned, int> > tautriggers;
    vector<pair<unsigned, int> > photontriggers;
    vector<pair<unsigned, int> > jettriggers;

    vector<string> bdisclabel;

    vector<int> testids;
    vector<GenParticle> GenPartons;

    double DR(const Candidate &A, const Candidate &B);

    vector<unsigned> HLTriggerIndexSelection;
    math::XYZPoint pv_position;
    Vertex primvertex;
    math::XYZPoint bs_position;
    //Data
    UInt_t errors;
    Double_t event_nr;
    UInt_t event_luminosityblock;
    UInt_t event_run;
    UInt_t event_timeunix;
    UInt_t event_timemicrosec;
    UChar_t trigger_level1bits[8];
    UChar_t trigger_level1[128];
    UChar_t trigger_HLT[128];

    Float_t beamspot_x;
    Float_t beamspot_y;
    Float_t beamspot_z;
    Float_t beamspot_xwidth;
    Float_t beamspot_ywidth;
    Float_t beamspot_zsigma;
    Float_t beamspot_cov[6];

    UInt_t track_count;
    Int_t track_vtx[M_trackmaxcount];
    Float_t track_px[M_trackmaxcount];
    Float_t track_py[M_trackmaxcount];
    Float_t track_pz[M_trackmaxcount];
    Float_t track_outerx[M_trackmaxcount];
    Float_t track_outery[M_trackmaxcount];
    Float_t track_outerz[M_trackmaxcount];
    Float_t track_closestpointx[M_trackmaxcount];
    Float_t track_closestpointy[M_trackmaxcount];
    Float_t track_closestpointz[M_trackmaxcount];
    Float_t track_chi2[M_trackmaxcount];
    Float_t track_ndof[M_trackmaxcount];
    Float_t track_dxy[M_trackmaxcount];
    Float_t track_dxyerr[M_trackmaxcount];
    Float_t track_dz[M_trackmaxcount];
    Float_t track_dzerr[M_trackmaxcount];
    Float_t track_dedxharmonic2[M_trackmaxcount];
    Int_t track_charge[M_trackmaxcount];
    UChar_t track_nhits[M_trackmaxcount];
    UChar_t track_nmissinghits[M_trackmaxcount];
    UChar_t track_npixelhits[M_trackmaxcount];
    UChar_t track_npixellayers[M_trackmaxcount];
    UChar_t track_nstriplayers[M_trackmaxcount];

    UInt_t primvertex_count;
    Float_t primvertex_x[M_primvertexmaxcount];
    Float_t primvertex_y[M_primvertexmaxcount];
    Float_t primvertex_z[M_primvertexmaxcount];
    UInt_t primvertex_info[M_primvertexmaxcount];
    Float_t primvertex_chi2[M_primvertexmaxcount];
    Float_t primvertex_ndof[M_primvertexmaxcount];
    Float_t primvertex_ptq[M_primvertexmaxcount];
    Int_t primvertex_ntracks[M_primvertexmaxcount];
    Float_t primvertex_cov[M_primvertexmaxcount][6];

    UInt_t supercluster_count;
    Float_t supercluster_e[M_superclustermaxcount];
    Float_t supercluster_x[M_superclustermaxcount];
    Float_t supercluster_y[M_superclustermaxcount];
    Float_t supercluster_z[M_superclustermaxcount];
    Float_t supercluster_rawe[M_superclustermaxcount];
    Float_t supercluster_phiwidth[M_superclustermaxcount];
    Float_t supercluster_etawidth[M_superclustermaxcount];
    Int_t supercluster_nbasiccluster[M_superclustermaxcount];
    Int_t supercluster_basicclusterbegin[M_superclustermaxcount];
    Int_t supercluster_esclusterbegin[M_superclustermaxcount];

    UInt_t supercluster_basiccluster_count;
    Float_t supercluster_basiccluster_e[M_superclustermembermaxcount];
    Float_t supercluster_basiccluster_x[M_superclustermembermaxcount];
    Float_t supercluster_basiccluster_y[M_superclustermembermaxcount];
    Float_t supercluster_basiccluster_z[M_superclustermembermaxcount];
    Int_t supercluster_basiccluster_nhit[M_superclustermembermaxcount];
    Int_t supercluster_basiccluster_hitbegin[M_superclustermembermaxcount];

    UInt_t supercluster_basiccluster_hit_count;
    Float_t supercluster_basiccluster_hit_e[M_superclusterhitmaxcount];
    Float_t supercluster_basiccluster_hit_x[M_superclusterhitmaxcount];
    Float_t supercluster_basiccluster_hit_y[M_superclusterhitmaxcount];
    Float_t supercluster_basiccluster_hit_z[M_superclusterhitmaxcount];

    UInt_t supercluster_escluster_count;
    Float_t supercluster_escluster_e[M_superclustermembermaxcount];
    Float_t supercluster_escluster_x[M_superclustermembermaxcount];
    Float_t supercluster_escluster_y[M_superclustermembermaxcount];
    Float_t supercluster_escluster_z[M_superclustermembermaxcount];
    Int_t supercluster_escluster_nhit[M_superclustermembermaxcount];
    Int_t supercluster_escluster_hitbegin[M_superclustermembermaxcount];

    UInt_t supercluster_escluster_hit_count;
    Float_t supercluster_escluster_hit_e[M_superclusterhitmaxcount];
    Float_t supercluster_escluster_hit_x[M_superclusterhitmaxcount];
    Float_t supercluster_escluster_hit_y[M_superclusterhitmaxcount];
    Float_t supercluster_escluster_hit_z[M_superclusterhitmaxcount];

    UInt_t muon_count;
    Int_t muon_muID[M_muonmaxcount];
    Float_t muon_px[M_muonmaxcount];
    Float_t muon_py[M_muonmaxcount];
    Float_t muon_pz[M_muonmaxcount];
    Float_t muon_pt[M_muonmaxcount];
    Float_t muon_phi[M_muonmaxcount];
    Float_t muon_eta[M_muonmaxcount];
    Float_t muon_pterror[M_muonmaxcount];
    Float_t muon_chi2[M_muonmaxcount];
    Float_t muon_ndof[M_muonmaxcount];
    Float_t muon_dB[M_muonmaxcount];

    Int_t muon_is_tracker[M_muonmaxcount];
    Int_t muon_is_global[M_muonmaxcount];
    Int_t muon_is_standalone[M_muonmaxcount];

    Int_t muon_has_gen_particle[M_muonmaxcount];
    Int_t muon_gen_particle_pdgid[M_muonmaxcount];
    Int_t muon_has_gen_mother[M_muonmaxcount];
    Int_t muon_gen_mother_pdgid[M_muonmaxcount];

    Int_t muon_innertrack_vtx[M_muonmaxcount];
    Float_t muon_innertrack_px[M_muonmaxcount];
    Float_t muon_innertrack_py[M_muonmaxcount];
    Float_t muon_innertrack_pz[M_muonmaxcount];
    Float_t muon_innertrack_outerx[M_muonmaxcount];
    Float_t muon_innertrack_outery[M_muonmaxcount];
    Float_t muon_innertrack_outerz[M_muonmaxcount];
    Float_t muon_innertrack_closestpointx[M_muonmaxcount];
    Float_t muon_innertrack_closestpointy[M_muonmaxcount];
    Float_t muon_innertrack_closestpointz[M_muonmaxcount];
    Float_t muon_innertrack_chi2[M_muonmaxcount];
    Float_t muon_innertrack_ndof[M_muonmaxcount];
    Float_t muon_innertrack_dxy[M_muonmaxcount];
    Float_t muon_innertrack_dxyerr[M_muonmaxcount];
    Float_t muon_innertrack_dz[M_muonmaxcount];
    Float_t muon_innertrack_dzerr[M_muonmaxcount];
    Float_t muon_innertrack_dedxharmonic2[M_muonmaxcount];
    Int_t muon_innertrack_charge[M_muonmaxcount];
    UChar_t muon_innertrack_nhits[M_muonmaxcount];
    UChar_t muon_innertrack_nmissinghits[M_muonmaxcount];
    UChar_t muon_innertrack_npixelhits[M_muonmaxcount];
    UChar_t muon_innertrack_npixellayers[M_muonmaxcount];
    UChar_t muon_innertrack_nstriplayers[M_muonmaxcount];
    Float_t muon_outertrack_px[M_muonmaxcount];
    Float_t muon_outertrack_py[M_muonmaxcount];
    Float_t muon_outertrack_pz[M_muonmaxcount];
    UChar_t muon_outertrack_hits[M_muonmaxcount];
    UChar_t muon_outertrack_missinghits[M_muonmaxcount];
    Float_t muon_outertrack_chi2[M_muonmaxcount];
    Float_t muon_outertrack_ndof[M_muonmaxcount];
    Float_t muon_isolationr3track[M_muonmaxcount];
    Int_t muon_isolationr3ntrack[M_muonmaxcount];
    Float_t muon_isolationr3ecal[M_muonmaxcount];
    Float_t muon_isolationr3hcal[M_muonmaxcount];
    Float_t muon_ecalenergy[M_muonmaxcount];
    Float_t muon_hcalenergy[M_muonmaxcount];
    Float_t muon_pfisolationr3_sumchargedhadronpt[M_muonmaxcount];
    Float_t muon_pfisolationr3_sumchargedparticlept[M_muonmaxcount];
    Float_t muon_pfisolationr3_sumneutralhadronet[M_muonmaxcount];
    Float_t muon_pfisolationr3_sumphotonet[M_muonmaxcount];
    Float_t muon_pfisolationr3_sumneutralhadronethighthreshold[M_muonmaxcount];
    Float_t muon_pfisolationr3_sumphotonethighthreshold[M_muonmaxcount];
    Float_t muon_pfisolationr3_sumpupt[M_muonmaxcount];
    Float_t muon_pfisolationr4_sumchargedhadronpt[M_muonmaxcount];
    Float_t muon_pfisolationr4_sumchargedparticlept[M_muonmaxcount];
    Float_t muon_pfisolationr4_sumneutralhadronet[M_muonmaxcount];
    Float_t muon_pfisolationr4_sumphotonet[M_muonmaxcount];
    Float_t muon_pfisolationr4_sumneutralhadronethighthreshold[M_muonmaxcount];
    Float_t muon_pfisolationr4_sumphotonethighthreshold[M_muonmaxcount];
    Float_t muon_pfisolationr4_sumpupt[M_muonmaxcount];
    Int_t muon_charge[M_muonmaxcount];
    Int_t muon_numchambers[M_muonmaxcount];
    Int_t muon_numchamberswithsegments[M_muonmaxcount];
    Int_t muon_numvalidmuonhits[M_muonmaxcount];
    Int_t muon_nummatchedstations[M_muonmaxcount];
    UChar_t muon_type[M_muonmaxcount];
    UInt_t muon_trigger[M_muonmaxcount];
    UInt_t muon_trackermuonquality[M_muonmaxcount];

    UInt_t ak4calojet_count;
    Float_t ak4calojet_e[M_jetmaxcount];
    Float_t ak4calojet_px[M_jetmaxcount];
    Float_t ak4calojet_py[M_jetmaxcount];
    Float_t ak4calojet_pz[M_jetmaxcount];
    Float_t ak4calojet_hadronicenergy[M_jetmaxcount];
    Float_t ak4calojet_emenergy[M_jetmaxcount];
    Float_t ak4calojet_energycorr[M_jetmaxcount];
    Float_t ak4calojet_energycorrl7uds[M_jetmaxcount];
    Float_t ak4calojet_energycorrl7bottom[M_jetmaxcount];
    Float_t ak4calojet_fhpd[M_jetmaxcount];
    Float_t ak4calojet_restrictedemf[M_jetmaxcount];
    Float_t ak4calojet_btag[M_jetmaxcount][M_btagmax];
    UInt_t ak4calojet_n90[M_jetmaxcount];
    UInt_t ak4calojet_n60[M_jetmaxcount];

    UInt_t ak4jptjet_count;
    Float_t ak4jptjet_e[M_jetmaxcount];
    Float_t ak4jptjet_px[M_jetmaxcount];
    Float_t ak4jptjet_py[M_jetmaxcount];
    Float_t ak4jptjet_pz[M_jetmaxcount];
    Float_t ak4jptjet_hadronicenergy[M_jetmaxcount];
    Float_t ak4jptjet_chargedhadronicenergy[M_jetmaxcount];
    Float_t ak4jptjet_emenergy[M_jetmaxcount];
    Float_t ak4jptjet_chargedemenergy[M_jetmaxcount];
    UInt_t ak4jptjet_chargedmulti[M_jetmaxcount];
    Float_t ak4jptjet_energycorr[M_jetmaxcount];
    Float_t ak4jptjet_energycorrl7uds[M_jetmaxcount];
    Float_t ak4jptjet_energycorrl7bottom[M_jetmaxcount];
    Float_t ak4jptjet_fhpd[M_jetmaxcount];
    Float_t ak4jptjet_restrictedemf[M_jetmaxcount];
    Float_t ak4jptjet_btag[M_jetmaxcount][M_btagmax];
    UInt_t ak4jptjet_n90[M_jetmaxcount];

    UInt_t ak4pfjet_count;
    Float_t ak4pfjet_e[M_jetmaxcount];
    Float_t ak4pfjet_px[M_jetmaxcount];
    Float_t ak4pfjet_py[M_jetmaxcount];
    Float_t ak4pfjet_pz[M_jetmaxcount];
    Float_t ak4pfjet_area[M_jetmaxcount];
    Float_t ak4pfjet_hadronicenergy[M_jetmaxcount];
    Float_t ak4pfjet_chargedhadronicenergy[M_jetmaxcount];
    Float_t ak4pfjet_emenergy[M_jetmaxcount];
    Float_t ak4pfjet_chargedemenergy[M_jetmaxcount];
    Float_t ak4pfjet_hfemenergy[M_jetmaxcount];
    Float_t ak4pfjet_hfhadronicenergy[M_jetmaxcount];
    Float_t ak4pfjet_electronenergy[M_jetmaxcount];
    Float_t ak4pfjet_muonenergy[M_jetmaxcount];
    UInt_t ak4pfjet_chargedmulti[M_jetmaxcount];
    UInt_t ak4pfjet_neutralmulti[M_jetmaxcount];
    UInt_t ak4pfjet_hfhadronicmulti[M_jetmaxcount];
    UInt_t ak4pfjet_hfemmulti[M_jetmaxcount];
    UInt_t ak4pfjet_electronmulti[M_jetmaxcount];
    UInt_t ak4pfjet_muonmulti[M_jetmaxcount];
    Float_t ak4pfjet_chargeda[M_jetmaxcount];
    Float_t ak4pfjet_chargedb[M_jetmaxcount];
    Float_t ak4pfjet_neutrala[M_jetmaxcount];
    Float_t ak4pfjet_neutralb[M_jetmaxcount];
    Float_t ak4pfjet_alla[M_jetmaxcount];
    Float_t ak4pfjet_allb[M_jetmaxcount];
    Float_t ak4pfjet_chargedfractionmv[M_jetmaxcount];
    Float_t ak4pfjet_energycorr[M_jetmaxcount];
    Float_t ak4pfjet_energycorrunc[M_jetmaxcount];
    Float_t ak4pfjet_energycorrl7uds[M_jetmaxcount];
    Float_t ak4pfjet_energycorrl7bottom[M_jetmaxcount];
    Float_t ak4pfjet_puidfull[M_jetmaxcount];
    Float_t ak4pfjet_puidsimple[M_jetmaxcount];
    Float_t ak4pfjet_puidcutbased[M_jetmaxcount];
    Float_t ak4pfjet_btag[M_jetmaxcount][M_btagmax];
    UInt_t ak4pfjet_trigger[M_jetmaxcount];
    Int_t ak4pfjet_mcflavour[M_jetmaxcount];

    UInt_t ak4pfchsjet_count;
    Float_t ak4pfchsjet_e[M_jetmaxcount];
    Float_t ak4pfchsjet_px[M_jetmaxcount];
    Float_t ak4pfchsjet_py[M_jetmaxcount];
    Float_t ak4pfchsjet_pz[M_jetmaxcount];
    Float_t ak4pfchsjet_pt[M_jetmaxcount];
    Float_t ak4pfchsjet_phi[M_jetmaxcount];
    Float_t ak4pfchsjet_eta[M_jetmaxcount];
    Float_t ak4pfchsjet_area[M_jetmaxcount];
    Float_t ak4pfchsjet_hadronicenergy[M_jetmaxcount];
    Float_t ak4pfchsjet_chargedhadronicenergy[M_jetmaxcount];
    Float_t ak4pfchsjet_emenergy[M_jetmaxcount];
    Float_t ak4pfchsjet_chargedemenergy[M_jetmaxcount];
    Float_t ak4pfchsjet_hfemenergy[M_jetmaxcount];
    Float_t ak4pfchsjet_hfhadronicenergy[M_jetmaxcount];
    Float_t ak4pfchsjet_electronenergy[M_jetmaxcount];
    Float_t ak4pfchsjet_muonenergy[M_jetmaxcount];
    UInt_t ak4pfchsjet_chargedmulti[M_jetmaxcount];
    UInt_t ak4pfchsjet_neutralmulti[M_jetmaxcount];
    UInt_t ak4pfchsjet_hfhadronicmulti[M_jetmaxcount];
    UInt_t ak4pfchsjet_hfemmulti[M_jetmaxcount];
    UInt_t ak4pfchsjet_electronmulti[M_jetmaxcount];
    UInt_t ak4pfchsjet_muonmulti[M_jetmaxcount];
    Float_t ak4pfchsjet_chargeda[M_jetmaxcount];
    Float_t ak4pfchsjet_chargedb[M_jetmaxcount];
    Float_t ak4pfchsjet_neutrala[M_jetmaxcount];
    Float_t ak4pfchsjet_neutralb[M_jetmaxcount];
    Float_t ak4pfchsjet_alla[M_jetmaxcount];
    Float_t ak4pfchsjet_allb[M_jetmaxcount];
    Float_t ak4pfchsjet_chargedfractionmv[M_jetmaxcount];
    Float_t ak4pfchsjet_energycorr[M_jetmaxcount];
    Float_t ak4pfchsjet_energycorrunc[M_jetmaxcount];
    Float_t ak4pfchsjet_energycorrl7uds[M_jetmaxcount];
    Float_t ak4pfchsjet_energycorrl7bottom[M_jetmaxcount];
    Float_t ak4pfchsjet_btag[M_jetmaxcount][M_btagmax];
    UInt_t ak4pfchsjet_trigger[M_jetmaxcount];
    Int_t ak4pfchsjet_mcflavour[M_jetmaxcount];

    UInt_t ak4pfchspuppijet_count;
    Float_t ak4pfchspuppijet_e[M_jetmaxcount];
    Float_t ak4pfchspuppijet_px[M_jetmaxcount];
    Float_t ak4pfchspuppijet_py[M_jetmaxcount];
    Float_t ak4pfchspuppijet_pz[M_jetmaxcount];
    Float_t ak4pfchspuppijet_pt[M_jetmaxcount];
    Float_t ak4pfchspuppijet_phi[M_jetmaxcount];
    Float_t ak4pfchspuppijet_eta[M_jetmaxcount];
    Float_t ak4pfchspuppijet_area[M_jetmaxcount];
    Float_t ak4pfchspuppijet_hadronicenergy[M_jetmaxcount];
    Float_t ak4pfchspuppijet_chargedhadronicenergy[M_jetmaxcount];
    Float_t ak4pfchspuppijet_emenergy[M_jetmaxcount];
    Float_t ak4pfchspuppijet_chargedemenergy[M_jetmaxcount];
    Float_t ak4pfchspuppijet_hfemenergy[M_jetmaxcount];
    Float_t ak4pfchspuppijet_hfhadronicenergy[M_jetmaxcount];
    Float_t ak4pfchspuppijet_electronenergy[M_jetmaxcount];
    Float_t ak4pfchspuppijet_muonenergy[M_jetmaxcount];
    UInt_t ak4pfchspuppijet_chargedmulti[M_jetmaxcount];
    UInt_t ak4pfchspuppijet_neutralmulti[M_jetmaxcount];
    UInt_t ak4pfchspuppijet_hfhadronicmulti[M_jetmaxcount];
    UInt_t ak4pfchspuppijet_hfemmulti[M_jetmaxcount];
    UInt_t ak4pfchspuppijet_electronmulti[M_jetmaxcount];
    UInt_t ak4pfchspuppijet_muonmulti[M_jetmaxcount];
    Float_t ak4pfchspuppijet_chargeda[M_jetmaxcount];
    Float_t ak4pfchspuppijet_chargedb[M_jetmaxcount];
    Float_t ak4pfchspuppijet_neutrala[M_jetmaxcount];
    Float_t ak4pfchspuppijet_neutralb[M_jetmaxcount];
    Float_t ak4pfchspuppijet_alla[M_jetmaxcount];
    Float_t ak4pfchspuppijet_allb[M_jetmaxcount];
    Float_t ak4pfchspuppijet_chargedfractionmv[M_jetmaxcount];
    Float_t ak4pfchspuppijet_energycorr[M_jetmaxcount];
    Float_t ak4pfchspuppijet_energycorrunc[M_jetmaxcount];
    Float_t ak4pfchspuppijet_energycorrl7uds[M_jetmaxcount];
    Float_t ak4pfchspuppijet_energycorrl7bottom[M_jetmaxcount];
    Float_t ak4pfchspuppijet_btag[M_jetmaxcount][M_btagmax];
    UInt_t ak4pfchspuppijet_trigger[M_jetmaxcount];
    Int_t ak4pfchspuppijet_mcflavour[M_jetmaxcount];

    UInt_t electron_count;
    Int_t electron_vtx[M_electronmaxcount];
    Float_t electron_px[M_electronmaxcount];
    Float_t electron_py[M_electronmaxcount];
    Float_t electron_pz[M_electronmaxcount];
    Float_t electron_pt[M_electronmaxcount];
    Float_t electron_phi[M_electronmaxcount];
    Float_t electron_eta[M_electronmaxcount];
    Float_t electron_correctedecalenergy[M_electronmaxcount];
    Float_t electron_trackchi2[M_electronmaxcount];
    Float_t electron_trackndof[M_electronmaxcount];

    Int_t electron_cbID[M_electronmaxcount];
    Int_t electron_heepID[M_electronmaxcount];
    Int_t electron_mvaID[M_electronmaxcount];

    Int_t electron_has_gen_particle[M_electronmaxcount];
    Int_t electron_gen_particle_pdgid[M_electronmaxcount];
    Int_t electron_has_gen_mother[M_electronmaxcount];
    Int_t electron_gen_mother_pdgid[M_electronmaxcount];

    Float_t electron_outerx[M_electronmaxcount];
    Float_t electron_outery[M_electronmaxcount];
    Float_t electron_outerz[M_electronmaxcount];
    Float_t electron_closestpointx[M_electronmaxcount];
    Float_t electron_closestpointy[M_electronmaxcount];
    Float_t electron_closestpointz[M_electronmaxcount];
    Float_t electron_esuperclusterovertrack[M_electronmaxcount];
    Float_t electron_eseedclusterovertrack[M_electronmaxcount];
    Float_t electron_deltaetasuperclustertrack[M_electronmaxcount];
    Float_t electron_deltaphisuperclustertrack[M_electronmaxcount];
    Float_t electron_e1x5[M_electronmaxcount];
    Float_t electron_e2x5[M_electronmaxcount];
    Float_t electron_e5x5[M_electronmaxcount];
    Float_t electron_r9[M_electronmaxcount];
    Float_t electron_sigmaetaeta[M_electronmaxcount];
    Float_t electron_sigmaietaieta[M_electronmaxcount];
    Float_t electron_sigmaiphiiphi[M_electronmaxcount];
    Float_t electron_ehcaloverecaldepth1[M_electronmaxcount];
    Float_t electron_ehcaloverecaldepth2[M_electronmaxcount];
    Float_t electron_ehcaltoweroverecaldepth1[M_electronmaxcount];
    Float_t electron_ehcaltoweroverecaldepth2[M_electronmaxcount];
    Float_t electron_isolationr3track[M_electronmaxcount];
    Float_t electron_isolationr3ecal[M_electronmaxcount];
    Float_t electron_isolationr3hcal[M_electronmaxcount];
    Float_t electron_isolationr4track[M_electronmaxcount];
    Float_t electron_isolationr4ecal[M_electronmaxcount];
    Float_t electron_isolationr4hcal[M_electronmaxcount];
    Float_t electron_isolationpfr3charged[M_electronmaxcount];
    Float_t electron_isolationpfr3photon[M_electronmaxcount];
    Float_t electron_isolationpfr3neutral[M_electronmaxcount];
    Int_t electron_charge[M_electronmaxcount];
    UChar_t electron_nhits[M_electronmaxcount];
    UChar_t electron_nmissinghits[M_electronmaxcount];
    UChar_t electron_npixelhits[M_electronmaxcount];
    UChar_t electron_npixellayers[M_electronmaxcount];
    UChar_t electron_nstriplayers[M_electronmaxcount];
    UChar_t electron_nhitsexpected[M_electronmaxcount];
    Float_t electron_dxy[M_electronmaxcount];
    Float_t electron_dxyerr[M_electronmaxcount];
    Float_t electron_dz[M_electronmaxcount];
    Float_t electron_dzerr[M_electronmaxcount];
    Float_t electron_convdist[M_electronmaxcount];
    Float_t electron_convdcot[M_electronmaxcount];
    Float_t electron_convradius[M_electronmaxcount];
    UInt_t electron_gapinfo[M_electronmaxcount];
    Float_t electron_fbrems[M_electronmaxcount];
    Int_t electron_numbrems[M_electronmaxcount];
    UChar_t electron_info[M_electronmaxcount];
    UInt_t electron_trigger[M_electronmaxcount];
    UChar_t electron_eID[M_electronmaxcount];
    Float_t electron_supercluster_e[M_electronmaxcount];
    Float_t electron_supercluster_x[M_electronmaxcount];
    Float_t electron_supercluster_y[M_electronmaxcount];
    Float_t electron_supercluster_z[M_electronmaxcount];
    Float_t electron_supercluster_rawe[M_electronmaxcount];
    Float_t electron_supercluster_phiwidth[M_electronmaxcount];
    Float_t electron_supercluster_etawidth[M_electronmaxcount];
    Int_t electron_supercluster_nbasiccluster[M_electronmaxcount];

    UInt_t photon_count;
    Float_t photon_px[M_photonmaxcount];
    Float_t photon_py[M_photonmaxcount];
    Float_t photon_pz[M_photonmaxcount];
    Float_t photon_pt[M_photonmaxcount];
    Float_t photon_phi[M_photonmaxcount];
    Float_t photon_eta[M_photonmaxcount];
    Float_t photon_e1x5[M_photonmaxcount];
    Float_t photon_e2x5[M_photonmaxcount];
    Float_t photon_e3x3[M_photonmaxcount];
    Float_t photon_e5x5[M_photonmaxcount];
    Float_t photon_sigmaietaieta[M_photonmaxcount];
    Float_t photon_sigmaietaiphi[M_photonmaxcount];
    Float_t photon_sigmaiphiiphi[M_photonmaxcount];
    Float_t photon_ehcaloverecaldepth1[M_photonmaxcount];
    Float_t photon_ehcaloverecaldepth2[M_photonmaxcount];
    Float_t photon_ehcaltoweroverecaldepth1[M_photonmaxcount];
    Float_t photon_ehcaltoweroverecaldepth2[M_photonmaxcount];
    Float_t photon_maxenergyxtal[M_photonmaxcount];
    Float_t photon_isolationr3track[M_photonmaxcount];
    Float_t photon_isolationr3trackhollow[M_photonmaxcount];
    UInt_t photon_isolationr3ntrack[M_photonmaxcount];
    UInt_t photon_isolationr3ntrackhollow[M_photonmaxcount];
    Float_t photon_isolationr3ecal[M_photonmaxcount];
    Float_t photon_isolationr3hcal[M_photonmaxcount];
    Float_t photon_isolationr4track[M_photonmaxcount];
    Float_t photon_isolationr4trackhollow[M_photonmaxcount];
    UInt_t photon_isolationr4ntrack[M_photonmaxcount];
    UInt_t photon_isolationr4ntrackhollow[M_photonmaxcount];
    Float_t photon_isolationr4ecal[M_photonmaxcount];
    Float_t photon_isolationr4hcal[M_photonmaxcount];
    Float_t photon_isolationpfr3charged[M_photonmaxcount];
    Float_t photon_isolationpfr3photon[M_photonmaxcount];
    Float_t photon_isolationpfr3neutral[M_photonmaxcount];
    Float_t photon_isolationpfr4charged[M_photonmaxcount];
    Float_t photon_isolationpfr4photon[M_photonmaxcount];
    Float_t photon_isolationpfr4neutral[M_photonmaxcount];
    Float_t photon_isolationpfr4noscfootprintcharged[M_photonmaxcount];
    Float_t photon_isolationpfr4noscfootprintphoton[M_photonmaxcount];
    Float_t photon_isolationpfr4noscfootprintneutral[M_photonmaxcount];
    Float_t photon_supercluster_e[M_photonmaxcount];
    Float_t photon_supercluster_x[M_photonmaxcount];
    Float_t photon_supercluster_y[M_photonmaxcount];
    Float_t photon_supercluster_z[M_photonmaxcount];
    Float_t photon_supercluster_rawe[M_photonmaxcount];
    Float_t photon_supercluster_phiwidth[M_photonmaxcount];
    Float_t photon_supercluster_etawidth[M_photonmaxcount];
    Int_t photon_supercluster_nbasiccluster[M_photonmaxcount];
    UChar_t photon_info[M_photonmaxcount];
    UInt_t photon_gapinfo[M_photonmaxcount];
    UInt_t photon_trigger[M_photonmaxcount];
    UInt_t photon_conversionbegin[M_photonmaxcount];

    UInt_t conversion_count;
    UChar_t conversion_info[M_conversionmaxcount];
    Float_t conversion_vx[M_conversionmaxcount];
    Float_t conversion_vy[M_conversionmaxcount];
    Float_t conversion_vz[M_conversionmaxcount];
    Float_t conversion_chi2[M_conversionmaxcount];
    Float_t conversion_ndof[M_conversionmaxcount];
    Float_t conversion_cov[M_conversionmaxcount][6];
    Float_t conversion_mvaout[M_conversionmaxcount];
    Float_t conversion_trackecalpointx[M_conversionmaxcount][2];
    Float_t conversion_trackecalpointy[M_conversionmaxcount][2];
    Float_t conversion_trackecalpointz[M_conversionmaxcount][2];
    Float_t conversion_trackpx[M_conversionmaxcount][2];
    Float_t conversion_trackpy[M_conversionmaxcount][2];
    Float_t conversion_trackpz[M_conversionmaxcount][2];
    Float_t conversion_trackclosestpointx[M_conversionmaxcount][2];
    Float_t conversion_trackclosestpointy[M_conversionmaxcount][2];
    Float_t conversion_trackclosestpointz[M_conversionmaxcount][2];
    Float_t conversion_trackchi2[M_conversionmaxcount][2];
    Float_t conversion_trackndof[M_conversionmaxcount][2];
    Float_t conversion_trackdxy[M_conversionmaxcount][2];
    Float_t conversion_trackdxyerr[M_conversionmaxcount][2];
    Float_t conversion_trackdz[M_conversionmaxcount][2];
    Float_t conversion_trackdzerr[M_conversionmaxcount][2];
    Int_t conversion_trackcharge[M_conversionmaxcount][2];
    UChar_t conversion_tracknhits[M_conversionmaxcount][2];
    UChar_t conversion_tracknmissinghits[M_conversionmaxcount][2];
    UChar_t conversion_tracknpixelhits[M_conversionmaxcount][2];
    UChar_t conversion_tracknpixellayers[M_conversionmaxcount][2];
    UChar_t conversion_tracknstriplayers[M_conversionmaxcount][2];

    UInt_t allconversion_count;
    UChar_t allconversion_info[M_conversionmaxcount];
    Float_t allconversion_vx[M_conversionmaxcount];
    Float_t allconversion_vy[M_conversionmaxcount];
    Float_t allconversion_vz[M_conversionmaxcount];
    Float_t allconversion_chi2[M_conversionmaxcount];
    Float_t allconversion_ndof[M_conversionmaxcount];
    Float_t allconversion_cov[M_conversionmaxcount][6];
    Float_t allconversion_mvaout[M_conversionmaxcount];
    Float_t allconversion_trackecalpointx[M_conversionmaxcount][2];
    Float_t allconversion_trackecalpointy[M_conversionmaxcount][2];
    Float_t allconversion_trackecalpointz[M_conversionmaxcount][2];
    Float_t allconversion_trackpx[M_conversionmaxcount][2];
    Float_t allconversion_trackpy[M_conversionmaxcount][2];
    Float_t allconversion_trackpz[M_conversionmaxcount][2];
    Float_t allconversion_trackclosestpointx[M_conversionmaxcount][2];
    Float_t allconversion_trackclosestpointy[M_conversionmaxcount][2];
    Float_t allconversion_trackclosestpointz[M_conversionmaxcount][2];
    Float_t allconversion_trackchi2[M_conversionmaxcount][2];
    Float_t allconversion_trackndof[M_conversionmaxcount][2];
    Float_t allconversion_trackdxy[M_conversionmaxcount][2];
    Float_t allconversion_trackdxyerr[M_conversionmaxcount][2];
    Float_t allconversion_trackdz[M_conversionmaxcount][2];
    Float_t allconversion_trackdzerr[M_conversionmaxcount][2];
    Int_t allconversion_trackcharge[M_conversionmaxcount][2];
    UChar_t allconversion_tracknhits[M_conversionmaxcount][2];
    UChar_t allconversion_tracknmissinghits[M_conversionmaxcount][2];
    UChar_t allconversion_tracknpixelhits[M_conversionmaxcount][2];
    UChar_t allconversion_tracknpixellayers[M_conversionmaxcount][2];
    UChar_t allconversion_tracknstriplayers[M_conversionmaxcount][2];

    UInt_t tau_count;
    Float_t tau_px[M_taumaxcount];
    Float_t tau_py[M_taumaxcount];
    Float_t tau_pz[M_taumaxcount];
    Float_t tau_pt[M_taumaxcount];
    Float_t tau_phi[M_taumaxcount];
    Float_t tau_eta[M_taumaxcount];
    Float_t tau_isolationneutralspt[M_taumaxcount];
    UInt_t tau_isolationneutralsnum[M_taumaxcount];
    Float_t tau_isolationchargedpt[M_taumaxcount];
    UInt_t tau_isolationchargednum[M_taumaxcount];
    Float_t tau_pucorrptsum[M_taumaxcount];
    Float_t tau_isolationgammapt[M_taumaxcount];
    UInt_t tau_isolationgammanum[M_taumaxcount];
    Int_t tau_charge[M_taumaxcount];
    UInt_t tau_dishps[M_taumaxcount];
    Float_t tau_emfraction[M_taumaxcount];
    Float_t tau_hcaltotoverplead[M_taumaxcount];
    Float_t tau_hcal3x3overplead[M_taumaxcount];
    Float_t tau_ecalstripsumeoverplead[M_taumaxcount];
    Float_t tau_bremsrecoveryeoverplead[M_taumaxcount];
    Float_t tau_calocomp[M_taumaxcount];
    Float_t tau_segcomp[M_taumaxcount];
    UInt_t tau_trigger[M_taumaxcount];
    Float_t tau_ak4pfjet_e[M_taumaxcount];
    Float_t tau_ak4pfjet_px[M_taumaxcount];
    Float_t tau_ak4pfjet_py[M_taumaxcount];
    Float_t tau_ak4pfjet_pz[M_taumaxcount];
    Float_t tau_ak4pfjet_hadronicenergy[M_taumaxcount];
    Float_t tau_ak4pfjet_chargedhadronicenergy[M_taumaxcount];
    Float_t tau_ak4pfjet_emenergy[M_taumaxcount];
    Float_t tau_ak4pfjet_chargedemenergy[M_taumaxcount];
    UInt_t tau_ak4pfjet_chargedmulti[M_taumaxcount];
    UInt_t tau_ak4pfjet_neutralmulti[M_taumaxcount];
    UInt_t tau_ak4pfjet_trigger[M_taumaxcount];
    UInt_t tau_chargedbegin[M_taumaxcount];
    UInt_t tau_charged_count;
    Float_t tau_charged_px[M_taumaxcount*10];
    Float_t tau_charged_py[M_taumaxcount*10];
    Float_t tau_charged_pz[M_taumaxcount*10];
    Float_t tau_charged_outerx[M_taumaxcount*10];
    Float_t tau_charged_outery[M_taumaxcount*10];
    Float_t tau_charged_outerz[M_taumaxcount*10];
    Float_t tau_charged_closestpointx[M_taumaxcount*10];
    Float_t tau_charged_closestpointy[M_taumaxcount*10];
    Float_t tau_charged_closestpointz[M_taumaxcount*10];
    Float_t tau_charged_chi2[M_taumaxcount*10];
    Float_t tau_charged_ndof[M_taumaxcount*10];
    Float_t tau_charged_dxy[M_taumaxcount*10];
    Float_t tau_charged_dxyerr[M_taumaxcount*10];
    Float_t tau_charged_dz[M_taumaxcount*10];
    Float_t tau_charged_dzerr[M_taumaxcount*10];
    Float_t tau_charged_dedxharmonic2[M_taumaxcount*10];
    Int_t tau_charged_charge[M_taumaxcount*10];
    UChar_t tau_charged_nhits[M_taumaxcount*10];
    UChar_t tau_charged_nmissinghits[M_taumaxcount*10];
    UChar_t tau_charged_npixelhits[M_taumaxcount*10];
    UChar_t tau_charged_npixellayers[M_taumaxcount*10];
    UChar_t tau_charged_nstriplayers[M_taumaxcount*10];

    Float_t ak4pfjet_rho;
    Float_t ak4pfjet_sigma;

    Float_t patmvamet_emt_ex;
    Float_t patmvamet_emt_ey;
    Float_t patmvamet_emt_cov_00;
    Float_t patmvamet_emt_cov_01;
    Float_t patmvamet_emt_cov_10;
    Float_t patmvamet_emt_cov_11;

    Float_t patmvamet_em_ex;
    Float_t patmvamet_em_ey;
    Float_t patmvamet_em_cov_00;
    Float_t patmvamet_em_cov_01;
    Float_t patmvamet_em_cov_10;
    Float_t patmvamet_em_cov_11;

    Float_t patmvamet_et_ex;
    Float_t patmvamet_et_ey;
    Float_t patmvamet_et_cov_00;
    Float_t patmvamet_et_cov_01;
    Float_t patmvamet_et_cov_10;
    Float_t patmvamet_et_cov_11;

    Float_t patmvamet_mt_ex;
    Float_t patmvamet_mt_ey;
    Float_t patmvamet_mt_cov_00;
    Float_t patmvamet_mt_cov_01;
    Float_t patmvamet_mt_cov_10;
    Float_t patmvamet_mt_cov_11;

    Float_t patmvamet_tt_ex;
    Float_t patmvamet_tt_ey;
    Float_t patmvamet_tt_cov_00;
    Float_t patmvamet_tt_cov_01;
    Float_t patmvamet_tt_cov_10;
    Float_t patmvamet_tt_cov_11;

    Float_t pfmet_ex;
    Float_t pfmet_ey;

    Float_t pfmettype1_ex;
    Float_t pfmettype1_ey;
    Float_t pfmettype1_cov_00;
    Float_t pfmettype1_cov_01;
    Float_t pfmettype1_cov_10;
    Float_t pfmettype1_cov_11;

    Float_t pfmetpuppitype1_ex;
    Float_t pfmetpuppitype1_ey;

    Float_t pfmettype0type1_ex;
    Float_t pfmettype0type1_ey;

    UInt_t secvertices_count;
    Float_t secvertices_vx[M_secverticesmaxcount];
    Float_t secvertices_vy[M_secverticesmaxcount];
    Float_t secvertices_vz[M_secverticesmaxcount];
    Float_t secvertices_chi2[M_secverticesmaxcount];
    Float_t secvertices_ndof[M_secverticesmaxcount];
    Float_t secvertices_cov[M_secverticesmaxcount][6];
    Float_t secvertices_track_px[M_secverticesmaxcount][2];
    Float_t secvertices_track_py[M_secverticesmaxcount][2];
    Float_t secvertices_track_pz[M_secverticesmaxcount][2];
    Float_t secvertices_track_closestpointx[M_secverticesmaxcount][2];
    Float_t secvertices_track_closestpointy[M_secverticesmaxcount][2];
    Float_t secvertices_track_closestpointz[M_secverticesmaxcount][2];
    Float_t secvertices_track_chi2[M_secverticesmaxcount][2];
    Float_t secvertices_track_ndof[M_secverticesmaxcount][2];
    Float_t secvertices_track_dxy[M_secverticesmaxcount][2];
    Float_t secvertices_track_dxyerr[M_secverticesmaxcount][2];
    Float_t secvertices_track_dz[M_secverticesmaxcount][2];
    Float_t secvertices_track_dzerr[M_secverticesmaxcount][2];
    Float_t secvertices_track_dedxharmonic2[M_secverticesmaxcount][2];
    Int_t secvertices_track_charge[M_secverticesmaxcount][2];
    UChar_t secvertices_track_nhits[M_secverticesmaxcount][2];
    UChar_t secvertices_track_nmissinghits[M_secverticesmaxcount][2];
    UChar_t secvertices_track_npixelhits[M_secverticesmaxcount][2];
    UChar_t secvertices_track_npixellayers[M_secverticesmaxcount][2];
    UChar_t secvertices_track_nstriplayers[M_secverticesmaxcount][2];

    UInt_t musecvertices_count;
    Float_t musecvertices_vx[M_musecverticesmaxcount];
    Float_t musecvertices_vy[M_musecverticesmaxcount];
    Float_t musecvertices_vz[M_musecverticesmaxcount];
    Float_t musecvertices_chi2[M_musecverticesmaxcount];
    Float_t musecvertices_ndof[M_musecverticesmaxcount];
    Float_t musecvertices_cov[M_musecverticesmaxcount][6];
    Float_t musecvertices_track_px[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_py[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_pz[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_closestpointx[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_closestpointy[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_closestpointz[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_chi2[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_ndof[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_dxy[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_dxyerr[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_dz[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_dzerr[M_musecverticesmaxcount][2];
    Float_t musecvertices_track_dedxharmonic2[M_musecverticesmaxcount][2];
    Int_t musecvertices_track_charge[M_musecverticesmaxcount][2];
    UChar_t musecvertices_track_nhits[M_musecverticesmaxcount][2];
    UChar_t musecvertices_track_nmissinghits[M_musecverticesmaxcount][2];
    UChar_t musecvertices_track_npixelhits[M_musecverticesmaxcount][2];
    UChar_t musecvertices_track_npixellayers[M_musecverticesmaxcount][2];
    UChar_t musecvertices_track_nstriplayers[M_musecverticesmaxcount][2];
    //Generator Information
    Float_t genweight;
    Float_t genid1;
    Float_t genx1;
    Float_t genid2;
    Float_t genx2;
    Float_t genScale;

    Int_t numpileupinteractionsminus;
    Int_t numpileupinteractions;
    Int_t numpileupinteractionsplus;
    Float_t numtruepileupinteractions;

    Float_t genmetcalo_ex;
    Float_t genmetcalo_ey;
    Float_t genmettrue_ex;
    Float_t genmettrue_ey;

    UInt_t genak4jet_count;
    Float_t genak4jet_e[M_genjetmaxcount];
    Float_t genak4jet_px[M_genjetmaxcount];
    Float_t genak4jet_py[M_genjetmaxcount];
    Float_t genak4jet_pz[M_genjetmaxcount];
    Float_t genak4jet_einvisible[M_genjetmaxcount];
    Int_t genak4jet_flavour[M_genjetmaxcount];
    UInt_t genak4jet_info[M_genjetmaxcount];

    UInt_t genparticles_count;
    Float_t genparticles_e[M_genparticlesmaxcount];
    Float_t genparticles_px[M_genparticlesmaxcount];
    Float_t genparticles_py[M_genparticlesmaxcount];
    Float_t genparticles_pz[M_genparticlesmaxcount];
    Float_t genparticles_vx[M_genparticlesmaxcount];
    Float_t genparticles_vy[M_genparticlesmaxcount];
    Float_t genparticles_vz[M_genparticlesmaxcount];
    Int_t genparticles_pdgid[M_genparticlesmaxcount];
    Int_t genparticles_status[M_genparticlesmaxcount];
    Int_t genparticles_indirectmother[M_genparticlesmaxcount];
    UInt_t genparticles_info[M_genparticlesmaxcount];

    UInt_t genallparticles_count;
    Float_t genallparticles_e[M_genallparticlesmaxcount];
    Float_t genallparticles_px[M_genallparticlesmaxcount];
    Float_t genallparticles_py[M_genallparticlesmaxcount];
    Float_t genallparticles_pz[M_genallparticlesmaxcount];
    Float_t genallparticles_vx[M_genallparticlesmaxcount];
    Float_t genallparticles_vy[M_genallparticlesmaxcount];
    Float_t genallparticles_vz[M_genallparticlesmaxcount];
    Int_t genallparticles_pdgid[M_genallparticlesmaxcount];
    Int_t genallparticles_status[M_genallparticlesmaxcount];
    UInt_t genallparticles_motherbeg[M_genallparticlesmaxcount];
    UInt_t genallparticles_daughterbeg[M_genallparticlesmaxcount];

    UInt_t genallparticlesmother_count;
    UInt_t genallparticles_mothers[M_genmotherdaughtermaxcount];

    UInt_t genallparticlesdaughter_count;
    UInt_t genallparticles_daughters[M_genmotherdaughtermaxcount];
    //lumitree
    UInt_t lumi_run;
    UInt_t lumi_block;
    Float_t lumi_value;
    Float_t lumi_valueerr;
    Float_t lumi_livefrac;
    Float_t lumi_deadfrac;
    UInt_t lumi_quality;
    UInt_t lumi_eventsprocessed;
    UInt_t lumi_eventsfiltered;
    UInt_t lumi_hltprescaletable;
    UInt_t lumi_l1algoprescaletable;
    UInt_t lumi_l1techprescaletable;

    //runtree
    UInt_t run_number;
    UInt_t run_hltcount;
    Char_t run_hltnames[20000];
    Char_t run_hltmunames[10000];
    Char_t run_hltelnames[10000];
    Char_t run_hlttaunames[10000];
    Char_t run_hltphotonnames[10000];
    Char_t run_hltjetnames[10000];
    Char_t run_taudiscriminators[10000];
    UInt_t run_hltprescaletablescount;
    UInt_t run_hltprescaletables[10000];
    UInt_t run_hltl1prescaletables[10000];
    UInt_t run_l1algocount;
    UInt_t run_l1algoprescaletablescount;
    UInt_t run_l1algoprescaletables[10000];
    UInt_t run_l1techcount;
    UInt_t run_l1techprescaletablescount;
    UInt_t run_l1techprescaletables[10000];
};

DEFINE_FWK_MODULE(RootMaker);

#endif

