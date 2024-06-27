void draw_pid_lg()
{
    auto file = new TFile("data/summary_run1045.root");
    auto tree = (TTree*) file -> Get("track");
    auto cvs = new TCanvas("cvs","",2200,1400);
    cvs -> Divide(2,2);
    cvs -> cd(1); tree -> Draw("dedx:hmom>>hist1(400,0,3000,400,0,1000)","nhit>10&&vz>-50&&vz<0&&lg==0","colz");
    cvs -> cd(2); tree -> Draw("dedx*100:hmom>>hist2(200,0,3000,200,0,100000)","vz>-50&&vz<0&&lg==1","colz");
    cvs -> cd(3); tree -> Draw("dedx:gmom>>hist3(400,0,3000,400,0,1000)","nhit>10&&vz>-50&&vz<0&&lg==0","colz");
    cvs -> cd(4); tree -> Draw("dedx*100:gmom>>hist4(200,0,3000,200,0,100000)","vz>-50&&vz<0&&lg==1","colz");
}
