#include "shapeAnalyzer.hh"
//using namespace std;
using std::cout;
using std::endl;
shapeAnalyzer::shapeAnalyzer(TFile* ifile){ //for histograms within one file
	iFile = ifile;
	// oFile = new TFile((ofile+".root").c_str(),"RECREATE");

	// nKeys = iFile->GetNkeys();
	// keyList = iFile->GetListOfKeys();
	// fakesVec{"Fakes_elf0","Fakes_elf1","Fakes_elf2"};
	// procVec{"","ttbar_","ZDY_"};
	gStyle->SetOptStat(0);


}

// shapeAnalyzer::shapeAnalyzer(string fNom, string fUp, string fDown, string ofile){ //for histograms across files
// 	//FIGURE THIS OUT -- or not???
// 	TFile* nomFile = TFile::Open(fNom);
// 	nKeys = nomFile->GetNkeys();
// 	nomKeyList = nomFile->GetListOfKeys();

// 	TFile* upFile = TFile::Open(fUp);
// 	Int_t nUpKeys = upFile->GetNkeys();
// 	upKeyList = upFile->GetListOfKeys();

// 	TFile* downFile = TFile::Open(fDown);
// 	Int_t nDownKeys = downFile->GetNkeys();
// 	upKeyList = downFile->GetListOfKeys();

// 	if(nKeys != nUpKeys || nUpKeys != nDownKeys || nKeys != nDownKeys){
// 		cout << "Error: uneven number of keys among files" << endl;
// 		break;
// 	}	

// 	// fakesVec{"Fakes_elf0","Fakes_elf1","Fakes_elf2"};
// 	// procVec{"","ttbar_","ZDY_"};
// 	oFile = new TFile((ofile+".root").c_str(),"RECREATE");
// 	gStyle->SetOptStat(0);
// } 

shapeAnalyzer::~shapeAnalyzer(){
	if(iFile->IsOpen()) iFile->Close();
	// if(oFile->IsOpen()) oFile->Close();
//	if(nomFile.IsOpen()) nomFile->Close();
//	if(upFile.IsOpen()) upFile->Close();
//	if(downFile.IsOpen()) downFile->Close();
}

// void shapeAnalyzer::Analyze(){
// 	for(int iKey = 0; iKey < nKeys-2; iKey++){
// 		getKeys(iKey);
// 		for(int iHist = 0; iHist < fakesVec.size(); iHist++){
// 			for(int iProc = 0; iProc < procVec.size(); iProc++){
// 			//	drawHists(iHist,iProc);
// 			//	formatPlots();
// 			}
// 		}
// 		newDir->Write();
// 		std::cout << "\n" << std::endl;
// 	}
// 	iFile->Close();
// 	oFile->Close();
// }

// void shapeAnalyzer::getKeys(Int_t iKey){
// 	TKey* key = (TKey*)keyList->At(iKey); //list of dirs
// 	if(key == NULL) return;//continue;
// 	std::cout << key->GetTitle() << std::endl;

// 	oldDir = iFile->GetDirectory(key->GetTitle());
// 	TList* listOfHists = oldDir->GetListOfKeys();
// 	if(!listOfHists->Contains("Fakes_elf0")) return;//continue;

// 	newDir = oFile->mkdir(key->GetTitle());
// 	newDir->cd();
// }

void shapeAnalyzer::drawHists(TDirectory* oldDir, int iHist, int iProc,std::string histName,std::string sys){
//	vector<TH1D*> vecHists;
//std::cout << "drawHists" << std::endl;
	TH1D* nomHist = (TH1D*)oldDir->Get(histName.c_str());
	if(nomHist == NULL){
	std::cout << "	" << histName << " not found" << std::endl;
	return;}
	std::cout <<"	" << histName << std::endl;

	nomHist->SetTitle((histName+"_"+sys).c_str());
	nomHist->Scale(1/nomHist->Integral(),"width");
//std::cout << "a" << std::endl;
	TH1D* upHist = (TH1D*)oldDir->Get((histName+"_"+sys+"Up").c_str());
	TH1D* downHist = (TH1D*)oldDir->Get((histName+"_"+sys+"Down").c_str());

	if(upHist == NULL){
		std::cout << "	" << histName+"_"+sys+"Up" << "not found" << std::endl;
	return;
	} 
	if(downHist == NULL){
                std::cout << "	" << histName+"_"+sys+"Down" << " not found" << std::endl;
        return;
        }

	upHist->Scale(1/upHist->Integral(),"width");
	downHist->Scale(1/downHist->Integral(),"width");
//std::cout << "b" << std::endl;
	upHist->SetTitle((histName+"_"+sys+"Up").c_str());
	downHist->SetTitle((histName+"_"+sys+"Down").c_str());
//std::cout << "c" << std::endl;
	upHist->SetLineColor(kRed);
	upHist->SetMarkerColor(kRed);
	upHist->SetMarkerStyle(22);
	upHist->SetStats(0);

	downHist->SetLineColor(kBlue);
	downHist->SetMarkerColor(kBlue);
	downHist->SetMarkerStyle(23);
	downHist->SetStats(0);

	nomHist->SetLineColor(kGreen);
	nomHist->SetMarkerColor(kGreen);
	nomHist->SetMarkerStyle(20);
	nomHist->SetStats(0);

	TH1D* ratioUpHist = (TH1D*)upHist->Clone();
	TH1D* ratioDownHist = (TH1D*)downHist->Clone();
    	ratioUpHist->Divide(nomHist);
	ratioDownHist->Divide(nomHist);
	ratioUpHist->SetLineColor(kRed);
	ratioUpHist->SetMarkerColor(kRed);
	ratioUpHist->SetMarkerStyle(22);
	ratioUpHist->SetStats(0);
	ratioUpHist->SetTitle((histName+"_"+sys+"Up").c_str());

	ratioDownHist->SetLineColor(kBlue);
	ratioDownHist->SetMarkerColor(kBlue);
	ratioDownHist->SetMarkerStyle(23);
	ratioDownHist->SetStats(0);
	ratioDownHist->SetTitle((histName+"_"+sys+"Down").c_str());
//	vecHists.push_back(nomHist);
//	vecHists.push_back(upHist);
//	vecHists.push_back(downHist);
//


	for(int i = 0; i < upHist->GetNbinsX(); i++){
		if(isnan(upHist->GetBinContent(i))) cout << "upHist bin # " << i << " has a NaN" << endl;
		if(isnan(downHist->GetBinContent(i))) cout << "downHist bin # " << i << " has a NaN" << endl;
		if(isnan(ratioUpHist->GetBinContent(i))) cout << "ratioUpHist bin # " << i << " has a NaN" << endl;
		if(isnan(ratioDownHist->GetBinContent(i))) cout << "ratioDownHist bin # " << i << " has a NaN" << endl;
	}


	TCanvas* c1 = new TCanvas(histName.c_str(),histName.c_str(),800,600);

	TLegend* leg = new TLegend(0.35,0.7,0.9,0.9);

	leg->AddEntry(nomHist);
	leg->AddEntry(upHist);
	leg->AddEntry(downHist);


	upHist->Draw("hist P goff");
	downHist->Draw("hist P same goff");
	nomHist->Draw("hist P same goff");

	leg->Draw("same goff");

	c1->Write();
	c1->Close();

	TCanvas* cRatio = new TCanvas((histName+"_ratio").c_str(),(histName+"_ratio").c_str(),800,600);
	TLegend* legRatio = new TLegend(0.35,0.8,0.9,0.9);
	legRatio->AddEntry(ratioUpHist);
	legRatio->AddEntry(ratioDownHist);
	
	ratioUpHist->Draw("hist P goff");
	ratioDownHist->Draw("hist P same goff");
	legRatio->Draw("hist P same goff");

	cRatio->Write();
	cRatio->Close();

	delete c1;
	delete cRatio;

}

//  void shapeAnalyzer::getHist(Int_t iHist, Int_t iProc,TString histName){
//	// TString histName = procVec.at(iProc)+fakesVec.at(iHist);
//	nomHist = (TH1D*)oldDir->Get(histName);
//	if(nomHist == NULL) continue;
//
//	cout << histName << endl;
//
//	nomHist->SetTitle(histName);
//	nomHist->Scale(1/nomHist->Integral(),"width");
//
//	upHist = (TH1D*)oldDir->Get((histName+sys+"Up").c_str());
//	downHist = (TH1D*)oldDir->Get((histName+sys+"_BTAG_SFDown").c_str());
//	upHist->Scale(1/upHist->Integral(),"width");
//	downHist->Scale(1/downHist->Integral(),"width");
//}
//
//void shapeAnalyzer::formatPlots(std::vector<TH1D*> hists){
//	for(int i = 0; i < hists.size(); i++){
//
//
//	}
//	upHist->SetTitle(histName+"_BTAG_SFUp");
//	downHist->SetTitle(histName+"_BTAG_SFDown");
//
//	nomHist->Scale(1/nomHist->Integral(),"width");
//	upHist->Scale(1/upHist->Integral(),"width");
//	downHist->Scale(1/downHist->Integral(),"width");
//
//	upHist->SetLineColor(kRed);
//	upHist->SetMarkerColor(kRed);
//	upHist->SetMarkerStyle(20);
//	upHist->SetStats(0);
//
//	downHist->SetLineColor(kBlue);
//	downHist->SetMarkerColor(kBlue);
//	downHist->SetMarkerStyle(20);
//	downHist->SetStats(0);
//
//	nomHist->SetLineColor(kGreen);
//	nomHist->SetMarkerColor(kGreen);
//	nomHist->SetMarkerStyle(20);
//	nomHist->SetStats(0);
//
//	TCanvas* c1 = new TCanvas(histName,histName,800,600);
//	TLegend* leg = new TLegend(0.65,0.7,0.9,0.9);
//
//	leg->AddEntry(nomHist);
//	leg->AddEntry(upHist);
//	leg->AddEntry(downHist);
//
//
//	upHist->Draw("goff");
//	downHist->Draw("same goff");
//	nomHist->Draw("same goff");
//
//	leg->Draw("same goff");
//
//	c1->Write();
//	c1->Close();
//
//	delete c1;
//}





