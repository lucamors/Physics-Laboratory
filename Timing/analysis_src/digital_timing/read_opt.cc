
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

  TNtupleD *nt = new TNtupleD("FWHM result","FWHM","Frac:Del:ZCL:FWHM:FWHMsig");
	TH1F * Time = 0;

  for (size_t i = 0; i < n_ev; i++)
  {
    branch->GetEntry(i);

		//get parameters
		float Fraction = opt_sett->factor;
		int   Delay    = opt_sett->delay;
		int   ZCL      = opt_sett->zcl;

		Time = new TH1F("Time", Form("Frac%.2f_Del%i_Zcl%i_Timing", Fraction, Delay, ZCL), 1000, -10, 10);


    for (size_t j = 0; j < opt_sett->TimeA.size() ; j++)
		{
			Time->Fill(opt_sett->TimeA[j]-opt_sett->TimeB[j]);

		}

		std::vector<Double_t> v = Fitter(Time,0.55);
		nt->Fill(Fraction,Delay,ZCL,v[0],v[1]);

  }

  TFile *OutFile=new TFile("FWHMntuple.root", "RECREATE");

	nt->Print();

	TCanvas * c = new TCanvas("c","Fraction vs FWHM",1280,720);
	c->cd();
	nt->Draw("Frac:FWHM");
	c->Update();
	nt->Write();
	OutFile->Write();
	OutFile->Close();

  return ;
}
