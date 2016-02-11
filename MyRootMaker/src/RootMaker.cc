// Test comments here
// second line of test comments here
#include "MyRootMaker/MyRootMaker/interface/RootMaker.h"
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"
#include <vector>
#include <boost/foreach.hpp>

using namespace reco;
typedef ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SMatrixSym3D;
typedef ROOT::Math::SVector<double, 3> SVector3;

RootMaker::RootMaker(const edm::ParameterSet &iConfig) :

    // tokens
    l1TriggerToken_(consumes<L1GlobalTriggerReadoutRecord>(iConfig.getParameter<edm::InputTag>("l1trigger"))),
    triggerBits_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"))),
    triggerObjects_(consumes<pat::TriggerObjectStandAloneCollection>(iConfig.getParameter<edm::InputTag>("objects"))),
    triggerPrescales_(consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<edm::InputTag>("prescales"))),
    dharmonicToken_(consumes<edm::ValueMap<DeDxData>>(iConfig.getParameter<edm::InputTag>("dEdxharmonic2"))),
    verticesToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
    conversionsToken_(consumes<vector<reco::Conversion> >(iConfig.getParameter<edm::InputTag>("conversions"))),
    ak4pfchsJetsToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("ak4pfchsjets"))),
    ak4pfchsJetsPuppiToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("ak4pfchsjetspuppi"))),
    genSimParticlesToken_(consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genSimParticles"))),
    genParticlesToken_(consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticles"))),
    //genParticlesToken_(consumes<edm::View<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("genParticles"))),
    genJetsToken_(consumes<reco::GenJetCollection>(iConfig.getParameter<edm::InputTag>("genJets"))),
    beamSpotToken_(consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamSpot"))),
    superClustersToken_(consumes<vector<reco::SuperCluster> >(iConfig.getParameter<edm::InputTag>("superClusters"))),
    genInfoToken_(consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genInfo"))),
    puInfoToken_(consumes<std::vector<PileupSummaryInfo> >(iConfig.getParameter<edm::InputTag>("puInfo"))),
    ebeeClustersToken_(consumes<vector<reco::CaloCluster> >(iConfig.getParameter<edm::InputTag>("ebeeClusters"))),
    esClustersToken_(consumes<vector<reco::CaloCluster> >(iConfig.getParameter<edm::InputTag>("esClusters"))),
    ebRecHitsToken_(consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > >(iConfig.getParameter<edm::InputTag>("barrelHits"))),
    eeRecHitsToken_(consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > >(iConfig.getParameter<edm::InputTag>("endcapHits"))),
    esRecHitsToken_(consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > >(iConfig.getParameter<edm::InputTag>("esHits"))),

    // AOD mayConsume
    recoTracksToken_(mayConsume<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("generalTracks"))),
    recoMuonsToken_(mayConsume<reco::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
    recoElectronsToken_(mayConsume<reco::GsfElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons"))),
    recoPhotonsToken_(mayConsume<reco::PhotonCollection>(iConfig.getParameter<edm::InputTag>("photons"))),
    recoTausToken_(mayConsume<reco::PFTauCollection>(iConfig.getParameter<edm::InputTag>("taus"))),
    tauJetsToken_(mayConsume<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("taujets"))),
    ak4caloJetsToken_(mayConsume<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("ak4calojets"))),
    ak4jptJetsToken_(mayConsume<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("ak4jptjets"))),
    ak4pfJetsToken_(mayConsume<reco::PFJetCollection>(iConfig.getParameter<edm::InputTag>("ak4pfjets"))),
    recoPFCandsToken_(mayConsume<reco::PFCandidateCollection>(iConfig.getParameter<edm::InputTag>("PfCands"))),
    recoMetToken_(mayConsume<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("pfmet"))),
    recoMetT1Token_(mayConsume<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("pfmett1"))),
    recoMetT1T0Token_(mayConsume<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("pfmett1t0"))),

    // miniAOD mayConsume
    lostTracksToken_(mayConsume<vector<pat::PackedCandidate> >(iConfig.getParameter<edm::InputTag>("lostTracks"))),
    unpackedTracksToken_(mayConsume<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("unpackedTracks"))),
    patMuonsToken_(mayConsume<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),
    patElectronsToken_(mayConsume<pat::ElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons"))),
    patPhotonsToken_(mayConsume<pat::PhotonCollection>(iConfig.getParameter<edm::InputTag>("photons"))),
    patTausToken_(mayConsume<pat::TauCollection>(iConfig.getParameter<edm::InputTag>("taus"))),
    packedPFCandsToken_(mayConsume<pat::PackedCandidateCollection>(iConfig.getParameter<edm::InputTag>("packedPfCands"))),
    patMVAMetEMTToken_(iConfig.getParameter<edm::InputTag>("pfMetMVAEMT")),
    patMVAMetEMToken_(iConfig.getParameter<edm::InputTag>("pfMetMVAEM")),
    patMVAMetETToken_(iConfig.getParameter<edm::InputTag>("pfMetMVAET")),
    patMVAMetMTToken_(iConfig.getParameter<edm::InputTag>("pfMetMVAMT")),
    patMVAMetTTToken_(iConfig.getParameter<edm::InputTag>("pfMetMVATT")),
    newMetLabel_(iConfig.getParameter<edm::InputTag>("patMETs")),
    patMetToken_(mayConsume<pat::METCollection>(iConfig.getParameter<edm::InputTag>("mets"))),
    patMetPuppiToken_(mayConsume<pat::METCollection>(iConfig.getParameter<edm::InputTag>("metspuppi"))),
    rhoToken_(mayConsume<double>(iConfig.getParameter<edm::InputTag>("rhoAll"))),
    gedGsfElectronCoresToken_(mayConsume<vector<reco::GsfElectronCore> >(iConfig.getParameter<edm::InputTag>("gedGsfElectronCores"))),
    eleVetoIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleVetoIdMap"))),
    eleLooseIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleLooseIdMap"))),
    eleMediumIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMediumIdMap"))),
    eleTightIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleTightIdMap"))),
    eleHeepV60IdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleHeepV60IdMap"))),
    eleMVAIdMap_wp80Token_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMVAIdMap_wp80"))),
    eleMVAIdMap_wp90Token_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleMVAIdMap_wp90"))),
    gedPhotonCoresToken_(mayConsume<vector<reco::PhotonCore> >(iConfig.getParameter<edm::InputTag>("gedPhotonCores"))),


    cisMiniAOD(iConfig.getUntrackedParameter<bool> ("isMiniAOD", false)),
    cisMC(iConfig.getUntrackedParameter<bool> ("isMC", false)),
    cdebug(iConfig.getUntrackedParameter<bool> ("debug", false)),
    cgen(iConfig.getUntrackedParameter<bool> ("GenSomeParticles", false)),
    cgenallparticles(iConfig.getUntrackedParameter<bool> ("GenAllParticles", false)),
    cgenak4jets(iConfig.getUntrackedParameter<bool> ("GenAK4Jets", false)),
    ctrigger(iConfig.getUntrackedParameter<bool> ("Trigger", false)),
    cbeamspot(iConfig.getUntrackedParameter<bool> ("RecBeamSpot", false)),
    crectrack(iConfig.getUntrackedParameter<bool> ("RecTrack", false)),
    crecprimvertex(iConfig.getUntrackedParameter<bool> ("RecPrimVertex", false)),
    crecsupercluster(iConfig.getUntrackedParameter<bool> ("RecSuperCluster", false)),
    crecsuperclusterFilterPtMin(iConfig.getUntrackedParameter<double> ("RecSuperClusterFilterPtMin", 0.)),
    crecsuperclusterFilterEtaMax(iConfig.getUntrackedParameter<double> ("RecSuperClusterFilterEtaMax", 100000.)),
    crecsuperclustermember(iConfig.getUntrackedParameter<bool> ("RecSuperClusterBasicCluster", false)),
    crecsuperclusterhit(iConfig.getUntrackedParameter<bool> ("RecSuperClusterHit", false)),
    crecmuon(iConfig.getUntrackedParameter<bool> ("RecMuon", false)),
    crectau(iConfig.getUntrackedParameter<bool> ("RecTau", false)),
    crecelectron(iConfig.getUntrackedParameter<bool> ("RecElectron", false)),
    crecphoton(iConfig.getUntrackedParameter<bool> ("RecPhoton", false)),
    crecallconversion(iConfig.getUntrackedParameter<bool> ("RecAllConversion", false)),
    crecak4calojet(iConfig.getUntrackedParameter<bool> ("RecAK4CaloJet", false)),
    crecak4jptjet(iConfig.getUntrackedParameter<bool> ("RecAK4JPTJet", false)),
    crecak4pfjet(iConfig.getUntrackedParameter<bool> ("RecAK4PFJet", false)),
    crecak4pfchsjet(iConfig.getUntrackedParameter<bool> ("RecAK4PFCHSJet", false)),
    crecak4pfchspuppijet(iConfig.getUntrackedParameter<bool> ("RecAK4PFCHSPuppiJet", false)),
    crecpfmet(iConfig.getUntrackedParameter<bool> ("RecPFMet", false)),
    crecsecvertices(iConfig.getUntrackedParameter<bool> ("RecSecVertices", false)),
    crecmusecvertices(iConfig.getUntrackedParameter<bool> ("RecMuSecVertices", false)),
    cHLTriggerNamesSelection(iConfig.getUntrackedParameter<vector<string> > ("HLTriggerSelection")),
    cTriggerProcess(iConfig.getUntrackedParameter<string> ("TriggerProcess", "HLT")),
    cMuPtMin(iConfig.getUntrackedParameter<double> ("RecMuonPtMin", 0.)),
    cMuTrackIso(iConfig.getUntrackedParameter<double> ("RecMuonTrackIso", 100000.)),
    cMuEtaMax(iConfig.getUntrackedParameter<double> ("RecMuonEtaMax", 1000000.)),
    cMuHLTriggerMatching(iConfig.getUntrackedParameter<vector<string> > ("RecMuonHLTriggerMatching")),
    cMuNum(iConfig.getUntrackedParameter<int> ("RecMuonNum", 0)),
    cElPtMin(iConfig.getUntrackedParameter<double> ("RecElectronPtMin", 0.)),
    cElTrackIso(iConfig.getUntrackedParameter<double> ("RecElectronTrackIso", 1000000.)),
    cElEtaMax(iConfig.getUntrackedParameter<double> ("RecElectronEtaMax", 1000000.)),
    cElHLTriggerMatching(iConfig.getUntrackedParameter<vector<string> > ("RecElectronHLTriggerMatching")),
    cElNum(iConfig.getUntrackedParameter<int> ("RecElectronNum", 0)),
    cElFilterPtMin(iConfig.getUntrackedParameter<double> ("RecElectronFilterPtMin", 0.)),
    cElFilterEtaMax(iConfig.getUntrackedParameter<double> ("RecElectronFilterEtaMax", 1000000.)),
    cTauPtMin(iConfig.getUntrackedParameter<double> ("RecTauPtMin", 0.)),
    cTauEtaMax(iConfig.getUntrackedParameter<double> ("RecTauEtaMax", 1000000.)),
    cTauHLTriggerMatching(iConfig.getUntrackedParameter<vector<string> > ("RecTauHLTriggerMatching")),
    cTauDiscriminators(iConfig.getUntrackedParameter<vector<string> > ("RecTauDiscriminators")),
    cTauNum(iConfig.getUntrackedParameter<int> ("RecTauNum", 0)),
    cTrackFilterPtMin(iConfig.getUntrackedParameter<double> ("RecTrackFilterPtMin", 0.)),
    cTrackPtMin(iConfig.getUntrackedParameter<double> ("RecTrackPtMin", 0.)),
    cTrackEtaMax(iConfig.getUntrackedParameter<double> ("RecTrackEtaMax", 1000000.)),
    cTrackNum(iConfig.getUntrackedParameter<int> ("RecTrackNum", 0)),
    cPhotonPtMin(iConfig.getUntrackedParameter<double> ("RecPhotonPtMin", 0.)),
    cPhotonEtaMax(iConfig.getUntrackedParameter<double> ("RecPhotonEtaMax", 1000000.)),
    cPhotonHLTriggerMatching(iConfig.getUntrackedParameter<vector<string> > ("RecPhotonHLTriggerMatching")),
    cPhotonNum(iConfig.getUntrackedParameter<int> ("RecPhotonNum", 0)),
    cPhotonFilterPtMin(iConfig.getUntrackedParameter<double> ("RecPhotonFilterPtMin", 0.)),
    cPhotonFilterEtaMax(iConfig.getUntrackedParameter<double> ("RecPhotonFilterEtaMax", 1000000.)),
    cAK4CaloFilterPtMin(iConfig.getUntrackedParameter<double> ("RecAK4CaloFilterPtMin", 0.)),
    cAK4CaloPtMin(iConfig.getUntrackedParameter<double> ("RecAK4CaloPtMin", 0.)),
    cAK4CaloEtaMax(iConfig.getUntrackedParameter<double> ("RecAK4CaloEtaMax", 1000000.)),
    cAK4CaloNum(iConfig.getUntrackedParameter<int> ("RecAK4CaloNum", 0)),
    cAK4JPTFilterPtMin(iConfig.getUntrackedParameter<double> ("RecAK4JPTFilterPtMin", 0.)),
    cAK4JPTPtMin(iConfig.getUntrackedParameter<double> ("RecAK4JPTPtMin", 0.)),
    cAK4JPTEtaMax(iConfig.getUntrackedParameter<double> ("RecAK4JPTEtaMax", 1000000.)),
    cAK4JPTNum(iConfig.getUntrackedParameter<int> ("RecAK4JPTNum", 0)),
    cAK4PFCHSFilterPtMin(iConfig.getUntrackedParameter<double> ("RecAK4PFCHSFilterPtMin", 0.)),
    cAK4PFCHSPtMin(iConfig.getUntrackedParameter<double> ("RecAK4PFCHSPtMin", 0.)),
    cAK4PFCHSEtaMax(iConfig.getUntrackedParameter<double> ("RecAK4PFCHSEtaMax", 1000000.)),
    cAK4PFCHSNum(iConfig.getUntrackedParameter<int> ("RecAK4PFCHSNum", 0)),
    cAK4PFCHSPuppiFilterPtMin(iConfig.getUntrackedParameter<double> ("RecAK4PFCHSPuppiFilterPtMin", 0.)),
    cAK4PFCHSPuppiPtMin(iConfig.getUntrackedParameter<double> ("RecAK4PFCHSPuppiPtMin", 0.)),
    cAK4PFCHSPuppiEtaMax(iConfig.getUntrackedParameter<double> ("RecAK4PFCHSPuppiEtaMax", 1000000.)),
    cAK4PFCHSPuppiNum(iConfig.getUntrackedParameter<int> ("RecAK4PFCHSPuppiNum", 0)),
    cAK4PFFilterPtMin(iConfig.getUntrackedParameter<double> ("RecAK4PFFilterPtMin", 0.)),
    cAK4PFPtMin(iConfig.getUntrackedParameter<double> ("RecAK4PFPtMin", 0.)),
    cAK4PFEtaMax(iConfig.getUntrackedParameter<double> ("RecAK4PFEtaMax", 1000000.)),
    cAK4PFNum(iConfig.getUntrackedParameter<int> ("RecAK4PFNum", 0)),
    cJetCorrection(iConfig.getUntrackedParameter<string> ("JetCorrection", "L1FastL2L3Residual")),
    cJetHLTriggerMatching(iConfig.getUntrackedParameter<vector<string> > ("RecJetHLTriggerMatching")),
    cMassMuMuMin(iConfig.getUntrackedParameter<double> ("RecMassMuMuMin", 0.)),
    cMassMuMuMax(iConfig.getUntrackedParameter<double> ("RecMassMuMuMax", 0.)),
    cVertexTRKChi2(iConfig.getUntrackedParameter<double> ("RecVertexTRKChi2", 10.)),
    cVertexTRKHitsMin(iConfig.getUntrackedParameter<int> ("RecVertexTRKHitsMin", 6)),
    cVertexChi2(iConfig.getUntrackedParameter<double> ("RecVertexChi2", 5.)),
    cVertexSig2D(iConfig.getUntrackedParameter<double> ("RecVertexSig2D", 15.)),
    cKaonMassWindow(iConfig.getUntrackedParameter<double> ("RecVertexKaonMassWin", 0.05)),
    cLambdaMassWindow(iConfig.getUntrackedParameter<double> ("RecVertexLambdaMassWin", 0.02)),
    propagatorWithMaterial(0) {
    testids.push_back(24);  //0
    testids.push_back(-24);  //1
    testids.push_back(22);  //2
    testids.push_back(23);  //3
    testids.push_back(25);  //4
    testids.push_back(35);  //5
    testids.push_back(36);  //6
    testids.push_back(37);  //7
    testids.push_back(-37);  //8
    testids.push_back(6);  //9
    testids.push_back(-6);  //10
    testids.push_back(5);  //11
    testids.push_back(-5);  //12
    testids.push_back(32);  //13
    testids.push_back(8);  //14
    testids.push_back(-8);  //15
    testids.push_back(15);  //16
    testids.push_back(-15);  //17
    bdisclabel.push_back("pfJetProbabilityBJetTags");
    bdisclabel.push_back("pfJetBProbabilityBJetTags");
    bdisclabel.push_back("pfCombinedInclusiveSecondaryVertexV2BJetTags");

    double barrelRadius = 129.; //p81, p50, ECAL TDR
    double endcapZ = 320.5; // fig 3.26, p81, ECAL TDR
    Surface::RotationType rot;

    ecalBarrel = Cylinder::build(Surface::PositionType(0, 0, 0), rot, barrelRadius);
    ecalNegativeEtaEndcap = Plane::build(Surface::PositionType(0, 0, -endcapZ), rot);
    ecalPositiveEtaEndcap = Plane::build(Surface::PositionType(0, 0, endcapZ), rot);
}

RootMaker::~RootMaker() {
    if(propagatorWithMaterial != 0) {
        delete propagatorWithMaterial;
    }
}

const PFCandidate &RootMaker::removeRef(const PFCandidatePtr &pfRef) {
    return *pfRef;
}

template<typename Collection, typename Function>
std::vector<double> RootMaker::extract(const Collection &cands, Function func) {
    if(cdebug) { cout<<"extract..."<<endl; }

    // #define CALL_MEMBER_FN(object,ptrToMember) ((object).*(ptrToMember))
    std::vector<double> output;
    output.reserve(cands.size());

    for(typename Collection::const_iterator cand = cands.begin(); cand != cands.end(); ++cand) {
        output.push_back(func(removeRef(*cand)));
    }

    return output;
}

void RootMaker::beginJob() {
    if(cdebug) { cout<<"begin job..."<<endl; }

    cout<<"isMiniAOD = "<<cisMiniAOD<<endl;
    cout<<"is monte carlo = "<<cisMC<<endl;
    cout<<"debug = "<<cdebug<<endl;
    edm::Service<TFileService> FS;
    tree = FS->make<TTree> ("AC1B", "AC1B", 1);
    drhist = FS->make<TH1D> ("drhist", "drhist", 10000, 0., 100.);

    tree->Branch("errors", &errors, "errors/i");
    tree->Branch("event_nr", &event_nr, "event_nr/D");
    tree->Branch("event_run", &event_run, "event_run/i");
    tree->Branch("event_timeunix", &event_timeunix, "event_timeunix/i");
    tree->Branch("event_timemicrosec", &event_timemicrosec, "event_timemicrosec/i");
    tree->Branch("event_luminosityblock", &event_luminosityblock, "event_luminosityblock/i");
    tree->Branch("trigger_level1bits", &trigger_level1bits, "trigger_level1bits[8]/b");
    tree->Branch("trigger_level1", &trigger_level1, "trigger_level1[128]/b");
    tree->Branch("trigger_HLT", &trigger_HLT, "trigger_HLT[128]/b");

    tree->Branch("beamspot_x", &beamspot_x, "beamspot_x/F");
    tree->Branch("beamspot_y", &beamspot_y, "beamspot_y/F");
    tree->Branch("beamspot_z", &beamspot_z, "beamspot_z/F");
    tree->Branch("beamspot_xwidth", &beamspot_xwidth, "beamspot_xwidth/F");
    tree->Branch("beamspot_ywidth", &beamspot_ywidth, "beamspot_ywidth/F");
    tree->Branch("beamspot_zsigma", &beamspot_zsigma, "beamspot_zsigma/F");
    tree->Branch("beamspot_cov", &beamspot_cov, "beamspot_cov[6]/F");

    tree->Branch("track_count", &track_count, "track_count/i");
    tree->Branch("track_vtx", track_vtx, "track_vtx[track_count]/I");
    tree->Branch("track_px", track_px, "track_px[track_count]/F");
    tree->Branch("track_py", track_py, "track_py[track_count]/F");
    tree->Branch("track_pz", track_pz, "track_pz[track_count]/F");
    tree->Branch("track_outerx", track_outerx, "track_outerx[track_count]/F");
    tree->Branch("track_outery", track_outery, "track_outery[track_count]/F");
    tree->Branch("track_outerz", track_outerz, "track_outerz[track_count]/F");
    tree->Branch("track_closestpointx", track_closestpointx, "track_closestpointx[track_count]/F");
    tree->Branch("track_closestpointy", track_closestpointy, "track_closestpointy[track_count]/F");
    tree->Branch("track_closestpointz", track_closestpointz, "track_closestpointz[track_count]/F");
    tree->Branch("track_chi2", track_chi2, "track_chi2[track_count]/F");
    tree->Branch("track_ndof", track_ndof, "track_ndof[track_count]/F");
    tree->Branch("track_dxy", track_dxy, "track_dxy[track_count]/F");
    tree->Branch("track_dxyerr", track_dxyerr, "track_dxyerr[track_count]/F");
    tree->Branch("track_dz", track_dz, "track_dz[track_count]/F");
    tree->Branch("track_dzerr", track_dzerr, "track_dzerr[track_count]/F");
    tree->Branch("track_dedxharmonic2", track_dedxharmonic2, "track_dedxharmonic2[track_count]/F");
    tree->Branch("track_charge", track_charge, "track_charge[track_count]/I");
    tree->Branch("track_nhits", track_nhits, "track_nhits[track_count]/b");
    tree->Branch("track_npixelhits", track_npixelhits, "track_npixelhits[track_count]/b");
    tree->Branch("track_nmissinghits", track_nmissinghits, "track_nmissinghits[track_count]/b");
    tree->Branch("track_npixellayers", track_npixellayers, "track_npixellayers[track_count]/b");
    tree->Branch("track_nstriplayers", track_nstriplayers, "track_nstriplayers[track_count]/b");

    tree->Branch("primvertex_count", &primvertex_count, "primvertex_count/i");
    tree->Branch("primvertex_x", primvertex_x, "primvertex_x[primvertex_count]/F");
    tree->Branch("primvertex_y", primvertex_y, "primvertex_y[primvertex_count]/F");
    tree->Branch("primvertex_z", primvertex_z, "primvertex_z[primvertex_count]/F");
    tree->Branch("primvertex_info", primvertex_info, "primvertex_info[primvertex_count]/i");
    tree->Branch("primvertex_chi2", primvertex_chi2, "primvertex_chi2[primvertex_count]/F");
    tree->Branch("primvertex_ndof", primvertex_ndof, "primvertex_ndof[primvertex_count]/F");
    tree->Branch("primvertex_ptq", primvertex_ptq, "primvertex_pdf[primvertex_count]/F");
    tree->Branch("primvertex_ntracks", primvertex_ntracks, "primvertex_ntracks[primvertex_count]/I");
    tree->Branch("primvertex_cov", primvertex_cov, "primvertex_cov[primvertex_count][6]/F");

    tree->Branch("supercluster_count", &supercluster_count, "supercluster_count/i");
    tree->Branch("supercluster_e", supercluster_e, "supercluster_e[supercluster_count]/F");
    tree->Branch("supercluster_x", supercluster_x, "supercluster_x[supercluster_count]/F");
    tree->Branch("supercluster_y", supercluster_y, "supercluster_y[supercluster_count]/F");
    tree->Branch("supercluster_z", supercluster_z, "supercluster_z[supercluster_count]/F");
    tree->Branch("supercluster_rawe", supercluster_rawe, "supercluster_rawe[supercluster_count]/F");
    tree->Branch("supercluster_phiwidth", supercluster_phiwidth, "supercluster_phiwidth[supercluster_count]/F");
    tree->Branch("supercluster_etawidth", supercluster_etawidth, "supercluster_etawidth[supercluster_count]/F");
    tree->Branch("supercluster_nbasiccluster", supercluster_nbasiccluster, "supercluster_nbasiccluster[supercluster_count]/I");
    tree->Branch("supercluster_basicclusterbegin", supercluster_basicclusterbegin, "supercluster_basicclusterbegin[supercluster_count]/I");
    tree->Branch("supercluster_esclusterbegin", supercluster_esclusterbegin, "supercluster_esclusterbegin[supercluster_count]/I");

    tree->Branch("supercluster_basiccluster_count", &supercluster_basiccluster_count, "supercluster_basiccluster_count/i");
    tree->Branch("supercluster_basiccluster_e", supercluster_basiccluster_e, "supercluster_basiccluster_e[supercluster_basiccluster_count]/F");
    tree->Branch("supercluster_basiccluster_x", supercluster_basiccluster_x, "supercluster_basiccluster_x[supercluster_basiccluster_count]/F");
    tree->Branch("supercluster_basiccluster_y", supercluster_basiccluster_y, "supercluster_basiccluster_y[supercluster_basiccluster_count]/F");
    tree->Branch("supercluster_basiccluster_z", supercluster_basiccluster_z, "supercluster_basiccluster_z[supercluster_basiccluster_count]/F");
    tree->Branch("supercluster_basiccluster_nhit", supercluster_basiccluster_nhit, "supercluster_basiccluster_nhit[supercluster_basiccluster_count]/I");
    tree->Branch("supercluster_basiccluster_hitbegin", supercluster_basiccluster_hitbegin, "supercluster_basiccluster_hitbegin[supercluster_basiccluster_count]/I");

    tree->Branch("supercluster_basiccluster_hit_count", &supercluster_basiccluster_hit_count, "supercluster_basiccluster_hit_count/i");
    tree->Branch("supercluster_basiccluster_hit_e", supercluster_basiccluster_hit_e, "supercluster_basiccluster_hit_e[supercluster_basiccluster_hit_count]/F");
    tree->Branch("supercluster_basiccluster_hit_x", supercluster_basiccluster_hit_x, "supercluster_basiccluster_hit_x[supercluster_basiccluster_hit_count]/F");
    tree->Branch("supercluster_basiccluster_hit_y", supercluster_basiccluster_hit_y, "supercluster_basiccluster_hit_y[supercluster_basiccluster_hit_count]/F");
    tree->Branch("supercluster_basiccluster_hit_z", supercluster_basiccluster_hit_z, "supercluster_basiccluster_hit_z[supercluster_basiccluster_hit_count]/F");

    tree->Branch("supercluster_escluster_count", &supercluster_escluster_count, "supercluster_escluster_count/i");
    tree->Branch("supercluster_escluster_e", supercluster_escluster_e, "supercluster_escluster_e[supercluster_escluster_count]/F");
    tree->Branch("supercluster_escluster_x", supercluster_escluster_x, "supercluster_escluster_x[supercluster_escluster_count]/F");
    tree->Branch("supercluster_escluster_y", supercluster_escluster_y, "supercluster_escluster_y[supercluster_escluster_count]/F");
    tree->Branch("supercluster_escluster_z", supercluster_escluster_z, "supercluster_escluster_z[supercluster_escluster_count]/F");
    tree->Branch("supercluster_escluster_nhit", supercluster_escluster_nhit, "supercluster_escluster_nhit[supercluster_escluster_count]/I");
    tree->Branch("supercluster_escluster_hitbegin", supercluster_escluster_hitbegin, "supercluster_escluster_hitbegin[supercluster_escluster_count]/I");

    tree->Branch("supercluster_escluster_hit_count", &supercluster_escluster_hit_count, "supercluster_escluster_hit_count/i");
    tree->Branch("supercluster_escluster_hit_e", supercluster_escluster_hit_e, "supercluster_escluster_hit_e[supercluster_escluster_hit_count]/F");
    tree->Branch("supercluster_escluster_hit_x", supercluster_escluster_hit_x, "supercluster_escluster_hit_x[supercluster_escluster_hit_count]/F");
    tree->Branch("supercluster_escluster_hit_y", supercluster_escluster_hit_y, "supercluster_escluster_hit_y[supercluster_escluster_hit_count]/F");
    tree->Branch("supercluster_escluster_hit_z", supercluster_escluster_hit_z, "supercluster_escluster_hit_z[supercluster_escluster_hit_count]/F");

    tree->Branch("muon_count", &muon_count, "muon_count/i");
    tree->Branch("muon_muID", &muon_muID, "muon_muID[muon_count]/I");
    tree->Branch("muon_px", muon_px, "muon_px[muon_count]/F");
    tree->Branch("muon_py", muon_py, "muon_py[muon_count]/F");
    tree->Branch("muon_pz", muon_pz, "muon_pz[muon_count]/F");
    tree->Branch("muon_pt", muon_pt, "muon_pt[muon_count]/F");
    tree->Branch("muon_phi", muon_phi, "muon_phi[muon_count]/F");
    tree->Branch("muon_eta", muon_eta, "muon_eta[muon_count]/F");
    tree->Branch("muon_pterror", muon_pterror, "muon_pterror[muon_count]/F");
    tree->Branch("muon_chi2", muon_chi2, "muon_chi2[muon_count]/F");
    tree->Branch("muon_ndof", muon_ndof, "muon_ndof[muon_count]/F");
    tree->Branch("muon_dB", muon_dB, "muon_dB[muon_count]/F");

    tree->Branch("muon_is_tracker", muon_is_tracker, "muon_is_tracker[muon_count]/I");
    tree->Branch("muon_is_global", muon_is_global, "muon_is_global[muon_count]/I");
    tree->Branch("muon_is_standalone", muon_is_standalone, "muon_is_standalone[muon_count]/I");

    tree->Branch("muon_has_gen_particle", muon_has_gen_particle, "muon_has_gen_particle[muon_count]/I");
    tree->Branch("muon_gen_particle_pdgid", muon_gen_particle_pdgid, "muon_gen_particle_pdgid[muon_count]/I");
    tree->Branch("muon_has_gen_mother", muon_has_gen_mother, "muon_has_gen_mother[muon_count]/I");
    tree->Branch("muon_gen_mother_pdgid", muon_gen_mother_pdgid, "muon_gen_mother_pdgid[muon_count]/I");

    tree->Branch("muon_innertrack_vtx", muon_innertrack_vtx, "muon_innertrack_vtx[muon_count]/I");
    tree->Branch("muon_innertrack_px", muon_innertrack_px, "muon_innertrack_px[muon_count]/F");
    tree->Branch("muon_innertrack_py", muon_innertrack_py, "muon_innertrack_py[muon_count]/F");
    tree->Branch("muon_innertrack_pz", muon_innertrack_pz, "muon_innertrack_pz[muon_count]/F");
    tree->Branch("muon_innertrack_outerx", muon_innertrack_outerx, "muon_innertrack_outerx[muon_count]/F");
    tree->Branch("muon_innertrack_outery", muon_innertrack_outery, "muon_innertrack_outery[muon_count]/F");
    tree->Branch("muon_innertrack_outerz", muon_innertrack_outerz, "muon_innertrack_outerz[muon_count]/F");
    tree->Branch("muon_innertrack_closestpointx", muon_innertrack_closestpointx, "muon_innertrack_closestpointx[muon_count]/F");
    tree->Branch("muon_innertrack_closestpointy", muon_innertrack_closestpointy, "muon_innertrack_closestpointy[muon_count]/F");
    tree->Branch("muon_innertrack_closestpointz", muon_innertrack_closestpointz, "muon_innertrack_closestpointz[muon_count]/F");
    tree->Branch("muon_innertrack_chi2", muon_innertrack_chi2, "muon_innertrack_chi2[muon_count]/F");
    tree->Branch("muon_innertrack_ndof", muon_innertrack_ndof, "muon_innertrack_ndof[muon_count]/F");
    tree->Branch("muon_innertrack_dxy", muon_innertrack_dxy, "muon_innertrack_dxy[muon_count]/F");
    tree->Branch("muon_innertrack_dxyerr", muon_innertrack_dxyerr, "muon_innertrack_dxyerr[muon_count]/F");
    tree->Branch("muon_innertrack_dz", muon_innertrack_dz, "muon_innertrack_dz[muon_count]/F");
    tree->Branch("muon_innertrack_dzerr", muon_innertrack_dzerr, "muon_innertrack_dzerr[muon_count]/F");
    tree->Branch("muon_innertrack_dedxharmonic2", muon_innertrack_dedxharmonic2, "muon_innertrack_dedxharmonic2[muon_count]/F");
    tree->Branch("muon_innertrack_charge", muon_innertrack_charge, "muon_innertrack_charge[muon_count]/I");
    tree->Branch("muon_innertrack_nhits", muon_innertrack_nhits, "muon_innertrack_nhits[muon_count]/b");
    tree->Branch("muon_innertrack_npixelhits", muon_innertrack_npixelhits, "muon_innertrack_npixelhits[muon_count]/b");
    tree->Branch("muon_innertrack_nmissinghits", muon_innertrack_nmissinghits, "muon_innertrack_nmissinghits[muon_count]/b");
    tree->Branch("muon_innertrack_npixellayers", muon_innertrack_npixellayers, "muon_innertrack_npixellayers[muon_count]/b");
    tree->Branch("muon_innertrack_nstriplayers", muon_innertrack_nstriplayers, "muon_innertrack_nstriplayers[muon_count]/b");
    tree->Branch("muon_outertrack_px", muon_outertrack_px, "muon_outertrack_px[muon_count]/F");
    tree->Branch("muon_outertrack_py", muon_outertrack_py, "muon_outertrack_py[muon_count]/F");
    tree->Branch("muon_outertrack_pz", muon_outertrack_pz, "muon_outertrack_pz[muon_count]/F");
    tree->Branch("muon_outertrack_hits", muon_outertrack_hits, "muon_outertrack_hits[muon_count]/b");
    tree->Branch("muon_outertrack_missinghits", muon_outertrack_missinghits, "muon_outertrack_missinghits[muon_count]/b");
    tree->Branch("muon_outertrack_chi2", muon_outertrack_chi2, "muon_outertrack_chi2[muon_count]/F");
    tree->Branch("muon_outertrack_ndof", muon_outertrack_ndof, "muon_outertrack_ndof[muon_count]/F");
    tree->Branch("muon_isolationr3track", muon_isolationr3track, "muon_isolationr3track[muon_count]/F");
    tree->Branch("muon_isolationr3ntrack", muon_isolationr3ntrack, "muon_isolationr3ntrack[muon_count]/I");
    tree->Branch("muon_isolationr3ecal", muon_isolationr3ecal, "muon_isolationr3ecal[muon_count]/F");
    tree->Branch("muon_isolationr3hcal", muon_isolationr3hcal, "muon_isolationr3hcal[muon_count]/F");
    tree->Branch("muon_pfisolationr3_sumchargedhadronpt", muon_pfisolationr3_sumchargedhadronpt, "muon_pfisolationr3_sumchargedhadronpt[muon_count]/F");
    tree->Branch("muon_pfisolationr3_sumchargedparticlept", muon_pfisolationr3_sumchargedparticlept, "muon_pfisolationr3_sumchargedparticlept[muon_count]/F");
    tree->Branch("muon_pfisolationr3_sumneutralhadronet", muon_pfisolationr3_sumneutralhadronet, "muon_pfisolationr3_sumneutralhadronet[muon_count]/F");
    tree->Branch("muon_pfisolationr3_sumphotonet", muon_pfisolationr3_sumphotonet, "muon_pfisolationr3_sumphotonet[muon_count]/F");
    tree->Branch("muon_pfisolationr3_sumneutralhadronethighthreshold", muon_pfisolationr3_sumneutralhadronethighthreshold, "muon_pfisolationr3_sumneutralhadronethighthreshold[muon_count]/F");
    tree->Branch("muon_pfisolationr3_sumphotonethighthreshold", muon_pfisolationr3_sumphotonethighthreshold, "muon_pfisolationr3_sumphotonethighthreshold[muon_count]/F");
    tree->Branch("muon_pfisolationr3_sumpupt", muon_pfisolationr3_sumpupt, "muon_pfisolationr3_sumpupt[muon_count]/F");
    tree->Branch("muon_pfisolationr4_sumchargedhadronpt", muon_pfisolationr4_sumchargedhadronpt, "muon_pfisolationr4_sumchargedhadronpt[muon_count]/F");
    tree->Branch("muon_pfisolationr4_sumchargedparticlept", muon_pfisolationr4_sumchargedparticlept, "muon_pfisolationr4_sumchargedparticlept[muon_count]/F");
    tree->Branch("muon_pfisolationr4_sumneutralhadronet", muon_pfisolationr4_sumneutralhadronet, "muon_pfisolationr4_sumneutralhadronet[muon_count]/F");
    tree->Branch("muon_pfisolationr4_sumphotonet", muon_pfisolationr4_sumphotonet, "muon_pfisolationr4_sumphotonet[muon_count]/F");
    tree->Branch("muon_pfisolationr4_sumneutralhadronethighthreshold", muon_pfisolationr4_sumneutralhadronethighthreshold, "muon_pfisolationr4_sumneutralhadronethighthreshold[muon_count]/F");
    tree->Branch("muon_pfisolationr4_sumphotonethighthreshold", muon_pfisolationr4_sumphotonethighthreshold, "muon_pfisolationr4_sumphotonethighthreshold[muon_count]/F");
    tree->Branch("muon_pfisolationr4_sumpupt", muon_pfisolationr4_sumpupt, "muon_pfisolationr4_sumpupt[muon_count]/F");
    tree->Branch("muon_ecalenergy", muon_ecalenergy, "muon_ecalenergy[muon_count]/F");
    tree->Branch("muon_hcalenergy", muon_hcalenergy, "muon_hcalenergy[muon_count]/F");
    tree->Branch("muon_charge", muon_charge, "muon_charge[muon_count]/I");
    tree->Branch("muon_numchambers", muon_numchambers, "muon_numchambers[muon_count]/I");
    tree->Branch("muon_numchamberswithsegments", muon_numchamberswithsegments, "muon_numchamberswithsegments[muon_count]/I");
    tree->Branch("muon_numvalidmuonhits", muon_numvalidmuonhits, "muon_numvalidmuonhits[muon_count]/I");
    tree->Branch("muon_nummatchedstations", muon_nummatchedstations, "muon_nummatchedstations[muon_count]/I");
    tree->Branch("muon_type", muon_type, "muon_type[muon_count]/b");
    tree->Branch("muon_trigger", muon_trigger, "muon_trigger[muon_count]/i");
    tree->Branch("muon_trackermuonquality", muon_trackermuonquality, "muon_trackermuonquality[muon_count]/i");

    tree->Branch("ak4calojet_count", &ak4calojet_count, "ak4calojet_count/i");
    tree->Branch("ak4calojet_e", ak4calojet_e, "ak4calojet_e[ak4calojet_count]/F");
    tree->Branch("ak4calojet_px", ak4calojet_px, "ak4calojet_px[ak4calojet_count]/F");
    tree->Branch("ak4calojet_py", ak4calojet_py, "ak4calojet_py[ak4calojet_count]/F");
    tree->Branch("ak4calojet_pz", ak4calojet_pz, "ak4calojet_pz[ak4calojet_count]/F");
    tree->Branch("ak4calojet_hadronicenergy", ak4calojet_hadronicenergy, "ak4calojet_hadronicenergy[ak4calojet_count]/F");
    tree->Branch("ak4calojet_emenergy", ak4calojet_emenergy, "ak4calojet_emenergy[ak4calojet_count]/F");
    tree->Branch("ak4calojet_energycorr", ak4calojet_energycorr, "ak4calojet_energycorr[ak4calojet_count]/F");
    tree->Branch("ak4calojet_energycorrl7uds", ak4calojet_energycorrl7uds, "ak4calojet_energycorrl7uds[ak4calojet_count]/F");
    tree->Branch("ak4calojet_energycorrl7bottom", ak4calojet_energycorrl7bottom, "ak4calojet_energycorrl7bottom[ak4calojet_count]/F");
    tree->Branch("ak4calojet_fhpd", ak4calojet_fhpd, "ak4calojet_fhpd[ak4calojet_count]/F");
    tree->Branch("ak4calojet_restrictedemf", ak4calojet_restrictedemf, "ak4calojet_restrictedemf[ak4calojet_count]/F");
    tree->Branch("ak4calojet_btag", ak4calojet_btag, "ak4calojet_btag[ak4calojet_count][4]/F");
    tree->Branch("ak4calojet_n90", ak4calojet_n90, "ak4calojet_n90[ak4calojet_count]/i");
    tree->Branch("ak4calojet_n60", ak4calojet_n60, "ak4calojet_n60[ak4calojet_count]/i");

    tree->Branch("ak4jptjet_count", &ak4jptjet_count, "ak4jptjet_count/i");
    tree->Branch("ak4jptjet_e", ak4jptjet_e, "ak4jptjet_e[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_px", ak4jptjet_px, "ak4jptjet_px[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_py", ak4jptjet_py, "ak4jptjet_py[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_pz", ak4jptjet_pz, "ak4jptjet_pz[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_hadronicenergy", ak4jptjet_hadronicenergy, "ak4jptjet_hadronicenergy[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_chargedhadronicenergy", ak4jptjet_chargedhadronicenergy, "ak4jptjet_chargedhadronicenergy[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_emenergy", ak4jptjet_emenergy, "ak4jptjet_emenergy[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_chargedemenergy", ak4jptjet_chargedemenergy, "ak4jptjet_chargedemenergy[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_chargedmulti", ak4jptjet_chargedmulti, "ak4jptjet_chargedmulti[ak4jptjet_count]/i");
    tree->Branch("ak4jptjet_energycorr", ak4jptjet_energycorr, "ak4jptjet_energycorr[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_energycorrl7uds", ak4jptjet_energycorrl7uds, "ak4jptjet_energycorrl7uds[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_energycorrl7bottom", ak4jptjet_energycorrl7bottom, "ak4jptjet_energycorrl7bottom[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_fhpd", ak4jptjet_fhpd, "ak4jptjet_fhpd[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_restrictedemf", ak4jptjet_restrictedemf, "ak4jptjet_restrictedemf[ak4jptjet_count]/F");
    tree->Branch("ak4jptjet_btag", ak4jptjet_btag, "ak4jptjet_btag[ak4jptjet_count][4]/F");
    tree->Branch("ak4jptjet_n90", ak4jptjet_n90, "ak4jptjet_n90[ak4jptjet_count]/i");

    tree->Branch("ak4pfjet_count", &ak4pfjet_count, "ak4pfjet_count/i");
    tree->Branch("ak4pfjet_e", ak4pfjet_e, "ak4pfjet_e[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_px", ak4pfjet_px, "ak4pfjet_px[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_py", ak4pfjet_py, "ak4pfjet_py[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_pz", ak4pfjet_pz, "ak4pfjet_pz[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_area", ak4pfjet_area, "ak4pfjet_area[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_hadronicenergy", ak4pfjet_hadronicenergy, "ak4pfjet_hadronicenergy[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_chargedhadronicenergy", ak4pfjet_chargedhadronicenergy, "ak4pfjet_chargedhadronicenergy[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_emenergy", ak4pfjet_emenergy, "ak4pfjet_emenergy[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_chargedemenergy", ak4pfjet_chargedemenergy, "ak4pfjet_chargedemenergy[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_hfhadronicenergy", ak4pfjet_hfhadronicenergy, "ak4pfjet_hfhadronicenergy[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_hfemenergy", ak4pfjet_hfemenergy, "ak4pfjet_hfemenergy[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_electronenergy", ak4pfjet_electronenergy, "ak4pfjet_electronenergy[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_muonenergy", ak4pfjet_muonenergy, "ak4pfjet_muonenergy[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_chargedmulti", ak4pfjet_chargedmulti, "ak4pfjet_chargedmulti[ak4pfjet_count]/i");
    tree->Branch("ak4pfjet_neutralmulti", ak4pfjet_neutralmulti, "ak4pfjet_neutralmulti[ak4pfjet_count]/i");
    tree->Branch("ak4pfjet_hfhadronicmulti", ak4pfjet_hfhadronicmulti, "ak4pfjet_hfhadronicmulti[ak4pfjet_count]/i");
    tree->Branch("ak4pfjet_hfemmulti", ak4pfjet_hfemmulti, "ak4pfjet_hfemmulti[ak4pfjet_count]/i");
    tree->Branch("ak4pfjet_electronmulti", ak4pfjet_electronmulti, "ak4pfjet_electronmulti[ak4pfjet_count]/i");
    tree->Branch("ak4pfjet_muonmulti", ak4pfjet_muonmulti, "ak4pfjet_muonmulti[ak4pfjet_count]/i");
    tree->Branch("ak4pfjet_chargeda", ak4pfjet_chargeda, "ak4pfjet_chargeda[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_chargedb", ak4pfjet_chargedb, "ak4pfjet_chargedb[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_neutrala", ak4pfjet_neutrala, "ak4pfjet_neutrala[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_neutralb", ak4pfjet_neutralb, "ak4pfjet_neutralb[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_alla", ak4pfjet_alla, "ak4pfjet_alla[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_allb", ak4pfjet_allb, "ak4pfjet_allb[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_chargedfractionmv", ak4pfjet_chargedfractionmv, "ak4pfjet_chargedfractionmv[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_energycorr", ak4pfjet_energycorr, "ak4pfjet_energycorr[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_energycorrunc", ak4pfjet_energycorrunc, "ak4pfjet_energycorrunc[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_energycorrl7uds", ak4pfjet_energycorrl7uds, "ak4pfjet_energycorrl7uds[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_energycorrl7bottom", ak4pfjet_energycorrl7bottom, "ak4pfjet_energycorrl7bottom[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_puidfull", ak4pfjet_puidfull, "ak4pfjet_puidfull[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_puidsimple", ak4pfjet_puidsimple, "ak4pfjet_puidsimple[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_puidcutbased", ak4pfjet_puidcutbased, "ak4pfjet_puidcutbased[ak4pfjet_count]/F");
    tree->Branch("ak4pfjet_btag", ak4pfjet_btag, "ak4pfjet_btag[ak4pfjet_count][6]/F");
    tree->Branch("ak4pfjet_trigger", ak4pfjet_trigger, "ak4pfjet_trigger[ak4pfjet_count]/i");
    tree->Branch("ak4pfjet_mcflavour", ak4pfjet_mcflavour, "ak4pfjet_mcflavour[ak4pfjet_count]/I");

    tree->Branch("ak4pfchsjet_count", &ak4pfchsjet_count, "ak4pfchsjet_count/i");
    tree->Branch("ak4pfchsjet_e", ak4pfchsjet_e, "ak4pfchsjet_e[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_px", ak4pfchsjet_px, "ak4pfchsjet_px[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_py", ak4pfchsjet_py, "ak4pfchsjet_py[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_pz", ak4pfchsjet_pz, "ak4pfchsjet_pz[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_pt", ak4pfchsjet_pt, "ak4pfchsjet_pt[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_phi", ak4pfchsjet_phi, "ak4pfchsjet_phi[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_eta", ak4pfchsjet_eta, "ak4pfchsjet_eta[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_area", ak4pfchsjet_area, "ak4pfchsjet_area[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_hadronicenergy", ak4pfchsjet_hadronicenergy, "ak4pfchsjet_hadronicenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargedhadronicenergy", ak4pfchsjet_chargedhadronicenergy, "ak4pfchsjet_chargedhadronicenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_emenergy", ak4pfchsjet_emenergy, "ak4pfchsjet_emenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargedemenergy", ak4pfchsjet_chargedemenergy, "ak4pfchsjet_chargedemenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_hfhadronicenergy", ak4pfchsjet_hfhadronicenergy, "ak4pfchsjet_hfhadronicenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_hfemenergy", ak4pfchsjet_hfemenergy, "ak4pfchsjet_hfemenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_electronenergy", ak4pfchsjet_electronenergy, "ak4pfchsjet_electronenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_muonenergy", ak4pfchsjet_muonenergy, "ak4pfchsjet_muonenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargedmulti", ak4pfchsjet_chargedmulti, "ak4pfchsjet_chargedmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_neutralmulti", ak4pfchsjet_neutralmulti, "ak4pfchsjet_neutralmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_hfhadronicmulti", ak4pfchsjet_hfhadronicmulti, "ak4pfchsjet_hfhadronicmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_hfemmulti", ak4pfchsjet_hfemmulti, "ak4pfchsjet_hfemmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_electronmulti", ak4pfchsjet_electronmulti, "ak4pfchsjet_electronmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_muonmulti", ak4pfchsjet_muonmulti, "ak4pfchsjet_muonmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_energycorr", ak4pfchsjet_energycorr, "ak4pfchsjet_energycorr[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargeda", ak4pfchsjet_chargeda, "ak4pfchsjet_chargeda[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargedb", ak4pfchsjet_chargedb, "ak4pfchsjet_chargedb[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_neutrala", ak4pfchsjet_neutrala, "ak4pfchsjet_neutrala[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_neutralb", ak4pfchsjet_neutralb, "ak4pfchsjet_neutralb[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_alla", ak4pfchsjet_alla, "ak4pfchsjet_alla[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_allb", ak4pfchsjet_allb, "ak4pfchsjet_allb[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargedfractionmv", ak4pfchsjet_chargedfractionmv, "ak4pfchsjet_chargedfractionmv[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_energycorrunc", ak4pfchsjet_energycorrunc, "ak4pfchsjet_energycorrunc[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_energycorrl7uds", ak4pfchsjet_energycorrl7uds, "ak4pfchsjet_energycorrl7uds[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_energycorrl7bottom", ak4pfchsjet_energycorrl7bottom, "ak4pfchsjet_energycorrl7bottom[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_btag", ak4pfchsjet_btag, "ak4pfchsjet_btag[ak4pfchsjet_count][6]/F");
    tree->Branch("ak4pfchsjet_trigger", ak4pfchsjet_trigger, "ak4pfchsjet_trigger[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_mcflavour", ak4pfchsjet_mcflavour, "ak4pfchsjet_mcflavour[ak4pfchsjet_count]/I");
    tree->Branch("ak4pfchsjet_count", &ak4pfchsjet_count, "ak4pfchsjet_count/i");
    tree->Branch("ak4pfchsjet_e", ak4pfchsjet_e, "ak4pfchsjet_e[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_px", ak4pfchsjet_px, "ak4pfchsjet_px[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_py", ak4pfchsjet_py, "ak4pfchsjet_py[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_pz", ak4pfchsjet_pz, "ak4pfchsjet_pz[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_pt", ak4pfchsjet_pt, "ak4pfchsjet_pt[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_phi", ak4pfchsjet_phi, "ak4pfchsjet_phi[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_eta", ak4pfchsjet_eta, "ak4pfchsjet_eta[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_area", ak4pfchsjet_area, "ak4pfchsjet_area[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_hadronicenergy", ak4pfchsjet_hadronicenergy, "ak4pfchsjet_hadronicenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargedhadronicenergy", ak4pfchsjet_chargedhadronicenergy, "ak4pfchsjet_chargedhadronicenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_emenergy", ak4pfchsjet_emenergy, "ak4pfchsjet_emenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargedemenergy", ak4pfchsjet_chargedemenergy, "ak4pfchsjet_chargedemenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_hfhadronicenergy", ak4pfchsjet_hfhadronicenergy, "ak4pfchsjet_hfhadronicenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_hfemenergy", ak4pfchsjet_hfemenergy, "ak4pfchsjet_hfemenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_electronenergy", ak4pfchsjet_electronenergy, "ak4pfchsjet_electronenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_muonenergy", ak4pfchsjet_muonenergy, "ak4pfchsjet_muonenergy[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargedmulti", ak4pfchsjet_chargedmulti, "ak4pfchsjet_chargedmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_neutralmulti", ak4pfchsjet_neutralmulti, "ak4pfchsjet_neutralmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_hfhadronicmulti", ak4pfchsjet_hfhadronicmulti, "ak4pfchsjet_hfhadronicmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_hfemmulti", ak4pfchsjet_hfemmulti, "ak4pfchsjet_hfemmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_electronmulti", ak4pfchsjet_electronmulti, "ak4pfchsjet_electronmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_muonmulti", ak4pfchsjet_muonmulti, "ak4pfchsjet_muonmulti[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_energycorr", ak4pfchsjet_energycorr, "ak4pfchsjet_energycorr[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargeda", ak4pfchsjet_chargeda, "ak4pfchsjet_chargeda[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargedb", ak4pfchsjet_chargedb, "ak4pfchsjet_chargedb[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_neutrala", ak4pfchsjet_neutrala, "ak4pfchsjet_neutrala[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_neutralb", ak4pfchsjet_neutralb, "ak4pfchsjet_neutralb[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_alla", ak4pfchsjet_alla, "ak4pfchsjet_alla[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_allb", ak4pfchsjet_allb, "ak4pfchsjet_allb[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_chargedfractionmv", ak4pfchsjet_chargedfractionmv, "ak4pfchsjet_chargedfractionmv[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_energycorrunc", ak4pfchsjet_energycorrunc, "ak4pfchsjet_energycorrunc[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_energycorrl7uds", ak4pfchsjet_energycorrl7uds, "ak4pfchsjet_energycorrl7uds[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_energycorrl7bottom", ak4pfchsjet_energycorrl7bottom, "ak4pfchsjet_energycorrl7bottom[ak4pfchsjet_count]/F");
    tree->Branch("ak4pfchsjet_btag", ak4pfchsjet_btag, "ak4pfchsjet_btag[ak4pfchsjet_count][6]/F");
    tree->Branch("ak4pfchsjet_trigger", ak4pfchsjet_trigger, "ak4pfchsjet_trigger[ak4pfchsjet_count]/i");
    tree->Branch("ak4pfchsjet_mcflavour", ak4pfchsjet_mcflavour, "ak4pfchsjet_mcflavour[ak4pfchsjet_count]/I");

    tree->Branch("ak4pfchspuppijet_count", &ak4pfchspuppijet_count, "ak4pfchspuppijet_count/i");
    tree->Branch("ak4pfchspuppijet_e", ak4pfchspuppijet_e, "ak4pfchspuppijet_e[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_px", ak4pfchspuppijet_px, "ak4pfchspuppijet_px[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_py", ak4pfchspuppijet_py, "ak4pfchspuppijet_py[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_pz", ak4pfchspuppijet_pz, "ak4pfchspuppijet_pz[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_pt", ak4pfchspuppijet_pt, "ak4pfchspuppijet_pt[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_phi", ak4pfchspuppijet_phi, "ak4pfchspuppijet_phi[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_eta", ak4pfchspuppijet_eta, "ak4pfchspuppijet_eta[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_area", ak4pfchspuppijet_area, "ak4pfchspuppijet_area[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_hadronicenergy", ak4pfchspuppijet_hadronicenergy, "ak4pfchspuppijet_hadronicenergy[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_chargedhadronicenergy", ak4pfchspuppijet_chargedhadronicenergy, "ak4pfchspuppijet_chargedhadronicenergy[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_emenergy", ak4pfchspuppijet_emenergy, "ak4pfchspuppijet_emenergy[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_chargedemenergy", ak4pfchspuppijet_chargedemenergy, "ak4pfchspuppijet_chargedemenergy[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_hfhadronicenergy", ak4pfchspuppijet_hfhadronicenergy, "ak4pfchspuppijet_hfhadronicenergy[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_hfemenergy", ak4pfchspuppijet_hfemenergy, "ak4pfchspuppijet_hfemenergy[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_electronenergy", ak4pfchspuppijet_electronenergy, "ak4pfchspuppijet_electronenergy[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_muonenergy", ak4pfchspuppijet_muonenergy, "ak4pfchspuppijet_muonenergy[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_chargedmulti", ak4pfchspuppijet_chargedmulti, "ak4pfchspuppijet_chargedmulti[ak4pfchspuppijet_count]/i");
    tree->Branch("ak4pfchspuppijet_neutralmulti", ak4pfchspuppijet_neutralmulti, "ak4pfchspuppijet_neutralmulti[ak4pfchspuppijet_count]/i");
    tree->Branch("ak4pfchspuppijet_hfhadronicmulti", ak4pfchspuppijet_hfhadronicmulti, "ak4pfchspuppijet_hfhadronicmulti[ak4pfchspuppijet_count]/i");
    tree->Branch("ak4pfchspuppijet_hfemmulti", ak4pfchspuppijet_hfemmulti, "ak4pfchspuppijet_hfemmulti[ak4pfchspuppijet_count]/i");
    tree->Branch("ak4pfchspuppijet_electronmulti", ak4pfchspuppijet_electronmulti, "ak4pfchspuppijet_electronmulti[ak4pfchspuppijet_count]/i");
    tree->Branch("ak4pfchspuppijet_muonmulti", ak4pfchspuppijet_muonmulti, "ak4pfchspuppijet_muonmulti[ak4pfchspuppijet_count]/i");
    tree->Branch("ak4pfchspuppijet_energycorr", ak4pfchspuppijet_energycorr, "ak4pfchspuppijet_energycorr[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_chargeda", ak4pfchspuppijet_chargeda, "ak4pfchspuppijet_chargeda[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_chargedb", ak4pfchspuppijet_chargedb, "ak4pfchspuppijet_chargedb[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_neutrala", ak4pfchspuppijet_neutrala, "ak4pfchspuppijet_neutrala[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_neutralb", ak4pfchspuppijet_neutralb, "ak4pfchspuppijet_neutralb[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_alla", ak4pfchspuppijet_alla, "ak4pfchspuppijet_alla[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_allb", ak4pfchspuppijet_allb, "ak4pfchspuppijet_allb[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_chargedfractionmv", ak4pfchspuppijet_chargedfractionmv, "ak4pfchspuppijet_chargedfractionmv[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_energycorrunc", ak4pfchspuppijet_energycorrunc, "ak4pfchspuppijet_energycorrunc[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_energycorrl7uds", ak4pfchspuppijet_energycorrl7uds, "ak4pfchspuppijet_energycorrl7uds[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_energycorrl7bottom", ak4pfchspuppijet_energycorrl7bottom, "ak4pfchspuppijet_energycorrl7bottom[ak4pfchspuppijet_count]/F");
    tree->Branch("ak4pfchspuppijet_btag", ak4pfchspuppijet_btag, "ak4pfchspuppijet_btag[ak4pfchspuppijet_count][6]/F");
    tree->Branch("ak4pfchspuppijet_trigger", ak4pfchspuppijet_trigger, "ak4pfchspuppijet_trigger[ak4pfchspuppijet_count]/i");
    tree->Branch("ak4pfchspuppijet_mcflavour", ak4pfchspuppijet_mcflavour, "ak4pfchspuppijet_mcflavour[ak4pfchspuppijet_count]/I");

    tree->Branch("electron_count", &electron_count, "electron_count/i");
    tree->Branch("electron_cbID", &electron_cbID, "electron_cbID[electron_count]/I");
    tree->Branch("electron_heepID", &electron_heepID, "electron_heepID[electron_count]/I");
    tree->Branch("electron_mvaID", &electron_mvaID, "electron_mvaID[electron_count]/I");
    tree->Branch("electron_vtx", electron_vtx, "electron_vtx[electron_count]/I");
    tree->Branch("electron_px", electron_px, "electron_px[electron_count]/F");
    tree->Branch("electron_py", electron_py, "electron_py[electron_count]/F");
    tree->Branch("electron_pz", electron_pz, "electron_pz[electron_count]/F");
    tree->Branch("electron_pt", electron_pt, "electron_pt[electron_count]/F");
    tree->Branch("electron_phi", electron_phi, "electron_phi[electron_count]/F");
    tree->Branch("electron_eta", electron_eta, "electron_eta[electron_count]/F");
    tree->Branch("electron_correctedecalenergy", electron_correctedecalenergy, "electron_correctedecalenergy[electron_count]/F");
    tree->Branch("electron_trackchi2", electron_trackchi2, "electron_trackchi2[electron_count]/F");
    tree->Branch("electron_trackndof", electron_trackndof, "electron_trackndof[electron_count]/F");

    tree->Branch("electron_has_gen_particle", electron_has_gen_particle, "electron_has_gen_particle[electron_count]/I");
    tree->Branch("electron_gen_particle_pdgid", electron_gen_particle_pdgid, "electron_gen_particle_pdgid[electron_count]/I");
    tree->Branch("electron_has_gen_mother", electron_has_gen_mother, "electron_has_gen_mother[electron_count]/I");
    tree->Branch("electron_gen_mother_pdgid", electron_gen_mother_pdgid, "electron_gen_mother_pdgid[electron_count]/I");


    tree->Branch("electron_outerx", electron_outerx, "electron_outerx[electron_count]/F");
    tree->Branch("electron_outery", electron_outery, "electron_outery[electron_count]/F");
    tree->Branch("electron_outerz", electron_outerz, "electron_outerz[electron_count]/F");
    tree->Branch("electron_closestpointx", electron_closestpointx, "electron_closestpointx[electron_count]/F");
    tree->Branch("electron_closestpointy", electron_closestpointy, "electron_closestpointy[electron_count]/F");
    tree->Branch("electron_closestpointz", electron_closestpointz, "electron_closestpointz[electron_count]/F");
    tree->Branch("electron_esuperclusterovertrack", electron_esuperclusterovertrack, "electron_esuperclusterovertrack[electron_count]/F");
    tree->Branch("electron_eseedclusterovertrack", electron_eseedclusterovertrack, "electron_eseedclusterovertrack[electron_count]/F");
    tree->Branch("electron_deltaetasuperclustertrack", electron_deltaetasuperclustertrack, "electron_deltaetasuperclustertrack[electron_count]/F");
    tree->Branch("electron_deltaphisuperclustertrack", electron_deltaphisuperclustertrack, "electron_deltaphisuperclustertrack[electron_count]/F");
    tree->Branch("electron_e1x5", electron_e1x5, "electron_e1x5[electron_count]/F");
    tree->Branch("electron_e2x5", electron_e2x5, "electron_e2x5[electron_count]/F");
    tree->Branch("electron_e5x5", electron_e5x5, "electron_e5x5[electron_count]/F");
    tree->Branch("electron_r9", electron_r9, "electron_r9[electron_count]/F");
    tree->Branch("electron_sigmaetaeta", electron_sigmaetaeta, "electron_sigmaetaeta[electron_count]/F");
    tree->Branch("electron_sigmaietaieta", electron_sigmaietaieta, "electron_sigmaietaieta[electron_count]/F");
    tree->Branch("electron_sigmaiphiiphi", electron_sigmaiphiiphi, "electron_sigmaiphiiphi[electron_count]/F");
    tree->Branch("electron_ehcaloverecaldepth1", electron_ehcaloverecaldepth1, "electron_ehcaloverecaldepth1[electron_count]/F");
    tree->Branch("electron_ehcaloverecaldepth2", electron_ehcaloverecaldepth2, "electron_ehcaloverecaldepth2[electron_count]/F");
    tree->Branch("electron_ehcaltoweroverecaldepth1", electron_ehcaltoweroverecaldepth1, "electron_ehcaltoweroverecaldepth1[electron_count]/F");
    tree->Branch("electron_ehcaltoweroverecaldepth2", electron_ehcaltoweroverecaldepth2, "electron_ehcaltoweroverecaldepth2[electron_count]/F");
    tree->Branch("electron_isolationr3track", electron_isolationr3track, "electron_isolationr3track[electron_count]/F");
    tree->Branch("electron_isolationr3ecal", electron_isolationr3ecal, "electron_isolationr3ecal[electron_count]/F");
    tree->Branch("electron_isolationr3hcal", electron_isolationr3hcal, "electron_isolationr3hcal[electron_count]/F");
    tree->Branch("electron_isolationr4track", electron_isolationr4track, "electron_isolationr4track[electron_count]/F");
    tree->Branch("electron_isolationr4ecal", electron_isolationr4ecal, "electron_isolationr4ecal[electron_count]/F");
    tree->Branch("electron_isolationr4hcal", electron_isolationr4hcal, "electron_isolationr4hcal[electron_count]/F");
    tree->Branch("electron_isolationpfr3charged", electron_isolationpfr3charged, "electron_isolationpfr3charged[electron_count]/F");
    tree->Branch("electron_isolationpfr3photon", electron_isolationpfr3photon, "electron_isolationpfr3photon[electron_count]/F");
    tree->Branch("electron_isolationpfr3neutral", electron_isolationpfr3neutral, "electron_isolationpfr3neutral[electron_count]/F");
    tree->Branch("electron_nhits", electron_nhits, "electron_nhits[electron_count]/b");
    tree->Branch("electron_npixelhits", electron_npixelhits, "electron_npixelhits[electron_count]/b");
    tree->Branch("electron_nmissinghits", electron_nmissinghits, "electron_nmissinghits[electron_count]/b");
    tree->Branch("electron_npixellayers", electron_npixellayers, "electron_npixellayers[electron_count]/b");
    tree->Branch("electron_nstriplayers", electron_nstriplayers, "electron_nstriplayers[electron_count]/b");
    tree->Branch("electron_nhitsexpected", electron_nhitsexpected, "electron_nhitsexpected[electron_count]/b");
    tree->Branch("electron_dxy", electron_dxy, "electron_dxy[electron_count]/F");
    tree->Branch("electron_dxyerr", electron_dxyerr, "electron_dxyerr[electron_count]/F");
    tree->Branch("electron_dz", electron_dz, "electron_dz[electron_count]/F");
    tree->Branch("electron_dzerr", electron_dzerr, "electron_dzerr[electron_count]/F");
    tree->Branch("electron_convdist", electron_convdist, "electron_convdist[electron_count]/F");
    tree->Branch("electron_convdcot", electron_convdcot, "electron_convdcot[electron_count]/F");
    tree->Branch("electron_convradius", electron_convradius, "electron_convradius[electron_count]/F");
    tree->Branch("electron_gapinfo", electron_gapinfo, "electron_gapinfo[electron_count]/i");
    tree->Branch("electron_fbrems", electron_fbrems, "electron_fbrems[electron_count]/F");
    tree->Branch("electron_numbrems", electron_numbrems, "electron_numbrems[electron_count]/I");
    tree->Branch("electron_charge", electron_charge, "electron_charge[electron_count]/I");
    tree->Branch("electron_info", electron_info, "electron_info[electron_count]/b");
    tree->Branch("electron_trigger", electron_trigger, "electron_trigger[electron_count]/i");
    tree->Branch("electron_eID", electron_eID, "electron_eID[electron_count]/b");
    tree->Branch("electron_supercluster_e", electron_supercluster_e, "electron_supercluster_e[electron_count]/F");
    tree->Branch("electron_supercluster_x", electron_supercluster_x, "electron_supercluster_x[electron_count]/F");
    tree->Branch("electron_supercluster_y", electron_supercluster_y, "electron_supercluster_y[electron_count]/F");
    tree->Branch("electron_supercluster_z", electron_supercluster_z, "electron_supercluster_z[electron_count]/F");
    tree->Branch("electron_supercluster_rawe", electron_supercluster_rawe, "electron_supercluster_rawe[electron_count]/F");
    tree->Branch("electron_supercluster_phiwidth", electron_supercluster_phiwidth, "electron_supercluster_phiwidth[electron_count]/F");
    tree->Branch("electron_supercluster_etawidth", electron_supercluster_etawidth, "electron_supercluster_etawidth[electron_count]/F");
    tree->Branch("electron_supercluster_nbasiccluster", electron_supercluster_nbasiccluster, "electron_supercluster_nbasiccluster[electron_count]/I");

    tree->Branch("photon_count", &photon_count, "photon_count/i");
    tree->Branch("photon_px", photon_px, "photon_px[photon_count]/F");
    tree->Branch("photon_py", photon_py, "photon_py[photon_count]/F");
    tree->Branch("photon_pz", photon_pz, "photon_pz[photon_count]/F");
    tree->Branch("photon_pt", photon_pt, "photon_pt[photon_count]/F");
    tree->Branch("photon_phi", photon_phi, "photon_phi[photon_count]/F");
    tree->Branch("photon_eta", photon_eta, "photon_eta[photon_count]/F");
    tree->Branch("photon_e1x5", photon_e1x5, "photon_e1x5[photon_count]/F");
    tree->Branch("photon_e2x5", photon_e2x5, "photon_e2x5[photon_count]/F");
    tree->Branch("photon_e3x3", photon_e3x3, "photon_e3x3[photon_count]/F");
    tree->Branch("photon_e5x5", photon_e5x5, "photon_e5x5[photon_count]/F");
    tree->Branch("photon_sigmaietaieta", photon_sigmaietaieta, "photon_sigmaietaieta[photon_count]/F");
    tree->Branch("photon_sigmaiphiiphi", photon_sigmaiphiiphi, "photon_sigmaiphiiphi[photon_count]/F");
    tree->Branch("photon_sigmaietaiphi", photon_sigmaietaiphi, "photon_sigmaietaiphi[photon_count]/F");
    tree->Branch("photon_ehcaloverecaldepth1", photon_ehcaloverecaldepth1, "photon_ehcaloverecaldepth1[photon_count]/F");
    tree->Branch("photon_ehcaloverecaldepth2", photon_ehcaloverecaldepth2, "photon_ehcaloverecaldepth2[photon_count]/F");
    tree->Branch("photon_ehcaltoweroverecaldepth1", photon_ehcaltoweroverecaldepth1, "photon_ehcaltoweroverecaldepth1[photon_count]/F");
    tree->Branch("photon_ehcaltoweroverecaldepth2", photon_ehcaltoweroverecaldepth2, "photon_ehcaltoweroverecaldepth2[photon_count]/F");
    tree->Branch("photon_maxenergyxtal", photon_maxenergyxtal, "photon_maxenergyxtal[photon_count]/F");
    tree->Branch("photon_isolationr3track", photon_isolationr3track, "photon_isolationr3track[photon_count]/F");
    tree->Branch("photon_isolationr3trackhollow", photon_isolationr3trackhollow, "photon_isolationr3trackhollow[photon_count]/F");
    tree->Branch("photon_isolationr3ntrack", photon_isolationr3ntrack, "photon_isolationr3ntrack[photon_count]/i");
    tree->Branch("photon_isolationr3ntrackhollow", photon_isolationr3ntrackhollow, "photon_isolationr3ntrackhollow[photon_count]/i");
    tree->Branch("photon_isolationr3ecal", photon_isolationr3ecal, "photon_isolationr3ecal[photon_count]/F");
    tree->Branch("photon_isolationr3hcal", photon_isolationr3hcal, "photon_isolationr3hcal[photon_count]/F");
    tree->Branch("photon_isolationr4track", photon_isolationr4track, "photon_isolationr4track[photon_count]/F");
    tree->Branch("photon_isolationr4trackhollow", photon_isolationr4trackhollow, "photon_isolationr4trackhollow[photon_count]/F");
    tree->Branch("photon_isolationr4ntrack", photon_isolationr4ntrack, "photon_isolationr4ntrack[photon_count]/i");
    tree->Branch("photon_isolationr4ntrackhollow", photon_isolationr4ntrackhollow, "photon_isolationr4ntrackhollow[photon_count]/i");
    tree->Branch("photon_isolationr4ecal", photon_isolationr4ecal, "photon_isolationr4ecal[photon_count]/F");
    tree->Branch("photon_isolationr4hcal", photon_isolationr4hcal, "photon_isolationr4hcal[photon_count]/F");
    tree->Branch("photon_isolationpfr3charged", photon_isolationpfr3charged, "photon_isolationpfr3charged[photon_count]/F");
    tree->Branch("photon_isolationpfr3photon", photon_isolationpfr3photon, "photon_isolationpfr3photon[photon_count]/F");
    tree->Branch("photon_isolationpfr3neutral", photon_isolationpfr3neutral, "photon_isolationpfr3neutral[photon_count]/F");
    tree->Branch("photon_isolationpfr4charged", photon_isolationpfr4charged, "photon_isolationpfr4charged[photon_count]/F");
    tree->Branch("photon_isolationpfr4photon", photon_isolationpfr4photon, "photon_isolationpfr4photon[photon_count]/F");
    tree->Branch("photon_isolationpfr4neutral", photon_isolationpfr4neutral, "photon_isolationpfr4neutral[photon_count]/F");
    tree->Branch("photon_isolationpfr4noscfootprintcharged", photon_isolationpfr4noscfootprintcharged, "photon_isolationpfr4noscfootprintcharged[photon_count]/F");
    tree->Branch("photon_isolationpfr4noscfootprintphoton", photon_isolationpfr4noscfootprintphoton, "photon_isolationpfr4noscfootprintphoton[photon_count]/F");
    tree->Branch("photon_isolationpfr4noscfootprintneutral", photon_isolationpfr4noscfootprintneutral, "photon_isolationpfr4noscfootprintneutral[photon_count]/F");
    tree->Branch("photon_supercluster_e", photon_supercluster_e, "photon_supercluster_e[photon_count]/F");
    tree->Branch("photon_supercluster_x", photon_supercluster_x, "photon_supercluster_x[photon_count]/F");
    tree->Branch("photon_supercluster_y", photon_supercluster_y, "photon_supercluster_y[photon_count]/F");
    tree->Branch("photon_supercluster_z", photon_supercluster_z, "photon_supercluster_z[photon_count]/F");
    tree->Branch("photon_supercluster_rawe", photon_supercluster_rawe, "photon_supercluster_rawe[photon_count]/F");
    tree->Branch("photon_supercluster_phiwidth", photon_supercluster_phiwidth, "photon_supercluster_phiwidth[photon_count]/F");
    tree->Branch("photon_supercluster_etawidth", photon_supercluster_etawidth, "photon_supercluster_etawidth[photon_count]/F");
    tree->Branch("photon_supercluster_nbasiccluster", photon_supercluster_nbasiccluster, "photon_supercluster_nbasiccluster[photon_count]/I");
    tree->Branch("photon_info", photon_info, "photon_info[photon_count]/b");
    tree->Branch("photon_gapinfo", photon_gapinfo, "photon_gapinfo[photon_count]/i");
    tree->Branch("photon_trigger", photon_trigger, "photon_trigger[photon_count]/i");
    tree->Branch("photon_conversionbegin", photon_conversionbegin, "photon_conversionbegin[photon_count]/i");

    tree->Branch("conversion_count", &conversion_count, "conversion_count/i");
    tree->Branch("conversion_info", conversion_info, "conversion_info[conversion_count]/b");
    tree->Branch("conversion_vx", conversion_vx, "conversion_vx[conversion_count]/F");
    tree->Branch("conversion_vy", conversion_vy, "conversion_vy[conversion_count]/F");
    tree->Branch("conversion_vz", conversion_vz, "conversion_vz[conversion_count]/F");
    tree->Branch("conversion_chi2", conversion_chi2, "conversion_chi2[conversion_count]/F");
    tree->Branch("conversion_ndof", conversion_ndof, "conversion_ndof[conversion_count]/F");
    tree->Branch("conversion_cov", conversion_cov, "conversion_cov[conversion_count][6]/F");
    tree->Branch("conversion_mvaout", conversion_mvaout, "conversion_mvaout[conversion_count]/F");
    tree->Branch("conversion_trackecalpointx", conversion_trackecalpointx, "conversion_trackecalpointx[conversion_count][2]/F");
    tree->Branch("conversion_trackecalpointy", conversion_trackecalpointy, "conversion_trackecalpointy[conversion_count][2]/F");
    tree->Branch("conversion_trackecalpointz", conversion_trackecalpointz, "conversion_trackecalpointz[conversion_count][2]/F");
    tree->Branch("conversion_trackpx", conversion_trackpx, "conversion_trackpx[conversion_count][2]/F");
    tree->Branch("conversion_trackpy", conversion_trackpy, "conversion_trackpy[conversion_count][2]/F");
    tree->Branch("conversion_trackpz", conversion_trackpz, "conversion_trackpz[conversion_count][2]/F");
    tree->Branch("conversion_trackclosestpointx", conversion_trackclosestpointx, "conversion_trackclosestpointx[conversion_count][2]/F");
    tree->Branch("conversion_trackclosestpointy", conversion_trackclosestpointy, "conversion_trackclosestpointy[conversion_count][2]/F");
    tree->Branch("conversion_trackclosestpointz", conversion_trackclosestpointz, "conversion_trackclosestpointz[conversion_count][2]/F");
    tree->Branch("conversion_trackchi2", conversion_trackchi2, "conversion_trackchi2[conversion_count][2]/F");
    tree->Branch("conversion_trackndof", conversion_trackndof, "conversion_trackndof[conversion_count][2]/F");
    tree->Branch("conversion_trackdxy", conversion_trackdxy, "conversion_trackdxy[conversion_count][2]/F");
    tree->Branch("conversion_trackdxyerr", conversion_trackdxyerr, "conversion_trackdxyerr[conversion_count][2]/F");
    tree->Branch("conversion_trackdz", conversion_trackdz, "conversion_trackdz[conversion_count][2]/F");
    tree->Branch("conversion_trackdzerr", conversion_trackdzerr, "conversion_trackdzerr[conversion_count][2]/F");
    tree->Branch("conversion_trackcharge", conversion_trackcharge, "conversion_trackcharge[conversion_count][2]/I");
    tree->Branch("conversion_tracknhits", conversion_tracknhits, "conversion_tracknhits[conversion_count][2]/b");
    tree->Branch("conversion_tracknmissinghits", conversion_tracknmissinghits, "conversion_tracknmissinghits[conversion_count][2]/b");
    tree->Branch("conversion_tracknpixelhits", conversion_tracknpixelhits, "conversion_tracknpixelhits[conversion_count][2]/b");
    tree->Branch("conversion_tracknpixellayers", conversion_tracknpixellayers, "conversion_tracknpixellayers[conversion_count][2]/b");
    tree->Branch("conversion_tracknstriplayers", conversion_tracknstriplayers, "conversion_tracknstriplayers[conversion_count][2]/b");

    tree->Branch("allconversion_count", &allconversion_count, "allconversion_count/i");
    tree->Branch("allconversion_info", allconversion_info, "allconversion_info[allconversion_count]/b");
    tree->Branch("allconversion_vx", allconversion_vx, "allconversion_vx[allconversion_count]/F");
    tree->Branch("allconversion_vy", allconversion_vy, "allconversion_vy[allconversion_count]/F");
    tree->Branch("allconversion_vz", allconversion_vz, "allconversion_vz[allconversion_count]/F");
    tree->Branch("allconversion_chi2", allconversion_chi2, "allconversion_chi2[allconversion_count]/F");
    tree->Branch("allconversion_ndof", allconversion_ndof, "allconversion_ndof[allconversion_count]/F");
    tree->Branch("allconversion_cov", allconversion_cov, "allconversion_cov[allconversion_count][6]/F");
    tree->Branch("allconversion_mvaout", allconversion_mvaout, "allconversion_mvaout[allconversion_count]/F");
    tree->Branch("allconversion_trackecalpointx", allconversion_trackecalpointx, "allconversion_trackecalpointx[allconversion_count][2]/F");
    tree->Branch("allconversion_trackecalpointy", allconversion_trackecalpointy, "allconversion_trackecalpointy[allconversion_count][2]/F");
    tree->Branch("allconversion_trackecalpointz", allconversion_trackecalpointz, "allconversion_trackecalpointz[allconversion_count][2]/F");
    tree->Branch("allconversion_trackpx", allconversion_trackpx, "allconversion_trackpx[allconversion_count][2]/F");
    tree->Branch("allconversion_trackpy", allconversion_trackpy, "allconversion_trackpy[allconversion_count][2]/F");
    tree->Branch("allconversion_trackpz", allconversion_trackpz, "allconversion_trackpz[allconversion_count][2]/F");
    tree->Branch("allconversion_trackclosestpointx", allconversion_trackclosestpointx, "allconversion_trackclosestpointx[allconversion_count][2]/F");
    tree->Branch("allconversion_trackclosestpointy", allconversion_trackclosestpointy, "allconversion_trackclosestpointy[allconversion_count][2]/F");
    tree->Branch("allconversion_trackclosestpointz", allconversion_trackclosestpointz, "allconversion_trackclosestpointz[allconversion_count][2]/F");
    tree->Branch("allconversion_trackchi2", allconversion_trackchi2, "allconversion_trackchi2[allconversion_count][2]/F");
    tree->Branch("allconversion_trackndof", allconversion_trackndof, "allconversion_trackndof[allconversion_count][2]/F");
    tree->Branch("allconversion_trackdxy", allconversion_trackdxy, "allconversion_trackdxy[allconversion_count][2]/F");
    tree->Branch("allconversion_trackdxyerr", allconversion_trackdxyerr, "allconversion_trackdxyerr[allconversion_count][2]/F");
    tree->Branch("allconversion_trackdz", allconversion_trackdz, "allconversion_trackdz[allconversion_count][2]/F");
    tree->Branch("allconversion_trackdzerr", allconversion_trackdzerr, "allconversion_trackdzerr[allconversion_count][2]/F");
    tree->Branch("allconversion_trackcharge", allconversion_trackcharge, "allconversion_trackcharge[allconversion_count][2]/I");
    tree->Branch("allconversion_tracknhits", allconversion_tracknhits, "allconversion_tracknhits[allconversion_count][2]/b");
    tree->Branch("allconversion_tracknmissinghits", allconversion_tracknmissinghits, "allconversion_tracknmissinghits[allconversion_count][2]/b");
    tree->Branch("allconversion_tracknpixelhits", allconversion_tracknpixelhits, "allconversion_tracknpixelhits[allconversion_count][2]/b");
    tree->Branch("allconversion_tracknpixellayers", allconversion_tracknpixellayers, "allconversion_tracknpixellayers[allconversion_count][2]/b");
    tree->Branch("allconversion_tracknstriplayers", allconversion_tracknstriplayers, "allconversion_tracknstriplayers[allconversion_count][2]/b");

    tree->Branch("tau_count", &tau_count, "tau_count/i");
    tree->Branch("tau_px", tau_px, "tau_px[tau_count]/F");
    tree->Branch("tau_py", tau_py, "tau_py[tau_count]/F");
    tree->Branch("tau_pz", tau_pz, "tau_pz[tau_count]/F");
    tree->Branch("tau_pt", tau_pt, "tau_pt[tau_count]/F");
    tree->Branch("tau_phi", tau_phi, "tau_phi[tau_count]/F");
    tree->Branch("tau_eta", tau_eta, "tau_eta[tau_count]/F");
    tree->Branch("tau_isolationneutralspt", tau_isolationneutralspt, "tau_isolationneutralspt[tau_count]/F");
    tree->Branch("tau_isolationneutralsnum", tau_isolationneutralsnum, "tau_isolationneutralsnum[tau_count]/i");
    tree->Branch("tau_isolationchargedpt", tau_isolationchargedpt, "tau_isolationchargedpt[tau_count]/F");
    tree->Branch("tau_isolationchargednum", tau_isolationchargednum, "tau_isolationchargednum[tau_count]/i");
    tree->Branch("tau_pucorrptsum", tau_pucorrptsum, "tau_pucorrptsum[tau_count]/F");
    tree->Branch("tau_isolationgammapt", tau_isolationgammapt, "tau_isolationgammapt[tau_count]/F");
    tree->Branch("tau_isolationgammanum", tau_isolationgammanum, "tau_isolationgammanum[tau_count]/i");
    tree->Branch("tau_charge", tau_charge, "tau_charge[tau_count]/I");
    tree->Branch("tau_emfraction", tau_emfraction, "tau_emfraction[tau_count]/F");
    tree->Branch("tau_hcaltotoverplead", tau_hcaltotoverplead, "tau_hcaltotoverplead[tau_count]/F");
    tree->Branch("tau_hcal3x3overplead", tau_hcal3x3overplead, "tau_hcal3x3overplead[tau_count]/F");
    tree->Branch("tau_ecalstripsumeoverplead", tau_ecalstripsumeoverplead, "tau_ecalstripsumeoverplead[tau_count]/F");
    tree->Branch("tau_bremsrecoveryeoverplead", tau_bremsrecoveryeoverplead, "tau_bremsrecoveryeoverplead[tau_count]/F");
    tree->Branch("tau_calocomp", tau_calocomp, "tau_calocomp[tau_count]/F");
    tree->Branch("tau_segcomp", tau_segcomp, "tau_segcomp[tau_count]/F");
    tree->Branch("tau_dishps", tau_dishps, "tau_dishps[tau_count]/i");
    tree->Branch("tau_trigger", tau_trigger, "tau_trigger[tau_count]/i");

    tree->Branch("tau_ak4pfjet_e", tau_ak4pfjet_e, "tau_ak4pfjet_e[tau_count]/F");
    tree->Branch("tau_ak4pfjet_px", tau_ak4pfjet_px, "tau_ak4pfjet_px[tau_count]/F");
    tree->Branch("tau_ak4pfjet_py", tau_ak4pfjet_py, "tau_ak4pfjet_py[tau_count]/F");
    tree->Branch("tau_ak4pfjet_pz", tau_ak4pfjet_pz, "tau_ak4pfjet_pz[tau_count]/F");
    tree->Branch("tau_ak4pfjet_hadronicenergy", tau_ak4pfjet_hadronicenergy, "tau_ak4pfjet_hadronicenergy[tau_count]/F");
    tree->Branch("tau_ak4pfjet_chargedhadronicenergy", tau_ak4pfjet_chargedhadronicenergy, "tau_ak4pfjet_chargedhadronicenergy[tau_count]/F");
    tree->Branch("tau_ak4pfjet_emenergy", tau_ak4pfjet_emenergy, "tau_ak4pfjet_emenergy[tau_count]/F");
    tree->Branch("tau_ak4pfjet_chargedemenergy", tau_ak4pfjet_chargedemenergy, "tau_ak4pfjet_chargedemenergy[tau_count]/F");
    tree->Branch("tau_ak4pfjet_chargedmulti", tau_ak4pfjet_chargedmulti, "tau_ak4pfjet_chargedmulti[tau_count]/i");
    tree->Branch("tau_ak4pfjet_neutralmulti", tau_ak4pfjet_neutralmulti, "tau_ak4pfjet_neutralmulti[tau_count]/i");
    tree->Branch("tau_ak4pfjet_trigger", tau_ak4pfjet_trigger, "tau_ak4pfjet_trigger[tau_count]/i");
    tree->Branch("tau_chargedbegin", tau_chargedbegin, "tau_chargedbegin[tau_count]/i");
    tree->Branch("tau_charged_count", &tau_charged_count, "tau_charged_count/i");
    tree->Branch("tau_charged_px", tau_charged_px, "tau_charged_px[tau_charged_count]/F");
    tree->Branch("tau_charged_py", tau_charged_py, "tau_charged_py[tau_charged_count]/F");
    tree->Branch("tau_charged_pz", tau_charged_pz, "tau_charged_pz[tau_charged_count]/F");
    tree->Branch("tau_charged_outerx", tau_charged_outerx, "tau_charged_outerx[tau_charged_count]/F");
    tree->Branch("tau_charged_outery", tau_charged_outery, "tau_charged_outery[tau_charged_count]/F");
    tree->Branch("tau_charged_outerz", tau_charged_outerz, "tau_charged_outerz[tau_charged_count]/F");
    tree->Branch("tau_charged_closestpointx", tau_charged_closestpointx, "tau_charged_closestpointx[tau_charged_count]/F");
    tree->Branch("tau_charged_closestpointy", tau_charged_closestpointy, "tau_charged_closestpointy[tau_charged_count]/F");
    tree->Branch("tau_charged_closestpointz", tau_charged_closestpointz, "tau_charged_closestpointz[tau_charged_count]/F");
    tree->Branch("tau_charged_chi2", tau_charged_chi2, "tau_charged_chi2[tau_charged_count]/F");
    tree->Branch("tau_charged_ndof", tau_charged_ndof, "tau_charged_ndof[tau_charged_count]/F");
    tree->Branch("tau_charged_dxy", tau_charged_dxy, "tau_charged_dxy[tau_charged_count]/F");
    tree->Branch("tau_charged_dxyerr", tau_charged_dxyerr, "tau_charged_dxyerr[tau_charged_count]/F");
    tree->Branch("tau_charged_dz", tau_charged_dz, "tau_charged_dz[tau_charged_count]/F");
    tree->Branch("tau_charged_dzerr", tau_charged_dzerr, "tau_charged_dzerr[tau_charged_count]/F");
    tree->Branch("tau_charged_dedxharmonic2", tau_charged_dedxharmonic2, "tau_charged_dedxharmonic2[tau_charged_count]/F");
    tree->Branch("tau_charged_charge", tau_charged_charge, "tau_charged_charge[tau_charged_count]/I");
    tree->Branch("tau_charged_nhits", tau_charged_nhits, "tau_charged_nhits[tau_charged_count]/b");
    tree->Branch("tau_charged_nmissinghits", tau_charged_nmissinghits, "tau_charged_nmissinghits[tau_charged_count]/b");
    tree->Branch("tau_charged_npixelhits", tau_charged_npixelhits, "tau_charged_npixelhits[tau_charged_count]/b");
    tree->Branch("tau_charged_npixellayers", tau_charged_npixellayers, "tau_charged_npixellayers[tau_charged_count]/b");
    tree->Branch("tau_charged_nstriplayers", tau_charged_nstriplayers, "tau_charged_nstriplayers[tau_charged_count]/b");

    tree->Branch("ak4pfjet_rho", &ak4pfjet_rho, "ak4pfjet_rho/F");
    tree->Branch("ak4pfjet_sigma", &ak4pfjet_sigma, "ak4pfjet_sigma/F");

    tree->Branch("patmvamet_emt_ex", &patmvamet_emt_ex, "patmvamet_emt_ex/F");
    tree->Branch("patmvamet_emt_ey", &patmvamet_emt_ey, "patmvamet_emt_ey/F");
    tree->Branch("patmvamet_emt_cov_00", &patmvamet_emt_cov_00, "patmvamet_emt_cov_00/F");
    tree->Branch("patmvamet_emt_cov_01", &patmvamet_emt_cov_01, "patmvamet_emt_cov_01/F");
    tree->Branch("patmvamet_emt_cov_10", &patmvamet_emt_cov_10, "patmvamet_emt_cov_10/F");
    tree->Branch("patmvamet_emt_cov_11", &patmvamet_emt_cov_11, "patmvamet_emt_cov_11/F");
    tree->Branch("patmvamet_et_ex", &patmvamet_et_ex, "patmvamet_et_ex/F");
    tree->Branch("patmvamet_et_ey", &patmvamet_et_ey, "patmvamet_et_ey/F");
    tree->Branch("patmvamet_et_cov_00", &patmvamet_et_cov_00, "patmvamet_et_cov_00/F");
    tree->Branch("patmvamet_et_cov_01", &patmvamet_et_cov_01, "patmvamet_et_cov_01/F");
    tree->Branch("patmvamet_et_cov_10", &patmvamet_et_cov_10, "patmvamet_et_cov_10/F");
    tree->Branch("patmvamet_et_cov_11", &patmvamet_et_cov_11, "patmvamet_et_cov_11/F");
    tree->Branch("patmvamet_em_ex", &patmvamet_em_ex, "patmvamet_em_ex/F");
    tree->Branch("patmvamet_em_ey", &patmvamet_em_ey, "patmvamet_em_ey/F");
    tree->Branch("patmvamet_em_cov_00", &patmvamet_em_cov_00, "patmvamet_em_cov_00/F");
    tree->Branch("patmvamet_em_cov_01", &patmvamet_em_cov_01, "patmvamet_em_cov_01/F");
    tree->Branch("patmvamet_em_cov_10", &patmvamet_em_cov_10, "patmvamet_em_cov_10/F");
    tree->Branch("patmvamet_em_cov_11", &patmvamet_em_cov_11, "patmvamet_em_cov_11/F");
    tree->Branch("patmvamet_mt_ex", &patmvamet_mt_ex, "patmvamet_mt_ex/F");
    tree->Branch("patmvamet_mt_ey", &patmvamet_mt_ey, "patmvamet_mt_ey/F");
    tree->Branch("patmvamet_mt_cov_00", &patmvamet_mt_cov_00, "patmvamet_mt_cov_00/F");
    tree->Branch("patmvamet_mt_cov_01", &patmvamet_mt_cov_01, "patmvamet_mt_cov_01/F");
    tree->Branch("patmvamet_mt_cov_10", &patmvamet_mt_cov_10, "patmvamet_mt_cov_10/F");
    tree->Branch("patmvamet_mt_cov_11", &patmvamet_mt_cov_11, "patmvamet_mt_cov_11/F");
    tree->Branch("patmvamet_tt_ex", &patmvamet_tt_ex, "patmvamet_tt_ex/F");
    tree->Branch("patmvamet_tt_ey", &patmvamet_tt_ey, "patmvamet_tt_ey/F");
    tree->Branch("patmvamet_tt_cov_00", &patmvamet_tt_cov_00, "patmvamet_tt_cov_00/F");
    tree->Branch("patmvamet_tt_cov_01", &patmvamet_tt_cov_01, "patmvamet_tt_cov_01/F");
    tree->Branch("patmvamet_tt_cov_10", &patmvamet_tt_cov_10, "patmvamet_tt_cov_10/F");
    tree->Branch("patmvamet_tt_cov_11", &patmvamet_tt_cov_11, "patmvamet_tt_cov_11/F");
    tree->Branch("pfmet_ex", &pfmet_ex, "pfmet_ex/F");
    tree->Branch("pfmet_ey", &pfmet_ey, "pfmet_ey/F");
    tree->Branch("pfmettype1_ex", &pfmettype1_ex, "pfmettype1_ex/F");
    tree->Branch("pfmettype1_ey", &pfmettype1_ey, "pfmettype1_ey/F");
    tree->Branch("pfmettype1_cov_00", &pfmettype1_cov_00, "pfmettype1_cov_00/F");
    tree->Branch("pfmettype1_cov_01", &pfmettype1_cov_01, "pfmettype1_cov_01/F");
    tree->Branch("pfmettype1_cov_10", &pfmettype1_cov_10, "pfmettype1_cov_10/F");
    tree->Branch("pfmettype1_cov_11", &pfmettype1_cov_11, "pfmettype1_cov_11/F");
    tree->Branch("pfmetpuppitype1_ex", &pfmetpuppitype1_ex, "pfmetpuppitype1_ex/F");
    tree->Branch("pfmetpuppitype1_ey", &pfmetpuppitype1_ey, "pfmetpuppitype1_ey/F");
    tree->Branch("pfmettype0type1_ex", &pfmettype0type1_ex, "pfmettype0type1_ex/F");
    tree->Branch("pfmettype0type1_ey", &pfmettype0type1_ey, "pfmettype0type1_ey/F");

    tree->Branch("secvertices_count", &secvertices_count, "secvertices_count/i");
    tree->Branch("secvertices_vx", secvertices_vx, "secvertices_vx[secvertices_count]/F");
    tree->Branch("secvertices_vy", secvertices_vy, "secvertices_vy[secvertices_count]/F");
    tree->Branch("secvertices_vz", secvertices_vz, "secvertices_vz[secvertices_count]/F");
    tree->Branch("secvertices_chi2", secvertices_chi2, "secvertices_chi2[secvertices_count]/F");
    tree->Branch("secvertices_ndof", secvertices_ndof, "secvertices_ndof[secvertices_count]/F");
    tree->Branch("secvertices_cov", secvertices_cov, "secvertices_cov[secvertices_count][6]/F");
    tree->Branch("secvertices_track_px", secvertices_track_px, "secvertices_track_px[secvertices_count][2]/F");
    tree->Branch("secvertices_track_py", secvertices_track_py, "secvertices_track_py[secvertices_count][2]/F");
    tree->Branch("secvertices_track_pz", secvertices_track_pz, "secvertices_track_pz[secvertices_count][2]/F");
    tree->Branch("secvertices_track_closestpointx", secvertices_track_closestpointx, "secvertices_track_closestpointx[secvertices_count][2]/F");
    tree->Branch("secvertices_track_closestpointy", secvertices_track_closestpointy, "secvertices_track_closestpointy[secvertices_count][2]/F");
    tree->Branch("secvertices_track_closestpointz", secvertices_track_closestpointz, "secvertices_track_closestpointz[secvertices_count][2]/F");
    tree->Branch("secvertices_track_chi2", secvertices_track_chi2, "secvertices_track_chi2[secvertices_count][2]/F");
    tree->Branch("secvertices_track_ndof", secvertices_track_ndof, "secvertices_track_ndof[secvertices_count][2]/F");
    tree->Branch("secvertices_track_dxy", secvertices_track_dxy, "secvertices_track_dxy[secvertices_count][2]/F");
    tree->Branch("secvertices_track_dxyerr", secvertices_track_dxyerr, "secvertices_track_dxyerr[secvertices_count][2]/F");
    tree->Branch("secvertices_track_dz", secvertices_track_dz, "secvertices_track_dz[secvertices_count][2]/F");
    tree->Branch("secvertices_track_dzerr", secvertices_track_dzerr, "secvertices_track_dzerr[secvertices_count][2]/F");
    tree->Branch("secvertices_track_dedxharmonic2", secvertices_track_dedxharmonic2, "secvertices_track_dedxharmonic2[track_count][2]/F");
    tree->Branch("secvertices_track_charge", secvertices_track_charge, "secvertices_track_charge[secvertices_count][2]/I");
    tree->Branch("secvertices_track_nhits", secvertices_track_nhits, "secvertices_track_nhits[secvertices_count][2]/b");
    tree->Branch("secvertices_track_nmissinghits", secvertices_track_nmissinghits, "secvertices_track_nmissinghits[secvertices_count][2]/b");
    tree->Branch("secvertices_track_npixelhits", secvertices_track_npixelhits, "secvertices_track_npixelhits[secvertices_count][2]/b");
    tree->Branch("secvertices_track_npixellayers", secvertices_track_npixellayers, "secvertices_track_npixellayers[secvertices_count][2]/b");
    tree->Branch("secvertices_track_nstriplayers", secvertices_track_nstriplayers, "secvertices_track_nstriplayers[secvertices_count][2]/b");

    tree->Branch("musecvertices_count", &musecvertices_count, "musecvertices_count/i");
    tree->Branch("musecvertices_vx", musecvertices_vx, "musecvertices_vx[musecvertices_count]/F");
    tree->Branch("musecvertices_vy", musecvertices_vy, "musecvertices_vy[musecvertices_count]/F");
    tree->Branch("musecvertices_vz", musecvertices_vz, "musecvertices_vz[musecvertices_count]/F");
    tree->Branch("musecvertices_chi2", musecvertices_chi2, "musecvertices_chi2[musecvertices_count]/F");
    tree->Branch("musecvertices_ndof", musecvertices_ndof, "musecvertices_ndof[musecvertices_count]/F");
    tree->Branch("musecvertices_cov", musecvertices_cov, "musecvertices_cov[musecvertices_count][6]/F");
    tree->Branch("musecvertices_track_px", musecvertices_track_px, "musecvertices_track_px[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_py", musecvertices_track_py, "musecvertices_track_py[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_pz", musecvertices_track_pz, "musecvertices_track_pz[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_closestpointx", musecvertices_track_closestpointx, "musecvertices_track_closestpointx[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_closestpointy", musecvertices_track_closestpointy, "musecvertices_track_closestpointy[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_closestpointz", musecvertices_track_closestpointz, "musecvertices_track_closestpointz[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_chi2", musecvertices_track_chi2, "musecvertices_track_chi2[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_ndof", musecvertices_track_ndof, "musecvertices_track_ndof[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_dxy", musecvertices_track_dxy, "musecvertices_track_dxy[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_dxyerr", musecvertices_track_dxyerr, "musecvertices_track_dxyerr[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_dz", musecvertices_track_dz, "musecvertices_track_dz[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_dzerr", musecvertices_track_dzerr, "musecvertices_track_dzerr[musecvertices_count][2]/F");
    tree->Branch("musecvertices_track_dedxharmonic2", musecvertices_track_dedxharmonic2, "musecvertices_track_dedxharmonic2[track_count][2]/F");
    tree->Branch("musecvertices_track_charge", musecvertices_track_charge, "musecvertices_track_charge[musecvertices_count][2]/I");
    tree->Branch("musecvertices_track_nhits", musecvertices_track_nhits, "musecvertices_track_nhits[musecvertices_count][2]/b");
    tree->Branch("musecvertices_track_nmissinghits", musecvertices_track_nmissinghits, "musecvertices_track_nmissinghits[musecvertices_count][2]/b");
    tree->Branch("musecvertices_track_npixelhits", musecvertices_track_npixelhits, "musecvertices_track_npixelhits[musecvertices_count][2]/b");
    tree->Branch("musecvertices_track_npixellayers", musecvertices_track_npixellayers, "musecvertices_track_npixellayers[musecvertices_count][2]/b");
    tree->Branch("musecvertices_track_nstriplayers", musecvertices_track_nstriplayers, "musecvertices_track_nstriplayers[musecvertices_count][2]/b");

    tree->Branch("genweight", &genweight, "genweight/F");
    tree->Branch("genid1", &genid1, "genid1/F");
    tree->Branch("genx1", &genx1, "genx1/F");
    tree->Branch("genid2", &genid2, "genid2/F");
    tree->Branch("genx2", &genx2, "genx2/F");
    tree->Branch("genScale", &genScale, "genScale/F");

    tree->Branch("numpileupinteractionsminus", &numpileupinteractionsminus, "numpileupinteractionsminus/I");
    tree->Branch("numpileupinteractions", &numpileupinteractions, "numpileupinteractions/I");
    tree->Branch("numpileupinteractionsplus", &numpileupinteractionsplus, "numpileupinteractionsplus/I");
    tree->Branch("numtruepileupinteractions", &numtruepileupinteractions, "numtruepileupinteractions/F");

    tree->Branch("genparticles_count", &genparticles_count, "genparticles_count/i");
    tree->Branch("genparticles_e", genparticles_e, "genparticles_e[genparticles_count]/F");
    tree->Branch("genparticles_px", genparticles_px, "genparticles_px[genparticles_count]/F");
    tree->Branch("genparticles_py", genparticles_py, "genparticles_py[genparticles_count]/F");
    tree->Branch("genparticles_pz", genparticles_pz, "genparticles_pz[genparticles_count]/F");
    tree->Branch("genparticles_vx", genparticles_vx, "genparticles_vx[genparticles_count]/F");
    tree->Branch("genparticles_vy", genparticles_vy, "genparticles_vy[genparticles_count]/F");
    tree->Branch("genparticles_vz", genparticles_vz, "genparticles_vz[genparticles_count]/F");
    tree->Branch("genparticles_pdgid", genparticles_pdgid, "genparticles_pdgid[genparticles_count]/I");
    tree->Branch("genparticles_status", genparticles_status, "genparticles_status[genparticles_count]/I");
    tree->Branch("genparticles_indirectmother", genparticles_indirectmother, "genparticles_indirectmother[genparticles_count]/I");
    tree->Branch("genparticles_info", genparticles_info, "genparticles_info[genparticles_count]/i");

    tree->Branch("genallparticles_count", &genallparticles_count, "genallparticles_count/i");
    tree->Branch("genallparticles_e", genallparticles_e, "genallparticles_e[genallparticles_count]/F");
    tree->Branch("genallparticles_px", genallparticles_px, "genallparticles_px[genallparticles_count]/F");
    tree->Branch("genallparticles_py", genallparticles_py, "genallparticles_py[genallparticles_count]/F");
    tree->Branch("genallparticles_pz", genallparticles_pz, "genallparticles_pz[genallparticles_count]/F");
    tree->Branch("genallparticles_vx", genallparticles_vx, "genallparticles_vx[genallparticles_count]/F");
    tree->Branch("genallparticles_vy", genallparticles_vy, "genallparticles_vy[genallparticles_count]/F");
    tree->Branch("genallparticles_vz", genallparticles_vz, "genallparticles_vz[genallparticles_count]/F");
    tree->Branch("genallparticles_pdgid", genallparticles_pdgid, "genallparticles_pdgid[genallparticles_count]/I");
    tree->Branch("genallparticles_status", genallparticles_status, "genallparticles_status[genallparticles_count]/I");
    tree->Branch("genallparticles_motherbeg", genallparticles_motherbeg, "genallparticles_motherbeg[genallparticles_count]/i");
    tree->Branch("genallparticles_daughterbeg", genallparticles_daughterbeg, "genallparticles_daughterbeg[genallparticles_count]/i");

    tree->Branch("genallparticlesmother_count", &genallparticlesmother_count, "genallparticlesmother_count/i");
    tree->Branch("genallparticles_mothers", genallparticles_mothers, "genallparticles_mothers[genallparticlesmother_count]/i");

    tree->Branch("genallparticlesdaughter_count", &genallparticlesdaughter_count, "genallparticlesdaughter_count/i");
    tree->Branch("genallparticles_daughters", genallparticles_daughters, "genallparticles_daughters[genallparticlesdaughter_count]/i");

    tree->Branch("genmetcalo_ex", &genmetcalo_ex, "genmetcalo_ex/F");
    tree->Branch("genmetcalo_ey", &genmetcalo_ey, "genmetcalo_ey/F");
    tree->Branch("genmettrue_ex", &genmettrue_ex, "genmettrue_ex/F");
    tree->Branch("genmettrue_ey", &genmettrue_ey, "genmettrue_ey/F");

    tree->Branch("genak4jet_count", &genak4jet_count, "genak4jet_count/i");
    tree->Branch("genak4jet_e", genak4jet_e, "genak4jet_e[genak4jet_count]/F");
    tree->Branch("genak4jet_px", genak4jet_px, "genak4jet_px[genak4jet_count]/F");
    tree->Branch("genak4jet_py", genak4jet_py, "genak4jet_py[genak4jet_count]/F");
    tree->Branch("genak4jet_pz", genak4jet_pz, "genak4jet_pz[genak4jet_count]/F");
    tree->Branch("genak4jet_einvisible", genak4jet_einvisible, "genak4jet_einvisible[genak4jet_count]/F");
    tree->Branch("genak4jet_flavour", genak4jet_flavour, "genak4jet_flavour[genak4jet_count]/I");
    tree->Branch("genak4jet_info", genak4jet_info, "genak4jet_info[genak4jet_count]/i");

    lumitree = FS->make<TTree> ("AC1Blumi", "AC1Blumi", 1);

    lumitree->Branch("lumi_run", &lumi_run, "lumi_run/i");
    lumitree->Branch("lumi_block", &lumi_block, "lumi_block/i");
    lumitree->Branch("lumi_value", &lumi_value, "lumi_value/F");
    lumitree->Branch("lumi_valueerr", &lumi_valueerr, "lumi_valueerr/F");
    lumitree->Branch("lumi_livefrac", &lumi_livefrac, "lumi_livefrac/F");
    lumitree->Branch("lumi_deadfrac", &lumi_deadfrac, "lumi_deadfrac/F");
    lumitree->Branch("lumi_quality", &lumi_quality, "lumi_quality/i");
    lumitree->Branch("lumi_eventsprocessed", &lumi_eventsprocessed, "lumi_eventsprocessed/i");
    lumitree->Branch("lumi_eventsfiltered", &lumi_eventsfiltered, "lumi_eventsfiltered/i");
    lumitree->Branch("lumi_hltprescaletable", &lumi_hltprescaletable, "lumi_hltprescaletable/i");
    lumitree->Branch("lumi_l1algoprescaletable", &lumi_l1algoprescaletable, "lumi_l1algoprescaletable/i");
    lumitree->Branch("lumi_l1techprescaletable", &lumi_l1techprescaletable, "lumi_l1techprescaletable/i");

    runtree = FS->make<TTree> ("AC1Brun", "AC1Brun", 1);

    runtree->Branch("run_number", &run_number, "run_number/i");
    runtree->Branch("run_hltcount", &run_hltcount, "run_hltcount/i");
    runtree->Branch("run_hltnames", run_hltnames, "run_hltnames/C");
    runtree->Branch("run_hltmunames", run_hltmunames, "run_hltmunames/C");
    runtree->Branch("run_hltelnames", run_hltelnames, "run_hltelnames/C");
    runtree->Branch("run_hlttaunames", run_hlttaunames, "run_hlttaunames/C");
    runtree->Branch("run_hltphotonnames", run_hltphotonnames, "run_hltphotonnames/C");
    runtree->Branch("run_hltjetnames", run_hltjetnames, "run_hltjetnames/C");
    runtree->Branch("run_taudiscriminators", run_taudiscriminators, "run_taudiscriminators/C");
    runtree->Branch("run_hltprescaletablescount", &run_hltprescaletablescount, "run_hltprescaletablescount/i");
    runtree->Branch("run_hltprescaletables", run_hltprescaletables, "run_hltprescaletables[run_hltprescaletablescount]/i");
    runtree->Branch("run_hltl1prescaletables", run_hltl1prescaletables, "run_hltl1prescaletables[run_hltprescaletablescount]/i");
    runtree->Branch("run_l1algocount", &run_l1algocount, "run_l1algocount/i");
    runtree->Branch("run_l1algoprescaletablescount", &run_l1algoprescaletablescount, "run_l1algoprescaletablescount/i");
    runtree->Branch("run_l1algoprescaletables", run_l1algoprescaletables, "run_l1algoprescaletables[run_l1algoprescaletablescount]/i");
    runtree->Branch("run_l1techcount", &run_l1techcount, "run_l1techcount/i");
    runtree->Branch("run_l1techprescaletablescount", &run_l1techprescaletablescount, "run_l1techprescaletablescount/i");
    runtree->Branch("run_l1techprescaletables", run_l1techprescaletables, "run_l1techprescaletables[run_l1techprescaletablescount]/i");
}

void RootMaker::beginRun(const edm::Run &iRun, const edm::EventSetup &iSetup) {
    if(cdebug) { cout<<"beginRun..."<<endl; }

    if(propagatorWithMaterial != 0) {
        delete propagatorWithMaterial;
    }

    iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
    propagatorWithMaterial = new PropagatorWithMaterial(alongMomentum, 0.10566, & (*magneticField));
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", TTrackBuilder);
    run_number = iRun.run();
    //L1 prescales
    edm::ESHandle<L1GtPrescaleFactors> l1GtPfAlgo;
    iSetup.get<L1GtPrescaleFactorsAlgoTrigRcd>().get(l1GtPfAlgo);

    unsigned numl1algo = (l1GtPfAlgo.product()->gtPrescaleFactors())[0].size();
    unsigned numl1algotables = (l1GtPfAlgo.product()->gtPrescaleFactors()).size();

    run_l1algoprescaletablescount = numl1algo*numl1algotables;
    run_l1algocount = numl1algo;

    if(l1GtPfAlgo.isValid()) {
        for(unsigned i = 0 ; i < numl1algotables ; i++) {
            for(unsigned j = 0 ; j < numl1algo ; j++) {
                run_l1algoprescaletables[j+numl1algo*i] = (l1GtPfAlgo.product()->gtPrescaleFactors())[i][j];
            }
        }
    }

    edm::ESHandle<L1GtPrescaleFactors> l1GtPfTech;
    iSetup.get<L1GtPrescaleFactorsTechTrigRcd>().get(l1GtPfTech);

    unsigned numl1tech = (l1GtPfTech.product()->gtPrescaleFactors())[0].size();
    unsigned numl1techtables = (l1GtPfTech.product()->gtPrescaleFactors()).size();

    run_l1techprescaletablescount = numl1tech*numl1techtables;
    run_l1techcount = numl1tech;

    if(l1GtPfTech.isValid()) {
        for(unsigned i = 0 ; i < numl1techtables ; i++) {
            for(unsigned j = 0 ; j < numl1tech ; j++) {
                run_l1techprescaletables[j+numl1tech*i] = (l1GtPfTech.product()->gtPrescaleFactors())[i][j];
            }
        }
    }

    //HLT names and prescales
    bool changed = true;

    if(ctrigger) {
        HLTConfiguration.init(iRun, iSetup, cTriggerProcess, changed);
    }

    run_hltcount = HLTConfiguration.size();

    boost::cmatch what;
    vector<boost::regex> trigregexes;

    for(unsigned i = 0 ; i < cHLTriggerNamesSelection.size() ; i++) {
        trigregexes.push_back(boost::regex(cHLTriggerNamesSelection[i].c_str()));
    }

    string allnames;

    for(unsigned i = 0 ; i < HLTConfiguration.size() ; i++) {
        unsigned TriggerIndex = HLTConfiguration.triggerIndex(HLTConfiguration.triggerName(i));

        for(unsigned j = 0 ; j < trigregexes.size() ; j++) {
            if(boost::regex_match(HLTConfiguration.triggerName(i).c_str(), what, trigregexes[j])) {
                HLTriggerIndexSelection.push_back(TriggerIndex);
            }
        }

        allnames += HLTConfiguration.triggerName(i) + string(" ");
    }

    string allmuonnames;
    string allelectronnames;
    string alltaunames;
    string allphotonnames;
    string alljetnames;

    TriggerIndexSelection(cMuHLTriggerMatching, muontriggers, allmuonnames);
    TriggerIndexSelection(cElHLTriggerMatching, electrontriggers, allelectronnames);
    TriggerIndexSelection(cTauHLTriggerMatching, tautriggers, alltaunames);
    TriggerIndexSelection(cPhotonHLTriggerMatching, photontriggers, allphotonnames);
    TriggerIndexSelection(cJetHLTriggerMatching, jettriggers, alljetnames);

    if(cdebug) {
        cout<<"\nallnames = "<<allnames<<"\n"<<endl;
        cout<<"allmuonnames = "<<allmuonnames<<"\n"<<endl;
    }

    if(allnames.size() > 20000) {
        allnames = string("TOOLONGTRIGGERNAMES::ERROR ");
    }

    strcpy(run_hltnames, allnames.c_str());
    strcpy(run_hltmunames, allmuonnames.c_str());
    strcpy(run_hltelnames, allelectronnames.c_str());
    strcpy(run_hlttaunames, alltaunames.c_str());
    strcpy(run_hltphotonnames, allphotonnames.c_str());
    strcpy(run_hltjetnames, alljetnames.c_str());

    string alltaudiscriminators;

    for(unsigned i = 0 ; i < cTauDiscriminators.size() ; i++) {
        alltaudiscriminators += cTauDiscriminators[i] + string(" ");
    }

    strcpy(run_taudiscriminators, alltaudiscriminators.c_str());
    L1GtUtils l1info;
    l1info.retrieveL1EventSetup(iSetup);

    run_hltprescaletablescount = HLTConfiguration.prescaleSize()*HLTConfiguration.size();

    for(unsigned j = 0 ; j < HLTConfiguration.prescaleSize() ; j++) {
        for(unsigned i = 0 ; i < HLTConfiguration.size() ; i++) {
            int l1bit = -1;
            int l1prescale = 0;
            L1GtUtils::TriggerCategory trigCategory;
            const vector<pair<bool, string> > l1seed = HLTConfiguration.hltL1GTSeeds(i);

            if(l1seed.size() == 1) {
                //if (cdebug) {
                //    cout<<"HLTConfiguration.triggerName("<<i<<") = "<< HLTConfiguration.triggerName(i)<<endl;
                //    cout<<"l1seed[0].second = "<<l1seed[0].second<<endl;
                //    cout<<"trigCategory = "<<trigCategory<<endl;
                //    cout<<"l1bit = "<<l1bit <<endl;
                //    cout<<"(l1GtPfAlgo.product()->gtPrescaleFactors())["<<j<<"][l1bit] = "<<(l1GtPfAlgo.product()->gtPrescaleFactors())[j][l1bit]<<"\n"<<endl;
                //}
                l1info.l1AlgoTechTrigBitNumber(l1seed[0].second, trigCategory, l1bit);
                l1prescale = (l1GtPfAlgo.product()->gtPrescaleFactors())[j][l1bit];
            }

            run_hltprescaletables[i+HLTConfiguration.size()*j] = HLTConfiguration.prescaleValue(j, HLTConfiguration.triggerName(i));
            run_hltl1prescaletables[i+HLTConfiguration.size()*j] = l1prescale;
            //if (cdebug) {
            //    cout<<"HLTConfiguration.triggerName("<<i<<") = "<< HLTConfiguration.triggerName(i)<<endl;
            //    cout<<"j = "<<j<<endl;
            //    cout<<"run_hltl1prescaletables[i+HLTConfiguration.size()*j] = "<<run_hltl1prescaletables[i+HLTConfiguration.size()*j]<<endl;
            //    cout<<"run_hltprescaletables[i+HLTConfiguration.size()*j] = "<<run_hltprescaletables[i+HLTConfiguration.size()*j]<<"\n"<<endl;
            //}
        }
    }

    if(cdebug) { cout<<"runtree->Fill();"<<endl; }

    runtree->Fill();
}

void RootMaker::beginLuminosityBlock(const edm::LuminosityBlock &iLumiBlock, const edm::EventSetup &iSetup) {
    if(cdebug) { cout<<"beginLuminosityBlock..."<<endl; }

    lumi_run = iLumiBlock.run();
    lumi_block = iLumiBlock.luminosityBlock();
    lumi_eventsprocessed = 0;
    lumi_eventsfiltered = 0;

    edm::Handle<LumiSummary> lumiSummary;
    iLumiBlock.getByLabel(edm::InputTag("lumiProducer"), lumiSummary);

    if(lumiSummary.isValid()) {
        lumi_value = lumiSummary->avgInsDelLumi();
        lumi_valueerr = lumiSummary->avgInsDelLumiErr();
        lumi_livefrac = lumiSummary->lumiSecQual();
        lumi_deadfrac = lumiSummary->deadFrac();
        lumi_quality = lumiSummary->liveFrac();
    } else {
        lumi_value = -1.;
        lumi_valueerr = -1.;
        lumi_livefrac = -1.;
        lumi_deadfrac = -1.;
        lumi_quality = 0;
    }

    if(cdebug) { cout<<"end beginLuminosityBlock"<<endl; }
}

void RootMaker::endLuminosityBlock(const edm::LuminosityBlock &iLumiBlock, const edm::EventSetup &iSetup) {
    if(cdebug) { cout<<"endLuminosityBlock... lumitree->Fill();"<<endl; }

    lumitree->Fill();
}

void RootMaker::analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
    if(cdebug) { cout<<"analyze..."<<endl; }

    track_count = 0;
    primvertex_count = 0;
    supercluster_count = 0;
    supercluster_basiccluster_count = 0;
    supercluster_basiccluster_hit_count = 0;
    supercluster_escluster_count = 0;
    supercluster_escluster_hit_count = 0;
    muon_count = 0;
    tau_count = 0;
    tau_charged_count = 0;
    ak4calojet_count = 0;
    ak4jptjet_count = 0;
    ak4pfjet_count = 0;
    ak4pfchsjet_count = 0;
    ak4pfchspuppijet_count = 0;
    electron_count = 0;
    photon_count = 0;
    conversion_count = 0;
    secvertices_count = 0;
    musecvertices_count = 0;
    allconversion_count = 0;
    genallparticles_count = 0;
    genparticles_count = 0;
    genak4jet_count = 0;
    genallparticlesmother_count = 0;
    genallparticlesdaughter_count = 0;
    MuVector.clear();
    TrackVector.clear();
    errors = 0;
    bool takeevent = false;

    pv_position = math::XYZPoint(0.,0.,0.);
    bs_position = math::XYZPoint(0.,0.,0.);
    lumi_eventsprocessed++;

    event_nr = iEvent.id().event();
    event_run = iEvent.id().run();
    event_timeunix = iEvent.time().unixTime();
    event_timemicrosec = iEvent.time().microsecondOffset();
    event_luminosityblock = iEvent.getLuminosityBlock().luminosityBlock();
    //L1TriggerBits
    //
    //edm::Handle<L1GlobalTriggerReadoutRecord> L1trigger;
    iEvent.getByToken(l1TriggerToken_, L1trigger);

    const TechnicalTriggerWord &L1triggerbits = L1trigger->technicalTriggerWord();

    for(int i  = 0  ; i < 8 ; i++) {
        trigger_level1bits[i] = 0;
    }

    for(unsigned i = 0 ; i < min(unsigned(L1triggerbits.size()), unsigned(64)) ; i++) {
        trigger_level1bits[i/8] |= (Byte_t)L1triggerbits[i] << (i%8);
    }

    //L1TriggerAlgos
    const DecisionWord &L1triggeralgos = L1trigger->decisionWord();

    for(int i = 0  ; i < 128 ; i++) {
        trigger_level1[i] = 0;
    }

    for(unsigned i = 0 ; i < min(unsigned(L1triggeralgos.size()), unsigned(1024)) ; i++) {
        trigger_level1[i/8] |= (Byte_t)L1triggeralgos[i] << (i%8);
    }

    lumi_l1techprescaletable = (L1trigger->gtFdlWord()).gtPrescaleFactorIndexTech();
    lumi_l1algoprescaletable = (L1trigger->gtFdlWord()).gtPrescaleFactorIndexAlgo();
    lumi_hltprescaletable = HLTConfiguration.prescaleSet(iEvent, iSetup);
    //if (cdebug) {
    //    cout<<"lumi_l1techprescaletable = "<<lumi_l1techprescaletable<<endl;
    //    cout<<"lumi_l1algoprescaletable = "<<lumi_l1algoprescaletable<<endl;
    //    cout<<"lumi_hltprescaletable = "<<lumi_hltprescaletable<<endl;
    //    //cout<<"HLTConfiguration.prescaleValues(iEvent, iSetup,\"HLT_Mu17_Mu8_v16\").first = "<<HLTConfiguration.prescaleValues(iEvent, iSetup,"HLT_Mu17_Mu8_v16").first<<endl;
    //    //cout<<"HLTConfiguration.prescaleValues(iEvent, iSetup,\"HLT_Mu17_Mu8_v16\").second"<<HLTConfiguration.prescaleValues(iEvent, iSetup,"HLT_Mu17_Mu8_v16").second<<"\n"<<endl;
    //}

    //HLTriggerResults
    iEvent.getByLabel(edm::InputTag("TriggerResults", "", cTriggerProcess), HLTrigger);

    for(int i = 0  ; i < 128 ; i++) {
        trigger_HLT[i] = 0;
    }

    for(unsigned i = 0 ; i < min(unsigned(HLTrigger->size()), unsigned(1024)) ; i++) {
        trigger_HLT[i/8] |= (Byte_t)HLTrigger->accept(i) << (i%8);

        if(HLTrigger->accept(i) && find(HLTriggerIndexSelection.begin(), HLTriggerIndexSelection.end(), i) != HLTriggerIndexSelection.end()) {
            takeevent = true;
        }
    }

    //TriggerEvent for matching
    iEvent.getByLabel(edm::InputTag("hltTriggerSummaryAOD", "", cTriggerProcess), HLTriggerEvent);

    if(!cisMiniAOD) {
        iEvent.getByLabel(edm::InputTag("hltTriggerSummaryAOD", "", cTriggerProcess), HLTriggerEvent);
    }

    //cout<<"PASSED! hlt"<<endl;
    if(cisMiniAOD) {
        // TRIGGER MINIAOD ////////////////////////////////////////////////////////////////////////////////
        edm::Handle<edm::TriggerResults> triggerBits;
        edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
        edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;

        iEvent.getByToken(triggerBits_, triggerBits);
        iEvent.getByToken(triggerObjects_, triggerObjects);
        iEvent.getByToken(triggerPrescales_, triggerPrescales);

        const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
        if (cdebug) {
            std::cout << "\n === TRIGGER PATHS === " << std::endl;
            for(unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
                std::cout << "Trigger " << names.triggerName(i) <<
                          ", prescale " << triggerPrescales->getPrescaleForIndex(i) <<
                          ": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)")
                          << std::endl;
            }
        }

        // Trigger Objects
        for(pat::TriggerObjectStandAlone obj : *triggerObjects) {  // note: not "const &" since we want to call unpackPathNames
            obj.unpackPathNames(names);
            if (cdebug) {
                std::cout << "\tTrigger object:  pt " << obj.pt() << ", eta " << obj.eta() << ", phi " << obj.phi() << std::endl;
                // Print trigger object collection and type
                std::cout << "\t   Collection: " << obj.collection() << std::endl;
                std::cout << "\t   Type IDs:   ";
                for (unsigned h = 0; h < obj.filterIds().size(); ++h) {
                    std::cout << " " << obj.filterIds()[h];
                }

               // Print associated trigger filters
               std::cout << "\t   Filters:    ";
               for (unsigned h = 0; h < obj.filterLabels().size(); ++h) {
                   std::cout << " " << obj.filterLabels()[h];
               }
               std::cout << std::endl;
            }
            std::vector<std::string> pathNamesAll  = obj.pathNames(false);
            std::vector<std::string> pathNamesLast = obj.pathNames(true);
            // Print all trigger paths, for each one record also if the object is associated to a 'l3' filter (always true for the
            // definition used in the PAT trigger producer) and if it's associated to the last filter of a successfull path (which
            // means that this object did cause this trigger to succeed; however, it doesn't work on some multi-object triggers)
            if (cdebug) {
                std::cout << "\t   Paths (" << pathNamesAll.size()<<"/"<<pathNamesLast.size()<<"):    ";
                for (unsigned h = 0, n = pathNamesAll.size(); h < n; ++h) {
                    bool isBoth = obj.hasPathName( pathNamesAll[h], true, true );
                    bool isL3   = obj.hasPathName( pathNamesAll[h], false, true );
                    bool isLF   = obj.hasPathName( pathNamesAll[h], true, false );
                    bool isNone = obj.hasPathName( pathNamesAll[h], false, false );
                    std::cout << "   " << pathNamesAll[h];
                    if (isBoth) std::cout << "(L,3)";
                    if (isL3 && !isBoth) std::cout << "(*,3)";
                    if (isLF && !isBoth) std::cout << "(L,*)";
                    if (isNone && !isBoth && !isL3 && !isLF) std::cout << "(*,*)";
                }
                std::cout << std::endl;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
    }

    if(HLTriggerIndexSelection.size() == 0 || !ctrigger) {
        takeevent = true;
    }

    if(!takeevent) {
        return;
    }

    if(crecprimvertex) {
        iEvent.getByToken(verticesToken_, Vertices);

        if(Vertices.isValid()) {
            for(unsigned i = 0 ; i < Vertices->size(); i++) {
                primvertex_x[i] = (*Vertices)[i].x();
                primvertex_y[i] = (*Vertices)[i].y();
                primvertex_z[i] = (*Vertices)[i].z();
                primvertex_info[i] = 0;

                if((*Vertices)[i].isFake()) {
                    primvertex_info[i] |= 1<<0;
                }

                if((*Vertices)[i].isValid()) {
                    primvertex_info[i] |= 1<<1;
                }

                primvertex_chi2[i] = (*Vertices)[i].chi2();
                primvertex_ndof[i] = (*Vertices)[i].ndof();
                primvertex_ntracks[i] = (*Vertices)[i].tracksSize();
                primvertex_cov[i][0] = (*Vertices)[i].covariance(0,0);
                primvertex_cov[i][1] = (*Vertices)[i].covariance(0,1);
                primvertex_cov[i][2] = (*Vertices)[i].covariance(0,2);
                primvertex_cov[i][3] = (*Vertices)[i].covariance(1,1);
                primvertex_cov[i][4] = (*Vertices)[i].covariance(1,2);
                primvertex_cov[i][5] = (*Vertices)[i].covariance(2,2);
                Float_t ptq = 0.;

                for(Vertex::trackRef_iterator it = (*Vertices)[i].tracks_begin() ; it != (*Vertices)[i].tracks_end() ; ++it) {
                    ptq += (*it)->pt() * (*it)->pt();
                }

                primvertex_ptq[i] = ptq;
                primvertex_count++;

                if(primvertex_count == M_primvertexmaxcount) {
                    cerr << "number of vertices > M_primvertexmaxcount. They are missing." << endl;
                    errors |= 1<<14;
                    break;
                }
            }

            if(Vertices->size() > 0) {
                pv_position = (*Vertices)[0].position();
                primvertex = (*Vertices)[0];
            } else {
                return;
            }
        }
    }

    if(cbeamspot) {
        edm::Handle<BeamSpot> TheBeamSpot;
        iEvent.getByToken(beamSpotToken_, TheBeamSpot);

        if(TheBeamSpot.isValid()) {
            beamspot_x = TheBeamSpot->x0();
            beamspot_y = TheBeamSpot->y0();
            beamspot_z = TheBeamSpot->z0();
            beamspot_xwidth = TheBeamSpot->BeamWidthX();
            beamspot_ywidth = TheBeamSpot->BeamWidthY();
            beamspot_zsigma = TheBeamSpot->sigmaZ();
            beamspot_cov[0] = TheBeamSpot->covariance(0,0);
            beamspot_cov[1] = TheBeamSpot->covariance(0,1);
            beamspot_cov[2] = TheBeamSpot->covariance(0,2);
            beamspot_cov[3] = TheBeamSpot->covariance(1,1);
            beamspot_cov[4] = TheBeamSpot->covariance(1,2);
            beamspot_cov[5] = TheBeamSpot->covariance(2,2);
            bs_position = TheBeamSpot->position();
        } else {
            beamspot_x = 0.;
            beamspot_y = 0.;
            beamspot_z = 0.;
            beamspot_xwidth = 0.;
            beamspot_ywidth = 0.;
            beamspot_zsigma = 0.;
            beamspot_cov[0] = 0.;
            beamspot_cov[1] = 0.;
            beamspot_cov[2] = 0.;
            beamspot_cov[3] = 0.;
            beamspot_cov[4] = 0.;
            beamspot_cov[5] = 0.;
        }
    }

    if(crecsupercluster) {
        bool error = false;
        edm::Handle<SuperClusterCollection> SCbarrel;
        // (edm::InputTag("correctedHybridSuperClusters"), SCbarrel);
        iEvent.getByToken(superClustersToken_, SCbarrel);
        edm::Handle<SuperClusterCollection> SCendcap;
        // (edm::InputTag("correctedMulti5x5SuperClustersWithPreshower"), SCendcap);
        iEvent.getByToken(superClustersToken_, SCendcap);

        edm::ESHandle<CaloGeometry> caloGeo;
        edm::Handle<EcalRecHitCollection> barrelHits;
        edm::Handle<EcalRecHitCollection> endcapHits;
        edm::Handle<EcalRecHitCollection> esHits;

        if(crecsuperclusterhit) {
            iSetup.get<CaloGeometryRecord>().get(caloGeo);
            iEvent.getByToken(ebRecHitsToken_, barrelHits);
            iEvent.getByToken(eeRecHitsToken_, endcapHits);
            iEvent.getByToken(esRecHitsToken_, esHits);
        }

        if(SCbarrel.isValid()) {
            for(SuperClusterCollection::const_iterator itsceb = SCbarrel->begin() ; itsceb != SCbarrel->end() ; ++itsceb) {
                const SuperCluster &sceb = *itsceb;
                TVector3 sc(sceb.x() - pv_position.x(), sceb.y() - pv_position.y(), sceb.z() - pv_position.z());
                sc *= sceb.energy()/sc.Mag();

                if(sc.Perp() > crecsuperclusterFilterPtMin && TMath::Abs(sc.PseudoRapidity()) < crecsuperclusterFilterEtaMax) {
                    supercluster_e[supercluster_count] = sceb.energy();
                    supercluster_x[supercluster_count] = sceb.x();
                    supercluster_y[supercluster_count] = sceb.y();
                    supercluster_z[supercluster_count] = sceb.z();
                    supercluster_rawe[supercluster_count] = sceb.rawEnergy();
                    supercluster_phiwidth[supercluster_count] = sceb.phiWidth();
                    supercluster_etawidth[supercluster_count] = sceb.etaWidth();
                    supercluster_nbasiccluster[supercluster_count] = sceb.clustersSize();

                    if(crecsuperclustermember) {
                        supercluster_basicclusterbegin[supercluster_count] = supercluster_basiccluster_count;

                        for(CaloCluster_iterator CC = sceb.clustersBegin() ; CC != sceb.clustersEnd() ; CC++) {
                            supercluster_basiccluster_e[supercluster_basiccluster_count] = (*CC)->energy();
                            supercluster_basiccluster_x[supercluster_basiccluster_count] = (*CC)->x();
                            supercluster_basiccluster_y[supercluster_basiccluster_count] = (*CC)->y();
                            supercluster_basiccluster_z[supercluster_basiccluster_count] = (*CC)->z();
                            supercluster_basiccluster_nhit[supercluster_basiccluster_count] = (*CC)->size();

                            if(crecsuperclusterhit && barrelHits.isValid()) {
                                supercluster_basiccluster_hitbegin[supercluster_basiccluster_count] = supercluster_basiccluster_hit_count;
                                const std::vector< std::pair<DetId, float> > &hits = (*CC)->hitsAndFractions();

                                for(unsigned m = 0 ; m < hits.size() ; m++) {
                                    float energy = -1;

                                    for(unsigned u = 0 ; u < barrelHits->size() ; u++) {
                                        if((*barrelHits)[u].id() == hits[m].first) {
                                            energy = (*barrelHits)[u].energy();
                                            break;
                                        }
                                    }

                                    supercluster_basiccluster_hit_e[supercluster_basiccluster_hit_count] = energy*hits[m].second;
                                    supercluster_basiccluster_hit_x[supercluster_basiccluster_hit_count] = caloGeo->getPosition(hits[m].first).x();
                                    supercluster_basiccluster_hit_y[supercluster_basiccluster_hit_count] = caloGeo->getPosition(hits[m].first).y();
                                    supercluster_basiccluster_hit_z[supercluster_basiccluster_hit_count] = caloGeo->getPosition(hits[m].first).z();
                                    supercluster_basiccluster_hit_count++;

                                    if(supercluster_basiccluster_hit_count == M_superclusterhitmaxcount) {
                                        error = true;
                                        break;
                                    }
                                }
                            }

                            supercluster_basiccluster_count++;

                            if(error || supercluster_basiccluster_count == M_superclustermembermaxcount) {
                                error = true;
                                break;
                            }
                        }
                    }

                    supercluster_count++;

                    if(error || supercluster_count == M_superclustermaxcount) {
                        cerr << "Error filling SuperClusters. They are missing." << endl;
                        errors |= 1<<12;
                        break;
                    }
                }

            }
        }

        //EndCap Clusters
        if(SCendcap.isValid() && !error) {
            for(SuperClusterCollection::const_iterator itscee = SCendcap->begin() ; itscee != SCendcap->end() ; ++itscee) {
                const SuperCluster &scee = *itscee;
                TVector3 sc(scee.x() - pv_position.x(), scee.y() - pv_position.y(), scee.z() - pv_position.z());
                sc *= scee.energy()/sc.Mag();

                if(sc.Perp() > crecsuperclusterFilterPtMin && TMath::Abs(sc.PseudoRapidity()) < crecsuperclusterFilterEtaMax) {
                    supercluster_e[supercluster_count] = scee.energy();
                    supercluster_x[supercluster_count] = scee.x();
                    supercluster_y[supercluster_count] = scee.y();
                    supercluster_z[supercluster_count] = scee.z();
                    supercluster_rawe[supercluster_count] = scee.rawEnergy();
                    supercluster_phiwidth[supercluster_count] = scee.phiWidth();
                    supercluster_etawidth[supercluster_count] = scee.etaWidth();
                    supercluster_nbasiccluster[supercluster_count] = scee.clustersSize();

                    if(crecsuperclustermember) {
                        supercluster_basicclusterbegin[supercluster_count] = supercluster_basiccluster_count;

                        for(CaloCluster_iterator CC = scee.clustersBegin() ; CC != scee.clustersEnd() ; CC++) {
                            supercluster_basiccluster_e[supercluster_basiccluster_count] = (*CC)->energy();
                            supercluster_basiccluster_x[supercluster_basiccluster_count] = (*CC)->x();
                            supercluster_basiccluster_y[supercluster_basiccluster_count] = (*CC)->y();
                            supercluster_basiccluster_z[supercluster_basiccluster_count] = (*CC)->z();
                            supercluster_basiccluster_nhit[supercluster_basiccluster_count] = (*CC)->size();

                            if(crecsuperclusterhit && endcapHits.isValid()) {
                                supercluster_basiccluster_hitbegin[supercluster_basiccluster_count] = supercluster_basiccluster_hit_count;
                                const std::vector< std::pair<DetId, float> > &hits = (*CC)->hitsAndFractions();

                                for(unsigned m = 0 ; m < hits.size() ; m++) {
                                    float energy = -1;

                                    for(unsigned u = 0 ; u < barrelHits->size() ; u++) {
                                        if((*endcapHits)[u].id() == hits[m].first) {
                                            energy = (*endcapHits)[u].energy();
                                            break;
                                        }
                                    }

                                    supercluster_basiccluster_hit_e[supercluster_basiccluster_hit_count] = energy*hits[m].second;
                                    supercluster_basiccluster_hit_x[supercluster_basiccluster_hit_count] = caloGeo->getPosition(hits[m].first).x();
                                    supercluster_basiccluster_hit_y[supercluster_basiccluster_hit_count] = caloGeo->getPosition(hits[m].first).y();
                                    supercluster_basiccluster_hit_z[supercluster_basiccluster_hit_count] = caloGeo->getPosition(hits[m].first).z();
                                    supercluster_basiccluster_hit_count++;

                                    if(supercluster_basiccluster_hit_count == M_superclusterhitmaxcount) {
                                        error = true;
                                        break;
                                    }
                                }
                            }

                            supercluster_basiccluster_count++;

                            if(error || supercluster_basiccluster_count == M_superclustermembermaxcount) {
                                error = true;
                                break;
                            }
                        }

                        //filling Preshower
                        supercluster_esclusterbegin[supercluster_count] = supercluster_escluster_count;

                        for(CaloCluster_iterator CC = scee.preshowerClustersBegin() ; CC != scee.preshowerClustersEnd() ; CC++) {
                            supercluster_escluster_e[supercluster_escluster_count] = (*CC)->energy();
                            supercluster_escluster_x[supercluster_escluster_count] = (*CC)->x();
                            supercluster_escluster_y[supercluster_escluster_count] = (*CC)->y();
                            supercluster_escluster_z[supercluster_escluster_count] = (*CC)->z();
                            supercluster_escluster_nhit[supercluster_escluster_count] = (*CC)->size();

                            if(crecsuperclusterhit && esHits.isValid()) {
                                supercluster_escluster_hitbegin[supercluster_escluster_count] = supercluster_escluster_hit_count;
                                const std::vector< std::pair<DetId, float> > &hits = (*CC)->hitsAndFractions();

                                for(unsigned m = 0 ; m < hits.size() ; m++) {
                                    float energy = -1;

                                    for(unsigned u = 0 ; u < barrelHits->size() ; u++) {
                                        if((*barrelHits)[u].id() == hits[m].first) {
                                            energy = (*esHits)[u].energy();
                                            break;
                                        }
                                    }

                                    supercluster_escluster_hit_e[supercluster_escluster_hit_count] = energy*hits[m].second;
                                    supercluster_escluster_hit_x[supercluster_escluster_hit_count] = caloGeo->getPosition(hits[m].first).x();
                                    supercluster_escluster_hit_y[supercluster_escluster_hit_count] = caloGeo->getPosition(hits[m].first).y();
                                    supercluster_escluster_hit_z[supercluster_escluster_hit_count] = caloGeo->getPosition(hits[m].first).z();
                                    supercluster_escluster_hit_count++;

                                    if(supercluster_escluster_hit_count == M_superclusterhitmaxcount) {
                                        error = true;
                                        break;
                                    }
                                }
                            }

                            supercluster_escluster_count++;

                            if(error || supercluster_escluster_count == M_superclustermembermaxcount) {
                                error = true;
                                break;
                            }
                        }

                    }

                    supercluster_count++;

                    if(error || supercluster_count == M_superclustermaxcount) {
                        cerr << "Error filling SuperClusters. They are missing." << endl;
                        errors |= 1<<12;
                        break;
                    }
                }
            }
        }

    }

    takeevent = false;

    if(crectrack) {
        takeevent = AddTracks(iEvent) || takeevent;
    }

    if(crecmuon && !cisMiniAOD) {
        takeevent = AddMuons(iEvent) || takeevent;
    }

    if(crecmuon && cisMiniAOD) {
        takeevent = AddPatMuons(iEvent) || takeevent;
    }

    if(crecelectron && !cisMiniAOD) {
        takeevent = AddElectrons(iEvent) || takeevent;
    }

    if(crecelectron && cisMiniAOD) {
        takeevent = AddPatElectrons(iEvent) || takeevent;
    }

    if(crecphoton && !cisMiniAOD) {
        takeevent = AddPhotons(iEvent, iSetup) || takeevent;
    }

    if(crecphoton && cisMiniAOD) {
        takeevent = AddPatPhotons(iEvent, iSetup) || takeevent;
    }

    if(crectau && !cisMiniAOD) {
        takeevent = AddTaus(iEvent) || takeevent;
    }

    if(crectau && cisMiniAOD) {
        takeevent = AddPatTaus(iEvent) || takeevent;
    }

    if(crecak4calojet && !cisMiniAOD) {
        takeevent = AddAK4CaloJets(iEvent, iSetup) || takeevent;
    }

    if(crecak4jptjet && !cisMiniAOD) {
        takeevent = AddAK4JPTJets(iEvent, iSetup) || takeevent;
    }

    if(crecak4pfjet && !cisMiniAOD) {
        takeevent = AddAK4PFJets(iEvent, iSetup) || takeevent;
    }

    if(crecak4pfchsjet) {
        takeevent = AddAK4PFCHSJets(iEvent, iSetup) || takeevent;
    }

    if(crecak4pfchspuppijet) {
        takeevent = AddAK4PFCHSPuppiJets(iEvent, iSetup) || takeevent;
    }

    if(crecmusecvertices) {
        AddMuVertices(iEvent);
    }

    if(crecallconversion) {
        AddAllConversions(iEvent);
    }

    if(!takeevent) {
        return;
    }

    if(cisMiniAOD) {
        edm::Handle<double> rho;
        iEvent.getByToken(rhoToken_, rho);
        ak4pfjet_rho = *rho;
    } else if(!cisMiniAOD) {
        edm::Handle<double> rho;
        iEvent.getByLabel(edm::InputTag("kt6PFJets", "rho", "ROOTMAKER"), rho);
        //edm::Handle<double> rho2;
        //iEvent.getByLabel(edm::InputTag("kt6PFJets", "rho", "RECO"), rho2);
        //cout << *rho << " " << *rho2 << endl;
        ak4pfjet_rho = *rho;
        edm::Handle<double> sigma;
        iEvent.getByLabel(edm::InputTag("kt6PFJets", "sigma", "ROOTMAKER"), sigma);
        ak4pfjet_sigma = *sigma;
    }

    if(cMassMuMuMax != cMassMuMuMin) {
        takeevent = false;

        for(unsigned i = 0 ; i < MuVector.size() ; i++) {
            for(unsigned j = 0 ; j < i ; j++) {
                double Mass = (MuVector[i] + MuVector[j]).M();

                if(Mass <= cMassMuMuMax && Mass >= cMassMuMuMin) {
                    takeevent = true;
                    break;
                }
            }

            if(takeevent) {
                break;
            }

            for(unsigned j = 0 ; j < TrackVector.size() ; j++) {
                double Mass = (MuVector[i] + TrackVector[j]).M();

                if(Mass <= cMassMuMuMax && Mass >= cMassMuMuMin) {
                    takeevent = true;
                    break;
                }
            }

            if(takeevent) {
                break;
            }
        }
    }

    if(!takeevent) {
        return;
    }


    if(crecpfmet) {
        if(cisMiniAOD) {
            // mvaMET considering: ele, mu and tau leptons
            edm::Handle<std::vector<reco::PFMET> > mvaMetEMT;
            iEvent.getByLabel(patMVAMetEMTToken_, mvaMetEMT);

            if(cdebug) { cout<<"mini mvaMetEMT.isValid() = "<<mvaMetEMT.isValid()<<endl; }

            if(mvaMetEMT.isValid() && mvaMetEMT->size() > 0) {
                patmvamet_emt_ex = (*mvaMetEMT)[0].px();
                patmvamet_emt_ey = (*mvaMetEMT)[0].py();
                patmvamet_emt_cov_00 = (*mvaMetEMT)[0].getSignificanceMatrix()(0,0);
                patmvamet_emt_cov_01 = (*mvaMetEMT)[0].getSignificanceMatrix()(0,1);
                patmvamet_emt_cov_10 = (*mvaMetEMT)[0].getSignificanceMatrix()(1,0);
                patmvamet_emt_cov_11 = (*mvaMetEMT)[0].getSignificanceMatrix()(1,1);
            } else {
                errors |= 1<<24;
            }

            // mvaMET considering: ele and mu leptons
            edm::Handle<std::vector<reco::PFMET> > mvaMetEM;
            iEvent.getByLabel(patMVAMetEMToken_, mvaMetEM);

            if(cdebug) { cout<<"mini mvaMetEM.isValid() = "<<mvaMetEM.isValid()<<endl; }

            if(mvaMetEM.isValid() && mvaMetEM->size() > 0) {
                patmvamet_em_ex = (*mvaMetEM)[0].px();
                patmvamet_em_ey = (*mvaMetEM)[0].py();
                patmvamet_em_cov_00 = (*mvaMetEM)[0].getSignificanceMatrix()(0,0);
                patmvamet_em_cov_01 = (*mvaMetEM)[0].getSignificanceMatrix()(0,1);
                patmvamet_em_cov_10 = (*mvaMetEM)[0].getSignificanceMatrix()(1,0);
                patmvamet_em_cov_11 = (*mvaMetEM)[0].getSignificanceMatrix()(1,1);
            } else {
                errors |= 1<<24;
            }

            // mvaMET considering: ele and tau leptons
            edm::Handle<std::vector<reco::PFMET> > mvaMetET;
            iEvent.getByLabel(patMVAMetETToken_, mvaMetET);

            if(cdebug) {
                cout<<"mini mvaMetET.isValid() = "<<mvaMetET.isValid()<<endl;
            }

            if(mvaMetET.isValid() && mvaMetET->size() > 0) {
                patmvamet_et_ex = (*mvaMetET)[0].px();
                patmvamet_et_ey = (*mvaMetET)[0].py();
                patmvamet_et_cov_00 = (*mvaMetET)[0].getSignificanceMatrix()(0,0);
                patmvamet_et_cov_01 = (*mvaMetET)[0].getSignificanceMatrix()(0,1);
                patmvamet_et_cov_10 = (*mvaMetET)[0].getSignificanceMatrix()(1,0);
                patmvamet_et_cov_11 = (*mvaMetET)[0].getSignificanceMatrix()(1,1);
            } else {
                errors |= 1<<24;
            }

            // mvaMET considering: mu and tau leptons
            edm::Handle<std::vector<reco::PFMET> > mvaMetMT;
            iEvent.getByLabel(patMVAMetMTToken_, mvaMetMT);

            if(cdebug) {
                cout<<"mini mvaMetMT.isValid() = "<<mvaMetMT.isValid()<<endl;
            }

            if(mvaMetMT.isValid() && mvaMetMT->size() > 0) {
                patmvamet_mt_ex = (*mvaMetMT)[0].px();
                patmvamet_mt_ey = (*mvaMetMT)[0].py();
                patmvamet_mt_cov_00 = (*mvaMetMT)[0].getSignificanceMatrix()(0,0);
                patmvamet_mt_cov_01 = (*mvaMetMT)[0].getSignificanceMatrix()(0,1);
                patmvamet_mt_cov_10 = (*mvaMetMT)[0].getSignificanceMatrix()(1,0);
                patmvamet_mt_cov_11 = (*mvaMetMT)[0].getSignificanceMatrix()(1,1);
            } else {
                errors |= 1<<24;
            }

            // mvaMET considering: tau and tau leptons
            edm::Handle<std::vector<reco::PFMET> > mvaMetTT;
            iEvent.getByLabel(patMVAMetTTToken_, mvaMetTT);

            if(cdebug) {
                cout<<"mini mvaMetTT.isValid() = "<<mvaMetTT.isValid()<<endl;
            }

            if(mvaMetTT.isValid() && mvaMetTT->size() > 0) {
                patmvamet_tt_ex = (*mvaMetTT)[0].px();
                patmvamet_tt_ey = (*mvaMetTT)[0].py();
                patmvamet_tt_cov_00 = (*mvaMetTT)[0].getSignificanceMatrix()(0,0);
                patmvamet_tt_cov_01 = (*mvaMetTT)[0].getSignificanceMatrix()(0,1);
                patmvamet_tt_cov_10 = (*mvaMetTT)[0].getSignificanceMatrix()(1,0);
                patmvamet_tt_cov_11 = (*mvaMetTT)[0].getSignificanceMatrix()(1,1);
            } else {
                errors |= 1<<24;
            }

            edm::Handle<pat::METCollection> pfMetType1;
            //iEvent.getByToken(patMetToken_, pfMetType1);
            iEvent.getByLabel(newMetLabel_, pfMetType1);

            //iEvent.getByLabel( "patMETs", pfMetType1);
            if(cdebug) { cout<<"mini patMetType1.isValid() = "<<pfMetType1.isValid()<<endl; }

            if(pfMetType1.isValid() && pfMetType1->size() > 0) {
                pfmettype1_ex = (*pfMetType1)[0].px();
                pfmettype1_ey = (*pfMetType1)[0].py();
                pfmettype1_cov_00 = (*pfMetType1)[0].getSignificanceMatrix()(0,0);
                pfmettype1_cov_01 = (*pfMetType1)[0].getSignificanceMatrix()(0,1);
                pfmettype1_cov_10 = (*pfMetType1)[0].getSignificanceMatrix()(1,0);
                pfmettype1_cov_11 = (*pfMetType1)[0].getSignificanceMatrix()(1,1);
            } else {
                errors |= 1<<24;
            }

            //
            edm::Handle<pat::METCollection> pfMetPuppiType1;
            iEvent.getByToken(patMetPuppiToken_, pfMetPuppiType1);

            if(cdebug) { cout<<"mini patMetPuppiType1.isValid() = "<<pfMetPuppiType1.isValid()<<endl; }

            if(pfMetPuppiType1.isValid() && pfMetPuppiType1->size() > 0) {
                pfmetpuppitype1_ex = (*pfMetPuppiType1)[0].px();
                pfmetpuppitype1_ey = (*pfMetPuppiType1)[0].py();
            } else {
                errors |= 1<<24;
            }
        } else if(!cisMiniAOD) {
            edm::Handle<reco::PFMETCollection> pfMet;
            iEvent.getByToken(recoMetToken_, pfMet);

            if(cdebug) {
                cout<<"pfMet.isValid() = "<<pfMet.isValid()<<endl;
            }

            if(pfMet.isValid() && pfMet->size() > 0) {
                pfmet_ex = (*pfMet)[0].px();
                pfmet_ey = (*pfMet)[0].py();
            } else {
                errors |= 1<<21;
            }

            edm::Handle<reco::PFMETCollection> pfMetType1;
            iEvent.getByToken(recoMetT1Token_, pfMetType1);

            if(cdebug) { cout<<"pfMetType1.isValid() = "<<pfMetType1.isValid()<<endl; }

            if(pfMetType1.isValid() && pfMetType1->size() > 0) {
                pfmettype1_ex = (*pfMetType1)[0].px();
                pfmettype1_ey = (*pfMetType1)[0].py();
            } else {
                errors |= 1<<24;
            }

            edm::Handle<reco::PFMETCollection> pfMetType0Type1;
            iEvent.getByToken(recoMetT1T0Token_, pfMetType0Type1);

            if(cdebug) { cout<<"pfMetType0Type1.isValid() = "<<pfMetType0Type1.isValid()<<endl; }

            if(pfMetType0Type1.isValid() && pfMetType0Type1->size() > 0) {
                pfmettype0type1_ex = (*pfMetType0Type1)[0].px();
                pfmettype0type1_ey = (*pfMetType0Type1)[0].py();
            } else {
                errors |= 1<<26;
            }
        }
    }

    genweight = 1.;
    numpileupinteractionsminus = -1;
    numpileupinteractions = -1;
    numpileupinteractionsplus = -1;
    numtruepileupinteractions = -1;

    if(cgen || cgenallparticles || cgenak4jets) {
        edm::Handle<GenEventInfoProduct> HEPMC;
        iEvent.getByToken(genInfoToken_, HEPMC);

        if(HEPMC.isValid()) {
            genweight = HEPMC->weight();
            genid1 = HEPMC->pdf()->id.first;
            genx1 = HEPMC->pdf()->x.first;
            genid2 = HEPMC->pdf()->id.second;
            genx2 = HEPMC->pdf()->x.second;
            genScale = HEPMC->qScale();
        }

        edm::Handle<vector<PileupSummaryInfo> > PUInfo;
        iEvent.getByToken(puInfoToken_, PUInfo);

        if(PUInfo.isValid()) {
            for(vector<PileupSummaryInfo>::const_iterator PVI = PUInfo->begin(); PVI != PUInfo->end(); ++PVI) {
                int BX = PVI->getBunchCrossing();

                if(BX == -1) {
                    numpileupinteractionsminus = PVI->getPU_NumInteractions();
                } else if(BX == 0) {
                    numpileupinteractions = PVI->getPU_NumInteractions();
                    numtruepileupinteractions = PVI->getTrueNumInteractions();
                } else if(BX == 1) {
                    numpileupinteractionsplus = PVI->getPU_NumInteractions();
                }
            }
        }

        edm::Handle<GenMETCollection> GenMetCalo;
        iEvent.getByLabel(edm::InputTag("genMetCalo"), GenMetCalo);
        edm::Handle<GenMETCollection> GenMetTrue;
        iEvent.getByLabel(edm::InputTag("genMetTrue"), GenMetTrue);

        if(GenMetCalo.isValid() && GenMetCalo->size() > 0) {
            genmetcalo_ex = (*GenMetCalo)[0].px();
            genmetcalo_ey = (*GenMetCalo)[0].py();
        } else {
            genmetcalo_ex = 0.;
            genmetcalo_ey = 0.;
            errors |= 1<<18;
        }

        if(GenMetTrue.isValid() && GenMetTrue->size() > 0) {
            genmettrue_ex = (*GenMetTrue)[0].px();
            genmettrue_ey = (*GenMetTrue)[0].py();
        } else {
            genmettrue_ex = 0.;
            genmettrue_ey = 0.;
            errors |= 1<<19;
        }

    }

    if(cgenallparticles) {
        edm::Handle<GenParticleCollection> GenParticles;
        iEvent.getByToken(genSimParticlesToken_, GenParticles);

        if(!GenParticles.isValid()) { iEvent.getByToken(genParticlesToken_, GenParticles); }

        if(GenParticles.isValid()) {
            GenPartons.clear();

            for(unsigned i = 0 ; i < GenParticles->size() ; i++) {
                if((abs((*GenParticles)[i].pdgId()) <= 5 || (*GenParticles)[i].pdgId() == 21) && (*GenParticles)[i].pt() > 10.) {
                    GenPartons.push_back((*GenParticles)[i]);
                }

                genallparticles_e[genallparticles_count] = (*GenParticles)[i].energy();
                genallparticles_px[genallparticles_count] = (*GenParticles)[i].px();
                genallparticles_py[genallparticles_count] = (*GenParticles)[i].py();
                genallparticles_pz[genallparticles_count] = (*GenParticles)[i].pz();
                genallparticles_vx[genallparticles_count] = (*GenParticles)[i].vx();
                genallparticles_vy[genallparticles_count] = (*GenParticles)[i].vy();
                genallparticles_vz[genallparticles_count] = (*GenParticles)[i].vz();
                genallparticles_pdgid[genallparticles_count] = (*GenParticles)[i].pdgId();
                genallparticles_status[genallparticles_count] = (*GenParticles)[i].status();

                genallparticles_count++;

                if(genallparticles_count == M_genallparticlesmaxcount) {
                    cerr << "Number of genallparticles > M_genallparticlesmaxcount. They are missing." << endl;
                    errors |= 1<<15;
                    break;
                }

            }

            for(unsigned i = 0 ; i < GenParticles->size() ; i++) {
                genallparticles_motherbeg[i] = genallparticlesmother_count;
                genallparticles_daughterbeg[i] = genallparticlesdaughter_count;

                for(unsigned j = 0 ; j < (*GenParticles)[i].numberOfMothers() ; j++) {
                    genallparticles_mothers[genallparticlesmother_count] = FindGenParticle((*GenParticles)[i].mother(j));
                    genallparticlesmother_count++;

                    if(genallparticlesmother_count == M_genmotherdaughtermaxcount) {
                        break;
                    }
                }

                for(unsigned j = 0 ; j < (*GenParticles)[i].numberOfDaughters() ; j++) {
                    genallparticles_daughters[genallparticlesdaughter_count] = FindGenParticle((*GenParticles)[i].daughter(j));
                    genallparticlesdaughter_count++;

                    if(genallparticlesdaughter_count == M_genmotherdaughtermaxcount) {
                        break;
                    }
                }

                if(genallparticlesmother_count >= M_genmotherdaughtermaxcount) {
                    cerr << "Too many mothers" << endl;
                    errors |= 1<<16;
                    break;
                }

                if(genallparticlesdaughter_count >= M_genmotherdaughtermaxcount) {
                    cerr << "Too many daughters" << endl;
                    errors |= 1<<17;
                    break;
                }
            }
        }
    }

    if(cgen) {
        edm::Handle<GenParticleCollection> GenParticles;
        iEvent.getByToken(genSimParticlesToken_, GenParticles);

        if(!GenParticles.isValid()) { iEvent.getByToken(genParticlesToken_, GenParticles); }

        if(GenParticles.isValid()) {
            GenPartons.clear();

            if(cdebug) { cout<<"GenParticles->size() = "<<GenParticles->size()<<endl; }

            for(unsigned i = 0 ; i < GenParticles->size() ; i++) {
                if((abs((*GenParticles)[i].pdgId()) <= 5 || (*GenParticles)[i].pdgId() == 21) && (*GenParticles)[i].pt() > 10.) { GenPartons.push_back((*GenParticles)[i]); }

                bool fill = false;

                if(
                    (abs((*GenParticles)[i].pdgId()) >= 11 && abs((*GenParticles)[i].pdgId()) <= 16)   //leptons
                    || (abs((*GenParticles)[i].pdgId()) <= 3 && (*GenParticles)[i].pt() > 15.)  //u,d,s
                    || (abs((*GenParticles)[i].pdgId()) >= 4 && abs((*GenParticles)[i].pdgId()) <= 8)   //c,t,b,t'
                    || ((*GenParticles)[i].pdgId() == 21 && (*GenParticles)[i].pt() > 10.) //gluon
                    || abs((*GenParticles)[i].pdgId()) == 24  //W
                    || ((*GenParticles)[i].pdgId() == 22 && (*GenParticles)[i].status() == 3)//gamma from ME
                    || (*GenParticles)[i].pdgId() == 23 //Z
                    || (*GenParticles)[i].pdgId() == 25 //h
                    || (*GenParticles)[i].pdgId() == 35 //H
                    || (*GenParticles)[i].pdgId() == 36 //A
                    || abs((*GenParticles)[i].pdgId()) == 37  //H+, H-
                    || (*GenParticles)[i].pdgId() == 32 //Z'
                ) { fill = true; }
                else if((*GenParticles)[i].pdgId() == 22 && (*GenParticles)[i].status() == 1 && (*GenParticles)[i].pt() > 10. && HasAnyMother(& (*GenParticles)[i], 111) == 0) { fill = true; }
                else if((*GenParticles)[i].pdgId() == 111 && (*GenParticles)[i].pt() > 10. && HasAnyMother(& (*GenParticles)[i], 111) == 0) { fill = true; }

                if(fill) {
                    genparticles_e[genparticles_count] = (*GenParticles)[i].energy();
                    genparticles_px[genparticles_count] = (*GenParticles)[i].px();
                    genparticles_py[genparticles_count] = (*GenParticles)[i].py();
                    genparticles_pz[genparticles_count] = (*GenParticles)[i].pz();
                    genparticles_vx[genparticles_count] = (*GenParticles)[i].vx();
                    genparticles_vy[genparticles_count] = (*GenParticles)[i].vy();
                    genparticles_vz[genparticles_count] = (*GenParticles)[i].vz();
                    genparticles_pdgid[genparticles_count] = (*GenParticles)[i].pdgId();
                    genparticles_status[genparticles_count] = (*GenParticles)[i].status();
                    pair<Int_t, Int_t> motherinfo = HasAnyMother(& (*GenParticles)[i], testids);
                    genparticles_info[genparticles_count] = motherinfo.first;
                    genparticles_indirectmother[genparticles_count] = motherinfo.second;
                    //if (cdebug) {
                    //    cout<<"(*GenParticles)["<<i<<"].pdgId() = "<<(*GenParticles)[i].pdgId()<<endl;
                    //    cout<<"genparticles_info[genparticles_count] = "<<genparticles_info[genparticles_count]<<endl;
                    //    cout<<"genparticles_indirectmother[genparticles_count] = "<<genparticles_indirectmother[genparticles_count]<<"\n"<<endl;
                    //}
                    genparticles_count++;
                }
            }

            if(cdebug) { cout << "Total gen particles: " << genparticles_count << endl; }
        }
    }

    if(cgenak4jets) {
        edm::Handle<GenJetCollection> GenAK4Jets;
        iEvent.getByToken(genJetsToken_, GenAK4Jets);

        if(GenAK4Jets.isValid()) {
            for(GenJetCollection::const_iterator it = GenAK4Jets->begin() ; it != GenAK4Jets->end() ; ++it) {
                if(it->pt() > 15.) {
                    genak4jet_e[genak4jet_count] = it->energy();
                    genak4jet_px[genak4jet_count] = it->px();
                    genak4jet_py[genak4jet_count] = it->py();
                    genak4jet_pz[genak4jet_count] = it->pz();
                    genak4jet_einvisible[genak4jet_count] = it->invisibleEnergy();
                    genak4jet_flavour[genak4jet_count] = 0;
                    genak4jet_info[genak4jet_count] = 0;
                    double ptmax = 0;

                    for(size_t j = 0 ; j < GenPartons.size() ; ++j) {
                        if(DR(GenPartons[j], *it) < 0.5) {
                            if(GenPartons[j].pdgId() == 5) { genak4jet_info[genak4jet_count] |= 1<<0; }
                            else if(GenPartons[j].pdgId() == -5) { genak4jet_info[genak4jet_count] |= 1<<1; }
                            else if(GenPartons[j].pdgId() == 4) { genak4jet_info[genak4jet_count] |= 1<<2; }
                            else if(GenPartons[j].pdgId() == -4) { genak4jet_info[genak4jet_count] |= 1<<3; }
                            else if(abs(GenPartons[j].pdgId()) <= 3) { genak4jet_info[genak4jet_count] |= 1<<4; }
                            else if(GenPartons[j].pdgId() == 21) { genak4jet_info[genak4jet_count] |= 1<<5; }

                            if(GenPartons[j].pt() > ptmax) {
                                ptmax = GenPartons[j].pt();
                                genak4jet_flavour[genak4jet_count] = GenPartons[j].pdgId();
                            }

                        }
                    }

                    genak4jet_count++;

                    if(genak4jet_count == M_genjetmaxcount) {
                        cerr << "Number of genak4jet > M_genjetmaxcount. They are missing." << endl;
                        errors |= 1<<25;
                        break;
                    }
                }
            }
        }
    }

    if(takeevent) {
        lumi_eventsfiltered++;
        tree->Fill();
    }
}

pair<Int_t, Int_t> RootMaker::HasAnyMother(const GenParticle *particle, vector<int> ids) {
    Int_t motherid = 0;
    vector<unsigned> bknummother;
    vector<const GenParticle *> bkparticle;
    bknummother.reserve(10);
    bkparticle.reserve(10);
    int level = 0;
    bkparticle.push_back(particle);
    bknummother.push_back(0);

    vector<int>::const_iterator it;
    UInt_t result = 0;
    unsigned j = 0;

    while(true) {
        if(j == bkparticle[level]->numberOfMothers()) {
            level--;

            if(level == -1) { break; }

            j = bknummother[level];
            bkparticle.resize(level+1);
            bknummother.resize(level+1);
            continue;
        }

        if(motherid == 0 && bkparticle[level]->mother(j)->pdgId() != particle->pdgId()) { motherid = bkparticle[level]->mother(j)->pdgId(); }

        it = find(ids.begin(), ids.end(), bkparticle[level]->mother(j)->pdgId());

        if(it != ids.end()) { result |= 1<< (it-ids.begin()); }

        if(bkparticle[level]->mother(j)->numberOfMothers() > 0) {
            bknummother[level] = j+1;
            bkparticle.push_back(dynamic_cast<const GenParticle *>(bkparticle[level]->mother(j)));
            bknummother.push_back(0);
            j = 0;
            level++;
            continue;
        }

        j++;
    }

    return (pair<Int_t, Int_t> (result, motherid));
}

Int_t RootMaker::HasAnyMother(const GenParticle *particle, int id) {
    vector<unsigned> bknummother;
    vector<const GenParticle *> bkparticle;
    bknummother.reserve(10);
    bkparticle.reserve(10);
    int level = 0;
    bkparticle.push_back(particle);
    bknummother.push_back(0);

    unsigned j = 0;

    while(true) {
        if(j == bkparticle[level]->numberOfMothers()) {
            level--;

            if(level == -1) { return (0); }

            j = bknummother[level];
            bkparticle.resize(level+1);
            bknummother.resize(level+1);
            continue;
        }

        if(bkparticle[level]->mother(j)->pdgId() == id) { return (2); }

        if(abs(bkparticle[level]->mother(j)->pdgId()) == abs(id)) { return (1); }

        if(bkparticle[level]->mother(j)->numberOfMothers() > 0) {
            bknummother[level] = j+1;
            bkparticle.push_back(dynamic_cast<const GenParticle *>(bkparticle[level]->mother(j)));
            bknummother.push_back(0);
            j = 0;
            level++;
            continue;
        }

        j++;
    }

    return (0);
}

void RootMaker::endJob() {
    if(cdebug) { cout<<"endJob..."<<endl; }
}

UInt_t RootMaker::FindGenParticle(const Candidate *particle) {
    //if(cdebug) cout<<"FindGenParticle..."<<endl;
    for(unsigned i = 0 ; i < genallparticles_count ; i++) {
        if(particle->pdgId() == genallparticles_pdgid[i] &&
                particle->status() == genallparticles_status[i] &&
                float(particle->energy()) == genallparticles_e[i] &&
                float(particle->px()) == genallparticles_px[i] &&
                float(particle->py()) == genallparticles_py[i] &&
                float(particle->pz()) == genallparticles_pz[i]) {
            return (i);
        }
    }

    return (genallparticles_count);
}

math::XYZPoint RootMaker::PositionOnECalSurface(TransientTrack &trTrack) {
    if(cdebug) { cout<<"PositionOnECalSurface..."<<endl; }

    math::XYZPoint ecalPosition(0.,0.,0.);
    const FreeTrajectoryState myTSOS = trTrack.initialFreeState();
    TrajectoryStateOnSurface stateAtECal = propagatorWithMaterial->propagate(myTSOS, *ecalBarrel);

    if(stateAtECal.isValid() && stateAtECal.globalPosition().eta() > 1.479) {
        stateAtECal= propagatorWithMaterial->propagate(myTSOS, *ecalPositiveEtaEndcap);
    }

    if(stateAtECal.isValid() && stateAtECal.globalPosition().eta() < -1.479) {
        stateAtECal= propagatorWithMaterial->propagate(myTSOS, *ecalNegativeEtaEndcap);
    }

    if(stateAtECal.isValid()) {
        ecalPosition = stateAtECal.globalPosition();
    }

    return (ecalPosition);
}

bool RootMaker::AddMuons(const edm::Event &iEvent) {
    if(cdebug) { cout<<"AddMuons..."<<endl; }

    int NumGood = 0;
    edm::Handle<reco::MuonCollection> Muons;
    iEvent.getByToken(recoMuonsToken_, Muons);

    if(cdebug) { cout<<"Muons.isValid() = "<<Muons.isValid()<<endl; }

    if(cdebug) { cout<<"Muons->size() = "<<Muons->size()<<endl; }

    if(Muons.isValid()) {
        for(unsigned i = 0 ; i < Muons->size() ; i++) {
            const reco::Muon &themu = (*Muons)[i];
            muon_px[muon_count] = themu.px();
            muon_py[muon_count] = themu.py();
            muon_pz[muon_count] = themu.pz();
            muon_pt[muon_count] = themu.pt();
            muon_phi[muon_count] = themu.phi();
            muon_eta[muon_count] = themu.eta();

            if(themu.globalTrack().isNonnull()) {
                muon_pterror[muon_count] = themu.globalTrack()->ptError();
                muon_chi2[muon_count] = themu.globalTrack()->chi2();
                muon_ndof[muon_count] = themu.globalTrack()->ndof();
                muon_numvalidmuonhits[muon_count] = themu.globalTrack()->hitPattern().numberOfValidMuonHits();
            } else {
                muon_pterror[muon_count] = -1.;
                muon_chi2[muon_count] = -1.;
                muon_ndof[muon_count] = 0;
                muon_numvalidmuonhits[muon_count] = -1.;
            }

            muon_isolationr3track[muon_count]  = themu.isolationR03().sumPt;
            muon_isolationr3ntrack[muon_count] = themu.isolationR03().nTracks;
            muon_isolationr3ecal[muon_count]   = themu.isolationR03().emEt;
            muon_isolationr3hcal[muon_count]   = themu.isolationR03().hadEt;

            if(cdebug) { cout<<"themu.isPFIsolationValid() = "<<endl; }

            if(themu.isPFIsolationValid()) {

                const reco::MuonPFIsolation pfisor03 = themu.pfIsolationR03();
                const reco::MuonPFIsolation pfisor04 = themu.pfIsolationR04();

                muon_pfisolationr3_sumchargedhadronpt[muon_count] = pfisor03.sumChargedHadronPt;
                muon_pfisolationr3_sumchargedparticlept[muon_count] = pfisor03.sumChargedParticlePt;
                muon_pfisolationr3_sumneutralhadronet[muon_count] = pfisor03.sumNeutralHadronEt;
                muon_pfisolationr3_sumphotonet[muon_count] = pfisor03.sumPhotonEt;
                muon_pfisolationr3_sumneutralhadronethighthreshold[muon_count] = pfisor03.sumNeutralHadronEtHighThreshold;
                muon_pfisolationr3_sumphotonethighthreshold[muon_count] = pfisor03.sumPhotonEtHighThreshold;
                muon_pfisolationr3_sumpupt[muon_count] = pfisor03.sumPUPt;

                muon_pfisolationr4_sumchargedhadronpt[muon_count] = pfisor04.sumChargedHadronPt;
                muon_pfisolationr4_sumchargedparticlept[muon_count] = pfisor04.sumChargedParticlePt;
                muon_pfisolationr4_sumneutralhadronet[muon_count] = pfisor04.sumNeutralHadronEt;
                muon_pfisolationr4_sumphotonet[muon_count] = pfisor04.sumPhotonEt;
                muon_pfisolationr4_sumneutralhadronethighthreshold[muon_count] = pfisor04.sumNeutralHadronEtHighThreshold;
                muon_pfisolationr4_sumphotonethighthreshold[muon_count] = pfisor04.sumPhotonEtHighThreshold;
                muon_pfisolationr4_sumpupt[muon_count] = pfisor04.sumPUPt;

            }

            muon_ecalenergy[muon_count] = themu.calEnergy().em;
            muon_hcalenergy[muon_count] = themu.calEnergy().had;
            muon_charge[muon_count] = themu.charge();
            muon_type[muon_count] = 0;
            muon_trackermuonquality[muon_count] = 0;

            if(themu.isGlobalMuon()) { muon_type[muon_count] |= 1 << 0; }

            if(themu.isTrackerMuon()) { muon_type[muon_count] |= 1 << 1; }

            if(themu.isStandAloneMuon()) { muon_type[muon_count] |= 1 << 2; }

            if(themu.isCaloMuon()) { muon_type[muon_count] |= 1 << 3; }

            if(themu.isPFMuon()) { muon_type[muon_count] |= 1 << 6; }

            {
                using namespace muon;

                if(isGoodMuon(themu, All))  { muon_trackermuonquality[muon_count] |= 1 << 0; }

                if(isGoodMuon(themu, AllGlobalMuons)) { muon_trackermuonquality[muon_count] |= 1 << 1; }

                if(isGoodMuon(themu, AllStandAloneMuons)) { muon_trackermuonquality[muon_count] |= 1 << 2; }

                if(isGoodMuon(themu, AllTrackerMuons)) { muon_trackermuonquality[muon_count] |= 1 << 3; }

                if(isGoodMuon(themu, TrackerMuonArbitrated)) { muon_trackermuonquality[muon_count] |= 1 << 4; }

                if(isGoodMuon(themu, AllArbitrated)) { muon_trackermuonquality[muon_count] |= 1 << 5; }

                if(isGoodMuon(themu, GlobalMuonPromptTight)) { muon_trackermuonquality[muon_count] |= 1 << 6; }

                if(isGoodMuon(themu, TMLastStationLoose)) { muon_trackermuonquality[muon_count] |= 1 << 7; }

                if(isGoodMuon(themu, TMLastStationTight)) { muon_trackermuonquality[muon_count] |= 1 << 8; }

                if(isGoodMuon(themu, TM2DCompatibilityLoose)) { muon_trackermuonquality[muon_count] |= 1 << 9; }

                if(isGoodMuon(themu, TM2DCompatibilityTight)) { muon_trackermuonquality[muon_count] |= 1 << 10; }

                if(isGoodMuon(themu, TMOneStationLoose)) { muon_trackermuonquality[muon_count] |= 1 << 11; }

                if(isGoodMuon(themu, TMOneStationTight)) { muon_trackermuonquality[muon_count] |= 1 << 12; }

                if(isGoodMuon(themu, TMLastStationOptimizedLowPtLoose)) { muon_trackermuonquality[muon_count] |= 1 << 13; }

                if(isGoodMuon(themu, TMLastStationOptimizedLowPtTight)) { muon_trackermuonquality[muon_count] |= 1 << 14; }

                if(isGoodMuon(themu, GMTkChiCompatibility)) { muon_trackermuonquality[muon_count] |= 1 << 15; }

                if(isGoodMuon(themu, GMStaChiCompatibility)) { muon_trackermuonquality[muon_count] |= 1 << 16; }

                if(isGoodMuon(themu, GMTkKinkTight)) { muon_trackermuonquality[muon_count] |= 1 << 17; }

                if(isGoodMuon(themu, TMLastStationAngLoose)) { muon_trackermuonquality[muon_count] |= 1 << 18; }

                if(isGoodMuon(themu, TMLastStationAngTight)) { muon_trackermuonquality[muon_count] |= 1 << 19; }

                if(isGoodMuon(themu, TMOneStationAngLoose)) { muon_trackermuonquality[muon_count] |= 1 << 20; }

                if(isGoodMuon(themu, TMOneStationAngTight)) { muon_trackermuonquality[muon_count] |= 1 << 21; }

                if(isGoodMuon(themu, TMLastStationOptimizedBarrelLowPtLoose)) { muon_trackermuonquality[muon_count] |= 1 << 22; }

                if(isGoodMuon(themu, TMLastStationOptimizedBarrelLowPtTight)) { muon_trackermuonquality[muon_count] |= 1 << 23; }

                muon_numchambers[muon_count] = themu.numberOfChambers();
                muon_numchamberswithsegments[muon_count] = themu.numberOfMatches(reco::Muon::SegmentAndTrackArbitration);
                muon_nummatchedstations[muon_count] = themu.numberOfMatchedStations();
            }

            if(themu.time().direction() == 1) {
                muon_trackermuonquality[muon_count] |= 1<<30;
            } else if(themu.time().direction() == -1) {
                muon_trackermuonquality[muon_count] |= 1<<31;
            }

            TrackRef innertrack = themu.innerTrack();

            if(innertrack.isNonnull()) {
                muon_type[muon_count] |= 1 << 4;
                int numvtx = getPrimVertex(*innertrack);
                TransientTrack TTrack = TTrackBuilder->build(innertrack);
                TrajectoryStateClosestToPoint TTrackState;

                if(numvtx == -1) {
                    TTrackState = TTrack.trajectoryStateClosestToPoint(GlobalPoint(pv_position.x(), pv_position.y(), pv_position.z()));
                } else {
                    TTrackState = TTrack.trajectoryStateClosestToPoint(GlobalPoint(primvertex_x[numvtx], primvertex_y[numvtx], primvertex_z[numvtx]));
                }

                edm::Handle<edm::ValueMap<DeDxData> > dEdxharmonic2;
                iEvent.getByToken(dharmonicToken_, dEdxharmonic2);

                muon_innertrack_vtx[muon_count] = numvtx;
                muon_innertrack_px[muon_count] = innertrack->px();
                muon_innertrack_py[muon_count] = innertrack->py();
                muon_innertrack_pz[muon_count] = innertrack->pz();
                muon_innertrack_closestpointx[muon_count] = TTrackState.position().x();
                muon_innertrack_closestpointy[muon_count] = TTrackState.position().y();
                muon_innertrack_closestpointz[muon_count] = TTrackState.position().z();
                //muon_innertrack_dxy[muon_count]    = TTrackState.perigeeParameters().transverseImpactParameter();
                //muon_innertrack_dxyerr[muon_count] = TTrackState.perigeeError().transverseImpactParameterError();
                //muon_innertrack_dz[muon_count]     = TTrackState.perigeeParameters().longitudinalImpactParameter();
                //muon_innertrack_dzerr[muon_count]  = TTrackState.perigeeError().longitudinalImpactParameterError();
                muon_innertrack_dxy[muon_count]    = innertrack->dxy(pv_position);
                muon_innertrack_dxyerr[muon_count] = innertrack->dxyError();
                muon_innertrack_dz[muon_count]     = innertrack->dz(pv_position);
                muon_innertrack_dzerr[muon_count]  = innertrack->dzError();
                muon_innertrack_chi2[muon_count]   = innertrack->chi2();
                muon_innertrack_ndof[muon_count]   = innertrack->ndof();
                muon_innertrack_charge[muon_count] = innertrack->charge();
                muon_innertrack_nhits[muon_count] = innertrack->numberOfValidHits();
                muon_innertrack_nmissinghits[muon_count] = innertrack->numberOfLostHits();
                muon_innertrack_npixelhits[muon_count] = innertrack->hitPattern().numberOfValidPixelHits();
                muon_innertrack_npixellayers[muon_count] = innertrack->hitPattern().pixelLayersWithMeasurement();
                muon_innertrack_nstriplayers[muon_count] = innertrack->hitPattern().stripLayersWithMeasurement();

                if(dEdxharmonic2.isValid()) {
                    muon_innertrack_dedxharmonic2[muon_count] = (*dEdxharmonic2)[innertrack].dEdx();
                } else {
                    muon_innertrack_dedxharmonic2[muon_count] = -1.;
                }

                math::XYZPoint ecalPos = PositionOnECalSurface(TTrack);
                muon_innertrack_outerx[muon_count] = ecalPos.x();
                muon_innertrack_outery[muon_count] = ecalPos.y();
                muon_innertrack_outerz[muon_count] = ecalPos.z();
            }

            TrackRef muontrack = themu.outerTrack();

            if(muontrack.isNonnull()) {
                muon_type[muon_count] |= 1 << 5;
                muon_outertrack_px[muon_count] = muontrack->px();
                muon_outertrack_py[muon_count] = muontrack->py();
                muon_outertrack_pz[muon_count] = muontrack->pz();
                muon_outertrack_hits[muon_count] = muontrack->numberOfValidHits();
                muon_outertrack_missinghits[muon_count] = muontrack->numberOfLostHits();
                muon_outertrack_chi2[muon_count] = muontrack->chi2();
                muon_outertrack_ndof[muon_count] = muontrack->ndof();
            }

            muon_trigger[muon_count] = GetTrigger(themu, muontriggers);
            muon_count++;

            if(muon_count == M_muonmaxcount) {
                cerr << "number of muon > M_muonmaxcount. They are missing." << endl;
                errors |= 1<<0;
                break;
            }

            if(themu.isGlobalMuon() && themu.isTrackerMuon() && themu.pt() >= cMuPtMin && fabs(themu.eta()) <= cMuEtaMax && themu.isolationR03().sumPt/themu.pt() <= cMuTrackIso) {
                NumGood++;
                double energy = sqrt(pow(muon_px[i],2) + pow(muon_py[i],2) + pow(muon_pz[i],2));
                MuVector.push_back(TLorentzVector(muon_px[i], muon_py[i], muon_pz[i], energy));

            }
        }
    }

    if(NumGood >= cMuNum) {
        return (true);
    }

    return (false);
}

bool RootMaker::AddPatMuons(const edm::Event &iEvent) {
    if(cdebug) { cout<<"AddPatMuons..."<<endl; }

    int NumGood = 0;
    edm::Handle<pat::MuonCollection> Muons;
    iEvent.getByToken(patMuonsToken_, Muons);

    if(cdebug) {
        cout<<"pat Muons.isValid() = "<<Muons.isValid()<<endl;
        cout<<"pat Muons->size() = "<<Muons->size()<<endl;
    }

    if(Muons.isValid()) {
        for(unsigned i = 0 ; i < Muons->size() ; i++) {
            const pat::Muon &themu = (*Muons)[i];
            muon_muID[muon_count] = -1;

            if (themu.isTightMuon((*Vertices)[0])) muon_muID[muon_count] = 4;
            else if (themu.isMediumMuon()) muon_muID[muon_count] = 3;
            else if (themu.isLooseMuon()) muon_muID[muon_count] = 2;
            else muon_muID[muon_count] = 0;

            if (cdebug) cout<<"muID = "<<muon_muID[muon_count]<<endl;

            muon_px[muon_count] = themu.px();
            muon_py[muon_count] = themu.py();
            muon_pz[muon_count] = themu.pz();
            muon_pt[muon_count] = themu.pt();
            muon_phi[muon_count] = themu.phi();
            muon_eta[muon_count] = themu.eta();

            if(themu.globalTrack().isNonnull()) {
                muon_pterror[muon_count] = themu.globalTrack()->ptError();
                muon_chi2[muon_count] = themu.globalTrack()->chi2();
                muon_ndof[muon_count] = themu.globalTrack()->ndof();
                muon_numvalidmuonhits[muon_count] = themu.globalTrack()->hitPattern().numberOfValidMuonHits();
            } else {
                muon_pterror[muon_count] = -1.;
                muon_chi2[muon_count] = -1.;
                muon_ndof[muon_count] = 0;
                muon_numvalidmuonhits[muon_count] = -1.;
            }

            muon_dB[muon_count] = themu.dB();
            muon_isolationr3track[muon_count]  = themu.isolationR03().sumPt;
            muon_isolationr3ntrack[muon_count] = themu.isolationR03().nTracks;
            muon_isolationr3ecal[muon_count]   = themu.isolationR03().emEt;
            muon_isolationr3hcal[muon_count]   = themu.isolationR03().hadEt;

            if(cdebug) { cout<<"themu.isPFIsolationValid() = "<<themu.isPFIsolationValid()<<endl; }

            if(themu.isPFIsolationValid()) {
                const reco::MuonPFIsolation pfisor03 = themu.pfIsolationR03();
                const reco::MuonPFIsolation pfisor04 = themu.pfIsolationR04();
                muon_pfisolationr3_sumchargedhadronpt[muon_count] = pfisor03.sumChargedHadronPt;
                muon_pfisolationr3_sumchargedparticlept[muon_count] = pfisor03.sumChargedParticlePt;
                muon_pfisolationr3_sumneutralhadronet[muon_count] = pfisor03.sumNeutralHadronEt;
                muon_pfisolationr3_sumphotonet[muon_count] = pfisor03.sumPhotonEt;
                muon_pfisolationr3_sumneutralhadronethighthreshold[muon_count] = pfisor03.sumNeutralHadronEtHighThreshold;
                muon_pfisolationr3_sumphotonethighthreshold[muon_count] = pfisor03.sumPhotonEtHighThreshold;
                muon_pfisolationr3_sumpupt[muon_count] = pfisor03.sumPUPt;
                muon_pfisolationr4_sumchargedhadronpt[muon_count] = pfisor04.sumChargedHadronPt;
                muon_pfisolationr4_sumchargedparticlept[muon_count] = pfisor04.sumChargedParticlePt;
                muon_pfisolationr4_sumneutralhadronet[muon_count] = pfisor04.sumNeutralHadronEt;
                muon_pfisolationr4_sumphotonet[muon_count] = pfisor04.sumPhotonEt;
                muon_pfisolationr4_sumneutralhadronethighthreshold[muon_count] = pfisor04.sumNeutralHadronEtHighThreshold;
                muon_pfisolationr4_sumphotonethighthreshold[muon_count] = pfisor04.sumPhotonEtHighThreshold;
                muon_pfisolationr4_sumpupt[muon_count] = pfisor04.sumPUPt;
            }

            muon_ecalenergy[muon_count] = themu.calEnergy().em;
            muon_hcalenergy[muon_count] = themu.calEnergy().had;
            muon_charge[muon_count] = themu.charge();
            muon_type[muon_count] = 0;
            muon_trackermuonquality[muon_count] = 0;

            if(themu.isGlobalMuon()) { muon_type[muon_count] |= 1 << 0; }

            if(themu.isTrackerMuon()) { muon_type[muon_count] |= 1 << 1; }

            if(themu.isStandAloneMuon()) { muon_type[muon_count] |= 1 << 2; }

            if(themu.isCaloMuon()) { muon_type[muon_count] |= 1 << 3; }

            if(themu.isPFMuon()) { muon_type[muon_count] |= 1 << 6; }

            {
                using namespace muon;

                if(isGoodMuon(themu, All)) { muon_trackermuonquality[muon_count] |= 1 << 0; }

                if(isGoodMuon(themu, AllGlobalMuons)) { muon_trackermuonquality[muon_count] |= 1 << 1; }

                if(isGoodMuon(themu, AllStandAloneMuons)) { muon_trackermuonquality[muon_count] |= 1 << 2; }

                if(isGoodMuon(themu, AllTrackerMuons)) { muon_trackermuonquality[muon_count] |= 1 << 3; }

                if(isGoodMuon(themu, TrackerMuonArbitrated)) { muon_trackermuonquality[muon_count] |= 1 << 4; }

                if(isGoodMuon(themu, AllArbitrated)) { muon_trackermuonquality[muon_count] |= 1 << 5; }

                if(isGoodMuon(themu, GlobalMuonPromptTight)) { muon_trackermuonquality[muon_count] |= 1 << 6; }

                if(isGoodMuon(themu, TMLastStationLoose)) { muon_trackermuonquality[muon_count] |= 1 << 7; }

                if(isGoodMuon(themu, TMLastStationTight)) { muon_trackermuonquality[muon_count] |= 1 << 8; }

                if(isGoodMuon(themu, TM2DCompatibilityLoose)) { muon_trackermuonquality[muon_count] |= 1 << 9; }

                if(isGoodMuon(themu, TM2DCompatibilityTight)) { muon_trackermuonquality[muon_count] |= 1 << 10; }

                if(isGoodMuon(themu, TMOneStationLoose)) { muon_trackermuonquality[muon_count] |= 1 << 11; }

                if(isGoodMuon(themu, TMOneStationTight)) { muon_trackermuonquality[muon_count] |= 1 << 12; }

                if(isGoodMuon(themu, TMLastStationOptimizedLowPtLoose)) { muon_trackermuonquality[muon_count] |= 1 << 13; }

                if(isGoodMuon(themu, TMLastStationOptimizedLowPtTight)) { muon_trackermuonquality[muon_count] |= 1 << 14; }

                if(isGoodMuon(themu, GMTkChiCompatibility)) { muon_trackermuonquality[muon_count] |= 1 << 15; }

                if(isGoodMuon(themu, GMStaChiCompatibility)) { muon_trackermuonquality[muon_count] |= 1 << 16; }

                if(isGoodMuon(themu, GMTkKinkTight)) { muon_trackermuonquality[muon_count] |= 1 << 17; }

                if(isGoodMuon(themu, TMLastStationAngLoose)) { muon_trackermuonquality[muon_count] |= 1 << 18; }

                if(isGoodMuon(themu, TMLastStationAngTight)) { muon_trackermuonquality[muon_count] |= 1 << 19; }

                if(isGoodMuon(themu, TMOneStationAngLoose)) { muon_trackermuonquality[muon_count] |= 1 << 20; }

                if(isGoodMuon(themu, TMOneStationAngTight)) { muon_trackermuonquality[muon_count] |= 1 << 21; }

                if(isGoodMuon(themu, TMLastStationOptimizedBarrelLowPtLoose)) { muon_trackermuonquality[muon_count] |= 1 << 22; }

                if(isGoodMuon(themu, TMLastStationOptimizedBarrelLowPtTight)) { muon_trackermuonquality[muon_count] |= 1 << 23; }

                muon_numchambers[muon_count] = themu.numberOfChambers();
                muon_numchamberswithsegments[muon_count] = themu.numberOfMatches(pat::Muon::SegmentAndTrackArbitration);
                muon_nummatchedstations[muon_count] = themu.numberOfMatchedStations();
            }

            if(themu.time().direction() == 1) {
                muon_trackermuonquality[muon_count] |= 1<<30;
            } else if(themu.time().direction() == -1) {
                muon_trackermuonquality[muon_count] |= 1<<31;
            }

            TrackRef innertrack = themu.innerTrack();

            if(innertrack.isNonnull()) {
                muon_type[muon_count] |= 1 << 4;
                int numvtx = getPrimVertex(*innertrack);
                TransientTrack TTrack = TTrackBuilder->build(innertrack);
                TrajectoryStateClosestToPoint TTrackState;

                if(numvtx == -1) {
                    TTrackState = TTrack.trajectoryStateClosestToPoint(GlobalPoint(pv_position.x(), pv_position.y(), pv_position.z()));
                } else {
                    TTrackState = TTrack.trajectoryStateClosestToPoint(GlobalPoint(primvertex_x[numvtx], primvertex_y[numvtx], primvertex_z[numvtx]));
                }

                edm::Handle<edm::ValueMap<DeDxData> > dEdxharmonic2;
                iEvent.getByToken(dharmonicToken_, dEdxharmonic2);

                muon_innertrack_vtx[muon_count] = numvtx;
                muon_innertrack_px[muon_count] = innertrack->px();
                muon_innertrack_py[muon_count] = innertrack->py();
                muon_innertrack_pz[muon_count] = innertrack->pz();
                muon_innertrack_closestpointx[muon_count] = TTrackState.position().x();
                muon_innertrack_closestpointy[muon_count] = TTrackState.position().y();
                muon_innertrack_closestpointz[muon_count] = TTrackState.position().z();
                muon_innertrack_dxy[muon_count]    = innertrack->dxy(pv_position);
                muon_innertrack_dxyerr[muon_count] = innertrack->dxyError();
                muon_innertrack_dz[muon_count]     = innertrack->dz(pv_position);
                muon_innertrack_dzerr[muon_count]  = innertrack->dzError();
                muon_innertrack_chi2[muon_count]   = innertrack->chi2();
                muon_innertrack_ndof[muon_count]   = innertrack->ndof();
                muon_innertrack_charge[muon_count] = innertrack->charge();
                muon_innertrack_nhits[muon_count] = innertrack->numberOfValidHits();
                muon_innertrack_nmissinghits[muon_count] = innertrack->numberOfLostHits();
                muon_innertrack_npixelhits[muon_count] = innertrack->hitPattern().numberOfValidPixelHits();
                muon_innertrack_npixellayers[muon_count] = innertrack->hitPattern().pixelLayersWithMeasurement();
                muon_innertrack_nstriplayers[muon_count] = innertrack->hitPattern().stripLayersWithMeasurement();

                if(dEdxharmonic2.isValid()) {
                    muon_innertrack_dedxharmonic2[muon_count] = (*dEdxharmonic2)[innertrack].dEdx();
                } else {
                    muon_innertrack_dedxharmonic2[muon_count] = -1.;
                }

                math::XYZPoint ecalPos = PositionOnECalSurface(TTrack);
                muon_innertrack_outerx[muon_count] = ecalPos.x();
                muon_innertrack_outery[muon_count] = ecalPos.y();
                muon_innertrack_outerz[muon_count] = ecalPos.z();
            }

            TrackRef muontrack = themu.outerTrack();

            if(muontrack.isNonnull()) {
                muon_type[muon_count] |= 1 << 5;
                muon_outertrack_px[muon_count] = muontrack->px();
                muon_outertrack_py[muon_count] = muontrack->py();
                muon_outertrack_pz[muon_count] = muontrack->pz();
                muon_outertrack_hits[muon_count] = muontrack->numberOfValidHits();
                muon_outertrack_missinghits[muon_count] = muontrack->numberOfLostHits();
                muon_outertrack_chi2[muon_count] = muontrack->chi2();
                muon_outertrack_ndof[muon_count] = muontrack->ndof();
            }

            muon_trigger[muon_count] = GetTrigger(themu, muontriggers);
            muon_count++;

            if(muon_count == M_muonmaxcount) {
                cerr << "number of muon > M_muonmaxcount. They are missing." << endl;
                errors |= 1<<0;
                break;
            }

            if(themu.isGlobalMuon() && themu.isTrackerMuon() && themu.pt() >= cMuPtMin && fabs(themu.eta()) <= cMuEtaMax && themu.isolationR03().sumPt/themu.pt() <= cMuTrackIso) {
                NumGood++;
                double energy = sqrt(pow(muon_px[i],2) + pow(muon_py[i],2) + pow(muon_pz[i],2));
                MuVector.push_back(TLorentzVector(muon_px[i], muon_py[i], muon_pz[i], energy));
                // fill baby trees
                muon_has_gen_particle[muon_count] = 0;
                muon_gen_particle_pdgid[muon_count] = 0;
                muon_has_gen_mother[muon_count] = 0;
                muon_gen_mother_pdgid[muon_count] = 0;

                if(themu.genParticle()) {
                    muon_has_gen_particle[muon_count] = 1;
                    muon_gen_particle_pdgid[muon_count] = themu.genParticle()->pdgId();

                    if(themu.genParticle()->mother()) {
                        muon_has_gen_mother[muon_count] = 1;
                        muon_gen_mother_pdgid[muon_count] = themu.genParticle()->mother()->pdgId();
                    }
                }

                // end baby trees
                muon_is_tracker[muon_count] = 0;
                muon_is_global[muon_count] = 0;
                muon_is_standalone[muon_count] = 0;

                if(themu.isGlobalMuon())     { muon_is_global[muon_count] = 1; }

                if(themu.isStandAloneMuon()) { muon_is_standalone[muon_count] = 1; }

                if(themu.isTrackerMuon())    { muon_is_tracker[muon_count] = 1; }
            }
        }
    }

    if(NumGood >= cMuNum) {
        return (true);
    }

    return (false);
}


UInt_t RootMaker::GetTrigger(const LeafCandidate &particle, vector<pair<unsigned, int> > &triggers) {
    UInt_t result = 0;

    if(cdebug) {
        cout<<"GetTrigger..."<<endl;
        cout<<"HLTrigger.isValid() = "<<HLTrigger.isValid()<<endl;
        cout<<"HLTriggerEvent.isValid() = "<<HLTriggerEvent.isValid()<<endl;
    }

    if(HLTrigger.isValid() && HLTriggerEvent.isValid()) {
        const trigger::TriggerObjectCollection &TOC(HLTriggerEvent->getObjects());

        for(unsigned n = 0 ; n < triggers.size() ; n++) {
            unsigned TriggerIndex = triggers[n].first;
            int Filter = abs(triggers[n].second);
            const vector<string> &ModuleLabels(HLTConfiguration.moduleLabels(TriggerIndex));
            const unsigned FilterIndex = HLTriggerEvent->filterIndex(edm::InputTag(ModuleLabels[Filter], "", cTriggerProcess));

            if(FilterIndex < HLTriggerEvent->sizeFilters()) {
                const trigger::Keys &KEYS(HLTriggerEvent->filterKeys(FilterIndex));

                for(unsigned j = 0 ; j < KEYS.size() ; j++) {
                    TLorentzVector TA(particle.px(), particle.py(), particle.pz(), particle.energy());
                    TLorentzVector TB(TOC[KEYS[j]].px(), TOC[KEYS[j]].py(), TOC[KEYS[j]].pz(), TOC[KEYS[j]].energy());
                    double dr = TA.DeltaR(TB);

                    //double dpt = TMath::Abs(particle.pt() - TOC[KEYS[j]].pt())/particle.pt();
                    if(dr < 0.3) {
                        if(triggers[n].second >= 0) {
                            result |= 1<<n;
                            break;
                        }

                        if(triggers[n].second < 0 && TOC[KEYS[j]].pt() > 10.) {
                            drhist->Fill(dr);
                            result |= 1<<n;
                            break;
                        }

                    }
                }
            }
        }
    }

    return (result);
}

bool RootMaker::AddPhotons(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
    if(cdebug) { cout<<"AddPhotons..."<<endl; }

    int NumGood = 0;
    edm::Handle<reco::PhotonCollection> Photons;
    iEvent.getByToken(recoPhotonsToken_, Photons);

    if(cdebug) {
        cout<<"Photons.isValid() = "<<Photons.isValid()<<endl;
        cout<<"Photons->size() = "<<Photons->size()<<endl;
    }

    if(Photons.isValid() && Photons->size() > 0) {
        edm::Handle<reco::GsfElectronCollection> Electrons;
        iEvent.getByToken(recoElectronsToken_, Electrons);
        edm::Handle<reco::ConversionCollection> Conversions;
        iEvent.getByToken(conversionsToken_, Conversions);

        PFIsolationEstimator isolator;
        VertexRef myprimvertex(Vertices, 0);
        edm::Handle<reco::PFCandidateCollection> PFCandidates;
        iEvent.getByToken(recoPFCandsToken_, PFCandidates);
        isolator.initializePhotonIsolation(kTRUE);

        EcalClusterLazyTools lazyTools(iEvent, iSetup, ebRecHitsToken_, eeRecHitsToken_);
        SuperClusterFootprintRemoval remover(iEvent, iSetup);

        for(size_t n = 0 ; n < Photons->size() ; n++) {
            const reco::Photon &theph = (*Photons)[n];
            reco::PhotonRef refph(Photons, n);

            if(theph.pt() > cPhotonFilterPtMin && TMath::Abs(theph.eta()) < cPhotonFilterEtaMax) {
                photon_px[photon_count] = theph.px();
                photon_py[photon_count] = theph.py();
                photon_pz[photon_count] = theph.pz();
                photon_pt[photon_count] = theph.pt();
                photon_phi[photon_count] = theph.phi();
                photon_eta[photon_count] = theph.eta();
                photon_e1x5[photon_count] = theph.e1x5();
                photon_e2x5[photon_count] = theph.e2x5();
                photon_e3x3[photon_count] = theph.e3x3();
                photon_e5x5[photon_count] = theph.e5x5();
                photon_maxenergyxtal[photon_count] = theph.maxEnergyXtal();
                photon_sigmaietaieta[photon_count] = theph.sigmaIetaIeta();
                vector<float> localcovariances = lazyTools.localCovariances(* (theph.superCluster()->seed()));
                photon_sigmaiphiiphi[photon_count] = TMath::Sqrt(localcovariances[2]);
                photon_sigmaietaiphi[photon_count] = TMath::Sqrt(localcovariances[1]);
                photon_ehcaloverecaldepth1[photon_count] = theph.hadronicDepth1OverEm();
                photon_ehcaloverecaldepth2[photon_count] = theph.hadronicDepth2OverEm();
                photon_ehcaltoweroverecaldepth1[photon_count] = theph.hadTowDepth1OverEm();
                photon_ehcaltoweroverecaldepth2[photon_count] = theph.hadTowDepth2OverEm();
                photon_isolationr3track[photon_count] = theph.trkSumPtSolidConeDR03();
                photon_isolationr3trackhollow[photon_count] = theph.trkSumPtHollowConeDR03();
                photon_isolationr3ecal[photon_count] = theph.ecalRecHitSumEtConeDR03();
                photon_isolationr3hcal[photon_count] = theph.hcalTowerSumEtConeDR03();
                photon_isolationr3ntrack[photon_count] = theph.nTrkSolidConeDR03();
                photon_isolationr3ntrackhollow[photon_count] = theph.nTrkHollowConeDR03();
                photon_isolationr4track[photon_count] = theph.trkSumPtSolidConeDR04();
                photon_isolationr4trackhollow[photon_count] = theph.trkSumPtHollowConeDR04();
                photon_isolationr4ecal[photon_count] = theph.ecalRecHitSumEtConeDR04();
                photon_isolationr4hcal[photon_count] = theph.hcalTowerSumEtConeDR04();
                photon_isolationr4ntrack[photon_count] = theph.nTrkSolidConeDR04();
                photon_isolationr4ntrackhollow[photon_count] = theph.nTrkHollowConeDR04();
                isolator.setConeSize(0.3);
                isolator.fGetIsolation(&theph, PFCandidates.product(), myprimvertex, Vertices);
                photon_isolationpfr3charged[photon_count] = isolator.getIsolationCharged();
                photon_isolationpfr3photon[photon_count] = isolator.getIsolationPhoton();
                photon_isolationpfr3neutral[photon_count] = isolator.getIsolationNeutral();
                isolator.setConeSize(0.4);
                isolator.fGetIsolation(&theph, PFCandidates.product(), myprimvertex, Vertices);
                photon_isolationpfr4charged[photon_count] = isolator.getIsolationCharged();
                photon_isolationpfr4photon[photon_count] = isolator.getIsolationPhoton();
                photon_isolationpfr4neutral[photon_count] = isolator.getIsolationNeutral();

                //Iso R4 with SC footprint removal https://twiki.cern.ch/twiki/bin/viewauth/CMS/SuperClusterFootprintRemoval
                //photon_isolationpfr4noscfootprintcharged[photon_count] = remover.PFIsolation("charged", theph.superCluster(), 0);
                //photon_isolationpfr4noscfootprintphoton[photon_count] = remover.PFIsolation("photon", theph.superCluster());
                //photon_isolationpfr4noscfootprintneutral[photon_count] = remover.PFIsolation("neutral", theph.superCluster());

                PFIsolation_struct tempPFIso_photon = remover.PFIsolation(theph.superCluster(), edm::Ptr<reco::Vertex> (Vertices, 0));
                photon_isolationpfr4noscfootprintcharged[photon_count] = tempPFIso_photon.chargediso_primvtx;
                photon_isolationpfr4noscfootprintphoton[photon_count]  = tempPFIso_photon.photoniso;
                photon_isolationpfr4noscfootprintneutral[photon_count] = tempPFIso_photon.neutraliso;

                //    photon_isolationpfr3charged[photon_count] = (*(photonIsoPF[0]))[refph];
                //    photon_isolationpfr3photon[photon_count] = (*(photonIsoPF[1]))[refph];
                //    photon_isolationpfr3neutral[photon_count] = (*(photonIsoPF[2]))[refph];
                photon_isolationpfr3charged[photon_count] = theph.chargedHadronIso();
                photon_isolationpfr3photon[photon_count] = theph.photonIso();
                photon_isolationpfr3neutral[photon_count] = theph.neutralHadronIso();
                photon_supercluster_e[photon_count] = theph.superCluster()->energy();
                photon_supercluster_x[photon_count] = theph.superCluster()->x();
                photon_supercluster_y[photon_count] = theph.superCluster()->y();
                photon_supercluster_z[photon_count] = theph.superCluster()->z();
                photon_supercluster_rawe[photon_count] = theph.superCluster()->rawEnergy();
                photon_supercluster_phiwidth[photon_count] = theph.superCluster()->phiWidth();
                photon_supercluster_etawidth[photon_count] = theph.superCluster()->etaWidth();
                photon_supercluster_nbasiccluster[photon_count] = theph.superCluster()->clustersSize();

                photon_info[photon_count] = 0;
                photon_info[photon_count] |= theph.isPhoton() << 0;
                photon_info[photon_count] |= theph.hasConversionTracks() << 1;
                photon_info[photon_count] |= theph.hasPixelSeed() << 2;
                photon_info[photon_count] |= ConversionTools::hasMatchedPromptElectron(theph.superCluster(), Electrons, Conversions, bs_position) << 3;
                photon_info[photon_count] |= theph.isPFlowPhoton() << 4;
                photon_gapinfo[photon_count] = 0;
                photon_gapinfo[photon_count] |= theph.isEB() << 0;
                photon_gapinfo[photon_count] |= theph.isEE() << 1;
                photon_gapinfo[photon_count] |= theph.isEBGap() << 2;
                photon_gapinfo[photon_count] |= theph.isEBEtaGap() << 3;
                photon_gapinfo[photon_count] |= theph.isEBPhiGap() << 4;
                photon_gapinfo[photon_count] |= theph.isEEGap() << 5;
                photon_gapinfo[photon_count] |= theph.isEERingGap() << 6;
                photon_gapinfo[photon_count] |= theph.isEEDeeGap() << 7;
                photon_gapinfo[photon_count] |= theph.isEBEEGap() << 8;
                photon_conversionbegin[photon_count] = conversion_count;
                photon_trigger[photon_count] = GetTrigger(theph, photontriggers);

                ConversionRefVector conversions = theph.conversions();

                for(unsigned j = 0 ; j < conversions.size() ; j++) {
                    ConversionRef currconv = conversions[j];

                    conversion_info[conversion_count] = 0;
                    conversion_info[conversion_count] |= currconv->isConverted() << 0;
                    conversion_info[conversion_count] |= currconv->conversionVertex().isValid() << 1;
                    conversion_vx[conversion_count] = currconv->conversionVertex().x();
                    conversion_vy[conversion_count] = currconv->conversionVertex().y();
                    conversion_vz[conversion_count] = currconv->conversionVertex().z();
                    conversion_chi2[conversion_count] = currconv->conversionVertex().chi2();
                    conversion_ndof[conversion_count] = currconv->conversionVertex().ndof();
                    conversion_cov[conversion_count][0] = currconv->conversionVertex().covariance(0,0);
                    conversion_cov[conversion_count][1] = currconv->conversionVertex().covariance(0,1);
                    conversion_cov[conversion_count][2] = currconv->conversionVertex().covariance(0,2);
                    conversion_cov[conversion_count][3] = currconv->conversionVertex().covariance(1,1);
                    conversion_cov[conversion_count][4] = currconv->conversionVertex().covariance(1,2);
                    conversion_cov[conversion_count][5] = currconv->conversionVertex().covariance(2,2);
                    conversion_mvaout[conversion_count] = currconv->MVAout();

                    conversion_trackndof[conversion_count][0] = -1.;
                    conversion_trackndof[conversion_count][1] = -1.;

                    if(currconv->nTracks() == 2) {
                        edm::RefToBase<Track> trA  = currconv->tracks()[0];
                        TransientTrack SVTTrackA = TTrackBuilder->build(*trA);
                        TrajectoryStateClosestToPoint TTrackStateA = SVTTrackA.trajectoryStateClosestToPoint(GlobalPoint(currconv->conversionVertex().x(), currconv->conversionVertex().y(), currconv->conversionVertex().z()));
                        edm::RefToBase<Track> trB  = currconv->tracks()[1];
                        TransientTrack SVTTrackB = TTrackBuilder->build(*trB);
                        TrajectoryStateClosestToPoint TTrackStateB = SVTTrackB.trajectoryStateClosestToPoint(GlobalPoint(currconv->conversionVertex().x(), currconv->conversionVertex().y(), currconv->conversionVertex().z()));

                        if(TTrackStateB.isValid() && TTrackStateA.isValid()) {

                            conversion_trackecalpointx[conversion_count][0] = currconv->ecalImpactPosition()[0].X();
                            conversion_trackecalpointy[conversion_count][0] = currconv->ecalImpactPosition()[0].Y();
                            conversion_trackecalpointz[conversion_count][0] = currconv->ecalImpactPosition()[0].Z();
                            conversion_trackpx[conversion_count][0] = TTrackStateA.momentum().x();
                            conversion_trackpy[conversion_count][0] = TTrackStateA.momentum().y();
                            conversion_trackpz[conversion_count][0] = TTrackStateA.momentum().z();
                            conversion_trackclosestpointx[conversion_count][0] =  TTrackStateA.position().x();
                            conversion_trackclosestpointy[conversion_count][0] =  TTrackStateA.position().y();
                            conversion_trackclosestpointz[conversion_count][0] =  TTrackStateA.position().z();
                            conversion_trackchi2[conversion_count][0] = currconv->tracks()[0]->chi2();
                            conversion_trackndof[conversion_count][0] = currconv->tracks()[0]->ndof();
                            conversion_trackdxy[conversion_count][0] = TTrackStateA.perigeeParameters().transverseImpactParameter();
                            conversion_trackdxyerr[conversion_count][0] = TTrackStateA.perigeeError().transverseImpactParameterError();
                            conversion_trackdz[conversion_count][0] = TTrackStateA.perigeeParameters().longitudinalImpactParameter();
                            conversion_trackdzerr[conversion_count][0] = TTrackStateA.perigeeError().longitudinalImpactParameterError();
                            conversion_trackcharge[conversion_count][0] = currconv->tracks()[0]->charge();
                            conversion_tracknhits[conversion_count][0] = currconv->tracks()[0]->numberOfValidHits();
                            conversion_tracknmissinghits[conversion_count][0] = currconv->tracks()[0]->numberOfLostHits();
                            conversion_tracknpixelhits[conversion_count][0] = currconv->tracks()[0]->hitPattern().numberOfValidPixelHits();
                            conversion_tracknpixellayers[conversion_count][0] = currconv->tracks()[0]->hitPattern().pixelLayersWithMeasurement();
                            conversion_tracknstriplayers[conversion_count][0] = currconv->tracks()[0]->hitPattern().stripLayersWithMeasurement();
                            conversion_trackecalpointx[conversion_count][1] = currconv->ecalImpactPosition()[1].X();
                            conversion_trackecalpointy[conversion_count][1] = currconv->ecalImpactPosition()[1].Y();
                            conversion_trackecalpointz[conversion_count][1] = currconv->ecalImpactPosition()[1].Z();
                            conversion_trackpx[conversion_count][1] = TTrackStateB.momentum().x();
                            conversion_trackpy[conversion_count][1] = TTrackStateB.momentum().y();
                            conversion_trackpz[conversion_count][1] = TTrackStateB.momentum().z();
                            conversion_trackclosestpointx[conversion_count][1] =  TTrackStateB.position().x();
                            conversion_trackclosestpointy[conversion_count][1] =  TTrackStateB.position().y();
                            conversion_trackclosestpointz[conversion_count][1] =  TTrackStateB.position().z();
                            conversion_trackchi2[conversion_count][1] = currconv->tracks()[1]->chi2();
                            conversion_trackndof[conversion_count][1] = currconv->tracks()[1]->ndof();
                            conversion_trackdxy[conversion_count][1] = TTrackStateB.perigeeParameters().transverseImpactParameter();
                            conversion_trackdxyerr[conversion_count][1] = TTrackStateB.perigeeError().transverseImpactParameterError();
                            conversion_trackdz[conversion_count][1] = TTrackStateB.perigeeParameters().longitudinalImpactParameter();
                            conversion_trackdzerr[conversion_count][1] = TTrackStateB.perigeeError().longitudinalImpactParameterError();
                            conversion_trackcharge[conversion_count][1] = currconv->tracks()[1]->charge();
                            conversion_tracknhits[conversion_count][1] = currconv->tracks()[1]->numberOfValidHits();
                            conversion_tracknmissinghits[conversion_count][1] = currconv->tracks()[1]->numberOfLostHits();
                            conversion_tracknpixelhits[conversion_count][1] = currconv->tracks()[1]->hitPattern().numberOfValidPixelHits();
                            conversion_tracknpixellayers[conversion_count][1] = currconv->tracks()[1]->hitPattern().pixelLayersWithMeasurement();
                            conversion_tracknstriplayers[conversion_count][1] = currconv->tracks()[1]->hitPattern().stripLayersWithMeasurement();
                        }
                    }

                    conversion_count++;

                    if(conversion_count == M_conversionmaxcount) {
                        cerr << "number of conversions > M_conversionmaxcount. They are missing." << endl;
                        errors |= 1<<4;
                        break;
                    }
                }

                photon_count++;

                if(photon_count == M_photonmaxcount || conversion_count == M_conversionmaxcount) {
                    cerr << "number of photon > M_photonmaxcount. They are missing." << endl;
                    errors |= 1<<3;
                    break;
                }

                if(theph.pt() >= cPhotonPtMin && fabs(theph.eta()) <= cPhotonEtaMax) {
                    NumGood++;
                }
            }
        }
    }

    if(NumGood >= cPhotonNum) {
        return (true);
    }

    return (false);
}

bool RootMaker::AddPatPhotons(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
    if(cdebug) { cout<<"AddPatPhotons..."<<endl; }

    int NumGood = 0;
    edm::Handle<pat::PhotonCollection> Photons;
    iEvent.getByToken(patPhotonsToken_, Photons);

    if(cdebug) {
        cout<<"pat Photons.isValid() = "<<Photons.isValid()<<endl;
        cout<<"pat Photons->size() = "<<Photons->size()<<endl;
    }

    if(Photons.isValid() && Photons->size() > 0) {
        edm::Handle<pat::ElectronCollection> Electrons;
        iEvent.getByToken(patElectronsToken_, Electrons);
        edm::Handle<reco::ConversionCollection> Conversions;
        iEvent.getByToken(conversionsToken_, Conversions);

        PFIsolationEstimator isolator;
        VertexRef myprimvertex(Vertices, 0);
        edm::Handle<pat::PackedCandidateCollection> PFCandidates;
        iEvent.getByToken(packedPFCandsToken_, PFCandidates);
        isolator.initializePhotonIsolation(kTRUE);

        EcalClusterLazyTools lazyTools(iEvent, iSetup, ebRecHitsToken_, eeRecHitsToken_);

        SuperClusterFootprintRemoval remover(iEvent, iSetup);

        for(size_t n = 0 ; n < Photons->size() ; n++) {
            const pat::Photon &theph = (*Photons)[n];
            pat::PhotonRef refph(Photons, n);

            if(theph.pt() > cPhotonFilterPtMin && TMath::Abs(theph.eta()) < cPhotonFilterEtaMax) {
                photon_px[photon_count] = theph.px();
                photon_py[photon_count] = theph.py();
                photon_pz[photon_count] = theph.pz();
                photon_pt[photon_count] = theph.pt();
                photon_phi[photon_count] = theph.phi();
                photon_eta[photon_count] = theph.eta();
                photon_e1x5[photon_count] = theph.e1x5();
                photon_e2x5[photon_count] = theph.e2x5();
                photon_e3x3[photon_count] = theph.e3x3();
                photon_e5x5[photon_count] = theph.e5x5();
                photon_maxenergyxtal[photon_count] = theph.maxEnergyXtal();
                photon_sigmaietaieta[photon_count] = theph.sigmaIetaIeta();
                vector<float> localcovariances = lazyTools.localCovariances(* (theph.superCluster()->seed()));
                photon_sigmaiphiiphi[photon_count] = TMath::Sqrt(localcovariances[2]);
                photon_sigmaietaiphi[photon_count] = TMath::Sqrt(localcovariances[1]);
                photon_ehcaloverecaldepth1[photon_count] = theph.hadronicDepth1OverEm();
                photon_ehcaloverecaldepth2[photon_count] = theph.hadronicDepth2OverEm();
                photon_ehcaltoweroverecaldepth1[photon_count] = theph.hadTowDepth1OverEm();
                photon_ehcaltoweroverecaldepth2[photon_count] = theph.hadTowDepth2OverEm();
                photon_isolationr3track[photon_count] = theph.trkSumPtSolidConeDR03();
                photon_isolationr3trackhollow[photon_count] = theph.trkSumPtHollowConeDR03();
                photon_isolationr3ecal[photon_count] = theph.ecalRecHitSumEtConeDR03();
                photon_isolationr3hcal[photon_count] = theph.hcalTowerSumEtConeDR03();
                photon_isolationr3ntrack[photon_count] = theph.nTrkSolidConeDR03();
                photon_isolationr3ntrackhollow[photon_count] = theph.nTrkHollowConeDR03();
                photon_isolationr4track[photon_count] = theph.trkSumPtSolidConeDR04();
                photon_isolationr4trackhollow[photon_count] = theph.trkSumPtHollowConeDR04();
                photon_isolationr4ecal[photon_count] = theph.ecalRecHitSumEtConeDR04();
                photon_isolationr4hcal[photon_count] = theph.hcalTowerSumEtConeDR04();
                photon_isolationr4ntrack[photon_count] = theph.nTrkSolidConeDR04();
                photon_isolationr4ntrackhollow[photon_count] = theph.nTrkHollowConeDR04();

                /*
                                isolator.setConeSize(0.3);
                                isolator.fGetIsolation(&theph, PFCandidates.product(), myprimvertex, Vertices);
                                photon_isolationpfr3charged[photon_count] = isolator.getIsolationCharged();
                                photon_isolationpfr3photon[photon_count] = isolator.getIsolationPhoton();
                                photon_isolationpfr3neutral[photon_count] = isolator.getIsolationNeutral();
                                isolator.setConeSize(0.4);
                                isolator.fGetIsolation(&theph, PFCandidates.product(), myprimvertex, Vertices);
                                photon_isolationpfr4charged[photon_count] = isolator.getIsolationCharged();
                                photon_isolationpfr4photon[photon_count] = isolator.getIsolationPhoton();
                                photon_isolationpfr4neutral[photon_count] = isolator.getIsolationNeutral();

                                //Iso R4 with SC footprint removal https://twiki.cern.ch/twiki/bin/viewauth/CMS/SuperClusterFootprintRemoval
                                //photon_isolationpfr4noscfootprintcharged[photon_count] = remover.PFIsolation("charged", theph.superCluster(), 0);
                                //photon_isolationpfr4noscfootprintphoton[photon_count] = remover.PFIsolation("photon", theph.superCluster());
                                //photon_isolationpfr4noscfootprintneutral[photon_count] = remover.PFIsolation("neutral", theph.superCluster());

                                PFIsolation_struct tempPFIso_photon = remover.PFIsolation(theph.superCluster(), edm::Ptr<reco::Vertex> (Vertices, 0));
                                photon_isolationpfr4noscfootprintcharged[photon_count] = tempPFIso_photon.chargediso_primvtx;
                                photon_isolationpfr4noscfootprintphoton[photon_count]  = tempPFIso_photon.photoniso;
                                photon_isolationpfr4noscfootprintneutral[photon_count] = tempPFIso_photon.neutraliso;
                */
                //    photon_isolationpfr3charged[photon_count] = (*(photonIsoPF[0]))[refph];
                //    photon_isolationpfr3photon[photon_count] = (*(photonIsoPF[1]))[refph];
                //    photon_isolationpfr3neutral[photon_count] = (*(photonIsoPF[2]))[refph];
                photon_isolationpfr3charged[photon_count] = theph.chargedHadronIso();
                photon_isolationpfr3photon[photon_count] = theph.photonIso();
                photon_isolationpfr3neutral[photon_count] = theph.neutralHadronIso();
                photon_supercluster_e[photon_count] = theph.superCluster()->energy();
                photon_supercluster_x[photon_count] = theph.superCluster()->x();
                photon_supercluster_y[photon_count] = theph.superCluster()->y();
                photon_supercluster_z[photon_count] = theph.superCluster()->z();
                photon_supercluster_rawe[photon_count] = theph.superCluster()->rawEnergy();
                photon_supercluster_phiwidth[photon_count] = theph.superCluster()->phiWidth();
                photon_supercluster_etawidth[photon_count] = theph.superCluster()->etaWidth();
                photon_supercluster_nbasiccluster[photon_count] = theph.superCluster()->clustersSize();

                photon_info[photon_count] = 0;
                photon_info[photon_count] |= theph.isPhoton() << 0;
                photon_info[photon_count] |= theph.hasConversionTracks() << 1;
                photon_info[photon_count] |= theph.hasPixelSeed() << 2;
                //photon_info[photon_count] |= ConversionTools::hasMatchedPromptElectron(theph.superCluster(), Electrons, Conversions, bs_position) << 3;
                photon_info[photon_count] |= theph.isPFlowPhoton() << 4;
                photon_gapinfo[photon_count] = 0;
                photon_gapinfo[photon_count] |= theph.isEB() << 0;
                photon_gapinfo[photon_count] |= theph.isEE() << 1;
                photon_gapinfo[photon_count] |= theph.isEBGap() << 2;
                photon_gapinfo[photon_count] |= theph.isEBEtaGap() << 3;
                photon_gapinfo[photon_count] |= theph.isEBPhiGap() << 4;
                photon_gapinfo[photon_count] |= theph.isEEGap() << 5;
                photon_gapinfo[photon_count] |= theph.isEERingGap() << 6;
                photon_gapinfo[photon_count] |= theph.isEEDeeGap() << 7;
                photon_gapinfo[photon_count] |= theph.isEBEEGap() << 8;
                photon_conversionbegin[photon_count] = conversion_count;
                photon_trigger[photon_count] = GetTrigger(theph, photontriggers);
                /*
                                ConversionRefVector conversions = theph.conversions();
                                for(unsigned j = 0 ; j < conversions.size() ; j++) {
                                    ConversionRef currconv = conversions[j];

                                    conversion_info[conversion_count] = 0;
                                    conversion_info[conversion_count] |= currconv->isConverted() << 0;
                                    conversion_info[conversion_count] |= currconv->conversionVertex().isValid() << 1;
                                    conversion_vx[conversion_count] = currconv->conversionVertex().x();
                                    conversion_vy[conversion_count] = currconv->conversionVertex().y();
                                    conversion_vz[conversion_count] = currconv->conversionVertex().z();
                                    conversion_chi2[conversion_count] = currconv->conversionVertex().chi2();
                                    conversion_ndof[conversion_count] = currconv->conversionVertex().ndof();
                                    conversion_cov[conversion_count][0] = currconv->conversionVertex().covariance(0,0);
                                    conversion_cov[conversion_count][1] = currconv->conversionVertex().covariance(0,1);
                                    conversion_cov[conversion_count][2] = currconv->conversionVertex().covariance(0,2);
                                    conversion_cov[conversion_count][3] = currconv->conversionVertex().covariance(1,1);
                                    conversion_cov[conversion_count][4] = currconv->conversionVertex().covariance(1,2);
                                    conversion_cov[conversion_count][5] = currconv->conversionVertex().covariance(2,2);
                                    conversion_mvaout[conversion_count] = currconv->MVAout();

                                    conversion_trackndof[conversion_count][0] = -1.;
                                    conversion_trackndof[conversion_count][1] = -1.;
                                    if(currconv->nTracks() == 2) {
                                        edm::RefToBase<Track> trA  = currconv->tracks()[0];
                                        TransientTrack SVTTrackA = TTrackBuilder->build(*trA);
                                        TrajectoryStateClosestToPoint TTrackStateA = SVTTrackA.trajectoryStateClosestToPoint(GlobalPoint(currconv->conversionVertex().x(), currconv->conversionVertex().y(), currconv->conversionVertex().z()));
                                        edm::RefToBase<Track> trB  = currconv->tracks()[1];
                                        TransientTrack SVTTrackB = TTrackBuilder->build(*trB);
                                        TrajectoryStateClosestToPoint TTrackStateB = SVTTrackB.trajectoryStateClosestToPoint(GlobalPoint(currconv->conversionVertex().x(), currconv->conversionVertex().y(), currconv->conversionVertex().z()));

                                        if(TTrackStateB.isValid() && TTrackStateA.isValid()) {

                                            conversion_trackecalpointx[conversion_count][0] = currconv->ecalImpactPosition()[0].X();
                                            conversion_trackecalpointy[conversion_count][0] = currconv->ecalImpactPosition()[0].Y();
                                            conversion_trackecalpointz[conversion_count][0] = currconv->ecalImpactPosition()[0].Z();
                                            conversion_trackpx[conversion_count][0] = TTrackStateA.momentum().x();
                                            conversion_trackpy[conversion_count][0] = TTrackStateA.momentum().y();
                                            conversion_trackpz[conversion_count][0] = TTrackStateA.momentum().z();
                                            conversion_trackclosestpointx[conversion_count][0] =  TTrackStateA.position().x();
                                            conversion_trackclosestpointy[conversion_count][0] =  TTrackStateA.position().y();
                                            conversion_trackclosestpointz[conversion_count][0] =  TTrackStateA.position().z();
                                            conversion_trackchi2[conversion_count][0] = currconv->tracks()[0]->chi2();
                                            conversion_trackndof[conversion_count][0] = currconv->tracks()[0]->ndof();
                                            conversion_trackdxy[conversion_count][0] = TTrackStateA.perigeeParameters().transverseImpactParameter();
                                            conversion_trackdxyerr[conversion_count][0] = TTrackStateA.perigeeError().transverseImpactParameterError();
                                            conversion_trackdz[conversion_count][0] = TTrackStateA.perigeeParameters().longitudinalImpactParameter();
                                            conversion_trackdzerr[conversion_count][0] = TTrackStateA.perigeeError().longitudinalImpactParameterError();
                                            conversion_trackcharge[conversion_count][0] = currconv->tracks()[0]->charge();
                                            conversion_tracknhits[conversion_count][0] = currconv->tracks()[0]->numberOfValidHits();
                                            conversion_tracknmissinghits[conversion_count][0] = currconv->tracks()[0]->numberOfLostHits();
                                            conversion_tracknpixelhits[conversion_count][0] = currconv->tracks()[0]->hitPattern().numberOfValidPixelHits();
                                            conversion_tracknpixellayers[conversion_count][0] = currconv->tracks()[0]->hitPattern().pixelLayersWithMeasurement();
                                            conversion_tracknstriplayers[conversion_count][0] = currconv->tracks()[0]->hitPattern().stripLayersWithMeasurement();
                                            conversion_trackecalpointx[conversion_count][1] = currconv->ecalImpactPosition()[1].X();
                                            conversion_trackecalpointy[conversion_count][1] = currconv->ecalImpactPosition()[1].Y();
                                            conversion_trackecalpointz[conversion_count][1] = currconv->ecalImpactPosition()[1].Z();
                                            conversion_trackpx[conversion_count][1] = TTrackStateB.momentum().x();
                                            conversion_trackpy[conversion_count][1] = TTrackStateB.momentum().y();
                                            conversion_trackpz[conversion_count][1] = TTrackStateB.momentum().z();
                                            conversion_trackclosestpointx[conversion_count][1] =  TTrackStateB.position().x();
                                            conversion_trackclosestpointy[conversion_count][1] =  TTrackStateB.position().y();
                                            conversion_trackclosestpointz[conversion_count][1] =  TTrackStateB.position().z();
                                            conversion_trackchi2[conversion_count][1] = currconv->tracks()[1]->chi2();
                                            conversion_trackndof[conversion_count][1] = currconv->tracks()[1]->ndof();
                                            conversion_trackdxy[conversion_count][1] = TTrackStateB.perigeeParameters().transverseImpactParameter();
                                            conversion_trackdxyerr[conversion_count][1] = TTrackStateB.perigeeError().transverseImpactParameterError();
                                            conversion_trackdz[conversion_count][1] = TTrackStateB.perigeeParameters().longitudinalImpactParameter();
                                            conversion_trackdzerr[conversion_count][1] = TTrackStateB.perigeeError().longitudinalImpactParameterError();
                                            conversion_trackcharge[conversion_count][1] = currconv->tracks()[1]->charge();
                                            conversion_tracknhits[conversion_count][1] = currconv->tracks()[1]->numberOfValidHits();
                                            conversion_tracknmissinghits[conversion_count][1] = currconv->tracks()[1]->numberOfLostHits();
                                            conversion_tracknpixelhits[conversion_count][1] = currconv->tracks()[1]->hitPattern().numberOfValidPixelHits();
                                            conversion_tracknpixellayers[conversion_count][1] = currconv->tracks()[1]->hitPattern().pixelLayersWithMeasurement();
                                            conversion_tracknstriplayers[conversion_count][1] = currconv->tracks()[1]->hitPattern().stripLayersWithMeasurement();
                                        }
                                    }
                                    conversion_count++;
                                    if(conversion_count == M_conversionmaxcount) {cerr << "number of conversions > M_conversionmaxcount. They are missing." << endl; errors |= 1<<4; break;}
                                }
                */
                photon_count++;

                if(photon_count == M_photonmaxcount || conversion_count == M_conversionmaxcount) {
                    cerr << "number of photon > M_photonmaxcount. They are missing." << endl;
                    errors |= 1<<3;
                    break;
                }

                if(theph.pt() >= cPhotonPtMin && fabs(theph.eta()) <= cPhotonEtaMax) {
                    NumGood++;
                }
            }
        }
    }

    if(NumGood >= cPhotonNum) {
        return (true);
    }

    return (false);
}

bool RootMaker::AddAllConversions(const edm::Event &iEvent) {
    if(cdebug) { cout<<"AddAllConversions..."<<endl; }

    edm::Handle<ConversionCollection> Conversions;
    iEvent.getByToken(conversionsToken_, Conversions);

    if(cdebug) { cout<<"Conversions.isValid() = "<<Conversions.isValid()<<endl; }

    if(Conversions.isValid()) {
        for(unsigned i = 0 ; i < Conversions->size() ; i++) {
            allconversion_info[allconversion_count] = 0;
            allconversion_info[allconversion_count] |= (*Conversions)[i].isConverted() << 0;
            allconversion_info[allconversion_count] |= (*Conversions)[i].conversionVertex().isValid() << 1;
            allconversion_vx[allconversion_count] = (*Conversions)[i].conversionVertex().x();
            allconversion_vy[allconversion_count] = (*Conversions)[i].conversionVertex().y();
            allconversion_vz[allconversion_count] = (*Conversions)[i].conversionVertex().z();
            allconversion_chi2[allconversion_count] = (*Conversions)[i].conversionVertex().chi2();
            allconversion_ndof[allconversion_count] = (*Conversions)[i].conversionVertex().ndof();
            allconversion_cov[allconversion_count][0] = (*Conversions)[i].conversionVertex().covariance(0,0);
            allconversion_cov[allconversion_count][1] = (*Conversions)[i].conversionVertex().covariance(0,1);
            allconversion_cov[allconversion_count][2] = (*Conversions)[i].conversionVertex().covariance(0,2);
            allconversion_cov[allconversion_count][3] = (*Conversions)[i].conversionVertex().covariance(1,1);
            allconversion_cov[allconversion_count][4] = (*Conversions)[i].conversionVertex().covariance(1,2);
            allconversion_cov[allconversion_count][5] = (*Conversions)[i].conversionVertex().covariance(2,2);
            allconversion_mvaout[allconversion_count] = (*Conversions)[i].MVAout();

            allconversion_trackndof[allconversion_count][0] = -1.;
            allconversion_trackndof[allconversion_count][1] = -1.;

            if((*Conversions)[i].nTracks() == 2) {
                edm::RefToBase<Track> trA  = (*Conversions)[i].tracks()[0];
                TransientTrack SVTTrackA = TTrackBuilder->build(*trA);
                TrajectoryStateClosestToPoint
                TTrackStateA = SVTTrackA.trajectoryStateClosestToPoint(GlobalPoint((*Conversions)[i].conversionVertex().x(),
                               (*Conversions)[i].conversionVertex().y(),
                               (*Conversions)[i].conversionVertex().z()));
                edm::RefToBase<Track> trB  = (*Conversions)[i].tracks()[1];
                TransientTrack SVTTrackB = TTrackBuilder->build(*trB);
                TrajectoryStateClosestToPoint
                TTrackStateB = SVTTrackB.trajectoryStateClosestToPoint(GlobalPoint((*Conversions)[i].conversionVertex().x(),
                               (*Conversions)[i].conversionVertex().y(),
                               (*Conversions)[i].conversionVertex().z()));

                if(TTrackStateB.isValid() && TTrackStateA.isValid()) {
                    allconversion_trackecalpointx[allconversion_count][0] = (*Conversions)[i].ecalImpactPosition()[0].X();
                    allconversion_trackecalpointy[allconversion_count][0] = (*Conversions)[i].ecalImpactPosition()[0].Y();
                    allconversion_trackecalpointz[allconversion_count][0] = (*Conversions)[i].ecalImpactPosition()[0].Z();
                    allconversion_trackpx[allconversion_count][0] = TTrackStateA.momentum().x();
                    allconversion_trackpy[allconversion_count][0] = TTrackStateA.momentum().y();
                    allconversion_trackpz[allconversion_count][0] = TTrackStateA.momentum().z();
                    allconversion_trackclosestpointx[allconversion_count][0] =  TTrackStateA.position().x();
                    allconversion_trackclosestpointy[allconversion_count][0] =  TTrackStateA.position().y();
                    allconversion_trackclosestpointz[allconversion_count][0] =  TTrackStateA.position().z();
                    allconversion_trackchi2[allconversion_count][0] = (*Conversions)[i].tracks()[0]->chi2();
                    allconversion_trackndof[allconversion_count][0] = (*Conversions)[i].tracks()[0]->ndof();
                    allconversion_trackdxy[allconversion_count][0] = TTrackStateA.perigeeParameters().transverseImpactParameter();
                    allconversion_trackdxyerr[allconversion_count][0] = TTrackStateA.perigeeError().transverseImpactParameterError();
                    allconversion_trackdz[allconversion_count][0] = TTrackStateA.perigeeParameters().longitudinalImpactParameter();
                    allconversion_trackdzerr[allconversion_count][0] = TTrackStateA.perigeeError().longitudinalImpactParameterError();
                    allconversion_trackcharge[allconversion_count][0] = (*Conversions)[i].tracks()[0]->charge();
                    allconversion_tracknhits[allconversion_count][0] = (*Conversions)[i].tracks()[0]->numberOfValidHits();
                    allconversion_tracknmissinghits[allconversion_count][0] = (*Conversions)[i].tracks()[0]->numberOfLostHits();
                    allconversion_tracknpixelhits[allconversion_count][0] = (*Conversions)[i].tracks()[0]->hitPattern().numberOfValidPixelHits();
                    allconversion_tracknpixellayers[allconversion_count][0] = (*Conversions)[i].tracks()[0]->hitPattern().pixelLayersWithMeasurement();
                    allconversion_tracknstriplayers[allconversion_count][0] = (*Conversions)[i].tracks()[0]->hitPattern().stripLayersWithMeasurement();
                    allconversion_trackecalpointx[allconversion_count][1] = (*Conversions)[i].ecalImpactPosition()[1].X();
                    allconversion_trackecalpointy[allconversion_count][1] = (*Conversions)[i].ecalImpactPosition()[1].Y();
                    allconversion_trackecalpointz[allconversion_count][1] = (*Conversions)[i].ecalImpactPosition()[1].Z();
                    allconversion_trackpx[allconversion_count][1] = TTrackStateB.momentum().x();
                    allconversion_trackpy[allconversion_count][1] = TTrackStateB.momentum().y();
                    allconversion_trackpz[allconversion_count][1] = TTrackStateB.momentum().z();
                    allconversion_trackclosestpointx[allconversion_count][1] =  TTrackStateB.position().x();
                    allconversion_trackclosestpointy[allconversion_count][1] =  TTrackStateB.position().y();
                    allconversion_trackclosestpointz[allconversion_count][1] =  TTrackStateB.position().z();
                    allconversion_trackchi2[allconversion_count][1] = (*Conversions)[i].tracks()[1]->chi2();
                    allconversion_trackndof[allconversion_count][1] = (*Conversions)[i].tracks()[1]->ndof();
                    allconversion_trackdxy[allconversion_count][1] = TTrackStateB.perigeeParameters().transverseImpactParameter();
                    allconversion_trackdxyerr[allconversion_count][1] = TTrackStateB.perigeeError().transverseImpactParameterError();
                    allconversion_trackdz[allconversion_count][1] = TTrackStateB.perigeeParameters().longitudinalImpactParameter();
                    allconversion_trackdzerr[allconversion_count][1] = TTrackStateB.perigeeError().longitudinalImpactParameterError();
                    allconversion_trackcharge[allconversion_count][1] = (*Conversions)[i].tracks()[1]->charge();
                    allconversion_tracknhits[allconversion_count][1] = (*Conversions)[i].tracks()[1]->numberOfValidHits();
                    allconversion_tracknmissinghits[allconversion_count][1] = (*Conversions)[i].tracks()[1]->numberOfLostHits();
                    allconversion_tracknpixelhits[allconversion_count][1] = (*Conversions)[i].tracks()[1]->hitPattern().numberOfValidPixelHits();
                    allconversion_tracknpixellayers[allconversion_count][1] = (*Conversions)[i].tracks()[1]->hitPattern().pixelLayersWithMeasurement();
                    allconversion_tracknstriplayers[allconversion_count][1] = (*Conversions)[i].tracks()[1]->hitPattern().stripLayersWithMeasurement();
                }
            }

            allconversion_count++;

            if(allconversion_count == M_conversionmaxcount) {
                cerr << "number of conversions > M_conversionmaxcount. They are missing." << endl;
                errors |= 1<<4;
                break;
            }
        }
    }

    return (true);
}

bool RootMaker::AddTaus(const edm::Event &iEvent) {
    if(cdebug) { cout<<"AddTaus..."<<endl; }

    int NumGood = 0;
    edm::Handle<PFTauCollection> Taus;
    iEvent.getByToken(recoTausToken_, Taus);
    edm::Handle<pat::JetCollection> ak4pfJets;
    iEvent.getByToken(tauJetsToken_, ak4pfJets);

    if(cdebug) {
        cout<<"Taus.isValid() = "<<Taus.isValid()<<endl;
        cout<<"Taus->size() = "<<Taus->size()<<endl;
    }

    if(Taus.isValid()) {
        vector<edm::Handle<PFTauDiscriminator> > PFTauDiscriminatiors(cTauDiscriminators.size());

        for(unsigned n = 0 ; n < cTauDiscriminators.size() ; n++) {
            iEvent.getByLabel(cTauDiscriminators[n].c_str(), PFTauDiscriminatiors[n]);
        }

        edm::Handle<PFTauDiscriminator> hpsPFTauDiscriminationByLooseIsolation;
        iEvent.getByLabel("hpsPFTauDiscriminationByLooseIsolation", hpsPFTauDiscriminationByLooseIsolation);

        for(unsigned i = 0 ; i < Taus->size() ; i++) {
            int numtrack = (*Taus)[i].signalPFChargedHadrCands().size();
            PFTauRef tauCandidate(Taus, i);

            if((*hpsPFTauDiscriminationByLooseIsolation)[tauCandidate] < 0.5 || !(numtrack == 3 || numtrack == 1)) {
                continue;
            }

            tau_dishps[tau_count] = 0;

            for(unsigned n = 0 ; n < cTauDiscriminators.size() ; n++) {
                if((*PFTauDiscriminatiors[n])[tauCandidate] > 0.5) {
                    tau_dishps[tau_count] |= 1<<n;
                }
            }

            tau_px[tau_count] = (*Taus)[i].px();
            tau_py[tau_count] = (*Taus)[i].py();
            tau_pz[tau_count] = (*Taus)[i].pz();
            tau_pt[tau_count] = (*Taus)[i].pt();
            tau_phi[tau_count] = (*Taus)[i].phi();
            tau_eta[tau_count] = (*Taus)[i].eta();
            tau_emfraction[tau_count] = (*Taus)[i].emFraction();
            tau_hcaltotoverplead[tau_count] = (*Taus)[i].hcalTotOverPLead();
            tau_hcal3x3overplead[tau_count] = (*Taus)[i].hcalMaxOverPLead();
            tau_ecalstripsumeoverplead[tau_count] = (*Taus)[i].hcal3x3OverPLead();
            tau_bremsrecoveryeoverplead[tau_count] = (*Taus)[i].ecalStripSumEOverPLead();
            tau_calocomp[tau_count] = (*Taus)[i].caloComp();
            tau_segcomp[tau_count] = (*Taus)[i].segComp();

            tau_charge[tau_count] = (*Taus)[i].charge();
            tau_chargedbegin[tau_count] = tau_charged_count;
            PFJetRef thejet = (*Taus)[i].jetRef();
            bool jetfound = false;

            if(ak4pfJets.isValid()) {
                jetfound = true;
                tau_ak4pfjet_e[tau_count] = thejet->energy();
                tau_ak4pfjet_px[tau_count] = thejet->px();
                tau_ak4pfjet_py[tau_count] = thejet->py();
                tau_ak4pfjet_pz[tau_count] = thejet->pz();
                tau_ak4pfjet_hadronicenergy[tau_count] = thejet->chargedHadronEnergy() + thejet->neutralHadronEnergy();
                tau_ak4pfjet_chargedhadronicenergy[tau_count] = thejet->chargedHadronEnergy();
                tau_ak4pfjet_emenergy[tau_count] = thejet->chargedEmEnergy() + thejet->neutralEmEnergy();
                tau_ak4pfjet_chargedemenergy[tau_count] = thejet->chargedEmEnergy();
                tau_ak4pfjet_chargedmulti[tau_count] = thejet->chargedMultiplicity();
                tau_ak4pfjet_neutralmulti[tau_count] = thejet->neutralMultiplicity();
                tau_ak4pfjet_trigger[tau_count] = GetTrigger(*thejet, jettriggers);
                break;
            }

            if(!jetfound) {
                tau_ak4pfjet_e[tau_count] = -1.;
            }

            tau_isolationchargednum[tau_count] = 0;
            tau_isolationchargedpt[tau_count] = 0.;
            tau_isolationneutralsnum[tau_count] = 0;
            tau_isolationneutralspt[tau_count] = 0.;
            tau_pucorrptsum[tau_count] = 0.;
            tau_isolationgammanum[tau_count] = (*Taus)[i].isolationPFGammaCands().size();
            tau_isolationgammapt[tau_count] = (*Taus)[i].isolationPFGammaCandsEtSum();

            tau_isolationchargednum[tau_count]  = (*Taus)[i].isolationPFChargedHadrCands().size();
            tau_isolationchargedpt[tau_count]   = (*Taus)[i].isolationPFChargedHadrCandsPtSum();
            tau_isolationneutralsnum[tau_count] = (*Taus)[i].isolationPFNeutrHadrCands().size();

            std::vector<double> isocands = extract((*Taus)[i].isolationPFNeutrHadrCands(),std::mem_fun_ref(&PFCandidate::pt));
            BOOST_FOREACH(double pt, isocands) {
                tau_isolationneutralspt[tau_count] += pt;
            }
            tau_trigger[tau_count] = GetTrigger((*Taus)[i], tautriggers);
            TrackRef track;

            const std::vector<reco::PFCandidatePtr> &tauSignalPFCands = (*Taus)[i].signalPFCands();

            for(std::vector<reco::PFCandidatePtr>::const_iterator tauSignalPFCand = tauSignalPFCands.begin(); tauSignalPFCand != tauSignalPFCands.end(); ++tauSignalPFCand) {
                track = (*tauSignalPFCand)->trackRef();

                if(!track.isNull()) {
                    iEvent.getByToken(dharmonicToken_, dEdxharmonic2);
                    TransientTrack TTrack = TTrackBuilder->build(track);
                    TrajectoryStateClosestToPoint TTrackState = TTrack.trajectoryStateClosestToPoint(GlobalPoint(pv_position.x(), pv_position.y(), pv_position.z()));
                    tau_charged_px[tau_charged_count] = TTrackState.momentum().x();
                    tau_charged_py[tau_charged_count] = TTrackState.momentum().y();
                    tau_charged_pz[tau_charged_count] = TTrackState.momentum().z();
                    tau_charged_closestpointx[tau_charged_count] = TTrackState.position().x();
                    tau_charged_closestpointy[tau_charged_count] = TTrackState.position().y();
                    tau_charged_closestpointz[tau_charged_count] = TTrackState.position().z();
                    tau_charged_dxy[tau_charged_count]    = TTrackState.perigeeParameters().transverseImpactParameter();
                    tau_charged_dxyerr[tau_charged_count] = TTrackState.perigeeError().transverseImpactParameterError();
                    tau_charged_dz[tau_charged_count]     = TTrackState.perigeeParameters().longitudinalImpactParameter();
                    tau_charged_dzerr[tau_charged_count]  = TTrackState.perigeeError().longitudinalImpactParameterError();
                    tau_charged_chi2[tau_charged_count]   = track->chi2();
                    tau_charged_ndof[tau_charged_count]   = track->ndof();
                    tau_charged_charge[tau_charged_count] = track->charge();
                    tau_charged_nhits[tau_charged_count] = track->numberOfValidHits();
                    tau_charged_nmissinghits[tau_charged_count] = track->numberOfLostHits();
                    tau_charged_npixelhits[tau_charged_count] = track->hitPattern().numberOfValidPixelHits();
                    tau_charged_npixellayers[tau_charged_count] = track->hitPattern().pixelLayersWithMeasurement();
                    tau_charged_nstriplayers[tau_charged_count] = track->hitPattern().stripLayersWithMeasurement();

                    if(dEdxharmonic2.isValid()) {
                        tau_charged_dedxharmonic2[tau_charged_count] = (*dEdxharmonic2)[track].dEdx();
                    } else {
                        tau_charged_dedxharmonic2[tau_charged_count] = -1.;
                    }

                    math::XYZPoint ecalPos = PositionOnECalSurface(TTrack);
                    tau_charged_outerx[tau_charged_count] = ecalPos.x();
                    tau_charged_outery[tau_charged_count] = ecalPos.y();
                    tau_charged_outerz[tau_charged_count] = ecalPos.z();
                    tau_charged_count++;

                    if(tau_charged_count == M_taumaxcount*10) {
                        break;
                    }
                }
            }

            tau_count++;

            if(tau_count == M_taumaxcount || tau_charged_count == M_taumaxcount*10) {
                cerr << "number of taus > M_jetmaxcount. They are missing." << endl;
                errors |= 1<<10;
                break;
            }

            if((*Taus)[i].pt() >= cTauPtMin && fabs((*Taus)[i].eta()) < cTauEtaMax) {
                NumGood++;
            }
        }
    }

    if(NumGood >= cTauNum) {
        return (true);
    }

    return (false);
}

bool RootMaker::AddPatTaus(const edm::Event &iEvent) {
    if(cdebug) { cout<<"AddPatTaus..."<<endl; }

    int NumGood = 0;
    edm::Handle<pat::TauCollection> Taus;
    iEvent.getByToken(patTausToken_, Taus);
    edm::Handle<pat::JetCollection> ak4pfchspuppiJets;
    iEvent.getByToken(tauJetsToken_, ak4pfchspuppiJets);

    if(cdebug) {
        cout<<"pat Taus.isValid() = "<<Taus.isValid()<<endl;
        cout<<"pat Taus->size() = "<<Taus->size()<<endl;
    }

    if(Taus.isValid()) {
        for(unsigned i = 0 ; i < Taus->size() ; i++) {
            tau_px[tau_count] = (*Taus)[i].px();
            tau_py[tau_count] = (*Taus)[i].py();
            tau_pz[tau_count] = (*Taus)[i].pz();
            tau_pt[tau_count] = (*Taus)[i].pt();
            tau_phi[tau_count] = (*Taus)[i].phi();
            tau_eta[tau_count] = (*Taus)[i].eta();

            for(unsigned n = 0; n < cTauDiscriminators.size(); n++) {
                if((*Taus)[i].tauID(cTauDiscriminators[n]) > 0.5) {
                    tau_dishps[tau_count] |= 1<<n;
                }
            }

            // if ((*Taus)[i].isPFTau()) {
            //     tau_emfraction[tau_count] = (*Taus)[i].emFraction();
            //     tau_hcaltotoverplead[tau_count] = (*Taus)[i].hcalTotOverPLead();
            //     tau_hcal3x3overplead[tau_count] = (*Taus)[i].hcalMaxOverPLead();
            //     tau_ecalstripsumeoverplead[tau_count] = (*Taus)[i].hcal3x3OverPLead();
            //     tau_bremsrecoveryeoverplead[tau_count] = (*Taus)[i].ecalStripSumEOverPLead();
            //     tau_calocomp[tau_count] = (*Taus)[i].caloComp();
            //     tau_segcomp[tau_count] = (*Taus)[i].segComp();

            //     tau_charge[tau_count] = (*Taus)[i].charge();
            //     tau_chargedbegin[tau_count] = tau_charged_count;
            //     PFJetRef thejet = (*Taus)[i].pfJetRef();
            //     bool jetfound = false;
            //     if(ak4pfchsJets.isValid()) {
            //         jetfound = true;
            //         tau_ak4pfjet_e[tau_count] = thejet->energy();
            //         tau_ak4pfjet_px[tau_count] = thejet->px();
            //         tau_ak4pfjet_py[tau_count] = thejet->py();
            //         tau_ak4pfjet_pz[tau_count] = thejet->pz();
            //         tau_ak4pfjet_hadronicenergy[tau_count] = thejet->chargedHadronEnergy() + thejet->neutralHadronEnergy();
            //         tau_ak4pfjet_chargedhadronicenergy[tau_count] = thejet->chargedHadronEnergy();
            //         tau_ak4pfjet_emenergy[tau_count] = thejet->chargedEmEnergy() + thejet->neutralEmEnergy();
            //         tau_ak4pfjet_chargedemenergy[tau_count] = thejet->chargedEmEnergy();
            //         tau_ak4pfjet_chargedmulti[tau_count] = thejet->chargedMultiplicity();
            //         tau_ak4pfjet_neutralmulti[tau_count] = thejet->neutralMultiplicity();
            //         tau_ak4pfjet_trigger[tau_count] = GetTrigger(*thejet, jettriggers);
            //         break;
            //     }
            //     if(!jetfound) {
            //         tau_ak4pfjet_e[tau_count] = -1.;
            //     }
            // }

            //tau_isolationchargednum[tau_count] = 0;
            tau_isolationchargedpt[tau_count] = (*Taus)[i].tauID("chargedIsoPtSum");
            //tau_isolationneutralsnum[tau_count] = 0.;
            tau_isolationneutralspt[tau_count] = (*Taus)[i].tauID("neutralIsoPtSum");
            tau_pucorrptsum[tau_count] = (*Taus)[i].tauID("puCorrPtSum");
            tau_isolationgammanum[tau_count] = (*Taus)[i].isolationPFGammaCands().size();
            //tau_isolationgammapt[tau_count] = (*Taus)[i].isolationPFGammaCandsEtSum();
            //tau_isolationchargednum[tau_count]  = (*Taus)[i].isolationPFChargedHadrCands().size();
            //tau_isolationneutralsnum[tau_count] = (*Taus)[i].isolationPFNeutrHadrCands().size();
            tau_trigger[tau_count] = GetTrigger((*Taus)[i], tautriggers);
            TrackRef track;

            const std::vector<reco::PFCandidatePtr> &tauSignalPFCands = (*Taus)[i].signalPFCands();

            for(std::vector<reco::PFCandidatePtr>::const_iterator tauSignalPFCand = tauSignalPFCands.begin(); tauSignalPFCand != tauSignalPFCands.end(); ++tauSignalPFCand) {
                track = (*tauSignalPFCand)->trackRef();

                if(!track.isNull()) {
                    iEvent.getByToken(dharmonicToken_, dEdxharmonic2);
                    TransientTrack TTrack = TTrackBuilder->build(track);
                    TrajectoryStateClosestToPoint TTrackState = TTrack.trajectoryStateClosestToPoint(GlobalPoint(pv_position.x(), pv_position.y(), pv_position.z()));
                    tau_charged_px[tau_charged_count] = TTrackState.momentum().x();
                    tau_charged_py[tau_charged_count] = TTrackState.momentum().y();
                    tau_charged_pz[tau_charged_count] = TTrackState.momentum().z();
                    tau_charged_closestpointx[tau_charged_count] = TTrackState.position().x();
                    tau_charged_closestpointy[tau_charged_count] = TTrackState.position().y();
                    tau_charged_closestpointz[tau_charged_count] = TTrackState.position().z();
                    tau_charged_dxy[tau_charged_count]    = TTrackState.perigeeParameters().transverseImpactParameter();
                    tau_charged_dxyerr[tau_charged_count] = TTrackState.perigeeError().transverseImpactParameterError();
                    tau_charged_dz[tau_charged_count]     = TTrackState.perigeeParameters().longitudinalImpactParameter();
                    tau_charged_dzerr[tau_charged_count]  = TTrackState.perigeeError().longitudinalImpactParameterError();
                    tau_charged_chi2[tau_charged_count]   = track->chi2();
                    tau_charged_ndof[tau_charged_count]   = track->ndof();
                    tau_charged_charge[tau_charged_count] = track->charge();
                    tau_charged_nhits[tau_charged_count] = track->numberOfValidHits();
                    tau_charged_nmissinghits[tau_charged_count] = track->numberOfLostHits();
                    tau_charged_npixelhits[tau_charged_count] = track->hitPattern().numberOfValidPixelHits();
                    tau_charged_npixellayers[tau_charged_count] = track->hitPattern().pixelLayersWithMeasurement();
                    tau_charged_nstriplayers[tau_charged_count] = track->hitPattern().stripLayersWithMeasurement();

                    if(dEdxharmonic2.isValid()) {
                        tau_charged_dedxharmonic2[tau_charged_count] = (*dEdxharmonic2)[track].dEdx();
                    } else {
                        tau_charged_dedxharmonic2[tau_charged_count] = -1.;
                    }

                    math::XYZPoint ecalPos = PositionOnECalSurface(TTrack);
                    tau_charged_outerx[tau_charged_count] = ecalPos.x();
                    tau_charged_outery[tau_charged_count] = ecalPos.y();
                    tau_charged_outerz[tau_charged_count] = ecalPos.z();
                    tau_charged_count++;

                    if(tau_charged_count == M_taumaxcount*10) {
                        break;
                    }
                }
            }

            tau_count++;

            if(tau_count == M_taumaxcount || tau_charged_count == M_taumaxcount*10) {
                cerr << "number of taus > M_jetmaxcount. They are missing." << endl;
                errors |= 1<<10;
                break;
            }

            if((*Taus)[i].pt() >= cTauPtMin && fabs((*Taus)[i].eta()) < cTauEtaMax) {
                NumGood++;
            }
        }
    }

    if(NumGood >= cTauNum) {
        return (true);
    }

    return (false);
}

bool RootMaker::AddTracks(const edm::Event &iEvent) {
    if(cdebug) { cout<<"AddTracks..."<<endl; }

    int NumGood = 0;
    edm::Handle<TrackCollection> Tracks;
    iEvent.getByToken(recoTracksToken_, Tracks);
    iEvent.getByToken(dharmonicToken_, dEdxharmonic2);

    if(cdebug) { cout<<"Tracks.isValid() = "<<Tracks.isValid()<<endl; }

    if(Tracks.isValid()) {
        for(unsigned i = 0 ; i < Tracks->size() ; i++) {
            int numvtx = getPrimVertex((*Tracks)[i]);
            TransientTrack TTrack = TTrackBuilder->build((*Tracks)[i]);
            TrajectoryStateClosestToPoint TTrackState;

            if(numvtx == -1) {
                TTrackState = TTrack.trajectoryStateClosestToPoint(GlobalPoint(pv_position.x(), pv_position.y(), pv_position.z()));
            } else {
                TTrackState = TTrack.trajectoryStateClosestToPoint(GlobalPoint(primvertex_x[numvtx], primvertex_y[numvtx], primvertex_z[numvtx]));
            }

            if(TTrackState.pt() >= cTrackFilterPtMin) {
                track_vtx[track_count] = numvtx;
                track_px[track_count] = TTrackState.momentum().x();
                track_py[track_count] = TTrackState.momentum().y();
                track_pz[track_count] = TTrackState.momentum().z();
                track_closestpointx[track_count] = TTrackState.position().x();
                track_closestpointy[track_count] = TTrackState.position().y();
                track_closestpointz[track_count] = TTrackState.position().z();
                track_dxy[track_count]    = TTrackState.perigeeParameters().transverseImpactParameter();
                track_dxyerr[track_count] = TTrackState.perigeeError().transverseImpactParameterError();
                track_dz[track_count]     = TTrackState.perigeeParameters().longitudinalImpactParameter();
                track_dzerr[track_count]  = TTrackState.perigeeError().longitudinalImpactParameterError();
                track_chi2[track_count]   = (*Tracks)[i].chi2();
                track_ndof[track_count]   = (*Tracks)[i].ndof();
                track_charge[track_count] = (*Tracks)[i].charge();
                track_nhits[track_count] = (*Tracks)[i].numberOfValidHits();
                track_nmissinghits[track_count] = (*Tracks)[i].numberOfLostHits();
                track_npixelhits[track_count] = (*Tracks)[i].hitPattern().numberOfValidPixelHits();
                track_npixellayers[track_count] = (*Tracks)[i].hitPattern().pixelLayersWithMeasurement();
                track_nstriplayers[track_count] = (*Tracks)[i].hitPattern().stripLayersWithMeasurement();
                TrackRef tr  = TrackRef(Tracks, i);

                if(dEdxharmonic2.isValid()) {
                    track_dedxharmonic2[track_count] = (*dEdxharmonic2)[tr].dEdx();
                } else {
                    track_dedxharmonic2[track_count] = -1.;
                }

                math::XYZPoint ecalPos = PositionOnECalSurface(TTrack);
                track_outerx[track_count] = ecalPos.x();
                track_outery[track_count] = ecalPos.y();
                track_outerz[track_count] = ecalPos.z();
                track_count++;

                if(track_count == M_trackmaxcount) {
                    cerr << "number of tracks > M_trackmaxcount. They are missing." << endl;
                    errors |= 1<<2;
                    break;
                }

                if(TTrackState.pt() >= cTrackPtMin && fabs((*Tracks)[i].eta()) <= cTrackEtaMax) {
                    NumGood++;
                    //double energy = sqrt(pow(track_px[i],2) + pow(track_py[i],2) + pow(track_pz[i],2));
                    //TrackVector.push_back(TLorentzVector(track_px[i], track_py[i], track_pz[i], energy));
                }
            }
        }

    }

    if(NumGood >= cTrackNum) {
        return (true);
    }

    return (false);
}

bool RootMaker::AddAK4CaloJets(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
    if(cdebug) { cout<<"AddAK4CaloJets..."<<endl; }

    int NumGood = 0;
    edm::Handle<pat::JetCollection> ak4caloJets;
    iEvent.getByToken(ak4caloJetsToken_, ak4caloJets);

    if(cdebug) { cout<<"ak4caloJets.isValid() = "<<ak4caloJets.isValid()<<endl; }

    if(ak4caloJets.isValid()) {
        for(unsigned i = 0 ; i < ak4caloJets->size() ; i++) {
            if((*ak4caloJets)[i].pt() >= cAK4CaloFilterPtMin) {
                ak4calojet_e[ak4calojet_count] = (*ak4caloJets)[i].energy();
                ak4calojet_px[ak4calojet_count] = (*ak4caloJets)[i].px();
                ak4calojet_py[ak4calojet_count] = (*ak4caloJets)[i].py();
                ak4calojet_pz[ak4calojet_count] = (*ak4caloJets)[i].pz();
                ak4calojet_hadronicenergy[ak4calojet_count] = (*ak4caloJets)[i].energyFractionHadronic() * (*ak4caloJets)[i].energy();
                ak4calojet_emenergy[ak4calojet_count] = (*ak4caloJets)[i].emEnergyFraction() * (*ak4caloJets)[i].energy();

                if((*ak4caloJets)[i].jecSetsAvailable()) {
                    ak4calojet_energycorr[ak4calojet_count] = (*ak4caloJets)[i].jecFactor("Uncorrected");
                    ak4calojet_energycorrl7uds[ak4calojet_count] = -1.;//(*ak4caloJets)[i].jecFactor("L7Parton", "UDS");
                    ak4calojet_energycorrl7bottom[ak4calojet_count] = -1;//(*ak4caloJets)[i].jecFactor("L7Parton", "BOTTOM");
                } else {
                    ak4calojet_energycorr[ak4calojet_count] = -1.;
                    ak4calojet_energycorrl7uds[ak4calojet_count] = -1.;
                    ak4calojet_energycorrl7bottom[ak4calojet_count] = -1.;
                }

                for(unsigned n = 0 ; n < bdisclabel.size() ; n++) {
                    ak4calojet_btag[ak4calojet_count][n] = (*ak4caloJets)[i].bDiscriminator(bdisclabel[n]);
                    // if(bdisclabel[n] != "F") {
                    //     ak4calojet_btag[ak4calojet_count][n] = (*ak4caloJets)[i].bDiscriminator(bdisclabel[n]);
                    // } else {
                    //     ak4calojet_btag[ak4calojet_count][n] = -1000;
                    // }
                }

                ak4calojet_fhpd[ak4calojet_count] = (*ak4caloJets)[i].jetID().fHPD;
                ak4calojet_restrictedemf[ak4calojet_count] = (*ak4caloJets)[i].jetID().restrictedEMF;
// 13-01
                //for(unsigned n = 0 ; n < min((*ak4caloJets)[i].getPairDiscri().size(), unsigned(M_btagmax)) ; n++) {
                //  cout << (*ak4caloJets)[i].getPairDiscri()[n].first << " " << (*ak4caloJets)[i].getPairDiscri()[n].second << " " << ak4calojet_btag[i][n] << endl;
                //}
                ak4calojet_n90[ak4calojet_count] = (*ak4caloJets)[i].n90();
                ak4calojet_n60[ak4calojet_count] = (*ak4caloJets)[i].n60();
                ak4calojet_count++;

                if(ak4calojet_count == M_jetmaxcount) {
                    cerr << "number of ak4calojet > M_jetmaxcount. They are missing." << endl;
                    errors |= 1<<8;
                    break;
                }

                if((*ak4caloJets)[i].pt() >= cAK4CaloPtMin && fabs((*ak4caloJets)[i].eta()) < cAK4CaloEtaMax) {
                    NumGood++;
                }
            }
        }
    }

    if(NumGood >= cAK4CaloNum) {
        return (true);
    }

    return (false);
}

bool RootMaker::AddAK4JPTJets(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
    if(cdebug) { cout<<"AddAK4JPTJets..."<<endl; }

    int NumGood = 0;
    edm::Handle<pat::JetCollection> ak4jptJets;
    iEvent.getByToken(ak4jptJetsToken_, ak4jptJets);

    if(cdebug) { cout<<"ak4jptJets.isValid() = "<<ak4jptJets.isValid()<<endl; }

    if(ak4jptJets.isValid()) {
        for(unsigned i = 0 ; i < ak4jptJets->size() ; i++) {
            if((*ak4jptJets)[i].pt() >= cAK4JPTFilterPtMin) {
                ak4jptjet_e[ak4jptjet_count] = (*ak4jptJets)[i].energy();
                ak4jptjet_px[ak4jptjet_count] = (*ak4jptJets)[i].px();
                ak4jptjet_py[ak4jptjet_count] = (*ak4jptJets)[i].py();
                ak4jptjet_pz[ak4jptjet_count] = (*ak4jptJets)[i].pz();
                ak4jptjet_hadronicenergy[ak4jptjet_count] = (*ak4jptJets)[i].chargedHadronEnergy() + (*ak4jptJets)[i].neutralHadronEnergy();
                ak4jptjet_chargedhadronicenergy[ak4jptjet_count] = (*ak4jptJets)[i].chargedHadronEnergy();
                ak4jptjet_emenergy[ak4jptjet_count] = (*ak4jptJets)[i].chargedEmEnergy() + (*ak4jptJets)[i].neutralEmEnergy();
                ak4jptjet_chargedemenergy[ak4jptjet_count] = (*ak4jptJets)[i].chargedEmEnergy();
                ak4jptjet_chargedmulti[ak4jptjet_count] = (*ak4jptJets)[i].chargedMultiplicity();

                if((*ak4jptJets)[i].jecSetsAvailable()) {
                    ak4jptjet_energycorr[ak4jptjet_count] = (*ak4jptJets)[i].jecFactor("Uncorrected");
                    ak4jptjet_energycorrl7uds[ak4jptjet_count] = -1.;//(*ak4jptJets)[i].jecFactor("L7Parton", "UDS");
                    ak4jptjet_energycorrl7bottom[ak4jptjet_count] = -1.;//(*ak4jptJets)[i].jecFactor("L7Parton", "BOTTOM");
                } else {
                    ak4jptjet_energycorr[ak4jptjet_count] = -1.;
                    ak4jptjet_energycorrl7uds[ak4jptjet_count] = -1.;
                    ak4jptjet_energycorrl7bottom[ak4jptjet_count] = -1.;
                }

                for(unsigned n = 0 ; n < bdisclabel.size() ; n++) {
                    ak4jptjet_btag[ak4jptjet_count][n] = (*ak4jptJets)[i].bDiscriminator(bdisclabel[n]);
                    // if(bdisclabel[n] != "F") {
                    //     ak4jptjet_btag[ak4jptjet_count][n] = (*ak4jptJets)[i].bDiscriminator(bdisclabel[n]);
                    // } else {
                    //     ak4jptjet_btag[ak4jptjet_count][n] = -1000;
                    // }
                }

                ak4jptjet_fhpd[ak4jptjet_count] = (*ak4jptJets)[i].jetID().fHPD;
                ak4jptjet_restrictedemf[ak4jptjet_count] = (*ak4jptJets)[i].jetID().restrictedEMF;
                ak4jptjet_n90[ak4jptjet_count] = (*ak4jptJets)[i].jetID().n90Hits;
                ak4jptjet_count++;

                if(ak4jptjet_count == M_jetmaxcount) {
                    cerr << "number of ak4jptjet > M_jetmaxcount. They are missing." << endl;
                    errors |= 1<<9;
                    break;
                }

                if((*ak4jptJets)[i].pt() >= cAK4JPTPtMin && fabs((*ak4jptJets)[i].eta()) < cAK4JPTEtaMax) {
                    NumGood++;
                }
            }
        }
    }

    if(NumGood >= cAK4JPTNum) {
        return (true);
    }

    return (false);
}

bool RootMaker::AddAK4PFCHSJets(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
    if(cdebug)  { cout<<"AddAK4PFCHSJets..."<<endl; }

    int NumGood = 0;
    edm::Handle<pat::JetCollection> ak4pfJets;
    iEvent.getByToken(ak4pfchsJetsToken_, ak4pfJets);
    edm::Handle<JetFlavourMatchingCollection> jetMCFlHandle;
    iEvent.getByLabel("AK4byValAlgo", jetMCFlHandle);

    if(cdebug) {
        cout<<"chs ak4pfJets.isValid() = "<<ak4pfJets.isValid()<<endl;
        cout<<"chs ak4pfJets->size() = "<<ak4pfJets->size()<<endl;
    }

    if(ak4pfJets.isValid()) {
        for(unsigned i = 0 ; i < ak4pfJets->size() ; i++) {
            pat::Jet corjet((*ak4pfJets)[i]);

            if(corjet.pt() >= cAK4PFCHSFilterPtMin) {
                ak4pfchsjet_e[ak4pfchsjet_count] = corjet.energy();
                ak4pfchsjet_px[ak4pfchsjet_count] = corjet.px();
                ak4pfchsjet_py[ak4pfchsjet_count] = corjet.py();
                ak4pfchsjet_pz[ak4pfchsjet_count] = corjet.pz();
                ak4pfchsjet_pt[ak4pfchsjet_count] = corjet.pt();
                ak4pfchsjet_phi[ak4pfchsjet_count] = corjet.phi();
                ak4pfchsjet_eta[ak4pfchsjet_count] = corjet.eta();
                ak4pfchsjet_area[ak4pfchsjet_count] = corjet.jetArea();
                ak4pfchsjet_hadronicenergy[ak4pfchsjet_count] = corjet.chargedHadronEnergy() + corjet.neutralHadronEnergy();
                ak4pfchsjet_chargedhadronicenergy[ak4pfchsjet_count] = corjet.chargedHadronEnergy();
                ak4pfchsjet_emenergy[ak4pfchsjet_count] = corjet.chargedEmEnergy() + corjet.neutralEmEnergy();
                ak4pfchsjet_chargedemenergy[ak4pfchsjet_count] = corjet.chargedEmEnergy();
                ak4pfchsjet_hfhadronicenergy[ak4pfchsjet_count] = corjet.HFHadronEnergy();
                ak4pfchsjet_hfemenergy[ak4pfchsjet_count] = corjet.HFEMEnergy();
                ak4pfchsjet_electronenergy[ak4pfchsjet_count] = corjet.electronEnergy();
                ak4pfchsjet_muonenergy[ak4pfchsjet_count] = corjet.muonEnergy();
                ak4pfchsjet_chargedmulti[ak4pfchsjet_count] = corjet.chargedMultiplicity();
                ak4pfchsjet_neutralmulti[ak4pfchsjet_count] = corjet.neutralMultiplicity();
                ak4pfchsjet_hfhadronicmulti[ak4pfchsjet_count] = corjet.HFHadronMultiplicity();
                ak4pfchsjet_hfemmulti[ak4pfchsjet_count] = corjet.HFEMMultiplicity();
                ak4pfchsjet_electronmulti[ak4pfchsjet_count] = corjet.electronMultiplicity();
                ak4pfchsjet_muonmulti[ak4pfchsjet_count] = corjet.muonMultiplicity();
                ak4pfchsjet_energycorr[ak4pfchsjet_count] = corjet.jecFactor("Uncorrected");
                ak4pfchsjet_energycorrunc[ak4pfchsjet_count] = -1;
                ak4pfchsjet_energycorrl7uds[ak4pfchsjet_count] = -1.;//corjet.jecFactor("L7Parton", "UDS");
                ak4pfchsjet_energycorrl7bottom[ak4pfchsjet_count] = -1.;//corjet.jecFactor("L7Parton", "BOTTOM");

                JetShape shape;

                if(!cisMiniAOD) {
                    shape = getJetShape(corjet);
                }

                if(cisMiniAOD) {
                    shape = getSlimmedJetShape(corjet);
                }

                ak4pfchsjet_chargeda[ak4pfchsjet_count] = shape.chargeda;
                ak4pfchsjet_chargedb[ak4pfchsjet_count] = shape.chargedb;
                ak4pfchsjet_neutrala[ak4pfchsjet_count] = shape.neutrala;
                ak4pfchsjet_neutralb[ak4pfchsjet_count] = shape.neutralb;
                ak4pfchsjet_alla[ak4pfchsjet_count] = shape.alla;
                ak4pfchsjet_allb[ak4pfchsjet_count] = shape.allb;
                ak4pfchsjet_chargedfractionmv[ak4pfchsjet_count] = shape.chargedfractionmv;

                ak4pfchsjet_mcflavour[ak4pfchsjet_count] = 0;

                if(jetMCFlHandle.isValid()) {
                    const JetFlavourMatchingCollection &jetMCFl = * (jetMCFlHandle.product());
                    double drmin = 0.5;
                    int num = -1;

                    for(size_t u = 0; u < jetMCFl.size(); u++) {
                        double dr =  DR(* (jetMCFl[u].first), corjet);

                        if(dr < drmin) {
                            drmin = dr;
                            num = u;
                        }
                    }

                    if(num != -1) {
                        ak4pfchsjet_mcflavour[ak4pfchsjet_count] = jetMCFl[num].second.getFlavour();
                    }
                }

                for(unsigned n = 0; n < bdisclabel.size(); n++) {
                    ak4pfchsjet_btag[ak4pfchsjet_count][n] = corjet.bDiscriminator(bdisclabel[n]);
                    // ak4pfchsjet_btag[ak4pfchsjet_count][n] = -1000000;
                    // edm::Handle<JetTagCollection> bTagHandle;
                    // iEvent.getByLabel(edm::InputTag(bdisclabel[n], "", "ROOTMAKER"), bTagHandle);
                    // if(bTagHandle.isValid()) {
                    //     const JetTagCollection &bTags = * (bTagHandle.product());
                    //     double drmin = 0.5;
                    //     int num = -1;
                    //     for(size_t u = 0; u < bTags.size(); u++) {
                    //         double dr =  DR(* (bTags[u].first), corjet);
                    //         if(dr < drmin) {
                    //             drmin = dr;
                    //             num = u;
                    //         }
                    //     }
                    //     if(num != -1) {
                    //         ak4pfchsjet_btag[ak4pfchsjet_count][n] = bTags[num].second;
                    //     }
                    // }
                }

                ak4pfchsjet_trigger[ak4pfchsjet_count] = GetTrigger(corjet, jettriggers);
                ak4pfchsjet_count++;

                if(ak4pfchsjet_count == M_jetmaxcount) {
                    cerr << "number of ak4pfchsjet > M_jetmaxcount. They are missing." << endl;
                    errors |= 1<<25;
                    break;
                }

                if(corjet.pt() >= cAK4PFCHSPtMin && fabs(corjet.eta()) < cAK4PFCHSEtaMax) {
                    NumGood++;
                }
            }
        }
    }

    if(NumGood >= cAK4PFCHSNum) {
        return (true);
    }

    return (false);
}
// pileup mitigated jets
bool RootMaker::AddAK4PFCHSPuppiJets(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
    if(cdebug) { cout<<"AddAK4PFCHSPuppiJets..."<<endl; }

    int NumGood = 0;
    edm::Handle<pat::JetCollection> ak4pfJets;
    iEvent.getByToken(ak4pfchsJetsPuppiToken_, ak4pfJets);
    edm::Handle<JetFlavourMatchingCollection> jetMCFlHandle;
    iEvent.getByLabel("AK4byValAlgo", jetMCFlHandle);

    if(cdebug) {
        cout<<"chs ak4pfJets.isValid() = "<<ak4pfJets.isValid()<<endl;
        cout<<"chs ak4pfJets->size() = "<<ak4pfJets->size()<<endl;
    }

    if(ak4pfJets.isValid()) {
        for(unsigned i = 0 ; i < ak4pfJets->size() ; i++) {
            pat::Jet corjet((*ak4pfJets)[i]);

            if(corjet.pt() >= cAK4PFCHSPuppiFilterPtMin) {
                ak4pfchspuppijet_e[ak4pfchspuppijet_count] = corjet.energy();
                ak4pfchspuppijet_px[ak4pfchspuppijet_count] = corjet.px();
                ak4pfchspuppijet_py[ak4pfchspuppijet_count] = corjet.py();
                ak4pfchspuppijet_pz[ak4pfchspuppijet_count] = corjet.pz();
                ak4pfchspuppijet_pt[ak4pfchspuppijet_count] = corjet.pt();
                ak4pfchspuppijet_phi[ak4pfchspuppijet_count] = corjet.phi();
                ak4pfchspuppijet_eta[ak4pfchspuppijet_count] = corjet.eta();
                ak4pfchspuppijet_area[ak4pfchspuppijet_count] = corjet.jetArea();
                ak4pfchspuppijet_hadronicenergy[ak4pfchspuppijet_count] = corjet.chargedHadronEnergy() + corjet.neutralHadronEnergy();
                ak4pfchspuppijet_chargedhadronicenergy[ak4pfchspuppijet_count] = corjet.chargedHadronEnergy();
                ak4pfchspuppijet_emenergy[ak4pfchspuppijet_count] = corjet.chargedEmEnergy() + corjet.neutralEmEnergy();
                ak4pfchspuppijet_chargedemenergy[ak4pfchspuppijet_count] = corjet.chargedEmEnergy();
                ak4pfchspuppijet_hfhadronicenergy[ak4pfchspuppijet_count] = corjet.HFHadronEnergy();
                ak4pfchspuppijet_hfemenergy[ak4pfchspuppijet_count] = corjet.HFEMEnergy();
                ak4pfchspuppijet_electronenergy[ak4pfchspuppijet_count] = corjet.electronEnergy();
                ak4pfchspuppijet_muonenergy[ak4pfchspuppijet_count] = corjet.muonEnergy();
                ak4pfchspuppijet_chargedmulti[ak4pfchspuppijet_count] = corjet.chargedMultiplicity();
                ak4pfchspuppijet_neutralmulti[ak4pfchspuppijet_count] = corjet.neutralMultiplicity();
                ak4pfchspuppijet_hfhadronicmulti[ak4pfchspuppijet_count] = corjet.HFHadronMultiplicity();
                ak4pfchspuppijet_hfemmulti[ak4pfchspuppijet_count] = corjet.HFEMMultiplicity();
                ak4pfchspuppijet_electronmulti[ak4pfchspuppijet_count] = corjet.electronMultiplicity();
                ak4pfchspuppijet_muonmulti[ak4pfchspuppijet_count] = corjet.muonMultiplicity();
                ak4pfchspuppijet_energycorr[ak4pfchspuppijet_count] = corjet.jecFactor("Uncorrected");
                ak4pfchspuppijet_energycorrunc[ak4pfchspuppijet_count] = -1;
                ak4pfchspuppijet_energycorrl7uds[ak4pfchspuppijet_count] = -1.;//corjet.jecFactor("L7Parton", "UDS");
                ak4pfchspuppijet_energycorrl7bottom[ak4pfchspuppijet_count] = -1.;//corjet.jecFactor("L7Parton", "BOTTOM");

                JetShape shape;

                if(!cisMiniAOD) {
                    shape = getJetShape(corjet);
                }

                if(cisMiniAOD) {
                    shape = getSlimmedJetShape(corjet);
                }

                ak4pfchspuppijet_chargeda[ak4pfchspuppijet_count] = shape.chargeda;
                ak4pfchspuppijet_chargedb[ak4pfchspuppijet_count] = shape.chargedb;
                ak4pfchspuppijet_neutrala[ak4pfchspuppijet_count] = shape.neutrala;
                ak4pfchspuppijet_neutralb[ak4pfchspuppijet_count] = shape.neutralb;
                ak4pfchspuppijet_alla[ak4pfchspuppijet_count] = shape.alla;
                ak4pfchspuppijet_allb[ak4pfchspuppijet_count] = shape.allb;
                ak4pfchspuppijet_chargedfractionmv[ak4pfchspuppijet_count] = shape.chargedfractionmv;

                ak4pfchspuppijet_mcflavour[ak4pfchspuppijet_count] = 0;

                if(jetMCFlHandle.isValid()) {
                    const JetFlavourMatchingCollection &jetMCFl = * (jetMCFlHandle.product());
                    double drmin = 0.5;
                    int num = -1;

                    for(size_t u = 0; u < jetMCFl.size(); u++) {
                        double dr =  DR(* (jetMCFl[u].first), corjet);

                        if(dr < drmin) {
                            drmin = dr;
                            num = u;
                        }
                    }

                    if(num != -1) {
                        ak4pfchspuppijet_mcflavour[ak4pfchspuppijet_count] = jetMCFl[num].second.getFlavour();
                    }
                }

                for(unsigned n = 0; n < bdisclabel.size(); n++) {
                    ak4pfchspuppijet_btag[ak4pfchspuppijet_count][n] = corjet.bDiscriminator(bdisclabel[n]);
                    // ak4pfchspuppijet_btag[ak4pfchspuppijet_count][n] = -1000000;
                    // edm::Handle<JetTagCollection> bTagHandle;
                    // iEvent.getByLabel(edm::InputTag(bdisclabel[n], "", "ROOTMAKER"), bTagHandle);
                    // if(bTagHandle.isValid()) {
                    //     const JetTagCollection &bTags = * (bTagHandle.product());
                    //     double drmin = 0.5;
                    //     int num = -1;
                    //     for(size_t u = 0; u < bTags.size(); u++) {
                    //         double dr =  DR(* (bTags[u].first), corjet);
                    //         if(dr < drmin) {
                    //             drmin = dr;
                    //             num = u;
                    //         }
                    //     }
                    //     if(num != -1) {
                    //         ak4pfchspuppijet_btag[ak4pfchspuppijet_count][n] = bTags[num].second;
                    //     }
                    // }
                }

                ak4pfchspuppijet_trigger[ak4pfchspuppijet_count] = GetTrigger(corjet, jettriggers);
                ak4pfchspuppijet_count++;

                if(ak4pfchspuppijet_count == M_jetmaxcount) {
                    cerr << "number of ak4pfchspuppijet > M_jetmaxcount. They are missing." << endl;
                    errors |= 1<<25;
                    break;
                }

                if(corjet.pt() >= cAK4PFCHSPtMin && fabs(corjet.eta()) < cAK4PFCHSEtaMax) {
                    NumGood++;
                }
            }
        }
    }

    if(NumGood >= cAK4PFCHSPuppiNum) {
        return (true);
    }

    return (false);
}

bool RootMaker::AddAK4PFJets(const edm::Event &iEvent, const edm::EventSetup &iSetup) {
    if(cdebug) { cout<<"AddAK4PFJets..."<<endl; }

    int NumGood = 0;
    edm::Handle<PFJetCollection> ak4pfJets;
    iEvent.getByToken(ak4pfJetsToken_, ak4pfJets);
    const JetCorrector *corrector = JetCorrector::getJetCorrector((string("ak4PF")+cJetCorrection).c_str(), iSetup);
    edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
    iSetup.get<JetCorrectionsRecord>().get("AK4PF",JetCorParColl);
    JetCorrectorParameters const &JetCorPar = (*JetCorParColl)["Uncertainty"];
    JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(JetCorPar);
    edm::Handle<JetFlavourMatchingCollection> jetMCFlHandle;
    iEvent.getByLabel("AK4byValAlgo", jetMCFlHandle);
    edm::Handle<ValueMap<float> > puidfullHandle;
    iEvent.getByLabel(edm::InputTag("recoPuJetMva", "fullDiscriminant", "ROOTMAKER"), puidfullHandle);
    //edm::Handle<ValueMap<float> > puidsimpleHandle;
    //iEvent.getByLabel(edm::InputTag("recoPuJetMva", "simpleDiscriminant", "ROOTMAKER"), puidsimpleHandle);
    edm::Handle<ValueMap<float> > puidcutbasedHandle;
    iEvent.getByLabel(edm::InputTag("recoPuJetMva", "cutbasedDiscriminant", "ROOTMAKER"), puidcutbasedHandle);

    if(cdebug) { cout<<"ak4pfJets.isValid() = "<<ak4pfJets.isValid()<<endl; }

    if(ak4pfJets.isValid()) {
        for(unsigned i = 0 ; i < ak4pfJets->size() ; i++) {
            PFJet corjet((*ak4pfJets)[i]);
            PFJetRef jetref(ak4pfJets, i);
            double jec = corrector->correction(corjet, iEvent, iSetup);
            corjet.scaleEnergy(jec);

            if(corjet.pt() >= cAK4PFFilterPtMin) {
                ak4pfjet_e[ak4pfjet_count] = corjet.energy();
                ak4pfjet_px[ak4pfjet_count] = corjet.px();
                ak4pfjet_py[ak4pfjet_count] = corjet.py();
                ak4pfjet_pz[ak4pfjet_count] = corjet.pz();
                ak4pfjet_area[ak4pfjet_count] = corjet.jetArea();
                ak4pfjet_hadronicenergy[ak4pfjet_count] = corjet.chargedHadronEnergy() + corjet.neutralHadronEnergy();
                ak4pfjet_chargedhadronicenergy[ak4pfjet_count] = corjet.chargedHadronEnergy();
                ak4pfjet_emenergy[ak4pfjet_count] = corjet.chargedEmEnergy() + corjet.neutralEmEnergy();
                ak4pfjet_chargedemenergy[ak4pfjet_count] = corjet.chargedEmEnergy();
                ak4pfjet_hfhadronicenergy[ak4pfjet_count] = corjet.HFHadronEnergy();
                ak4pfjet_hfemenergy[ak4pfjet_count] = corjet.HFEMEnergy();
                ak4pfjet_electronenergy[ak4pfjet_count] = corjet.electronEnergy();
                ak4pfjet_muonenergy[ak4pfjet_count] = corjet.muonEnergy();
                ak4pfjet_chargedmulti[ak4pfjet_count] = corjet.chargedMultiplicity();
                ak4pfjet_neutralmulti[ak4pfjet_count] = corjet.neutralMultiplicity();
                ak4pfjet_hfhadronicmulti[ak4pfjet_count] = corjet.HFHadronMultiplicity();
                ak4pfjet_hfemmulti[ak4pfjet_count] = corjet.HFEMMultiplicity();
                ak4pfjet_electronmulti[ak4pfjet_count] = corjet.electronMultiplicity();
                ak4pfjet_muonmulti[ak4pfjet_count] = corjet.muonMultiplicity();
                ak4pfjet_energycorr[ak4pfjet_count] = 1./jec;
                jecUnc->setJetEta(corjet.eta());
                jecUnc->setJetPt(corjet.pt());
                ak4pfjet_energycorrunc[ak4pfjet_count] = jecUnc->getUncertainty(true);
                //cout << ak4pfjet_energycorr[ak4pfjet_count] << endl;
                ak4pfjet_energycorrl7uds[ak4pfjet_count] = -1.;//corjet.jecFactor("L7Parton", "UDS");
                ak4pfjet_energycorrl7bottom[ak4pfjet_count] = -1.;//corjet.jecFactor("L7Parton", "BOTTOM");
                ak4pfjet_puidfull[ak4pfjet_count] = (*puidfullHandle)[jetref];
                //ak4pfjet_puidsimple[ak4pfjet_count] = (*puidsimpleHandle)[jetref];
                ak4pfjet_puidcutbased[ak4pfjet_count] = (*puidcutbasedHandle)[jetref];
                JetShape shape = getJetShape(corjet);
                ak4pfjet_chargeda[ak4pfjet_count] = shape.chargeda;
                ak4pfjet_chargedb[ak4pfjet_count] = shape.chargedb;
                ak4pfjet_neutrala[ak4pfjet_count] = shape.neutrala;
                ak4pfjet_neutralb[ak4pfjet_count] = shape.neutralb;
                ak4pfjet_alla[ak4pfjet_count] = shape.alla;
                ak4pfjet_allb[ak4pfjet_count] = shape.allb;
                ak4pfjet_chargedfractionmv[ak4pfjet_count] = shape.chargedfractionmv;
                ak4pfjet_mcflavour[ak4pfjet_count] = 0;

                if(jetMCFlHandle.isValid()) {
                    const JetFlavourMatchingCollection &jetMCFl = * (jetMCFlHandle.product());
                    double drmin = 0.5;
                    int num = -1;

                    for(size_t u = 0; u < jetMCFl.size(); u++) {
                        double dr =  DR(* (jetMCFl[u].first), corjet);

                        if(dr < drmin) {
                            drmin = dr;
                            num = u;
                        }
                    }

                    if(num != -1) {
                        ak4pfjet_mcflavour[ak4pfjet_count] = jetMCFl[num].second.getFlavour();
                    }
                }

                for(unsigned n = 0 ; n < bdisclabel.size() ; n++) {
                    ak4pfjet_btag[ak4pfjet_count][n] = -1000000;
                    //ak4pfjet_btag[ak4pfjet_count][n] = corjet.bDiscriminator(bdisclabel[n]);
                    edm::Handle<JetTagCollection> bTagHandle;
                    iEvent.getByLabel(edm::InputTag(bdisclabel[n], "", "ROOTMAKER"), bTagHandle);

                    if(bTagHandle.isValid()) {
                        const JetTagCollection &bTags = * (bTagHandle.product());
                        double drmin = 0.5;
                        int num = -1;

                        for(size_t u = 0; u < bTags.size(); u++) {
                            double dr =  DR(* (bTags[u].first), corjet);

                            if(dr < drmin) {
                                drmin = dr;
                                num = u;
                            }
                        }

                        if(num != -1) {
                            ak4pfjet_btag[ak4pfjet_count][n] = bTags[num].second;
                        }
                    }
                }

                ak4pfjet_trigger[ak4pfjet_count] = GetTrigger(corjet, jettriggers);
                ak4pfjet_count++;

                if(ak4pfjet_count == M_jetmaxcount) {
                    cerr << "number of ak4pfjet > M_jetmaxcount. They are missing." << endl;
                    errors |= 1<<10;
                    break;
                }

                if(corjet.pt() >= cAK4PFPtMin && fabs(corjet.eta()) < cAK4PFEtaMax) {
                    NumGood++;
                }
            }
        }
    }

    delete jecUnc;

    if(NumGood >= cAK4PFNum) {
        return (true);
    }

    return (false);
}

RootMaker::JetShape RootMaker::getJetShape(const pat::Jet &jet) {
    if(cdebug) { cout<<"getJetShape..."<<endl; }

    using namespace TMath;
    RootMaker::JetShape res;
    float chargedetaeta1 = 0.;
    float chargedphiphi1 = 0.;
    float chargedetaeta2 = 0.;
    float chargedphiphi2 = 0.;
    float chargedetaphi = 0.;
    float chargedptsum = 0.;
    float chargedptsummv = 0.;
    float neutraletaeta1 = 0.;
    float neutralphiphi1 = 0.;
    float neutraletaeta2 = 0.;
    float neutralphiphi2 = 0.;
    float neutraletaphi = 0.;
    float neutralptsum = 0.;
    float alletaeta1 = 0.;
    float alletaeta2 = 0.;
    float alletaphi = 0.;
    float allphiphi1 = 0.;
    float allphiphi2 = 0.;
    float allptsum = 0.;
    vector<PFCandidatePtr> constituents(jet.getPFConstituents());

    for(size_t i = 0 ; i < constituents.size() ; ++i) {
        const PFCandidate &con = * (constituents[i]);
        float deta = jet.eta() - con.eta();
        float dphi = jet.phi() - con.phi();

        if(dphi > 4.*atan(1.)) {
            dphi = dphi-8.*atan(1.);
        }

        if(dphi < -1.*4.*atan(1.)) {
            dphi = dphi+8.*atan(1.);
        }

        if(con.trackRef().isNonnull()) {
            chargedptsum += con.pt();
            chargedetaeta1 += deta*con.pt();
            chargedetaeta2 += deta*deta*con.pt();
            chargedetaphi += deta*dphi*con.pt();
            chargedphiphi1 += dphi*con.pt();
            chargedphiphi2 += dphi*dphi*con.pt();
            int vertex = getPrimVertex(* (con.trackRef()));

            if(vertex == 0 || vertex == -1) {
                chargedptsummv += con.pt();
            }
        } else {
            neutralptsum += con.pt();
            neutraletaeta1 += deta*con.pt();
            neutraletaeta2 += deta*deta*con.pt();
            neutraletaphi += deta*dphi*con.pt();
            neutralphiphi1 += dphi*con.pt();
            neutralphiphi2 += dphi*dphi*con.pt();
        }

        allptsum += con.pt();
        alletaeta1 += deta*con.pt();
        alletaeta2 += deta*deta*con.pt();
        alletaphi += deta*dphi*con.pt();
        allphiphi1 += dphi*con.pt();
        allphiphi2 += dphi*dphi*con.pt();
    }

    if(chargedptsum != 0) {
        chargedetaeta1/=chargedptsum;
        chargedetaeta2/=chargedptsum;
        chargedetaphi/=chargedptsum;
        chargedphiphi1/=chargedptsum;
        chargedphiphi2/=chargedptsum;
    } else {
        chargedetaeta1 = 0.;
        chargedetaeta2 = 0.;
        chargedetaphi = 0.;
        chargedphiphi1 = 0.;
        chargedphiphi2 = 0.;
    }

    if(neutralptsum != 0) {
        neutraletaeta1/=neutralptsum;
        neutraletaeta2/=neutralptsum;
        neutraletaphi/=neutralptsum;
        neutralphiphi1/=neutralptsum;
        neutralphiphi2/=neutralptsum;
    } else {
        neutraletaeta1 = 0.;
        neutraletaeta2 = 0.;
        neutraletaphi = 0.;
        neutralphiphi1 = 0.;
        neutralphiphi2 = 0.;
    }

    if(allptsum != 0) {
        alletaeta1/=allptsum;
        alletaeta2/=allptsum;
        alletaphi/=allptsum;
        allphiphi1/=allptsum;
        allphiphi2/=allptsum;
    } else {
        alletaeta1 = 0.;
        alletaeta2 = 0.;
        alletaphi = 0.;
        allphiphi1 = 0.;
        allphiphi2 = 0.;
    }

    double chargedetavar = chargedetaeta2-chargedetaeta1*chargedetaeta1;
    double chargedphivar = chargedphiphi2-chargedphiphi1*chargedphiphi1;
    double chargedphidetacov = chargedetaphi - chargedetaeta1*chargedphiphi1;

    double chargeddet = (chargedetavar-chargedphivar)* (chargedetavar-chargedphivar)+4*chargedphidetacov*chargedphidetacov;
    double chargedx1 = (chargedetavar+chargedphivar+sqrt(chargeddet))/2.;
    double chargedx2 = (chargedetavar+chargedphivar-sqrt(chargeddet))/2.;

    double neutraletavar = neutraletaeta2-neutraletaeta1*neutraletaeta1;
    double neutralphivar = neutralphiphi2-neutralphiphi1*neutralphiphi1;
    double neutralphidetacov = neutraletaphi - neutraletaeta1*neutralphiphi1;

    double neutraldet = (neutraletavar-neutralphivar)* (neutraletavar-neutralphivar)+4*neutralphidetacov*neutralphidetacov;
    double neutralx1 = (neutraletavar+neutralphivar+sqrt(neutraldet))/2.;
    double neutralx2 = (neutraletavar+neutralphivar-sqrt(neutraldet))/2.;

    double alletavar = alletaeta2-alletaeta1*alletaeta1;
    double allphivar = allphiphi2-allphiphi1*allphiphi1;
    double allphidetacov = alletaphi - alletaeta1*allphiphi1;

    double alldet = (alletavar-allphivar)* (alletavar-allphivar)+4*allphidetacov*allphidetacov;
    double allx1 = (alletavar+allphivar+sqrt(alldet))/2.;
    double allx2 = (alletavar+allphivar-sqrt(alldet))/2.;

    res.chargeda = chargedx1;
    res.chargedb = chargedx2;
    res.neutrala = neutralx1;
    res.neutralb = neutralx2;
    res.alla = allx1;
    res.allb = allx2;
    res.chargedfractionmv = chargedptsummv/chargedptsum;
    return (res);
}

RootMaker::JetShape RootMaker::getJetShape(const PFJet &jet) {
    if(cdebug) { cout<<"getJetShape..."<<endl; }

    using namespace TMath;
    RootMaker::JetShape res;
    float chargedetaeta1 = 0.;
    float chargedphiphi1 = 0.;
    float chargedetaeta2 = 0.;
    float chargedphiphi2 = 0.;
    float chargedetaphi = 0.;
    float chargedptsum = 0.;
    float chargedptsummv = 0.;
    float neutraletaeta1 = 0.;
    float neutralphiphi1 = 0.;
    float neutraletaeta2 = 0.;
    float neutralphiphi2 = 0.;
    float neutraletaphi = 0.;
    float neutralptsum = 0.;
    float alletaeta1 = 0.;
    float alletaeta2 = 0.;
    float alletaphi = 0.;
    float allphiphi1 = 0.;
    float allphiphi2 = 0.;
    float allptsum = 0.;
    vector<PFCandidatePtr> constituents(jet.getPFConstituents());

    for(size_t i = 0 ; i < constituents.size() ; ++i) {
        const PFCandidate &con = * (constituents[i]);
        float deta = jet.eta() - con.eta();
        float dphi = jet.phi() - con.phi();

        if(dphi > 4.*atan(1.)) {
            dphi = dphi-8.*atan(1.);
        }

        if(dphi < -1.*4.*atan(1.)) {
            dphi = dphi+8.*atan(1.);
        }

        if(con.trackRef().isNonnull()) {
            chargedptsum += con.pt();
            chargedetaeta1 += deta*con.pt();
            chargedetaeta2 += deta*deta*con.pt();
            chargedetaphi += deta*dphi*con.pt();
            chargedphiphi1 += dphi*con.pt();
            chargedphiphi2 += dphi*dphi*con.pt();
            int vertex = getPrimVertex(* (con.trackRef()));

            if(vertex == 0 || vertex == -1) {
                chargedptsummv += con.pt();
            }
        } else {
            neutralptsum += con.pt();
            neutraletaeta1 += deta*con.pt();
            neutraletaeta2 += deta*deta*con.pt();
            neutraletaphi += deta*dphi*con.pt();
            neutralphiphi1 += dphi*con.pt();
            neutralphiphi2 += dphi*dphi*con.pt();
        }

        allptsum += con.pt();
        alletaeta1 += deta*con.pt();
        alletaeta2 += deta*deta*con.pt();
        alletaphi += deta*dphi*con.pt();
        allphiphi1 += dphi*con.pt();
        allphiphi2 += dphi*dphi*con.pt();
    }

    if(chargedptsum != 0) {
        chargedetaeta1/=chargedptsum;
        chargedetaeta2/=chargedptsum;
        chargedetaphi/=chargedptsum;
        chargedphiphi1/=chargedptsum;
        chargedphiphi2/=chargedptsum;
    } else {
        chargedetaeta1 = 0.;
        chargedetaeta2 = 0.;
        chargedetaphi = 0.;
        chargedphiphi1 = 0.;
        chargedphiphi2 = 0.;
    }

    if(neutralptsum != 0) {
        neutraletaeta1/=neutralptsum;
        neutraletaeta2/=neutralptsum;
        neutraletaphi/=neutralptsum;
        neutralphiphi1/=neutralptsum;
        neutralphiphi2/=neutralptsum;
    } else {
        neutraletaeta1 = 0.;
        neutraletaeta2 = 0.;
        neutraletaphi = 0.;
        neutralphiphi1 = 0.;
        neutralphiphi2 = 0.;
    }

    if(allptsum != 0) {
        alletaeta1/=allptsum;
        alletaeta2/=allptsum;
        alletaphi/=allptsum;
        allphiphi1/=allptsum;
        allphiphi2/=allptsum;
    } else {
        alletaeta1 = 0.;
        alletaeta2 = 0.;
        alletaphi = 0.;
        allphiphi1 = 0.;
        allphiphi2 = 0.;
    }

    double chargedetavar = chargedetaeta2-chargedetaeta1*chargedetaeta1;
    double chargedphivar = chargedphiphi2-chargedphiphi1*chargedphiphi1;
    double chargedphidetacov = chargedetaphi - chargedetaeta1*chargedphiphi1;

    double chargeddet = (chargedetavar-chargedphivar)* (chargedetavar-chargedphivar)+4*chargedphidetacov*chargedphidetacov;
    double chargedx1 = (chargedetavar+chargedphivar+sqrt(chargeddet))/2.;
    double chargedx2 = (chargedetavar+chargedphivar-sqrt(chargeddet))/2.;

    double neutraletavar = neutraletaeta2-neutraletaeta1*neutraletaeta1;
    double neutralphivar = neutralphiphi2-neutralphiphi1*neutralphiphi1;
    double neutralphidetacov = neutraletaphi - neutraletaeta1*neutralphiphi1;

    double neutraldet = (neutraletavar-neutralphivar)* (neutraletavar-neutralphivar)+4*neutralphidetacov*neutralphidetacov;
    double neutralx1 = (neutraletavar+neutralphivar+sqrt(neutraldet))/2.;
    double neutralx2 = (neutraletavar+neutralphivar-sqrt(neutraldet))/2.;

    double alletavar = alletaeta2-alletaeta1*alletaeta1;
    double allphivar = allphiphi2-allphiphi1*allphiphi1;
    double allphidetacov = alletaphi - alletaeta1*allphiphi1;

    double alldet = (alletavar-allphivar)* (alletavar-allphivar)+4*allphidetacov*allphidetacov;
    double allx1 = (alletavar+allphivar+sqrt(alldet))/2.;
    double allx2 = (alletavar+allphivar-sqrt(alldet))/2.;

    res.chargeda = chargedx1;
    res.chargedb = chargedx2;
    res.neutrala = neutralx1;
    res.neutralb = neutralx2;
    res.alla = allx1;
    res.allb = allx2;
    res.chargedfractionmv = chargedptsummv/chargedptsum;
    return (res);

}

RootMaker::JetShape RootMaker::getSlimmedJetShape(const pat::Jet &jet) {
    if(cdebug) { cout<<"getSlimmedJetShape pat::Jet"<<endl; }

    using namespace TMath;
    RootMaker::JetShape res;
    float chargedetaeta1 = 0.;
    float chargedphiphi1 = 0.;
    float chargedetaeta2 = 0.;
    float chargedphiphi2 = 0.;
    float chargedetaphi = 0.;
    float chargedptsum = 0.;
    float chargedptsummv = 0.;
    float neutraletaeta1 = 0.;
    float neutralphiphi1 = 0.;
    float neutraletaeta2 = 0.;
    float neutralphiphi2 = 0.;
    float neutraletaphi = 0.;
    float neutralptsum = 0.;
    float alletaeta1 = 0.;
    float alletaeta2 = 0.;
    float alletaphi = 0.;
    float allphiphi1 = 0.;
    float allphiphi2 = 0.;
    float allptsum = 0.;

    for(unsigned int id = 0, nd = jet.numberOfDaughters(); id < nd; ++id) {
        const pat::PackedCandidate &con = dynamic_cast<const pat::PackedCandidate &>(*jet.daughter(id));

        float deta = jet.eta() - con.eta();
        float dphi = jet.phi() - con.phi();

        if(dphi > 4.*atan(1.)) {
            dphi = dphi-8.*atan(1.);
        }

        if(dphi < -1.*4.*atan(1.)) {
            dphi = dphi+8.*atan(1.);
        }

        if(con.charge() != 0) {
            chargedptsum += con.pt();
            chargedetaeta1 += deta*con.pt();
            chargedetaeta2 += deta*deta*con.pt();
            chargedetaphi += deta*dphi*con.pt();
            chargedphiphi1 += dphi*con.pt();
            chargedphiphi2 += dphi*dphi*con.pt();
            int vertex = getPrimVertex(& (con));

            if(vertex == 0 || vertex == -1) {
                chargedptsummv += con.pt();
            }
        } else {
            neutralptsum += con.pt();
            neutraletaeta1 += deta*con.pt();
            neutraletaeta2 += deta*deta*con.pt();
            neutraletaphi += deta*dphi*con.pt();
            neutralphiphi1 += dphi*con.pt();
            neutralphiphi2 += dphi*dphi*con.pt();
        }

        allptsum += con.pt();
        alletaeta1 += deta*con.pt();
        alletaeta2 += deta*deta*con.pt();
        alletaphi += deta*dphi*con.pt();
        allphiphi1 += dphi*con.pt();
        allphiphi2 += dphi*dphi*con.pt();
    }

    if(chargedptsum != 0) {
        chargedetaeta1/=chargedptsum;
        chargedetaeta2/=chargedptsum;
        chargedetaphi/=chargedptsum;
        chargedphiphi1/=chargedptsum;
        chargedphiphi2/=chargedptsum;
    } else {
        chargedetaeta1 = 0.;
        chargedetaeta2 = 0.;
        chargedetaphi = 0.;
        chargedphiphi1 = 0.;
        chargedphiphi2 = 0.;
    }

    if(neutralptsum != 0) {
        neutraletaeta1/=neutralptsum;
        neutraletaeta2/=neutralptsum;
        neutraletaphi/=neutralptsum;
        neutralphiphi1/=neutralptsum;
        neutralphiphi2/=neutralptsum;
    } else {
        neutraletaeta1 = 0.;
        neutraletaeta2 = 0.;
        neutraletaphi = 0.;
        neutralphiphi1 = 0.;
        neutralphiphi2 = 0.;
    }

    if(allptsum != 0) {
        alletaeta1/=allptsum;
        alletaeta2/=allptsum;
        alletaphi/=allptsum;
        allphiphi1/=allptsum;
        allphiphi2/=allptsum;
    } else {
        alletaeta1 = 0.;
        alletaeta2 = 0.;
        alletaphi = 0.;
        allphiphi1 = 0.;
        allphiphi2 = 0.;
    }

    double chargedetavar = chargedetaeta2-chargedetaeta1*chargedetaeta1;
    double chargedphivar = chargedphiphi2-chargedphiphi1*chargedphiphi1;
    double chargedphidetacov = chargedetaphi - chargedetaeta1*chargedphiphi1;

    double chargeddet = (chargedetavar-chargedphivar)* (chargedetavar-chargedphivar)+4*chargedphidetacov*chargedphidetacov;
    double chargedx1 = (chargedetavar+chargedphivar+sqrt(chargeddet))/2.;
    double chargedx2 = (chargedetavar+chargedphivar-sqrt(chargeddet))/2.;

    double neutraletavar = neutraletaeta2-neutraletaeta1*neutraletaeta1;
    double neutralphivar = neutralphiphi2-neutralphiphi1*neutralphiphi1;
    double neutralphidetacov = neutraletaphi - neutraletaeta1*neutralphiphi1;

    double neutraldet = (neutraletavar-neutralphivar)* (neutraletavar-neutralphivar)+4*neutralphidetacov*neutralphidetacov;
    double neutralx1 = (neutraletavar+neutralphivar+sqrt(neutraldet))/2.;
    double neutralx2 = (neutraletavar+neutralphivar-sqrt(neutraldet))/2.;

    double alletavar = alletaeta2-alletaeta1*alletaeta1;
    double allphivar = allphiphi2-allphiphi1*allphiphi1;
    double allphidetacov = alletaphi - alletaeta1*allphiphi1;

    double alldet = (alletavar-allphivar)* (alletavar-allphivar)+4*allphidetacov*allphidetacov;
    double allx1 = (alletavar+allphivar+sqrt(alldet))/2.;
    double allx2 = (alletavar+allphivar-sqrt(alldet))/2.;

    res.chargeda = chargedx1;
    res.chargedb = chargedx2;
    res.neutrala = neutralx1;
    res.neutralb = neutralx2;
    res.alla = allx1;
    res.allb = allx2;
    res.chargedfractionmv = chargedptsummv/chargedptsum;
    return (res);
}


bool RootMaker::AddElectrons(const edm::Event &iEvent) {
    if(cdebug) { cout<<"AddElectrons..."<<endl; }

    int NumGood = 0;
    /*
    if(crecelectrontrigger) {
        iEvent.getByLabel(edm::InputTag("l1extraParticles", "NonIsolated"), L1Electrons);
        iEvent.getByLabel(edm::InputTag("l1extraParticles", "Isolated"), L1ElectronsIso);
    }
    */
    edm::Handle<reco::GsfElectronCollection> Electrons;
    iEvent.getByToken(recoElectronsToken_, Electrons);
    edm::Handle<reco::ConversionCollection> Conversions;
    iEvent.getByToken(conversionsToken_, Conversions);

    vector< edm::Handle< edm::ValueMap<double> > > electronIsoPF(3);

    iEvent.getByLabel(edm::InputTag("elPFIsoValueCharged03PFIdPFIso"), electronIsoPF[0]);
    iEvent.getByLabel(edm::InputTag("elPFIsoValueGamma03PFIdPFIso"), electronIsoPF[1]);
    iEvent.getByLabel(edm::InputTag("elPFIsoValueNeutral03PFIdPFIso"), electronIsoPF[2]);

    edm::Handle<edm::ValueMap<float> > eIDValueMapHyperTight1;
    iEvent.getByLabel("eidHyperTight1MC", eIDValueMapHyperTight1);
    const edm::ValueMap<float> &eIDHyperTight1map = * eIDValueMapHyperTight1;

    edm::Handle<edm::ValueMap<float> > eIDValueMapLoose;
    iEvent.getByLabel("eidLooseMC", eIDValueMapLoose);
    const edm::ValueMap<float> &eIDLoosemap = * eIDValueMapLoose;

    if(cdebug) {
        cout<<"Electrons.isValid() = "<<Electrons.isValid()<<endl;
        cout<<"Electrons->size() = "<<Electrons->size()<<endl;
    }

    if(Electrons.isValid()) {
        //for(GsfElectronCollection::const_iterator itel = Electrons->begin() ; itel != Electrons->end() ; ++itel)
        for(size_t n = 0 ; n < Electrons->size() ; n++) {
            //const GsfElectron& theel = *itel;
            const GsfElectron &theel = (*Electrons)[n];
            GsfElectronRef refel(Electrons, n);

            if(theel.pt() > cElFilterPtMin && TMath::Abs(theel.eta()) < cElFilterEtaMax) {
                electron_px[electron_count] = theel.px();
                electron_py[electron_count] = theel.py();
                electron_pz[electron_count] = theel.pz();
                electron_pt[electron_count] = theel.pt();
                electron_phi[electron_count] = theel.phi();
                electron_eta[electron_count] = theel.eta();
                electron_correctedecalenergy[electron_count] = theel.ecalEnergy();
                electron_charge[electron_count] = theel.charge();
                electron_esuperclusterovertrack[electron_count] = theel.eSuperClusterOverP();
                electron_eseedclusterovertrack[electron_count] = theel.eSeedClusterOverP();
                electron_deltaetasuperclustertrack[electron_count] = theel.deltaEtaSuperClusterTrackAtVtx();
                electron_deltaphisuperclustertrack[electron_count] = theel.deltaPhiSuperClusterTrackAtVtx();
                electron_e1x5[electron_count] = theel.e1x5();
                electron_e2x5[electron_count] = theel.e2x5Max();
                electron_e5x5[electron_count] = theel.e5x5();
                electron_r9[electron_count] = theel.r9();
                electron_sigmaetaeta[electron_count] = theel.sigmaEtaEta();
                electron_sigmaietaieta[electron_count] = theel.sigmaIetaIeta();
                electron_sigmaiphiiphi[electron_count] = theel.sigmaIphiIphi();
                electron_ehcaloverecaldepth1[electron_count] = theel.hcalDepth1OverEcal();
                electron_ehcaloverecaldepth2[electron_count] = theel.hcalDepth2OverEcal();
                electron_ehcaltoweroverecaldepth1[electron_count] = theel.hcalDepth1OverEcalBc();
                electron_ehcaltoweroverecaldepth2[electron_count] = theel.hcalDepth2OverEcalBc();
                electron_isolationr3track[electron_count] = theel.dr03TkSumPt();
                electron_isolationr3ecal[electron_count] = theel.dr03EcalRecHitSumEt();
                electron_isolationr3hcal[electron_count] = theel.dr03HcalTowerSumEt();
                electron_isolationr4track[electron_count] = theel.dr04TkSumPt();
                electron_isolationr4ecal[electron_count] = theel.dr04EcalRecHitSumEt();
                electron_isolationr4hcal[electron_count] = theel.dr04HcalTowerSumEt();

                electron_isolationpfr3charged[electron_count] = (* (electronIsoPF[0]))[refel];
                electron_isolationpfr3photon[electron_count] = (* (electronIsoPF[1]))[refel];
                electron_isolationpfr3neutral[electron_count] = (* (electronIsoPF[2]))[refel];
                //electron_isolationpfr3charged[electron_count] = theel.pfIsolationVariables().chargedHadronIso;
                //electron_isolationpfr3photon[electron_count] = theel.pfIsolationVariables().photonIso;
                //electron_isolationpfr3neutral[electron_count] = theel.pfIsolationVariables().neutralHadronIso;

                electron_info[electron_count] = 0;
                electron_info[electron_count] |= theel.isElectron() << 0;
                electron_info[electron_count] |= ConversionTools::hasMatchedConversion(theel, Conversions, bs_position) << 1;
                electron_info[electron_count] |= theel.ecalDrivenSeed() << 2;
                electron_info[electron_count] |= theel.trackerDrivenSeed() << 3;

                electron_gapinfo[electron_count] = 0;
                electron_gapinfo[electron_count] |= theel.isEB() << 0;
                electron_gapinfo[electron_count] |= theel.isEE() << 1;
                electron_gapinfo[electron_count] |= theel.isEBGap() << 2;
                electron_gapinfo[electron_count] |= theel.isEBEtaGap() << 3;
                electron_gapinfo[electron_count] |= theel.isEBPhiGap() << 4;
                electron_gapinfo[electron_count] |= theel.isEEGap() << 5;
                electron_gapinfo[electron_count] |= theel.isEERingGap() << 6;
                electron_gapinfo[electron_count] |= theel.isEEDeeGap() << 7;
                electron_gapinfo[electron_count] |= theel.isEBEEGap() << 8;

                edm::Handle<reco::TrackCollection> ctfTracks;
                iEvent.getByToken(recoTracksToken_, ctfTracks);
                ConversionFinder convFinder;
                ConversionInfo convInfo = convFinder.getConversionInfo(theel, ctfTracks, magneticField->inTesla(GlobalPoint(0.,0.,0.)).z());

                electron_convdist[electron_count] = convInfo.dist();
                electron_convdcot[electron_count] = convInfo.dcot();
                electron_convradius[electron_count] = convInfo.radiusOfConversion();
                electron_fbrems[electron_count] = theel.fbrem();
                electron_numbrems[electron_count] = theel.numberOfBrems();

                reco::GsfTrackRef gsfTr_e = theel.gsfTrack();
                TransientTrack TTrack = TTrackBuilder->build(gsfTr_e);
                math::XYZPoint ecalPos = PositionOnECalSurface(TTrack);
                electron_outerx[electron_count] = ecalPos.x();
                electron_outery[electron_count] = ecalPos.y();
                electron_outerz[electron_count] = ecalPos.z();
                //TrajectoryStateClosestToPoint TTrackState = TTrack.trajectoryStateClosestToPoint(GlobalPoint(pv_position.x(), pv_position.y(), pv_position.z()));


                electron_trackchi2[electron_count] = gsfTr_e->chi2();
                electron_trackndof[electron_count] = gsfTr_e->ndof();

                electron_nhits[electron_count]        = gsfTr_e->numberOfValidHits();
                electron_nmissinghits[electron_count] = gsfTr_e->numberOfLostHits();
                electron_npixelhits[electron_count]   = (gsfTr_e->hitPattern()).numberOfValidPixelHits();
                electron_npixellayers[electron_count] = (gsfTr_e->hitPattern()).pixelLayersWithMeasurement();
                electron_nstriplayers[electron_count] = (gsfTr_e->hitPattern()).stripLayersWithMeasurement();
                //         electron_nhitsexpected[electron_count] = gsfTr_e->trackerExpectedHitsInner().numberOfHits();
                electron_nhitsexpected[electron_count] = gsfTr_e->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
                electron_dxy[electron_count]    = gsfTr_e->dxy(pv_position);
                electron_dxyerr[electron_count] = gsfTr_e->dxyError();
                electron_dz[electron_count]     = gsfTr_e->dz(pv_position);
                electron_dzerr[electron_count]  = gsfTr_e->dzError();

                electron_vtx[electron_count] = -2;
                TrackRef closesttrack = theel.closestTrack();

                if(closesttrack.isNonnull()) {
                    int numvtx = getPrimVertex(*closesttrack);
                    electron_vtx[electron_count] = numvtx;
                    TransientTrack TTrackCL = TTrackBuilder->build(closesttrack);
                    TrajectoryStateClosestToPoint TTrackStateCL;

                    if(numvtx < 0) {
                        TTrackStateCL = TTrackCL.trajectoryStateClosestToPoint(GlobalPoint(pv_position.x(), pv_position.y(), pv_position.z()));
                    } else {
                        TTrackStateCL = TTrackCL.trajectoryStateClosestToPoint(GlobalPoint(primvertex_x[numvtx], primvertex_y[numvtx], primvertex_z[numvtx]));
                    }

                    electron_closestpointx[electron_count] = TTrackStateCL.position().x();
                    electron_closestpointy[electron_count] = TTrackStateCL.position().y();
                    electron_closestpointz[electron_count] = TTrackStateCL.position().z();
                }

                electron_trigger[electron_count] = GetTrigger(theel, electrontriggers);

                edm::Ref<reco::GsfElectronCollection> electronRef(Electrons,n);
                electron_eID[electron_count]=0;
                electron_eID[electron_count] |= (Byte_t)eIDHyperTight1map[electronRef];
                electron_eID[electron_count] |= ((Byte_t)eIDLoosemap[electronRef]<<4);

                electron_supercluster_e[electron_count] = theel.superCluster()->energy();
                electron_supercluster_x[electron_count] = theel.superCluster()->x();
                electron_supercluster_y[electron_count] = theel.superCluster()->y();
                electron_supercluster_z[electron_count] = theel.superCluster()->z();
                electron_supercluster_rawe[electron_count] = theel.superCluster()->rawEnergy();
                electron_supercluster_phiwidth[electron_count] = theel.superCluster()->phiWidth();
                electron_supercluster_etawidth[electron_count] = theel.superCluster()->etaWidth();
                electron_supercluster_nbasiccluster[electron_count] = theel.superCluster()->clustersSize();

                electron_count++;

                if(electron_count == M_electronmaxcount) {
                    cerr << "number of electron > M_electronmaxcount. They are missing." << endl;
                    errors |= 1<<1;
                    break;
                }

                if(theel.pt() >= cElPtMin && fabs(theel.eta()) <= cElEtaMax && theel.dr03TkSumPt()/theel.pt() <= cElTrackIso) {
                    NumGood++;
                }
            }
        }
    }

    if(NumGood >= cElNum) {
        return (true);
    }

    return (false);
}

bool RootMaker::AddPatElectrons(const edm::Event &iEvent) {
    if(cdebug) { cout<<"AddPatElectrons..."<<endl; }

    int NumGood = 0;

    edm::Handle<pat::ElectronCollection> Electrons;
    iEvent.getByToken(patElectronsToken_, Electrons);
    edm::Handle<reco::ConversionCollection> Conversions;
    iEvent.getByToken(conversionsToken_, Conversions);

    if(cdebug) {
        cout<<"pat Electrons.isValid() = "<<Electrons.isValid()<<endl;
        cout<<"pat Electrons->size() = "<<Electrons->size()<<endl;
    }

    edm::Handle<edm::ValueMap<bool> > veto_id_decisions;
    edm::Handle<edm::ValueMap<bool> > loose_id_decisions;
    edm::Handle<edm::ValueMap<bool> > medium_id_decisions;
    edm::Handle<edm::ValueMap<bool> > tight_id_decisions;
    edm::Handle<edm::ValueMap<bool> > heepV60_id_decisions;
    edm::Handle<edm::ValueMap<bool> > mvaWP80_id_decisions;
    edm::Handle<edm::ValueMap<bool> > mvaWP90_id_decisions;

    iEvent.getByToken(eleVetoIdMapToken_,     veto_id_decisions);
    iEvent.getByToken(eleLooseIdMapToken_,    loose_id_decisions);
    iEvent.getByToken(eleMediumIdMapToken_,   medium_id_decisions);
    iEvent.getByToken(eleTightIdMapToken_,    tight_id_decisions);
    iEvent.getByToken(eleHeepV60IdMapToken_,  heepV60_id_decisions);
    iEvent.getByToken(eleMVAIdMap_wp80Token_, mvaWP80_id_decisions);
    iEvent.getByToken(eleMVAIdMap_wp90Token_, mvaWP90_id_decisions);



    if(Electrons.isValid()) {
        for(size_t n = 0 ; n < Electrons->size() ; n++) {
            const pat::Electron &theel = (*Electrons)[n];
            pat::ElectronRef refel(Electrons, n);

            // Look up the ID decision for this electron in
            // the ValueMap object and store it. We need a Ptr object as the key.
            bool isPassVeto    = (*veto_id_decisions)[refel];
            bool isPassLoose   = (*loose_id_decisions)[refel];
            bool isPassMedium  = (*medium_id_decisions)[refel];
            bool isPassTight   = (*tight_id_decisions)[refel];
            bool isPassHeepV60 = (*heepV60_id_decisions)[refel];
            bool isPassMVAWP80 = (*mvaWP80_id_decisions)[refel];
            bool isPassMVAWP90 = (*mvaWP90_id_decisions)[refel];

            if(theel.pt() > cElFilterPtMin && TMath::Abs(theel.eta()) < cElFilterEtaMax) {
                if (isPassTight) electron_cbID[electron_count] = 4;
                else if (isPassMedium) electron_cbID[electron_count] = 3;
                else if (isPassLoose) electron_cbID[electron_count] = 2;
                else if (isPassVeto) electron_cbID[electron_count] = 1;
                else electron_cbID[electron_count] = 0;

                if (isPassHeepV60) electron_heepID[electron_count] = 1;
                else electron_heepID[electron_count] = 0;

                if (isPassMVAWP80) electron_mvaID[electron_count] = 2;
                else if (isPassMVAWP90) electron_mvaID[electron_count] = 1;
                else electron_mvaID[electron_count] = 0;

                electron_px[electron_count] = theel.px();
                electron_py[electron_count] = theel.py();
                electron_pz[electron_count] = theel.pz();
                electron_pt[electron_count] = theel.pt();
                electron_phi[electron_count] = theel.phi();
                electron_eta[electron_count] = theel.eta();
                electron_correctedecalenergy[electron_count] = theel.ecalEnergy();
                electron_charge[electron_count] = theel.charge();
                electron_esuperclusterovertrack[electron_count] = theel.eSuperClusterOverP();
                electron_eseedclusterovertrack[electron_count] = theel.eSeedClusterOverP();
                electron_deltaetasuperclustertrack[electron_count] = theel.deltaEtaSuperClusterTrackAtVtx();
                electron_deltaphisuperclustertrack[electron_count] = theel.deltaPhiSuperClusterTrackAtVtx();
                electron_e1x5[electron_count] = theel.e1x5();
                electron_e2x5[electron_count] = theel.e2x5Max();
                electron_e5x5[electron_count] = theel.e5x5();
                electron_r9[electron_count] = theel.r9();
                electron_sigmaetaeta[electron_count] = theel.sigmaEtaEta();
                electron_sigmaietaieta[electron_count] = theel.sigmaIetaIeta();
                electron_sigmaiphiiphi[electron_count] = theel.sigmaIphiIphi();
                electron_ehcaloverecaldepth1[electron_count] = theel.hcalDepth1OverEcal();
                electron_ehcaloverecaldepth2[electron_count] = theel.hcalDepth2OverEcal();
                electron_ehcaltoweroverecaldepth1[electron_count] = theel.hcalDepth1OverEcalBc();
                electron_ehcaltoweroverecaldepth2[electron_count] = theel.hcalDepth2OverEcalBc();
                electron_isolationr3track[electron_count] = theel.dr03TkSumPt();
                electron_isolationr3ecal[electron_count] = theel.dr03EcalRecHitSumEt();
                electron_isolationr3hcal[electron_count] = theel.dr03HcalTowerSumEt();
                electron_isolationr4track[electron_count] = theel.dr04TkSumPt();
                electron_isolationr4ecal[electron_count] = theel.dr04EcalRecHitSumEt();
                electron_isolationr4hcal[electron_count] = theel.dr04HcalTowerSumEt();

                /*
                electron_isolationpfr3charged[electron_count] = (* (electronIsoPF[0]))[refel];
                electron_isolationpfr3photon[electron_count] = (* (electronIsoPF[1]))[refel];
                electron_isolationpfr3neutral[electron_count] = (* (electronIsoPF[2]))[refel];
                //electron_isolationpfr3charged[electron_count] = theel.pfIsolationVariables().chargedHadronIso;
                //electron_isolationpfr3photon[electron_count] = theel.pfIsolationVariables().photonIso;
                //electron_isolationpfr3neutral[electron_count] = theel.pfIsolationVariables().neutralHadronIso;
                */
                electron_info[electron_count] = 0;
                electron_info[electron_count] |= theel.isElectron() << 0;
                electron_info[electron_count] |= ConversionTools::hasMatchedConversion(theel, Conversions, bs_position) << 1;
                electron_info[electron_count] |= theel.ecalDrivenSeed() << 2;
                electron_info[electron_count] |= theel.trackerDrivenSeed() << 3;

                electron_gapinfo[electron_count] = 0;
                electron_gapinfo[electron_count] |= theel.isEB() << 0;
                electron_gapinfo[electron_count] |= theel.isEE() << 1;
                electron_gapinfo[electron_count] |= theel.isEBGap() << 2;
                electron_gapinfo[electron_count] |= theel.isEBEtaGap() << 3;
                electron_gapinfo[electron_count] |= theel.isEBPhiGap() << 4;
                electron_gapinfo[electron_count] |= theel.isEEGap() << 5;
                electron_gapinfo[electron_count] |= theel.isEERingGap() << 6;
                electron_gapinfo[electron_count] |= theel.isEEDeeGap() << 7;
                electron_gapinfo[electron_count] |= theel.isEBEEGap() << 8;


// NOTE
                /*
                edm::Handle<reco::TrackCollection> ctfTracks;
                iEvent.getByToken(unpackedTracksToken_, ctfTracks);
                ConversionFinder convFinder;
                ConversionInfo convInfo = convFinder.getConversionInfo(theel, ctfTracks, magneticField->inTesla(GlobalPoint(0.,0.,0.)).z());
                electron_convdist[electron_count] = convInfo.dist();
                electron_convdcot[electron_count] = convInfo.dcot();
                electron_convradius[electron_count] = convInfo.radiusOfConversion();
                electron_fbrems[electron_count] = theel.fbrem();
                electron_numbrems[electron_count] = theel.numberOfBrems();
                */

                reco::GsfTrackRef gsfTr_e = theel.gsfTrack();
                TransientTrack TTrack = TTrackBuilder->build(gsfTr_e);
                math::XYZPoint ecalPos = PositionOnECalSurface(TTrack);
                electron_outerx[electron_count] = ecalPos.x();
                electron_outery[electron_count] = ecalPos.y();
                electron_outerz[electron_count] = ecalPos.z();
                //TrajectoryStateClosestToPoint TTrackState = TTrack.trajectoryStateClosestToPoint(GlobalPoint(pv_position.x(), pv_position.y(), pv_position.z()));


                electron_trackchi2[electron_count] = gsfTr_e->chi2();
                electron_trackndof[electron_count] = gsfTr_e->ndof();
                electron_nhits[electron_count] = gsfTr_e->numberOfValidHits();
                electron_nmissinghits[electron_count] = gsfTr_e->numberOfLostHits();
                electron_npixelhits[electron_count] = (gsfTr_e->hitPattern()).numberOfValidPixelHits();
                electron_npixellayers[electron_count] = (gsfTr_e->hitPattern()).pixelLayersWithMeasurement();
                electron_nstriplayers[electron_count] = (gsfTr_e->hitPattern()).stripLayersWithMeasurement();

                //         electron_nhitsexpected[electron_count] = gsfTr_e->trackerExpectedHitsInner().numberOfHits();
                electron_nhitsexpected[electron_count] = gsfTr_e->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);

                electron_dxy[electron_count] = gsfTr_e->dxy(pv_position);
                electron_dxyerr[electron_count] = gsfTr_e->dxyError();
                electron_dz[electron_count] = gsfTr_e->dz(pv_position);
                electron_dzerr[electron_count] = gsfTr_e->dzError();
                electron_vtx[electron_count] = -2;

// NOTE
                //TrackRef closesttrack = theel.closestTrack();
                TrackRef closesttrack = theel.closestCtfTrackRef();

                if(closesttrack.isNonnull()) {
                    int numvtx = getPrimVertex(*closesttrack);
                    electron_vtx[electron_count] = numvtx;
                    TransientTrack TTrackCL = TTrackBuilder->build(closesttrack);
                    TrajectoryStateClosestToPoint TTrackStateCL;

                    if(numvtx < 0) {
                        TTrackStateCL = TTrackCL.trajectoryStateClosestToPoint(GlobalPoint(pv_position.x(), pv_position.y(), pv_position.z()));
                    } else {
                        TTrackStateCL = TTrackCL.trajectoryStateClosestToPoint(GlobalPoint(primvertex_x[numvtx], primvertex_y[numvtx], primvertex_z[numvtx]));
                    }

                    electron_closestpointx[electron_count] = TTrackStateCL.position().x();
                    electron_closestpointy[electron_count] = TTrackStateCL.position().y();
                    electron_closestpointz[electron_count] = TTrackStateCL.position().z();
                }


                electron_trigger[electron_count] = GetTrigger(theel, electrontriggers);
                /*
                edm::Ref<reco::GsfElectronCollection> electronRef(Electrons,n);
                electron_eID[electron_count]=0;
                electron_eID[electron_count] |= (Byte_t)eIDHyperTight1map[electronRef];
                electron_eID[electron_count] |= ((Byte_t)eIDLoosemap[electronRef]<<4);
                */
                electron_supercluster_e[electron_count] = theel.superCluster()->energy();
                electron_supercluster_x[electron_count] = theel.superCluster()->x();
                electron_supercluster_y[electron_count] = theel.superCluster()->y();
                electron_supercluster_z[electron_count] = theel.superCluster()->z();
                electron_supercluster_rawe[electron_count] = theel.superCluster()->rawEnergy();
                electron_supercluster_phiwidth[electron_count] = theel.superCluster()->phiWidth();
                electron_supercluster_etawidth[electron_count] = theel.superCluster()->etaWidth();
                electron_supercluster_nbasiccluster[electron_count] = theel.superCluster()->clustersSize();

                electron_count++;

                if(electron_count == M_electronmaxcount) {
                    cerr << "number of electron > M_electronmaxcount. They are missing." << endl;
                    errors |= 1<<1;
                    break;
                }

                if(theel.pt() >= cElPtMin && fabs(theel.eta()) <= cElEtaMax && theel.dr03TkSumPt()/theel.pt() <= cElTrackIso) {
                    NumGood++;
                }

                // fill baby trees
                electron_has_gen_particle[electron_count] = 0;
                electron_gen_particle_pdgid[electron_count] = 0;
                electron_has_gen_mother[electron_count] = 0;
                electron_gen_mother_pdgid[electron_count] = 0;

                if(theel.genParticle()) {
                    electron_has_gen_particle[electron_count] = 1;
                    electron_gen_particle_pdgid[electron_count] = theel.genParticle()->pdgId();

                    if(theel.genParticle()->mother()) {
                        electron_has_gen_mother[electron_count] = 1;
                        electron_gen_mother_pdgid[electron_count] = theel.genParticle()->mother()->pdgId();
                    }
                }

                // end baby trees
            } // end good electron loop
        } // end all electron loop
    }

    if(NumGood >= cElNum) {
        return (true);
    }

    return (false);
}

Int_t RootMaker::getSuperClusterEl(const SuperClusterRef &A) {
    if(cdebug) { cout<<"getSuperClusterEl..."<<endl; }

    TVector3 testa(A->x(), A->y(), A->z());

    for(UInt_t i = 0 ; i < supercluster_count ; i++) {
        TVector3 testb(supercluster_x[i], supercluster_y[i], supercluster_z[i]);

        //cout << "CLUSTER " << i << " " <<  testb.DeltaR(testa) << " " << supercluster_rawe[i] << " " << A->rawEnergy() << " " << A->eta() <<endl;
        if(testb.DeltaR(testa) < 0.1) {
            return (i);
        }
    }

    return (-1);
}

Int_t RootMaker::getSuperClusterPh(const SuperClusterRef &A) {
    if(cdebug) { cout<<"getSuperClusterPh..."<<endl; }

    for(UInt_t i = 0 ; i < supercluster_count ; i++) {
        if(supercluster_rawe[i] == Float_t (A->rawEnergy())) {
            return (i);
        }
    }

    return (-1);
}


//// manual cut based electron ID
//Int_t RootMaker::getCBElectronID(const pat::Electron &theel) {
//    if(cdebug) {
//        cout<<"getCBElectronID..."<<endl;
//    }
//
//    float superClusterEta = theel.superCluster()->eta();
//    float full5x5 = theel.full5x5_sigmaIetaIeta();
//    float dEtaIn = abs(theel.deltaEtaSuperClusterTrackAtVtx());
//    float dPhiIn = abs(theel.deltaPhiSuperClusterTrackAtVtx());
//    float hOverE = theel.hadronicOverEm();
//    float relIso = (theel.pfIsolationVariables().sumChargedHadronPt + std::max(0.0, theel.pfIsolationVariables().sumNeutralHadronEt + theel.pfIsolationVariables().sumPhotonEt - 0.5*theel.pfIsolationVariables().sumPUPt)) / theel.pt();
//    float ooEmooP = fabs((1/theel.ecalEnergy()) - (1/(theel.ecalEnergy() / theel.eSuperClusterOverP())));
//    float absd0 = fabs(theel.gsfTrack()->dxy(pv_position));
//    float absdz = fabs(theel.gsfTrack()->dz(pv_position));
//    int   expHits = theel.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
//    int   passConv = theel.passConversionVeto();
//
//    // PHYS14 selection, conditions: PU20 bx25
//    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2
//    if(fabs(superClusterEta) <= 1.479) {
//        // BARREL TIGHT
//        if(
//            full5x5  < 0.010181 &&
//            dEtaIn   < 0.006574 &&
//            dPhiIn   < 0.022868 &&
//            hOverE   < 0.037553 &&
//            relIso   < 0.074355 &&
//            ooEmooP  < 0.131191 &&
//            absd0    < 0.009924 &&
//            absdz    < 0.015310 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (4);
//        }
//        // BARREL MEDIUM
//        if(
//            full5x5  < 0.010399 &&
//            dEtaIn   < 0.007641 &&
//            dPhiIn   < 0.032643 &&
//            hOverE   < 0.060662 &&
//            relIso   < 0.097213 &&
//            ooEmooP  < 0.153897 &&
//            absd0    < 0.011811 &&
//            absdz    < 0.070775 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (3);
//        }
//        // BARREL LOOSE
//        if(
//            full5x5  < 0.010557 &&
//            dEtaIn   < 0.012442 &&
//            dPhiIn   < 0.072624 &&
//            hOverE   < 0.121476 &&
//            relIso   < 0.120026 &&
//            ooEmooP  < 0.221803 &&
//            absd0    < 0.022664 &&
//            absdz    < 0.173670 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (2);
//        }
//        // BARREL VETO
//        if(
//            full5x5  < 0.011100 &&
//            dEtaIn   < 0.016315 &&
//            dPhiIn   < 0.252044 &&
//            hOverE   < 0.345843 &&
//            relIso   < 0.164369 &&
//            ooEmooP  < 0.248070 &&
//            absd0    < 0.060279 &&
//            absdz    < 0.800538 &&
//            expHits  <= 2       &&
//            passConv == 1
//        ) {
//            return (1);
//        }
//    }
//    // ENDCAP
//    else if(fabs(superClusterEta) > 1.479 && fabs(superClusterEta) <=2.5) {
//        // ENDCAP TIGHT
//        if(
//            full5x5  < 0.028766 &&
//            dEtaIn   < 0.005681 &&
//            dPhiIn   < 0.032046 &&
//            hOverE   < 0.081902 &&
//            relIso   < 0.090185 &&
//            ooEmooP  < 0.106055 &&
//            absd0    < 0.027261 &&
//            absdz    < 0.147154 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (4);
//        }
//        // ENDCAP MEDIUM
//        if(
//            full5x5  < 0.029524 &&
//            dEtaIn   < 0.009285 &&
//            dPhiIn   < 0.042447 &&
//            hOverE   < 0.104263 &&
//            relIso   < 0.116708 &&
//            ooEmooP  < 0.137468 &&
//            absd0    < 0.051682 &&
//            absdz    < 0.180720 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (3);
//        }
//        // ENDCAP LOOSE
//        if(
//            full5x5  < 0.032602 &&
//            dEtaIn   < 0.010654 &&
//            dPhiIn   < 0.145129 &&
//            hOverE   < 0.131862 &&
//            relIso   < 0.162914 &&
//            ooEmooP  < 0.142283 &&
//            absd0    < 0.097358 &&
//            absdz    < 0.198444 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (2);
//        }
//        // ENDCAP VETO
//        if(
//            full5x5  < 0.033987 &&
//            dEtaIn   < 0.010671 &&
//            dPhiIn   < 0.245263 &&
//            hOverE   < 0.134691 &&
//            relIso   < 0.212604 &&
//            ooEmooP  < 0.157160 &&
//            absd0    < 0.273097 &&
//            absdz    < 0.885860 &&
//            expHits  <= 3       &&
//            passConv == 1
//        ) {
//            return (1);
//        }
//    } // end endcap
//    return (-1);
//}
//
//// manual cut based electron ID
//Int_t RootMaker::getCBElectronID(const reco::GsfElectron &theel) {
//    if(cdebug) {
//        cout<<"getCBElectronID..."<<endl;
//    }
//
//    float superClusterEta = theel.superCluster()->eta();
//    float full5x5 = theel.full5x5_sigmaIetaIeta();
//    float dEtaIn = abs(theel.deltaEtaSuperClusterTrackAtVtx());
//    float dPhiIn = abs(theel.deltaPhiSuperClusterTrackAtVtx());
//    float hOverE = theel.hadronicOverEm();
//    float relIso = (theel.pfIsolationVariables().sumChargedHadronPt + std::max(0.0, theel.pfIsolationVariables().sumNeutralHadronEt + theel.pfIsolationVariables().sumPhotonEt - 0.5*theel.pfIsolationVariables().sumPUPt)) / theel.pt();
//    float ooEmooP = fabs((1/theel.ecalEnergy()) - (1/(theel.ecalEnergy() / theel.eSuperClusterOverP())));
//    float absd0 = fabs(theel.gsfTrack()->dxy(pv_position));
//    float absdz = fabs(theel.gsfTrack()->dz(pv_position));
//    int   expHits = theel.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
//    //int   passConv = theel.passConversionVeto();
//    int   passConv = 1;
//
//    // PHYS14 selection, conditions: PU20 bx25
//    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2
//    if(fabs(superClusterEta) <= 1.479) {
//        // BARREL TIGHT
//        if(
//            full5x5  < 0.010181 &&
//            dEtaIn   < 0.006574 &&
//            dPhiIn   < 0.022868 &&
//            hOverE   < 0.037553 &&
//            relIso   < 0.074355 &&
//            ooEmooP  < 0.131191 &&
//            absd0    < 0.009924 &&
//            absdz    < 0.015310 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (4);
//        }
//        // BARREL MEDIUM
//        if(
//            full5x5  < 0.010399 &&
//            dEtaIn   < 0.007641 &&
//            dPhiIn   < 0.032643 &&
//            hOverE   < 0.060662 &&
//            relIso   < 0.097213 &&
//            ooEmooP  < 0.153897 &&
//            absd0    < 0.011811 &&
//            absdz    < 0.070775 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (3);
//        }
//        // BARREL LOOSE
//        if(
//            full5x5  < 0.010557 &&
//            dEtaIn   < 0.012442 &&
//            dPhiIn   < 0.072624 &&
//            hOverE   < 0.121476 &&
//            relIso   < 0.120026 &&
//            ooEmooP  < 0.221803 &&
//            absd0    < 0.022664 &&
//            absdz    < 0.173670 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (2);
//        }
//        // BARREL VETO
//        if(
//            full5x5  < 0.011100 &&
//            dEtaIn   < 0.016315 &&
//            dPhiIn   < 0.252044 &&
//            hOverE   < 0.345843 &&
//            relIso   < 0.164369 &&
//            ooEmooP  < 0.248070 &&
//            absd0    < 0.060279 &&
//            absdz    < 0.800538 &&
//            expHits  <= 2       &&
//            passConv == 1
//        ) {
//            return (1);
//        }
//    }
//    // ENDCAP
//    else if(fabs(superClusterEta) > 1.479 && fabs(superClusterEta) <=2.5) {
//        // ENDCAP TIGHT
//        if(
//            full5x5  < 0.028766 &&
//            dEtaIn   < 0.005681 &&
//            dPhiIn   < 0.032046 &&
//            hOverE   < 0.081902 &&
//            relIso   < 0.090185 &&
//            ooEmooP  < 0.106055 &&
//            absd0    < 0.027261 &&
//            absdz    < 0.147154 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (4);
//        }
//        // ENDCAP MEDIUM
//        if(
//            full5x5  < 0.029524 &&
//            dEtaIn   < 0.009285 &&
//            dPhiIn   < 0.042447 &&
//            hOverE   < 0.104263 &&
//            relIso   < 0.116708 &&
//            ooEmooP  < 0.137468 &&
//            absd0    < 0.051682 &&
//            absdz    < 0.180720 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (3);
//        }
//        // ENDCAP LOOSE
//        if(
//            full5x5  < 0.032602 &&
//            dEtaIn   < 0.010654 &&
//            dPhiIn   < 0.145129 &&
//            hOverE   < 0.131862 &&
//            relIso   < 0.162914 &&
//            ooEmooP  < 0.142283 &&
//            absd0    < 0.097358 &&
//            absdz    < 0.198444 &&
//            expHits  <= 1       &&
//            passConv == 1
//        ) {
//            return (2);
//        }
//        // ENDCAP VETO
//        if(
//            full5x5  < 0.033987 &&
//            dEtaIn   < 0.010671 &&
//            dPhiIn   < 0.245263 &&
//            hOverE   < 0.134691 &&
//            relIso   < 0.212604 &&
//            ooEmooP  < 0.157160 &&
//            absd0    < 0.273097 &&
//            absdz    < 0.885860 &&
//            expHits  <= 3       &&
//            passConv == 1
//        ) {
//            return (1);
//        }
//    } // end endcap
//    return (-1);
//}

// for miniAOD, where trackRef is not available
Int_t RootMaker::getPrimVertex(const pat::PackedCandidate *con) {
    if(Vertices.isValid()) {
        if(con->PVUsedInFit) {
            return  0;
        }

        if(con->PVTight) {
            return -1;
        }

        if(con->PVLoose) {
            return -2;
        }

        if(con->NoPV) {
            return -2;
        }
    }

    return (-2);
}

Int_t RootMaker::getPrimVertex(const Track &trk) {
    if(Vertices.isValid()) {
        for(unsigned i = 0 ; i < Vertices->size(); i++) {
            if((*Vertices)[i].isValid() && !(*Vertices)[i].isFake()) {
                for(Vertex::trackRef_iterator it = (*Vertices)[i].tracks_begin() ; it != (*Vertices)[i].tracks_end() ; ++it) {
                    if(trk.px() - (*it)->px() < 0.01 && trk.py() - (*it)->py() < 0.01 && trk.pz() - (*it)->pz() < 0.01) {
                        return (i);
                    }
                }
            }
        }
    }

    return (-1);
}

bool RootMaker::AddVertices(const edm::Event &iEvent) {
    if(cdebug) { cout<<"AddVertices..."<<endl; }

    double mpi = 0.13957018;
    double mp = 0.93827203;
    edm::Handle<TrackCollection> Tracks;
    iEvent.getByToken(recoTracksToken_, Tracks);
    iEvent.getByToken(dharmonicToken_, dEdxharmonic2);

    if(cdebug) { cout<<"Tracks.isValid() = "<<Tracks.isValid()<<endl; }

    if(Tracks.isValid() && primvertex_count > 0) {
        KalmanVertexFitter fitter(true);

        for(unsigned a = 0 ; a < Tracks->size() ; a++) {
            if(!((*Tracks)[a].normalizedChi2() < cVertexTRKChi2 && (*Tracks)[a].numberOfValidHits() >= cVertexTRKHitsMin)) {
                continue;
            }

            for(unsigned b = 0 ; b < a ; b++) {
                if(!((*Tracks)[a].charge() != (*Tracks)[b].charge() && (*Tracks)[b].normalizedChi2() < cVertexTRKChi2 && (*Tracks)[b].numberOfValidHits() >= cVertexTRKHitsMin)) {
                    continue;
                }

                vector<TransientTrack> testvec;
                testvec.resize(2);
                testvec[0] = TTrackBuilder->build((*Tracks)[a]);
                testvec[1] = TTrackBuilder->build((*Tracks)[b]);

                TransientVertex testvertex = fitter.vertex(testvec);

                if(testvertex.isValid() && testvertex.normalisedChiSquared() < cVertexChi2) {
                    const Vertex &secvertex = (Vertex)testvertex;
                    SMatrixSym3D covmatrix = primvertex.covariance() + secvertex.covariance();
                    SVector3 dist(primvertex.x() - secvertex.x(), primvertex.y() - secvertex.y(), 0.);
                    double vertexsig2d =  ROOT::Math::Mag2(dist) / sqrt(ROOT::Math::Similarity(covmatrix, dist));

                    if(vertexsig2d > cVertexSig2D) {
                        using namespace TMath;
                        TLorentzVector A((*Tracks)[a].px(), (*Tracks)[a].py(), (*Tracks)[a].pz(), Sqrt(Power((*Tracks)[a].px(), 2) + Power((*Tracks)[a].py(),2) + Power((*Tracks)[a].pz(), 2) + mpi*mpi));
                        TLorentzVector B((*Tracks)[b].px(), (*Tracks)[b].py(), (*Tracks)[b].pz(), Sqrt(Power((*Tracks)[b].px(), 2) + Power((*Tracks)[b].py(),2) + Power((*Tracks)[b].pz(), 2) + mpi*mpi));
                        double mkaon = (A+B).M();
                        A.SetE(Sqrt(A.E()*A.E() - mpi*mpi + mp*mp));
                        double mlambdaa = (A+B).M();
                        A.SetE(Sqrt(A.E()*A.E() + mpi*mpi - mp*mp));
                        B.SetE(Sqrt(B.E()*B.E() - mpi*mpi + mp*mp));
                        double mlambdab = (A+B).M();

                        if(Abs(mkaon - 0.49765) < cKaonMassWindow || Abs(mlambdaa - 1.115683) < cLambdaMassWindow || Abs(mlambdab - 1.115683) < cLambdaMassWindow) {

                            secvertices_vx[secvertices_count] = secvertex.x();
                            secvertices_vy[secvertices_count] = secvertex.y();
                            secvertices_vz[secvertices_count] = secvertex.z();
                            secvertices_chi2[secvertices_count] = secvertex.chi2();
                            secvertices_ndof[secvertices_count] = secvertex.ndof();
                            secvertices_cov[secvertices_count][0] = secvertex.covariance(0,0);
                            secvertices_cov[secvertices_count][1] = secvertex.covariance(0,1);
                            secvertices_cov[secvertices_count][2] = secvertex.covariance(0,2);
                            secvertices_cov[secvertices_count][3] = secvertex.covariance(1,1);
                            secvertices_cov[secvertices_count][4] = secvertex.covariance(1,2);
                            secvertices_cov[secvertices_count][5] = secvertex.covariance(2,2);

                            for(Int_t j = 0 ; j < 2 ; j++) {
                                TrajectoryStateClosestToPoint TTrackState = testvec[j].trajectoryStateClosestToPoint(GlobalPoint(secvertex.x(), secvertex.y(), secvertex.z()));
                                TrackRef tr;

                                if(j==0) { tr = TrackRef(Tracks, a); }

                                if(j==1) { tr = TrackRef(Tracks, b); }

                                secvertices_track_px[secvertices_count][j] = TTrackState.momentum().x();
                                secvertices_track_py[secvertices_count][j] = TTrackState.momentum().y();
                                secvertices_track_pz[secvertices_count][j] = TTrackState.momentum().z();
                                secvertices_track_closestpointx[secvertices_count][j] = TTrackState.position().x();
                                secvertices_track_closestpointy[secvertices_count][j] = TTrackState.position().y();
                                secvertices_track_closestpointz[secvertices_count][j] = TTrackState.position().z();
                                secvertices_track_chi2[secvertices_count][j]   = tr->chi2();
                                secvertices_track_ndof[secvertices_count][j]   = tr->ndof();
                                secvertices_track_dxy[secvertices_count][j]    = TTrackState.perigeeParameters().transverseImpactParameter();
                                secvertices_track_dxyerr[secvertices_count][j] = TTrackState.perigeeError().transverseImpactParameterError();
                                secvertices_track_dz[secvertices_count][j]     = TTrackState.perigeeParameters().longitudinalImpactParameter();
                                secvertices_track_dzerr[secvertices_count][j]  = TTrackState.perigeeError().longitudinalImpactParameterError();

                                if(dEdxharmonic2.isValid()) {
                                    secvertices_track_dedxharmonic2[secvertices_count][j] = (*dEdxharmonic2)[tr].dEdx();
                                } else {
                                    secvertices_track_dedxharmonic2[secvertices_count][j] = -1.;
                                }

                                secvertices_track_charge[secvertices_count][j] = tr->charge();
                                secvertices_track_nhits[secvertices_count][j] = tr->numberOfValidHits();
                                secvertices_track_nmissinghits[secvertices_count][j] = tr->numberOfLostHits();
                                secvertices_track_npixelhits[secvertices_count][j] = tr->hitPattern().numberOfValidPixelHits();
                                secvertices_track_npixellayers[secvertices_count][j] = tr->hitPattern().pixelLayersWithMeasurement();
                                secvertices_track_nstriplayers[secvertices_count][j] = tr->hitPattern().stripLayersWithMeasurement();
                            }

                            secvertices_count++;

                            if(secvertices_count == M_secverticesmaxcount) {
                                cerr << "number of secvertices > M_secverticesmaxcount. They are missing." << endl;
                                break;
                            }
                        }

                    }
                }
            }
        }
    }

    return (true);
}

bool RootMaker::AddMuVertices(const edm::Event &iEvent) {
    if(cdebug) { cout<<"AddMuVertices..."<<endl; }

    iEvent.getByToken(dharmonicToken_, dEdxharmonic2);
    edm::Handle<reco::MuonCollection> Muons;
    iEvent.getByToken(recoMuonsToken_, Muons);
    int count = 0;

    if(Muons.isValid() && primvertex_count > 0) {
        vector<TransientTrack> testvec(2);
        KalmanVertexFitter fitter(true);

        // 1st loop over muons
        for(unsigned int i = 0; i < Muons->size(); ++i) {
            if(!((*Muons)[i].isGlobalMuon() && (*Muons)[i].isTrackerMuon() && (*Muons)[i].innerTrack().isNonnull())) {
                continue;
            }

            TrackRef trA = (*Muons)[i].innerTrack();

            // 2nd loop over muons
            for(unsigned int j = i+1; j < Muons->size(); ++j) {
                if(!((*Muons)[j].isGlobalMuon() && (*Muons)[j].isTrackerMuon() && (*Muons)[j].innerTrack().isNonnull())) {
                    continue;
                }

                if(!((*Muons)[i].charge()* (*Muons)[j].charge() < 0)) {
                    continue;
                }

                TrackRef trB = (*Muons)[j].innerTrack();
                testvec[0] = TTrackBuilder->build(trA);
                testvec[1] = TTrackBuilder->build(trB);

                TransientVertex testvertex = fitter.vertex(testvec);

                if(testvertex.isValid()) {
                    const Vertex &secvertex = (Vertex)testvertex;
                    using namespace TMath;
                    TLorentzVector A(trA->px(), trA->py(), trA->pz(), Sqrt(Power(trA->px(), 2) + Power(trA->py(), 2) + Power(trA->pz(), 2)));
                    TLorentzVector B(trB->px(), trB->py(), trB->pz(), Sqrt(Power(trB->px(), 2) + Power(trB->py(), 2) + Power(trB->pz(), 2)));
                    double mass = (A+B).M();

                    if(Abs(mass - 3.097) < 1.5 || Abs(mass - 9.46) < 1.5) {    //J/Psi and Y
                        count++;
                        musecvertices_vx[musecvertices_count] = secvertex.x();
                        musecvertices_vy[musecvertices_count] = secvertex.y();
                        musecvertices_vz[musecvertices_count] = secvertex.z();
                        musecvertices_chi2[musecvertices_count] = secvertex.chi2();
                        musecvertices_ndof[musecvertices_count] = secvertex.ndof();
                        musecvertices_cov[musecvertices_count][0] = secvertex.covariance(0,0);
                        musecvertices_cov[musecvertices_count][1] = secvertex.covariance(0,1);
                        musecvertices_cov[musecvertices_count][2] = secvertex.covariance(0,2);
                        musecvertices_cov[musecvertices_count][3] = secvertex.covariance(1,1);
                        musecvertices_cov[musecvertices_count][4] = secvertex.covariance(1,2);
                        musecvertices_cov[musecvertices_count][5] = secvertex.covariance(2,2);

                        for(int j = 0; j < 2; ++j) {
                            TrajectoryStateClosestToPoint TTrackState = testvec[j].trajectoryStateClosestToPoint(GlobalPoint(secvertex.x(), secvertex.y(), secvertex.z()));
                            TrackRef tr;

                            if(j==0) {
                                tr = (*Muons)[i].innerTrack();
                            }

                            if(j==1) {
                                tr = (*Muons)[j].innerTrack();
                            }

                            musecvertices_track_px[musecvertices_count][j]            = TTrackState.momentum().x();
                            musecvertices_track_py[musecvertices_count][j]            = TTrackState.momentum().y();
                            musecvertices_track_pz[musecvertices_count][j]            = TTrackState.momentum().z();
                            musecvertices_track_closestpointx[musecvertices_count][j] = TTrackState.position().x();
                            musecvertices_track_closestpointy[musecvertices_count][j] = TTrackState.position().y();
                            musecvertices_track_closestpointz[musecvertices_count][j] = TTrackState.position().z();
                            musecvertices_track_chi2[musecvertices_count][j]          = tr->chi2();
                            musecvertices_track_ndof[musecvertices_count][j]          = tr->ndof();
                            musecvertices_track_dxy[musecvertices_count][j]           = TTrackState.perigeeParameters().transverseImpactParameter();
                            musecvertices_track_dxyerr[musecvertices_count][j]        = TTrackState.perigeeError().transverseImpactParameterError();
                            musecvertices_track_dz[musecvertices_count][j]            = TTrackState.perigeeParameters().longitudinalImpactParameter();
                            musecvertices_track_dzerr[musecvertices_count][j]         = TTrackState.perigeeError().longitudinalImpactParameterError();

                            if(dEdxharmonic2.isValid()) {
                                musecvertices_track_dedxharmonic2[musecvertices_count][j] = (*dEdxharmonic2)[tr].dEdx();
                            } else {
                                musecvertices_track_dedxharmonic2[musecvertices_count][j] = -1;
                            }

                            musecvertices_track_charge[musecvertices_count][j]        = tr->charge();
                            musecvertices_track_nhits[musecvertices_count][j]         = tr->numberOfValidHits();
                            musecvertices_track_nmissinghits[musecvertices_count][j]  = tr->numberOfLostHits();
                            musecvertices_track_npixelhits[musecvertices_count][j]    = tr->hitPattern().numberOfValidPixelHits();
                            musecvertices_track_npixellayers[musecvertices_count][j]  = tr->hitPattern().pixelLayersWithMeasurement();
                            musecvertices_track_nstriplayers[musecvertices_count][j]  = tr->hitPattern().stripLayersWithMeasurement();
                        }

                        musecvertices_count++;

                        if(musecvertices_count == M_musecverticesmaxcount) {
                            cerr << "number of secvertices > M_musecverticesmaxcount. They are missing." << endl;
                            break;
                        }
                    }
                }
            }// end of 2nd loop over muons
        }// end of 1st loop over muons
    }

    if(crecsecvertices && count > 0) {
        AddVertices(iEvent);
    }

    return (true);
}

double RootMaker::DR(const Candidate &A, const Candidate &B) {
    //using namespace TMath;
    TLorentzVector TA(A.px(), A.py(), A.pz(), A.energy());
    TLorentzVector TB(B.px(), B.py(), B.pz(), B.energy());
    return (TA.DeltaR(TB));
}

void RootMaker::TriggerIndexSelection(vector<string> configstring, vector<pair<unsigned, int> > &triggers, string &allnames) {
    if(cdebug) { cout<<"TriggerIndexSelection..."<<endl; }

    triggers.clear();
    allnames.clear();
    boost::cmatch what;
    vector<pair<boost::regex, bool> > regexes;

//for (unsigned int i=0;i<triggers.size();i++) { cout<<"triggers["<<i<<"].first = "<<triggers[i].first<<endl;cout<<"triggers["<<i<<"].second = "<<triggers[i].second<<endl;}
    for(unsigned i = 0 ; i < configstring.size() ; i++) {
        vector<string> strs;
        boost::split(strs, configstring[i], boost::is_any_of(":"));
        bool dofilter = false;

        if(strs.size() == 2 && strs[1] == "FilterTrue") {
            dofilter = true;
        }

        regexes.push_back(pair<boost::regex, bool> (boost::regex(strs[0].c_str()), dofilter));
    }

    for(unsigned i = 0 ; i < HLTConfiguration.size() ; i++) {
        unsigned TriggerIndex = HLTConfiguration.triggerIndex(HLTConfiguration.triggerName(i));
        const vector<string> &ModuleLabels(HLTConfiguration.moduleLabels(TriggerIndex));

        for(unsigned j = 0 ; j < regexes.size() ; j++) {
            if(boost::regex_match(HLTConfiguration.triggerName(i).c_str(), what, regexes[j].first) && triggers.size() < 32) {
                for(int u = ModuleLabels.size()-1 ; u >= 0 ; u--) {
                    if(HLTConfiguration.saveTags(ModuleLabels[u])) {
                        allnames += HLTConfiguration.triggerName(i) + string(":") + ModuleLabels[u] + string(" ");
                        triggers.push_back(pair<unsigned, int> (TriggerIndex, u));

                        //if (cdebug) {
                        //    cout<<"triggers.size() = "<<triggers.size()<< ": "<<endl;
                        //    cout<<"HLTConfiguration.triggerName("<<i<<") = "<<HLTConfiguration.triggerName(i)<<endl;
                        //    cout<<"TriggerIndex = "<<TriggerIndex<<endl;
                        //    cout<<"ModuleLabels["<<u<<"] = "<<ModuleLabels[u]<<"\n"<<endl;
                        //}
                        if("hltL1sL1DoubleMu10MuOpen" == ModuleLabels[u]) {
                            allnames += HLTConfiguration.triggerName(i) + string(":") + ModuleLabels[u] + string("gt10 ");
                            triggers.push_back(pair<unsigned, int> (TriggerIndex, -1*u));
                        }

                        if(regexes[j].second == false) {
                            break;
                        }
                    }
                }
            }
        }
    }

    if(triggers.size() == 32) {
        cout << "ERROR: more than 32 triggers to match" << endl;
    }
}

