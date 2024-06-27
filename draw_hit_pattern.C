void draw_hit_pattern()
{
    //TString cutName = "nocut"; TString cut = "";
    TString nameHit = "STHit_1";
    //TString nameHit = "STHit";
    //TString cutName = "cut1"; TString cut = Form("@%s.GetEntries()<100 && %s.fY<-300 && (%s.fX<-200 || %s.fX>-50)",nameHit.Data(),nameHit.Data(),nameHit.Data(),nameHit.Data());
    TString cutName = "cut1"; TString cut = Form("%s.fY<-300 && (%s.fX<-200 || %s.fX>-50)",nameHit.Data(),nameHit.Data(),nameHit.Data());
    TString titleMain = "RUN1046";

    auto histNumHits = new TH1D("histNumHits",titleMain+" number of hits in event",200,0,300);
    //auto histZX = new TH2D("histZX","z vs x",112,0,1344,108,-432,432);
    auto histZX = new TH2D("histZX",titleMain+" x vs z", 112-84,84*12,112*12, 108,-432,432);
    auto histY = new TH1D("histY",titleMain+" y",200,-600,0);
    auto histE = new TH1D("histE",titleMain+" e",500,0,500);
    auto histLayer = new TH1D("histLayer",titleMain+" layer",112-84,84,112);
    histLayer -> SetMinimum(0);

    TString fileNames[] = {
        "/data/RB230064/ejungwoo/analysis202406/data/lgrun1045_s00.double_reco.root",
        //"data/lgrun1046_s00.split_test.root",
        //"data/lgrun1046_s01.split_test.root",
        //"data/lgrun1046_s02.split_test.root",
        //"data/lgrun1046_s03.split_test.root",
        //"data/lgrun1046_s04.split_test.root",
        //"data/lgrun1046_s05.split_test.root"
    };

    if (0)
    {
        for (auto fileName : fileNames)
        {
            auto file = new TFile(fileName,"read");
            auto tree = (TTree*) file -> Get("cbmsim");
            TClonesArray *hitArray = nullptr;
            tree -> SetBranchAddress(nameHit,&hitArray);
            auto numEvents = tree -> GetEntries();
            cout << fileName << " containing " << numEvents << " events" << endl;
            for (auto iEvent=0; iEvent<numEvents; ++iEvent)
            {
                tree -> GetEntry(iEvent);
                auto numHits = hitArray -> GetEntries();
                histNumHits -> Fill(numHits);
                for (auto iHit=0; iHit<numHits; ++iHit)
                {
                    auto hit = (STHit*) hitArray -> At(iHit);
                    auto x = hit -> GetX();
                    auto y = hit -> GetY();
                    auto z = hit -> GetZ();
                    auto e = hit -> GetCharge();
                    auto layer = hit -> GetLayer();
                    histZX -> Fill(z,x);
                    histY -> Fill(y);
                    histE -> Fill(e);
                    histLayer -> Fill(layer);
                }
            }
            file -> Close();
        }
    }
    else {
        auto tree = new TChain("cbmsim");
        for (auto fileName : fileNames)
            tree -> AddFile(fileName);
        tree -> Project("histNumHits",Form("@%s.GetEntries()",nameHit.Data()),cut);
        tree -> Project("histZX",Form("%s.fX:%s.fZ",nameHit.Data(),nameHit.Data()),cut);
        tree -> Project("histY",Form("%s.fY",nameHit.Data()),cut);
        tree -> Project("histE",Form("%s.fCharge",nameHit.Data()),cut);
        tree -> Project("histLayer",Form("%s.fLayer",nameHit.Data()),cut);
    }

    auto cvs = new TCanvas("cvs","",2200,1200);
    cvs -> Divide(3,2);
    cvs -> cd(1); histNumHits -> Draw();
    cvs -> cd(2); histZX -> Draw("colz");
    cvs -> cd(3); histY -> Draw();
    cvs -> cd(4); histE -> Draw();
    cvs -> cd(5); histLayer -> Draw();
    cvs -> SaveAs(Form("figures/figures_hitsummary_%s.%s.png",titleMain.Data(),cutName.Data()));
}
