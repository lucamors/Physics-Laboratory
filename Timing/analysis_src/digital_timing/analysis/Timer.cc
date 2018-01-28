
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

std::vector<Double_t> Fitter(TH1F * histo, Double_t range, TCanvas * c1, int pad)
{
	const double sqrt2ln2 = 1.17741002251547;
	c1->cd(pad);

	// TSpectrum Peak Search

	TSpectrum *search_peak = new TSpectrum(1);

	int found_peak = search_peak->Search(histo, 1, "", 0.05);

	// Retrieving X positions and print them out to std output

	Double_t  x_peak = search_peak->GetPositionX()[0];
	Double_t  y_peak = search_peak->GetPositionY()[0];
	Double_t peakmax=x_peak;
	Double_t peakheight= y_peak;

	// FWHM parameters declaration
	double max = histo->GetBinCenter(histo->GetSize()-2);
  double min = histo->GetBinCenter(0);
  histo->GetXaxis()->SetLimits(min-peakmax,max-peakmax);
  histo->Draw();


	TF1 *GausPeak = new TF1("GPeak","gaus", -range, range);
	GausPeak->SetParameter(0,peakheight);

  histo->Fit(GausPeak,"R+");

	x_peak         = GausPeak->GetParameter(1);
	y_peak         = GausPeak->GetParameter(0);
	Double_t Sigma = GausPeak->GetParameter(2);
	Double_t SigmaSig = GausPeak->GetParError(2);
	Double_t FWHM     = 2*sqrt2ln2*Sigma;
	Double_t FWHMsig  = 2*sqrt2ln2*SigmaSig;

	std::vector<Double_t> FitResult;
  FitResult.push_back(FWHM);
  FitResult.push_back(FWHMsig);


  delete GausPeak;
	delete search_peak;
	return FitResult;

}

void Timer(string filename)
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



	std::vector<Double_t> FWHM;
  std::vector<Double_t> FWHMsig;


    branch->GetEntry(0);

		TH2F * TACVsAmp0 = new TH2F("TAC vs Amplitude 0", "TAC vs Amplitude 0", 1000, 0, 500, 1000, -5, 10);
		TH1F * TAC_Gate_0 = new TH1F("Energy 0-50 keV"   ,"Energy 0-50 keV"    ,1000, -5, 10);
    TH1F * TAC_Gate_1 = new TH1F("Energy 50-100 keV" ,"Energy 50-100 keV"  ,1000, -5, 10);
    TH1F * TAC_Gate_2 = new TH1F("Energy 100-150 keV","Energy 100-150 keV" ,1000, -5, 10);
    TH1F * TAC_Gate_3 = new TH1F("Energy 150-200 keV","Energy 150-200 keV" ,1000, -5, 10);
    TH1F * TAC_Gate_4 = new TH1F("Energy 200-250 keV","Energy 200-250 keV" ,1000, -5, 10);
		TH1F * TAC_Gate_5 = new TH1F("Energy 250-300 keV","Energy 250-300 keV" ,1000, -5, 10);

		TAC_Gate_0->GetXaxis()->SetTitle("Timing [ns]");
		TAC_Gate_0->GetYaxis()->SetTitle("Count");
		TAC_Gate_1->GetXaxis()->SetTitle("Timing [ns]");
		TAC_Gate_1->GetYaxis()->SetTitle("Count");
		TAC_Gate_2->GetXaxis()->SetTitle("Timing [ns]");
		TAC_Gate_2->GetYaxis()->SetTitle("Count");
		TAC_Gate_3->GetXaxis()->SetTitle("Timing [ns]");
		TAC_Gate_3->GetYaxis()->SetTitle("Count");
		TAC_Gate_4->GetXaxis()->SetTitle("Timing [ns]");
		TAC_Gate_4->GetYaxis()->SetTitle("Count");
		TAC_Gate_5->GetXaxis()->SetTitle("Timing [ns]");
		TAC_Gate_5->GetYaxis()->SetTitle("Count");

		Double_t Energy_0=0;
		Double_t TAC=0;

    for (size_t j = 0; j < opt_sett->TimeA.size() ; j++)
		{
			Energy_0=opt_sett->Energy[j];
			TAC=opt_sett->TimeA[j]-opt_sett->TimeB[j];
			TACVsAmp0->Fill(Energy_0,TAC);
			if(Energy_0 > 0   && Energy_0 <= 50 ) TAC_Gate_0->Fill(TAC);
      if(Energy_0 > 50 && Energy_0 <= 100 ) TAC_Gate_1->Fill(TAC);
      if(Energy_0 > 100 && Energy_0 <= 150 ) TAC_Gate_2->Fill(TAC);
      if(Energy_0 > 150 && Energy_0 <= 200 ) TAC_Gate_3->Fill(TAC);
      if(Energy_0 > 200 && Energy_0 <= 250) TAC_Gate_4->Fill(TAC);
			if(Energy_0 > 250 && Energy_0 <= 300) TAC_Gate_5->Fill(TAC);
		}

		TACVsAmp0->Draw("COLZ");

		TCanvas * c0 = new TCanvas("c0","c0", 1920, 1080);
		c0->Divide(3,2);
		std::vector<Double_t> my_vec_0 = Fitter(TAC_Gate_0, 0.6, c0, 1);
		std::vector<Double_t> my_vec_1 = Fitter(TAC_Gate_1, 0.5, c0, 2);
		std::vector<Double_t> my_vec_2 = Fitter(TAC_Gate_2, 0.4, c0, 3);
		std::vector<Double_t> my_vec_3 = Fitter(TAC_Gate_3, 0.5, c0, 4);
		std::vector<Double_t> my_vec_4 = Fitter(TAC_Gate_4, 0.5, c0, 5);
		std::vector<Double_t> my_vec_5 = Fitter(TAC_Gate_5, 0.5, c0, 6);

		c0->Update();
		ofstream temp("temp.dat");

		temp<<"25 "<<my_vec_0[0]<<" "<<my_vec_0[1]<<endl;
    temp<<"75 "<<my_vec_1[0]<<" "<<my_vec_1[1]<<endl;
    temp<<"125 "<<my_vec_2[0]<<" "<<my_vec_2[1]<<endl;
    temp<<"175 "<<my_vec_3[0]<<" "<<my_vec_3[1]<<endl;
    temp<<"225 "<<my_vec_4[0]<<" "<<my_vec_4[1]<<endl;
		temp<<"275 "<<my_vec_5[0]<<" "<<my_vec_5[1]<<endl;

		temp.close();


				TCanvas *C = new TCanvas("c","c",1280,1080);

		    TPad* upperPad = new TPad("upperPad", "upperPad", .005, .700, .995, .995);
		    TPad* lowerPad = new TPad("lowerPad", "lowerPad", .005, .005, .995, .700);
		    upperPad->Draw();
		    lowerPad->Draw();
		    Float_t small = 1e-4;
		    lowerPad->SetTopMargin(small);
		    upperPad->SetBottomMargin(small);

		    lowerPad->SetGridx();
		    upperPad->SetGridx();

		    lowerPad->SetTickx(1);
		    upperPad->cd();


		   TGraphErrors *Graph = new TGraphErrors("temp.dat","%lg%lg%lg");
		   Graph->GetXaxis()->SetTitle("Energy [KeV]");
		   Graph->GetYaxis()->SetTitle("FWHM [ns]");
		   Graph->GetXaxis()->SetLimits(0,500);
		   Graph->GetYaxis()->SetLimits(0.53,0.67);

		   Graph->GetYaxis()->SetTitleSize(0.06);
		   Graph->GetYaxis()->SetTitleOffset(0.8);
		   Graph->GetYaxis()->SetLabelSize(0.05);
		   Graph->GetYaxis()->SetNdivisions(5);

		   Graph->GetXaxis()->SetNdivisions(14);

		   Graph->GetXaxis()->SetTitleSize(0.06);
		   Graph->GetXaxis()->SetLabelSize(0.04);
		   Graph->GetXaxis()->SetTitleOffset(0.75);

		   Graph->SetTitle("");

		   Graph->SetFillStyle(0);
		   Graph->SetLineColor(kBlue);
		   Graph->SetMarkerStyle(20);
		   Graph->SetMarkerColor(kRed);
		   Graph->SetLineWidth(2);
		   Graph->Draw();

		   lowerPad->cd();

			 gStyle->SetOptStat(false);
			 double max = TACVsAmp0->GetYaxis()->GetBinCenter(1000);
			 double min = TACVsAmp0->GetYaxis()->GetBinCenter(0);
			 TACVsAmp0->GetYaxis()->SetLimits(min-19.5,max-19.5);
			 TACVsAmp0->GetYaxis()->SetRangeUser(-7,7);
		   TACVsAmp0->GetXaxis()->SetNdivisions(14);
		   //TACVsAmp0->SetOptStat(kFALSE);
		   TACVsAmp0->SetTitle("");
		   TACVsAmp0->Draw("COLZ");

	return ;
}
