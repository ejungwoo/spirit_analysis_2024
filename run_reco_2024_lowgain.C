#include <unistd.h>

void run_reco_2024_lowgain (
        Int_t   fRunMode    = 2,
        Int_t   fRunNo      = 1045,
        Int_t   fSplitNo    = 0,
        Int_t   fNumEvents  = 100,
        TString fTag        = "test",
        Int_t   fTMLGReco   = 20,
        TString fPathToData = "/data/RB230064/ejungwoo/analysis202406/data/",
        TString outputName  = "",
        TString loggerName  = "",
        Bool_t  fUseMeta    = kFALSE,
        TString fSupplePath = "metadata",
        TString fMCFile     = "",
        std::vector<Int_t> fSkipEventArray = {}
        )
{
    TString fSpiRITROOTPath = TString(gSystem -> Getenv("VMCWORKDIR"))+"/";
    TString parName     = "ST.parameters.Commissioning_20240618.par";
    TString geomName    = fSpiRITROOTPath+"geometry/geomSpiRIT.man.root";

    TString rawListName = TString::Format("%s/list_run%04d.txt",(gSystem -> Getenv("PWD")), fRunNo);
    if (outputName.IsNull()) outputName  = TString::Format("%srun%04d_s%02d.%s.root", fPathToData.Data(),fRunNo,fSplitNo,fTag.Data());
    if (loggerName.IsNull()) loggerName  = TString::Format("%srun%04d_s%02d.%s.log", fPathToData.Data(),fRunNo,fSplitNo,fTag.Data());

    std::cout << "parameter: " << parName << endl;
    std::cout << "geometry:  " << geomName << endl;
    std::cout << "output:    " << outputName << endl;
    std::cout << "logger:    " << loggerName << endl;

    FairLogger *fLogger = FairLogger::GetLogger();
    fLogger -> SetLogFileName(loggerName);
    fLogger -> SetLogToScreen(kTRUE);
    fLogger -> SetLogToFile(kTRUE);
    fLogger -> SetLogVerbosityLevel("HIGH");
    fLogger -> SetLogFileLevel("HIGH");
    fLogger -> SetLogScreenLevel("HIGH");
    // above logger for screen doesn't work some how..........................................

    /* ======= This part you need initial configuration ========= */
    // Parameter database file - files should be in parameters folder.
    //TString systemDB = "systemDB.csv";
    //TString runDB = "runDB.csv";

    // Data paths - must have one %d for run number
    // If you don't need either of them, pass it blank.
    TString ggDataPathWithFormat = "";
    TString beamDataPathWithFormat = "";

    // Meta data path

    //TString fSupplePath = "/mnt/spirit/rawdata/misc/rawdataSupplement";

    // Use relative gain matching - Amplify low gain section
    Bool_t fUseGainMatching = kFALSE;
    /* ========================================================== */

    //TString fSystemDB = fSpiRITROOTPath + "parameters/" + systemDB;
    //TString fRunDB = fSpiRITROOTPath + "parameters/" + runDB;

    //auto fParamSetter = new STParameters(fRunNo, fSystemDB, fRunDB);
    //auto fNumEventsInRun = fParamSetter -> GetNumTotalEvents();
    //auto fParameterFile = fParamSetter -> GetParameterFile();
    //auto fParameterFile = fSpiRITROOTPath+"parameters/"+fParameterFile;
    //auto fParameterFile = "ST.parameters.Commissioning_20240618.par";
    //auto fYPedestalOffset = fParamSetter -> GetYPedestal();
    //auto fFieldOffsetX = fParamSetter -> GetFieldOffsetX();
    //auto fFieldOffsetY = fParamSetter -> GetFieldOffsetY();
    //auto fFieldOffsetZ = fParamSetter -> GetFieldOffsetZ();
    //auto fSheetChargeDensity = fParamSetter -> GetSheetChargeDensity();
    //auto fSystemID = fParamSetter -> GetSystemID();
    //auto fTargetZ = fParamSetter -> GetTargetZ();
    auto fTargetZ = -13.2;
    //auto fBDCOffsetX = fParamSetter -> GetBDCOffsetX();
    //auto fBDCOffsetY = fParamSetter -> GetBDCOffsetY();
    //auto fGGRunID = fParamSetter -> GetGGRunID();
    //auto fRelativeGainRunID = fParamSetter -> GetRelativeGainRunID();

    cout << "done with pars" << endl;

    auto fIsGGDataSet = !ggDataPathWithFormat.IsNull();
    auto fIsBeamDataSet = !beamDataPathWithFormat.IsNull();
    TString fGGData = "";
    TString fBeamData = "";
    //if (fIsGGDataSet)   fGGData = Form(ggDataPathWithFormat.Data(), fGGRunID);
    //if (fIsBeamDataSet) fBeamData = Form(beamDataPathWithFormat.Data(), fRunNo);
    //TString fGainMatchingFile = fSpiRITROOTPath + Form("parameters/RelativeGainRun%d.list", fRelativeGainRunID);

    cout << "set gain matching file" << endl;

    Int_t start = fSplitNo * fNumEvents;
    /*if (start >= fNumEventsInRun) {
      cout << start << " is >= " << fNumEventsInRun << endl;
      return;
      }
      if (start + fNumEvents > fNumEventsInRun) {
      cout << start + fNumEvents << " is > " << fNumEventsInRun << endl;
      fNumEvents = fNumEventsInRun - start;
      }*/

    TString sRunNo   = TString::Itoa(fRunNo, 10);
    TString sSplitNo = TString::Itoa(fSplitNo, 10);

    TString version; {
        TString name = fSpiRITROOTPath + "VERSION.compiled";
        std::ifstream vfile(name);
        vfile >> version;
        vfile.close();
    }

    if (TString(gSystem -> Which(".", rawListName)).IsNull() && !fUseMeta) {
        cout << "data list not found" << endl;
        gSystem -> Exec("./createList_FRIBDAQ.sh "+sRunNo);
    }

    TString fMetaDataList;
    if (fUseMeta) {
        rawListName = Form("%s/run_%04d/dataList.txt", fSupplePath.Data(), fRunNo);
        fMetaDataList = Form("%s/run_%04d/metadataList.txt", fSupplePath.Data(), fRunNo);
    }
    cout << "using data list " << rawListName << endl;

    FairLogger *logger = FairLogger::GetLogger();
    logger -> SetLogToScreen(false);

    //FairLogger::SetLogVerbosity(FairLogger::kSilent);

    FairParAsciiFileIo* parReader = new FairParAsciiFileIo();
    parReader -> open(parName);

    FairRunAna* run = new FairRunAna();
    run -> SetGeomFile(geomName);
    run -> SetOutputFile(outputName);
    run -> GetRuntimeDb() -> setSecondInput(parReader);

    STDecoderTask *decoder = new STDecoderTask();
    decoder -> SetUseFRIBDAQData();
    decoder -> SetUseSeparatedData(true);
    decoder -> SetPersistence(true);
    // By default, if SetUseGainCalibration(true) is called, reading gain calibration information from parameter file.
    //decoder -> SetUseGainCalibration(true);
    /* Manual calibration parameter setters. You need to provide both calibration root file and reference values to match.
     * Use default automatic one if you're not sure what you're doing. */
    // decoder -> SetGainCalibrationData("filename.root");
    // decoder -> SetGainReference(constant, linear, quadratic);
    //if (fIsGGDataSet)
    //decoder -> SetGGNoiseData(fGGData);
    cout << rawListName << endl;
    decoder -> SetDataList(rawListName);
    //decoder -> SetEventID(start);
    decoder -> SetTbRange(30, 257); 
    // Low gain calibration. Don't forget you need to uncomment PSA part, too.
    //if (fUseGainMatching)
    //decoder -> SetGainMatchingData(fGainMatchingFile);
    // Method to select events to reconstruct
    // Format of the input file:
    //        runid eventid
    //        runid eventid
    //        runid eventid
    //        runid eventid
    //map<Int_t, vector<Int_t> *> events;
    //string FileName_PiEvt = "./Pick_PiEvt/Sn108_PiEvt/";
    //FileName_PiEvt = FileName_PiEvt+"Sn108_Run"+fRunNo+"_PiEvt";
    //cout<<"Reading the Event list for the pion events : "<<FileName_PiEvt<<endl;
    //readEventList(FileName_PiEvt, events);
    //cout <<"Number of events " << fNumEvents << " starting at " << start <<endl;

    //decoder -> SetEventList(*events[fRunNo]);
    decoder -> SetEventID(start);


    if (fUseMeta) 
    {
        std::ifstream metalistFile(fMetaDataList.Data());
        TString dataFileWithPath;
        for (Int_t iCobo = 0; iCobo < 12; iCobo++) {
            dataFileWithPath.ReadLine(metalistFile);
            dataFileWithPath = Form("%s/run_%04d/%s", fSupplePath.Data(), fRunNo, dataFileWithPath.Data());
            decoder -> SetMetaData(dataFileWithPath, iCobo);
        }
    }

    auto embedTask = new STEmbedTask();
    embedTask -> SetPersistence(false);
    embedTask -> SetEventID(start);
    embedTask -> SetEmbedFile(fMCFile);

    auto preview = new STEventPreviewTask();
    preview -> SetSkippingEvents(fSkipEventArray);
    preview -> SetPersistence(true);
    //preview -> SetSelectingEvents(*events[fRunNo]);

    auto psa = new STPSAETask();
    psa -> SetPersistence(true);
    psa -> SetPulserData("pulser_117ns_50tb.dat");
    psa -> SetYOffsets(fSpiRITROOTPath + "parameters/yOffsetCalibration.dat");
    //psa -> SetLayerCut(0,110); // low gain region

    auto psa2 = new STPSAETask();
    psa2 -> SetPersistence(true);
    psa2 -> SetPulserData("pulser_117ns_50tb.dat");
    psa2 -> SetYOffsets(fSpiRITROOTPath + "parameters/yOffsetCalibration.dat");
    psa2 -> SetLayerCut(83,112); // low gain region
    psa2 -> SetOutputBranch("LG");
    psa2 -> DoubleReconstructionMode(fTMLGReco);

    auto helix = new STHelixTrackingTask();
    helix -> SetPersistence(true);
    helix -> SetClusterPersistence(true);
    helix -> SetClusterCutLRTB(420, -420, -64, -522);
    helix -> SetEllipsoidCut(TVector3(0, -260, -11.9084), TVector3(120, 100, 220), 5); // current use
    //helix -> SetCylinderCut(TVector3(0, -226.06, -11.9084), 100, 100, 5);
    //helix -> SetSphereCut(TVector3(0, -226.06, -11.9084), 100, 5);
    //helix -> SetEllipsoidCut(TVector3(0, -206.34, -11.9084), TVector3(120, 55, 240), 5);
    //Changing clustering direction angle and margin. Default: 45 deg with 0 deg margin
    //helix -> SetClusteringAngleAndMargin(35., 3.);

    auto helix2 = new STHelixTrackingTask();
    helix2 -> SetPersistence(true);
    helix2 -> SetClusterPersistence(true);
    helix2 -> SetClusterCutLRTB(420, -420, -64, -522);
    helix2 -> SetEllipsoidCut(TVector3(0, -260, -11.9084), TVector3(120, 100, 220), 5); // current use
    helix2 -> SetInputBranch("LG");
    helix2 -> SetOutputBranch("LG");

    auto genfitPID = new STGenfitPIDTask();
    genfitPID -> SetTargetPlane(0, 0, fTargetZ); // unit: mm
    genfitPID -> SetPersistence(true);
    genfitPID -> SetBDCFile("");
    genfitPID -> SetListPersistence(true);
    //genfitPID -> SetMaxDCluster(60);

    auto genfitPID2 = new STGenfitPIDTask();
    genfitPID2 -> SetTargetPlane(0, 0, fTargetZ); // unit: mm
    genfitPID2 -> SetPersistence(true);
    genfitPID2 -> SetBDCFile("");
    genfitPID2 -> SetListPersistence(true);
    genfitPID2 -> SetInputBranch("LG");
    genfitPID2 -> SetOutputBranch("LG");

    run -> AddTask(decoder);
    if(!fMCFile.IsNull())
        run -> AddTask(embedTask);
    run -> AddTask(preview);

    if (fRunMode==99)
        ;
    else if (fRunMode==0) {
        run -> AddTask(psa);
    }
    else if (fRunMode==1) {
        run -> AddTask(psa);
        run -> AddTask(helix);
        run -> AddTask(genfitPID);
    }
    else if (fRunMode==2) {
        run -> AddTask(psa);
        run -> AddTask(helix);
        run -> AddTask(psa2);
        run -> AddTask(helix2);
        run -> AddTask(genfitPID2);
        run -> AddTask(genfitPID);
    }

    run -> Init();
    //run -> Run(0,1000);
    cout << "processing run " << fRunNo << " with " << fNumEvents << " events." << endl;
    run -> Run(0,fNumEvents);

    auto outFile = FairRootManager::Instance() -> GetOutFile();
    auto recoHeader = new STRecoHeader("RecoHeader","");
    recoHeader -> SetPar("version", version);
    recoHeader -> SetPar("eventStart", start);
    recoHeader -> SetPar("numEvents", fNumEvents);
    //recoHeader -> SetPar("parameter", fParameterFile);
    if (fIsGGDataSet)
        recoHeader -> SetPar("GGData", fGGData);
    //recoHeader -> SetPar("yPedestal", fYPedestalOffset);
    //recoHeader -> SetPar("fieldOffsetX", fFieldOffsetX);
    //recoHeader -> SetPar("fieldOffsetY", fFieldOffsetY);
    //recoHeader -> SetPar("fieldOffsetZ", fFieldOffsetZ);
    //recoHeader -> SetPar("sheetChargeDensity", fSheetChargeDensity);
    //recoHeader -> SetPar("BDCOffsetX", fBDCOffsetX);
    //recoHeader -> SetPar("BDCOffsetY", fBDCOffsetY);
    auto driftVelocityInParameterFile = gSystem -> GetFromPipe("cat " + parName + " | grep DriftVelocity | awk '{print $2}'");
    recoHeader -> SetPar("driftVelocity", driftVelocityInParameterFile);
    recoHeader -> Write("RecoHeader");

    cout << "Log    : " << loggerName << endl;
    cout << "Input  : " << rawListName << endl;
    cout << "Output : " << outputName << endl;

    gApplication -> Terminate();
}

