void ana_lowgain()
{
    auto hist1 = new TH2D("hist1","all pid",     400,0,4000,400,0,20000);
    auto hist2 = new TH2D("hist2","Low gain pid",400,0,4000,400,0,20000);

    int runNo = 1048;

    TString fileNames[] = {
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.00.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.01.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.02.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.03.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.04.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.05.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.06.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.07.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.08.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.09.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.10.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.11.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.12.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.13.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.14.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.15.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.16.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.17.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.18.reco_nevent_test.root",runNo,runNo),
        Form("/data/RB230064/ejungwoo/production/reco_nevent_test_run%d/spirit24_%d.19.reco_nevent_test.root",runNo,runNo),

        //Form("/data/RB230064/ejungwoo/production/recoLG_run%d/spirit24_%d.00.recoLG.root",runNo,runNo),
        //Form("/data/RB230064/ejungwoo/production/recoLG_run%d/spirit24_%d.01.recoLG.root",runNo,runNo),
        //Form("/data/RB230064/ejungwoo/production/recoLG_run%d/spirit24_%d.02.recoLG.root",runNo,runNo),
        //Form("/data/RB230064/ejungwoo/production/recoLG_run%d/spirit24_%d.03.recoLG.root",runNo,runNo),
        //Form("/data/RB230064/ejungwoo/production/recoLG_run%d/spirit24_%d.04.recoLG.root",runNo,runNo),
        //Form("/data/RB230064/ejungwoo/production/recoLG_run%d/spirit24_%d.05.recoLG.root",runNo,runNo),
        //Form("/data/RB230064/ejungwoo/production/recoLG_run%d/spirit24_%d.06.recoLG.root",runNo,runNo),
        //Form("/data/RB230064/ejungwoo/production/recoLG_run%d/spirit24_%d.07.recoLG.root",runNo,runNo),
        //Form("/data/RB230064/ejungwoo/production/recoLG_run%d/spirit24_%d.08.recoLG.root",runNo,runNo),
        //Form("/data/RB230064/ejungwoo/production/recoLG_run%d/spirit24_%d.09.recoLG.root",runNo,runNo),

        //Form("/data/RB230064/ejungwoo/analysis202406/data/run%04d_s01.Submit_LowGain_Mult20.root",runNo),
        //Form("/data/RB230064/ejungwoo/analysis202406/data/run%04d_s02.Submit_LowGain_Mult20.root",runNo),
        //Form("/data/RB230064/ejungwoo/analysis202406/data/run%04d_s03.Submit_LowGain_Mult20.root",runNo),
        //Form("/data/RB230064/ejungwoo/analysis202406/data/run%04d_s04.Submit_LowGain_Mult20.root",runNo),
        //Form("/data/RB230064/ejungwoo/analysis202406/data/run%04d_s05.Submit_LowGain_Mult20.root",runNo),
        //Form("/data/RB230064/ejungwoo/analysis202406/data/run%04d_s06.Submit_LowGain_Mult20.root",runNo),
        //"/data/RB230064/ejungwoo/analysis202406/data/lgrun1045_s00.test.root"
    };

    TFile* file = new TFile(Form("data/summary_run%04d.root",runNo),"recreate");
    TTree* tree = new TTree("track","");
    double vx, vy, vz, dedx, hmom, gmom;
    bool isLG;
    int nhit;
    tree -> Branch("vx",&vx);
    tree -> Branch("vy",&vy);
    tree -> Branch("vz",&vz);
    tree -> Branch("dedx",&dedx);
    tree -> Branch("hmom",&hmom);
    tree -> Branch("gmom",&gmom);
    tree -> Branch("lg",&isLG);
    tree -> Branch("nhit",&nhit);

    auto tspirit = new TChain("cbmsim");
    for (auto fileName : fileNames)
        tspirit -> AddFile(fileName);
    tspirit -> Print("toponly");
    TClonesArray *STRecoArray = nullptr;
    TClonesArray *LGRecoArray = nullptr;
    TClonesArray *STHelixArray = nullptr;
    TClonesArray *LGHelixArray = nullptr;
    TClonesArray *STVertexArray = nullptr;
    tspirit -> SetBranchAddress("STRecoTrack",&STRecoArray);
    tspirit -> SetBranchAddress("LGRecoTrack",&LGRecoArray);
    tspirit -> SetBranchAddress("STHelixTrack",&STHelixArray);
    tspirit -> SetBranchAddress("LGHelixTrack",&LGHelixArray);
    tspirit -> SetBranchAddress("STVertex",&STVertexArray);
    auto numEvents = tspirit -> GetEntries();
    cout << numEvents << " events" << endl;
    return;
    for (auto iEvent=0; iEvent<numEvents; ++iEvent)
    {
        if (iEvent%100==0) cout << iEvent << " / " << numEvents << endl;
        tspirit -> GetEntry(iEvent);
        if (STVertexArray -> GetEntries()==1)
        {
            auto vertex = (STVertex*) STVertexArray -> At(0);
            auto pos = vertex -> GetPos();
            vx = pos.x();
            vy = pos.y();
            vz = pos.z();
            if (vz<-50||vz>0)
                continue;
        }

        auto numTracks = STRecoArray -> GetEntries();
        for (auto iTrack=0; iTrack<numTracks; ++iTrack)
        {
            auto track = (STRecoTrack*) STRecoArray -> At(iTrack);
            isLG = false;
            dedx = track -> GetdEdxWithCut(0,0.8);
            gmom = track -> GetMomentum().Mag();
            hmom = ((STHelixTrack*) STHelixArray -> At(track->GetHelixID())) -> Momentum();
            nhit = track -> GetNumRowClusters() + track -> GetNumLayerClusters();
            tree -> Fill();
            hist1 -> Fill(gmom, dedx);
        }
        auto numTracks2 = LGRecoArray -> GetEntries();
        for (auto iTrack=0; iTrack<numTracks2; ++iTrack)
        {
            auto track = (STRecoTrack*) LGRecoArray -> At(iTrack);
            isLG = true;
            dedx = track -> GetdEdxWithCut(0,0.8);
            gmom = track -> GetMomentum().Mag();
            hmom = ((STHelixTrack*) LGHelixArray -> At(track->GetHelixID())) -> Momentum();
            nhit = track -> GetNumRowClusters() + track -> GetNumLayerClusters();
            tree -> Fill();
            hist2 -> Fill(gmom, dedx*100);
        }
    }

    gStyle -> SetPalette(kRainBow);

    file -> cd();
    tree -> Write();

    new TCanvas("cvs1","",1200,800);
    hist1 -> Draw("colz");
    hist1 -> Write();

    new TCanvas("cvs2","",1200,800);
    hist2 -> Draw("colz");
    hist2 -> Write();
}
