//TODO: add in histograms from other variations: pT/eta/pileup weighting, lepton definition change, etc.
void shapeAnalyzer(TString file){
	TFile* iFile = TFile::Open(file);
	TFile* oFile = new TFile("varHists17.root","RECREATE");

	Int_t nKeys = iFile->GetNkeys();
	TList* keyList = iFile->GetListOfKeys();
	std::vector<TString> fakesVec{"Fakes_elf0","Fakes_elf1","Fakes_elf2"};
	std::vector<TString> procVec{"","ttbar_","ZDY_"};

	gStyle->SetOptStat(0);


	for(int iKey = 0; iKey < nKeys-2; iKey++){
		TKey* key = (TKey*)keyList->At(iKey); //list of dirs
		if(key == NULL) continue;
		cout << key->GetTitle() << endl;

		TDirectory* oldDir = iFile->GetDirectory(key->GetTitle());
		TList* listOfHists = oldDir->GetListOfKeys();
		if(!listOfHists->Contains("Fakes_elf0")) continue;

		TDirectory* newDir = oFile->mkdir(key->GetTitle());
		newDir->cd();
		

		
		for(int iHist = 0; iHist < fakesVec.size(); iHist++){
			for(int iProc = 0; iProc < procVec.size(); iProc++){
				TString histName = procVec.at(iProc)+fakesVec.at(iHist);
				TH1D* nomHist = (TH1D*)oldDir->Get(histName);
				if(nomHist == NULL) continue;

				cout << histName << endl;

				nomHist->SetTitle(histName);

				TH1D* upHist = (TH1D*)oldDir->Get(histName+"_BTAG_SFUp");
				TH1D* downHist = (TH1D*)oldDir->Get(histName+"_BTAG_SFDown");

				float nomUp = upHist->Integral()/nomHist->Integral();
				float nomDown = downHist->Integral()/nomHist->Integral();
				



				//if(nomUp > 0.5 || nomUp-1 > 0.5) cout << "nom, up variation" << endl;
				//if(nomDown > 0.5 || nomDown-1 > 0.5) cout << "nom, down variation" << endl;
				

				upHist->SetTitle(histName+"_BTAG_SFUp");
				downHist->SetTitle(histName+"_BTAG_SFDown");


				nomHist->Scale(1/nomHist->Integral(),"width");
				upHist->Scale(1/upHist->Integral(),"width");
				downHist->Scale(1/downHist->Integral(),"width");

			

				
				upHist->SetLineColor(kRed);
				upHist->SetMarkerColor(kRed);
				upHist->SetMarkerStyle(20);
				upHist->SetStats(0);

				downHist->SetLineColor(kBlue);
				downHist->SetMarkerColor(kBlue);
				downHist->SetMarkerStyle(20);
				downHist->SetStats(0);

				nomHist->SetLineColor(kGreen);
				nomHist->SetMarkerColor(kGreen);
				nomHist->SetMarkerStyle(20);
				nomHist->SetStats(0);




				TCanvas* c1 = new TCanvas(histName,histName,800,600);
				TLegend* leg = new TLegend(0.65,0.7,0.9,0.9);

				leg->AddEntry(nomHist);
				leg->AddEntry(upHist);
				leg->AddEntry(downHist);

				
				upHist->Draw("goff");
				downHist->Draw("same goff");
				nomHist->Draw("same goff");

				leg->Draw("same goff");

				c1->Write();
				c1->Close();

				delete c1;
			}

		}
		newDir->Write();
		cout << "\n" << endl;
		
		
	}
	iFile->Close();
	oFile->Close();


}
