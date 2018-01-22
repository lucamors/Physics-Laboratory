/*
*
* Analog Timing Macro.
* Fitting TAC spectrum with a gaussian with exponential tails.
*/

#include <string>
#include <vector>


struct acqEventPSD_t {

	ULong64_t	timetag;
	UInt_t		baseline;
	UShort_t	qshort;
	UShort_t	qlong;
	UShort_t	pur;
	UShort_t	samples[4096];
};


struct psd_params_t {
	UInt_t		channel;
	UInt_t		threshold;
	UInt_t		pretrigger;
	UInt_t		pregate;
	UInt_t		shortgate;
	UInt_t		longgate;
	UInt_t		numsamples;
};

// Declaring Fitting function
double peak(double *x, double *par)
{
       double xx= x[0];
       double gaus = par[0]*exp(-0.5*(xx-par[1])*(xx-par[1])/(par[2]*par[2]));
       double expl = par[0]*exp(((par[3]*par[3])/2)+(par[3]*((xx-par[1])/par[2])));
       double expr = par[0]*exp(((par[4]*par[4])/2)-(par[4]*((xx-par[1])/par[2])));
       if (xx<par[1]-par[3]*par[2]){return expl;}
       else if(xx>par[1]+par[4]*par[2]){return expr;}
       else {return gaus;}
}

std::vector<Double_t> Fitter(TH1F * histo, Double_t range, TCanvas * c1, int pad)
{
	const double sqrt2ln2 = 1.17741002251547;
	c1->cd(pad);

	// TSpectrum Peak Search

	TSpectrum *search_peak = new TSpectrum(1);

	int found_peak = search_peak->Search(histo, 1, "", 0.05);

	// Retrieving X positions and print them out to std output

	Double_t * x_peak = search_peak->GetPositionX();
	Double_t * y_peak = search_peak->GetPositionY();
	Double_t peakmax=x_peak[0];
	Double_t peakheight= y_peak[0];

	// FWHM parameters declaration
	double max = histo->GetBinCenter(histo->GetSize()-2);
  double min = histo->GetBinCenter(0);
  histo->GetXaxis()->SetLimits(min-peakmax,max-peakmax);

	peakmax=0;

	Double_t par0 = 0;
	Double_t par1 = 0;
	Double_t par2 = 0;
	Double_t par3 = 0;
	Double_t par4 = 0;
	Double_t par2sig = 0;
	Double_t par3sig = 0;
	Double_t par4sig = 0;
	Double_t sig23 = 0;
	Double_t sig24 = 0;
	Double_t sig43 = 0;
	Double_t hwhm1, hwhm2, FWHM;
	Double_t FWHMsig=0;
	int checkR=0, checkL=0;

	TF1 *GausPeak = new TF1("GPeak","gaus", peakmax-0.4, peakmax+0.3);
	GausPeak->SetParameter(0,peakheight);
	GausPeak->SetParLimits(0,peakheight-2*peakheight/100,peakheight+2*peakheight/100);
	GausPeak->SetParameter(1,peakmax);

  histo->Fit(GausPeak,"R+");

	peakmax        = GausPeak->GetParameter(1);
	peakheight     = GausPeak->GetParameter(0);
	Double_t Sigma = GausPeak->GetParameter(2);
	// Setting Fit parameters

	TF1 *fpeak = new TF1("fpeak",peak, peakmax-range, peakmax+range, 5);
	fpeak->SetParameter(0,peakheight);
	fpeak->SetParameter(1,peakmax);
	fpeak->SetParameter(2,Sigma);
	//fpeak->SetParameter(3,0);
	//fpeak->SetParameter(4,0);

	// Fitting
	TFitResultPtr res=histo->Fit(fpeak,"RSNME");
	cout<<"Status fit = "<<res->Status()<<endl;

	// Retrieving Fit Parameters
	par0 = fpeak->GetParameter(0);
	par1 = fpeak->GetParameter(1);
	par2 = fpeak->GetParameter(2);
	par3 = fpeak->GetParameter(3);
	par4 = fpeak->GetParameter(4);

	par2sig = fpeak->GetParError(2);
	par3sig = fpeak->GetParError(3);
	par4sig = fpeak->GetParError(4);

	sig23=res->CovMatrix(2,3);
	sig24=res->CovMatrix(2,4);
	sig43=res->CovMatrix(4,3);

	if((par1-par2*sqrt2ln2)<par1-(par3*par2)){hwhm1=par1-par2*((0.5*par3)+(0.6931471806/par3));}
	   else {hwhm1=par1-par2*sqrt2ln2; checkR=1;}

	if((par1+par2*sqrt2ln2)>par1+(par4*par2)){hwhm2=par1+par2*((0.5*par4)+(0.6931471806/par4));}
	   else {hwhm2=par1+par2*sqrt2ln2; checkL=1;}

	fpeak->SetFillColor(19);
	fpeak->SetFillStyle(1);
	fpeak->SetLineWidth(2);
	histo->GetXaxis()->SetRangeUser(peakmax-3,peakmax+3);
	histo->Draw();
	GausPeak->SetLineColor(kGreen);
	GausPeak->Draw("SAME");
	fpeak->Draw("SAME");
	TArrow *ar3 = new TArrow(hwhm1,peakheight/2.0,hwhm2,peakheight/2.0,0.005,"<|>");
   //ar3->SetLineWidth(1);
  ar3->Draw();

  FWHM=hwhm2-hwhm1;

	// FWHM computation


	if(checkR==0 && checkL==0) FWHMsig=sqrt((((FWHM*par2sig)/par2)*((FWHM*par2sig)/par2))+((par3sig*par2*(0.5-(0.6931471806/(par3*par3))))*(par3sig*par2*(0.5-(0.6931471806/(par3*par3)))))+((par4sig*par2*(0.5-(0.6931471806/(par4*par4))))*(par4sig*par2*(0.5-(0.6931471806/(par4*par4)))))+2*(FWHM/par2)*(par2*(0.5-(0.6931471806/(par3*par3))))*sig23+2*(FWHM/par2)*(par2*(0.5-(0.6931471806/(par4*par4))))*sig24+2*(par2*(0.5-(0.6931471806/(par4*par4))))*(par2*(0.5-(0.6931471806/(par3*par3))))*sig43);
	if(checkR==0 && checkL==1) FWHMsig=sqrt((((FWHM*par2sig)/par2)*((FWHM*par2sig)/par2))+((par3sig*par2*(0.5-(0.6931471806/(par3*par3))))*(par3sig*par2*(0.5-(0.6931471806/(par3*par3)))))+2*(FWHM/par2)*(par2*(0.5-(0.6931471806/(par3*par3))))*sig23);
	if(checkR==1 && checkL==0) FWHMsig=sqrt((((FWHM*par2sig)/par2)*((FWHM*par2sig)/par2))+((par4sig*par2*(0.5-(0.6931471806/(par4*par4))))*(par4sig*par2*(0.5-(0.6931471806/(par4*par4)))))+2*(FWHM/par2)*(par2*(0.5-(0.6931471806/(par4*par4))))*sig24);
	if(checkR==1 && checkL==1) FWHMsig=(FWHM*par2sig)/par2;
  std::cout << "FWHM" <<FWHM<< '\n';
	// Store the FWHMs
	std::vector<Double_t> FitResult;
  FitResult.push_back(FWHM);
  FitResult.push_back(FWHMsig);

	return FitResult;

}
void Timer(string infile_name)
{
  // Retrieving data

  	acqEventPSD_t inc_data_ch0;
  	acqEventPSD_t inc_data_ch1;
  	acqEventPSD_t inc_data_ch2;
  	acqEventPSD_t inc_data_ch3;


  	TFile * incfile = new TFile(infile_name.c_str());

  	TTree *inctree = (TTree*) incfile->Get("acq_tree_0");

  	TBranch * incbranch_0;
  	TBranch * incbranch_1;
  	TBranch * incbranch_2;
  	TBranch * incbranch_3;

  	// Detector #1
  	incbranch_0 = inctree->GetBranch("acq_ch0");
  	incbranch_0->SetAddress(&inc_data_ch0.timetag);

  	// Detector #2
  	incbranch_1 = inctree->GetBranch("acq_ch1");
  	incbranch_1->SetAddress(&inc_data_ch1.timetag);

  	// TAC
  	incbranch_2 = inctree->GetBranch("acq_ch2");
  	incbranch_2->SetAddress(&inc_data_ch2.timetag);

  	// CH3 - empty channel
  	// incbranch_3 = inctree->GetBranch("acq_ch4");
  	// incbranch_3->SetAddress(&inc_data_ch3.timetag);


  	psd_params_t params;
  	TBranch *config = inctree->GetBranch("psd_params");
  	config->SetAddress(&params.channel);
  	config->GetEntry(0);


  	long int tot_event = incbranch_2->GetEntries();
  	// long int tot_event_ch1 = incbranch_1->GetEntries();
  	// long int tot_event_ch2 = incbranch_2->GetEntries();
  	// long int tot_event_ch3 = incbranch_3->GetEntries();

  	long int counter = 0;

  	// 2-D plot Timing vs Energy
  	TH2F * TACVsAmp0 = new TH2F("TAC vs Amplitude 0", "TAC vs Amplitude 0", 1000, 0, 1400, 1000, 0, 30);
    TH2F * TACVsAmp1 = new TH2F("TAC vs Amplitude 1", "TAC vs Amplitude 1", 1000, 0, 1400, 1000, 0, 30);

    TACVsAmp0->GetXaxis()->SetTitle("Energy [keV]");
  	TACVsAmp0->GetYaxis()->SetTitle("Analog Timing [ns]");

    TACVsAmp1->GetXaxis()->SetTitle("Energy [keV]");
  	TACVsAmp1->GetYaxis()->SetTitle("Analog Timing [ns]");

    //Timing resolution as function of energy
    TH1F * TAC_Gate_0 = new TH1F("Energy 100-200 keV"   ,"Energy 100-200 keV"   ,1000, 0, 30);
    TH1F * TAC_Gate_1 = new TH1F("Energy 200-300 keV" ,"Energy 200-300 keV" ,1000, 0, 30);
    TH1F * TAC_Gate_2 = new TH1F("Energy 300-400 keV" ,"Energy 300-400 keV" ,1000, 0, 30);
    TH1F * TAC_Gate_3 = new TH1F("Energy 400-500 keV" ,"Energy 400-500 keV" ,1000, 0, 30);
    TH1F * TAC_Gate_4 = new TH1F("Energy 500-600 keV","Energy 500-600 keV",1000, 0, 30);
		TH1F * TAC_Gate_5 = new TH1F("Energy 600-700 keV","Energy 600-700 keV",1000, 0, 30);
    TH1F * TAC_Gate_6 = new TH1F("Energy 700-800 keV","Energy 700-800 keV",1000, 0, 30);
    TH1F * TAC_Gate_7 = new TH1F("Energy 800-900 keV","Energy 800-900 keV",1000, 0, 30);
    TH1F * TAC_Gate_8 = new TH1F("Energy 900-1000 keV","Energy 900-1000 keV",1000, 0, 30);
    TH1F * TAC_Gate_9 = new TH1F("Energy 1000-1100 keV","Energy 1000-1100 keV",1000, 0, 30);

    TAC_Gate_0->GetXaxis()->SetTitle("TAC [ns]");
  	TAC_Gate_0->GetYaxis()->SetTitle("Count");
    TAC_Gate_1->GetXaxis()->SetTitle("TAC [ns]");
  	TAC_Gate_1->GetYaxis()->SetTitle("Count");
    TAC_Gate_2->GetXaxis()->SetTitle("TAC [ns]");
  	TAC_Gate_2->GetYaxis()->SetTitle("Count");
    TAC_Gate_3->GetXaxis()->SetTitle("TAC [ns]");
  	TAC_Gate_3->GetYaxis()->SetTitle("Count");
    TAC_Gate_4->GetXaxis()->SetTitle("TAC [ns]");
  	TAC_Gate_4->GetYaxis()->SetTitle("Count");
		TAC_Gate_5->GetXaxis()->SetTitle("TAC [ns]");
		TAC_Gate_5->GetYaxis()->SetTitle("Count");
    TAC_Gate_6->GetXaxis()->SetTitle("TAC [ns]");
    TAC_Gate_6->GetYaxis()->SetTitle("Count");
    TAC_Gate_7->GetXaxis()->SetTitle("TAC [ns]");
    TAC_Gate_7->GetYaxis()->SetTitle("Count");
    TAC_Gate_8->GetXaxis()->SetTitle("TAC [ns]");
    TAC_Gate_8->GetYaxis()->SetTitle("Count");
    TAC_Gate_9->GetXaxis()->SetTitle("TAC [ns]");
    TAC_Gate_9->GetYaxis()->SetTitle("Count");

	  TAC_Gate_0->SetStats(kFALSE);
    TAC_Gate_1->SetStats(kFALSE);
    TAC_Gate_2->SetStats(kFALSE);
    TAC_Gate_3->SetStats(kFALSE);
    TAC_Gate_4->SetStats(kFALSE);
    TAC_Gate_5->SetStats(kFALSE);
    TAC_Gate_6->SetStats(kFALSE);
    TAC_Gate_7->SetStats(kFALSE);
    TAC_Gate_8->SetStats(kFALSE);
    TAC_Gate_9->SetStats(kFALSE);

		Double_t TAC      = 0;
    Double_t Energy_0 = 0;
    Double_t Energy_1 = 0;
  	// Scanning over Branches
  	while( counter < tot_event)
  	{

  		incbranch_0->GetEntry(counter);
      incbranch_1->GetEntry(counter);
      incbranch_2->GetEntry(counter);

      TAC      = inc_data_ch2.qlong*0.000735898;         //calibrated
      Energy_0 = inc_data_ch0.qlong*0.0754713-49.1682;   //calibrated
      Energy_1 = inc_data_ch1.qlong*0.0824706-17.8629;   //calibrated

  		TACVsAmp0->Fill(Energy_0, TAC);
      TACVsAmp1->Fill(Energy_1, TAC);

      if(Energy_0 > 100   && Energy_0 <= 200 ) TAC_Gate_0->Fill(TAC);
      if(Energy_0 > 200 && Energy_0 <= 300 ) TAC_Gate_1->Fill(TAC);
      if(Energy_0 > 300 && Energy_0 <= 400 ) TAC_Gate_2->Fill(TAC);
      if(Energy_0 > 400 && Energy_0 <= 500 ) TAC_Gate_3->Fill(TAC);
      if(Energy_0 > 500 && Energy_0 <= 600) TAC_Gate_4->Fill(TAC);
			if(Energy_0 > 600 && Energy_0 <= 700) TAC_Gate_5->Fill(TAC);
      if(Energy_0 > 700 && Energy_0 <= 800) TAC_Gate_6->Fill(TAC);
      if(Energy_0 > 800 && Energy_0 <= 900) TAC_Gate_7->Fill(TAC);
      if(Energy_0 > 900 && Energy_0 <= 1000) TAC_Gate_8->Fill(TAC);
      if(Energy_0 > 1000 && Energy_0 <= 1100) TAC_Gate_9->Fill(TAC);

  		counter++;

  	}

		TCanvas * c0 = new TCanvas("c0","c0", 1920, 1080);
		c0->Divide(5,2);

		std::vector<Double_t> my_vec_0 = Fitter(TAC_Gate_0, 0.6, c0, 1);
		std::vector<Double_t> my_vec_1 = Fitter(TAC_Gate_1, 0.5, c0, 2);
		std::vector<Double_t> my_vec_2 = Fitter(TAC_Gate_2, 0.4, c0, 3);
		std::vector<Double_t> my_vec_3 = Fitter(TAC_Gate_3, 0.5, c0, 4);
		std::vector<Double_t> my_vec_4 = Fitter(TAC_Gate_4, 0.5, c0, 5);
    std::vector<Double_t> my_vec_5 = Fitter(TAC_Gate_5, 0.5, c0, 6);
    std::vector<Double_t> my_vec_6 = Fitter(TAC_Gate_6, 0.5, c0, 7);
    std::vector<Double_t> my_vec_7 = Fitter(TAC_Gate_7, 0.5, c0, 8);
    std::vector<Double_t> my_vec_8 = Fitter(TAC_Gate_8, 0.5, c0, 9);
    std::vector<Double_t> my_vec_9 = Fitter(TAC_Gate_9, 0.5, c0, 10);

    c0->Update();
		ofstream temp("temp.dat");

		temp<<"150 "<<my_vec_0[0]<<" "<<my_vec_0[1]<<endl;
    temp<<"250 "<<my_vec_1[0]<<" "<<my_vec_1[1]<<endl;
    temp<<"350 "<<my_vec_2[0]<<" "<<my_vec_2[1]<<endl;
    temp<<"450 "<<my_vec_3[0]<<" "<<my_vec_3[1]<<endl;
    temp<<"550 "<<my_vec_4[0]<<" "<<my_vec_4[1]<<endl;
		temp<<"650 "<<my_vec_5[0]<<" "<<my_vec_5[1]<<endl;
    temp<<"750 "<<my_vec_6[0]<<" "<<my_vec_6[1]<<endl;
    temp<<"850 "<<my_vec_7[0]<<" "<<my_vec_7[1]<<endl;
    temp<<"950 "<<my_vec_8[0]<<" "<<my_vec_8[1]<<endl;
    temp<<"1050 "<<my_vec_9[0]<<" "<<my_vec_9[1]<<endl;


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
   Graph->GetXaxis()->SetLimits(0,1400);
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
