
class Opt_par : public TObject
{
	public:

		int delay;
		float factor;
		int zcl;

		std::vector<double> TimeA;
		std::vector<double> TimeB;
		std::vector<double> Energy;

		ClassDef(Opt_par, 1)

};

ClassImp(Opt_par)

std::vector<Double_t> Fitter(TH1F * histo, Double_t range)
{
	const double sqrt2ln2 = 1.17741002251547;

	TSpectrum *search_peak = new TSpectrum(1);

	int found_peak = search_peak->Search(histo, 1, "", 0.5);

	// Retrieving X positions and print them out to std output

	Double_t  x_peak = search_peak->GetPositionX()[0];
	Double_t  y_peak = search_peak->GetPositionY()[0];

	TF1 *GausPeak = new TF1("GPeak","gaus", x_peak-range, x_peak+range);
	//GausPeak->SetParameter(0,y_peak);
	//GausPeak->SetParLimits(0,y_peak-6*y_peak/100,y_peak+1*y_peak/100);
	//GausPeak->SetParameter(1,x_peak);

  histo->Fit(GausPeak,"RNME");

	x_peak         = GausPeak->GetParameter(1);
	y_peak         = GausPeak->GetParameter(0);
	Double_t Sigma = GausPeak->GetParameter(2);
	Double_t SigmaSig = GausPeak->GetParError(2);
	Double_t FWHM    = 2*sqrt2ln2*Sigma;
	Double_t FWHMsig = 2*sqrt2ln2*SigmaSig;

	std::vector<Double_t> FitResult;
  FitResult.push_back(FWHM);
  FitResult.push_back(FWHMsig);

  delete GausPeak;
	delete search_peak;
	return FitResult;

}

void read_opt(string filename)
{

  // Retrieving File
  TFile * input_file = new TFile(filename.c_str());

  TTree * daq_tree;

  // Retrieving TTree from TFile

  daq_tree = (TTree*) input_file->Get("out_opt_par");


  TBranch * branch = daq_tree->GetBranch("parameters");

  long int n_ev = branch->GetEntries();

  Opt_par * opt_sett = new Opt_par;

  branch->SetAddress(&opt_sett);

	TH1F * Time = 0;

	std::vector<Float_t> Frac;
	std::vector<Int_t> Del;
	std::vector<Double_t> FWHM;
	std::vector<Int_t> Zcl;

  for (size_t i = 0; i < n_ev; i++)
  {
    branch->GetEntry(i);

		//get parameters
		float Fraction = opt_sett->factor;
		int   Delay    = opt_sett->delay;
		int   ZCL      = opt_sett->zcl;

		Time = new TH1F(Form("Frac%.2f_Del%i_Zcl%i_Timing",Fraction, Delay, ZCL), Form("Frac%.2f_Del%i_Zcl%i_Timing", Fraction, Delay, ZCL), 1000, -7, 7);


    for (size_t j = 0; j < opt_sett->TimeA.size() ; j++)
		{
			Time->Fill(opt_sett->TimeA[j]-opt_sett->TimeB[j]);
		}

		std::vector<Double_t> v = Fitter(Time,0.55);

		Frac.push_back(Fraction);
		Del.push_back(Delay);
    Zcl.push_back(ZCL);
		FWHM.push_back(v[0]);

		delete Time;

  }

  std::vector<Float_t> Frac_scanned;
	std::vector<Int_t> Del_scanned;
  //Number of different Fraction
	for (size_t i = 0; i < Frac.size(); i++) {
		if (Zcl[i]==0){
			if (Frac_scanned.size()==0) Frac_scanned.push_back(Frac[i]);
			int k = 0;
			for (size_t j = 0; j < Frac_scanned.size(); j++) {
				if (Frac_scanned[j]==Frac[i]) k++;
			}
			if(k == 0){
				Frac_scanned.push_back(Frac[i]);
				//cout<<Frac[i]<<endl;                                        //debug
			}

		}
	}
	//Number of different Delay
	for (size_t i = 0; i < Del.size(); i++) {
		if (Zcl[i]==0){
			if (Del_scanned.size()==0) Del_scanned.push_back(Del[i]);
			int k = 0;
			for (size_t j = 0; j < Del_scanned.size(); j++) {
				if (Del_scanned[j]==Del[i]) k++;
			}
			if(k == 0){
				Del_scanned.push_back(Del[i]);
				//cout<<Del[i]<<endl;                                        //debug
			}

		}
	}

	TRandom *color = new TRandom();
	//FWHMvsDelay
  std::vector<TGraph *> FWHMvsDelay;
	for (size_t i = 0; i < Frac_scanned.size(); i++) {
		cout<<"fraction set------>"<<Frac_scanned[i]<<endl;  //debug
		FWHMvsDelay.push_back(new TGraph(Del_scanned.size()));
		int j = 0;
		for (size_t k = 0; k < Frac.size(); k++) {
			if (Zcl[k]==0 && Frac[k]==Frac_scanned[i]){
				FWHMvsDelay[i]->SetPoint(j,Del[k],FWHM[k]);
				j++;
				//cout<<Del[k]<<" "<<FWHM[k]<<endl;           //debug
			}
		}

		FWHMvsDelay[i]->Sort();
		FWHMvsDelay[i]->SetTitle(Form("Fraction %.2f",Frac_scanned[i]));
		FWHMvsDelay[i]->SetLineColor(color->Integer(9)+1);
	}

	TMultiGraph *FWHMvsDelay_mg = new TMultiGraph();
	TCanvas * FWHMvsDelay_Canvas = new TCanvas("FWHMvsDelay_Canvas", "FWHM as function of Delay",1280,720);
	FWHMvsDelay_Canvas->cd();
	for (size_t i = 0; i < FWHMvsDelay.size(); i++) {
		FWHMvsDelay_mg->Add(FWHMvsDelay[i]);
	}
	FWHMvsDelay_mg->Draw("A*L");
	FWHMvsDelay_Canvas->Update();

	//FWHMvsFraction
  std::vector<TGraph *> FWHMvsFraction;
	for (size_t i = 0; i < Del_scanned.size(); i++) {
		cout<<"Delay set------>"<<Del_scanned[i]<<endl;  //debug
		FWHMvsFraction.push_back(new TGraph(Frac_scanned.size()));
		int j = 0;
		for (size_t k = 0; k < Del.size(); k++) {
			if (Zcl[k]==0 && Del[k]==Del_scanned[i]){
				FWHMvsFraction[i]->SetPoint(j,Frac[k],FWHM[k]);
				j++;
				//cout<<Frac[k]<<" "<<FWHM[k]<<endl;           //debug
			}
		}

		FWHMvsFraction[i]->Sort();
		FWHMvsFraction[i]->SetTitle(Form("Delay %i ns",Del_scanned[i]));
		FWHMvsFraction[i]->SetLineColor(color->Integer(9)+1);
	}

	TMultiGraph *FWHMvsFraction_mg = new TMultiGraph();
	TCanvas * FWHMvsFraction_Canvas = new TCanvas("FWHMvsFraction_Canvas", "FWHM as function of Fraction",1280,720);
	FWHMvsFraction_Canvas->cd();
	for (size_t i = 0; i < FWHMvsFraction.size(); i++) {
		FWHMvsFraction_mg->Add(FWHMvsFraction[i]);
	}
	FWHMvsFraction_mg->Draw("A*L");
	FWHMvsFraction_Canvas->Update();

	//2-D surface plot
	Float_t Frac_step  = Frac_scanned[1]-Frac_scanned[0];
	Float_t Frac_start = Frac_scanned[0]-0.5*Frac_step;
	Float_t Frac_stop  = Frac_scanned[Frac_scanned.size()-1]+0.5*Frac_step;

	Float_t Del_step  = Del_scanned[1] -Del_scanned[0];
	Float_t Del_start = Del_scanned[0]-0.5*Del_step;
	Float_t Del_stop  = Del_scanned[Del_scanned.size()-1]+0.5*Del_step;

	TCanvas * FWHM_surf = new TCanvas("FWHM_surf", "FWHM surface plot",1280,720);

	TH2D * FWHM_opt = new TH2D("surface plot", "2-D fraction vs delay",Frac_scanned.size(),Frac_start,Frac_stop,Del_scanned.size(),Del_start,Del_stop);

	for (size_t i = 0; i < Del_scanned.size(); i++) {

		for (size_t k = 0; k < Frac.size(); k++) {
			if (Zcl[k]==0 && Del[k]==Del_scanned[i]){
				FWHM_opt->Fill(Frac[k],Del[k],FWHM[k]);
				cout<<Frac[k]<<" "<<Del[k]<<" "<<FWHM[k]<<endl;           //debug
			}
		}
	}
  FWHM_surf->cd();
	FWHM_opt->Draw("COLZ");
	FWHM_surf->Update();
	return ;
}
