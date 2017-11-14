/*
*
* Reading TTree from CAEN Digitizer
* Channel aligning
*/

struct acqEventPSD_t {

	ULong64_t	timetag;
	UInt_t		baseline;
	UShort_t	qshort;
	UShort_t	qlong;
	UShort_t	pur;
	UShort_t	samples[4096];
};

// Declaring Fitting function
double peak(double *x, double *par){
       double xx= x[0];
       double gaus = par[0]*exp(-0.5*(xx-par[1])*(xx-par[1])/(par[2]*par[2]));
       double expl = par[0]*exp(((par[3]*par[3])/2)+(par[3]*((xx-par[1])/par[2])));
       double expr = par[0]*exp(((par[4]*par[4])/2)-(par[4]*((xx-par[1])/par[2])));
       if (xx<par[1]-par[3]*par[2]){return expl;}
       else if(xx>par[1]+par[4]*par[2]){return expr;}
       else {return gaus;}
}
// Defining Fitting algorithm
std::vector<Double_t> Fitter(TH1F * histo, Double_t range, TCanvas * c1){
	const double sqrt2ln2 = 1.17741002251547;
	c1->cd();

	// TSpectrum Peak Search

	TSpectrum *search_peak = new TSpectrum(1);

	int found_peak = search_peak->Search(histo, 1, "", 0.05);

	// Retrieving X positions and print them out to std output

	Double_t * x_peak = search_peak->GetPositionX();
	Double_t * y_peak = search_peak->GetPositionY();

	// FWHM parameters declaration

	Double_t peakmax=x_peak[0];
	Double_t peakheight= y_peak[0];
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

	TF1 *GausPeak = new TF1("GPeak","gaus", peakmax-range, peakmax+range);
	GausPeak->SetParameter(0,peakheight);
	GausPeak->SetParLimits(0,peakheight-6*peakheight/100,peakheight+1*peakheight/100);
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
	fpeak->SetParameter(3,0.001);
	fpeak->SetParameter(4,0.001);

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

	// FWHM computation

	FWHM=hwhm2-hwhm1;
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
// Defining Digital Timing algorithm
double TimingC2(double *data, int Buffer, double Fact, int delay, double Zcline, double BaseLine, int bsRange, TH1F *bipolar, int draw){

	double zcof[Buffer];
	for(int i=0;i<Buffer;i++){zcof[i]=0.0;}
    int Go20=0;

    // take now a proportional of the standard deviation
    double zcoth;
    double zcosigma=0;
    double shapth = 150;
    double shapsize = 0.0;
    int bs=0;

            for(int i=delay;i<Buffer;i++){

                zcof[i]=(Fact*(data[i]-BaseLine)-(data[i-delay]-BaseLine));
                if((i>10)&&(i<bsRange)){zcosigma+=zcof[i]*zcof[i];bs++;}
                if(i==bsRange) zcosigma=sqrt(zcosigma/bs);
                bipolar->SetBinContent(i,zcof[i]+BaseLine);
            }

                  zcoth=4.0*zcosigma;
            int HalfPassBin=10;

            for(int i=bsRange ;i<Buffer;i++){
				if(Go20==1){
                    shapsize=zcof[i-3]+2.*zcof[i-2]+3.*zcof[i-1]-3.*zcof[i+1]-2.*zcof[i+2]-zcof[i+3];
                    if((zcof[i]<Zcline)&&(shapsize>shapth)){HalfPassBin=i; break;}
                    }
                if(Go20==0 && zcof[i]>zcoth){Go20=1;}
            }
            //***********************************************************************
              double IntData[4];

            IntData[0]= (1.*zcof[HalfPassBin-3] - 8.*zcof[HalfPassBin-2] + 19.*zcof[HalfPassBin-1] - 19.*zcof[HalfPassBin] + 8.*zcof[HalfPassBin+1] - 1.*zcof[HalfPassBin+2])/18.0;
            IntData[1]= (-4.*zcof[HalfPassBin-3] + 32.*zcof[HalfPassBin-2] - 58.*zcof[HalfPassBin-1] + 37.*zcof[HalfPassBin] - 8.*zcof[HalfPassBin+1] + 1.*zcof[HalfPassBin+2])/30.0;
            IntData[2]= (7.*zcof[HalfPassBin-3] - 56.*zcof[HalfPassBin-2] - 11.*zcof[HalfPassBin-1] + 74.*zcof[HalfPassBin] - 16.*zcof[HalfPassBin+1] + 2.*zcof[HalfPassBin+2])/90.0;
            IntData[3]= zcof[HalfPassBin-1];
            char MyPlo3[200];
            sprintf(MyPlo3,"[0]*(x-%i)**3+[1]*(x-%i)**2+[2]*(x-%i)+[3]+%4.10f",HalfPassBin-1,HalfPassBin-1,HalfPassBin-1,BaseLine);

            TF1 *f2 = new TF1("f2",MyPlo3, HalfPassBin-1, HalfPassBin);
            f2->SetParameter(0,IntData[0]);
            f2->SetParameter(1,IntData[1]);
            f2->SetParameter(2,IntData[2]);
            f2->SetParameter(3,IntData[3]);
            f2->SetFillColor(19);
            f2->SetFillStyle(1);

            f2->SetLineWidth(1);
            //*************************************************************************

            double Time=f2->GetX(BaseLine+Zcline,(double)(HalfPassBin-1),(double)HalfPassBin);
						if(draw)std::cout<<"HalfPassBin-1:"<<HalfPassBin-1<<" zcosigma:"<<zcosigma<<" shapsize:"<<shapsize<<" Time:"<<Time<<std::endl;
            delete f2;
            return Time;
}

void read_CAEN(string input_file_name, int Draw)
{
  TFile * input_file = new TFile(input_file_name.c_str());
  TTree * daq_tree;
  // Retrieving TTree from TFile
  daq_tree = (TTree*) input_file->Get("acq_tree_0");

  // Linking

  acqEventPSD_t event_ch_A, event_ch_B;

  TBranch * branch_ch_A, * branch_ch_B;
  branch_ch_A = daq_tree->GetBranch(Form("acq_ch%i",0));
  branch_ch_A->SetAddress(&event_ch_A.timetag);
  branch_ch_B = daq_tree->GetBranch(Form("acq_ch%i",1));
  branch_ch_B->SetAddress(&event_ch_B.timetag);


  // Align channels

  long int nEventsA = (long int) branch_ch_A->GetEntries();
  long int nEventsB = (long int) branch_ch_B->GetEntries();

  int counter_A = 0;
  int skipA = 0;
  int counter_B = 0;
  int skipB = 0;
  int coincident_events = 0;

  int id_A = 0;
  int id_B = 0;

  long int time_difference = 0;

  // Defining waveform array and timing variables
	int Buffer = 350;
	int     Th = 10;
	int     bs = 0;
	int Thresh_ch_A, Thresh_ch_B;

	int bsmin   = 10;
	int bsRange = 75;

	double    bsvar_ch_A = 0.0;
	 double    Time_ch_A = 0.0;
	double BaseLine_ch_A = 0.0;

	double    bsvar_ch_B = 0.0;
	 double    Time_ch_B = 0.0;
	double BaseLine_ch_B = 0.0;

	Double_t * DataVect_ch_A=new Double_t[Buffer];
	Double_t * DataVect_ch_B=new Double_t[Buffer];

  // Defining Histograms

	if(Draw==1){TCanvas *show = new TCanvas("c0","c0", 1000, 500); show->Divide(2,1);}
	TH1F * Wave_ch_A   = new TH1F("", "", Buffer, 0, Buffer);
	TH1F * bpsig_ch_A  = new TH1F("", "", Buffer, 0, Buffer);
	TH1F * Wave_ch_B   = new TH1F("", "", Buffer, 0, Buffer);
  TH1F * bpsig_ch_B  = new TH1F("", "", Buffer, 0, Buffer);

	Wave_ch_A->GetXaxis()->SetTitle("Time [ns]");
	Wave_ch_A->GetYaxis()->SetTitle("Height [ch]");
	Wave_ch_B->GetXaxis()->SetTitle("Time [ns]");
	Wave_ch_B->GetYaxis()->SetTitle("Height [ch]");

	TH2F * TimeVsAmp0 = new TH2F("Time vs Amplitude 0", "Time vs Amplitude 0", 1000, 0, 1400, 1000, -15, 15);
	TH2F * TimeVsAmp1 = new TH2F("Time vs Amplitude 1", "Time vs Amplitude 1", 1000, 0, 1400, 1000, -15, 15);

	TimeVsAmp0->GetXaxis()->SetTitle("Energy [keV]");
	TimeVsAmp0->GetYaxis()->SetTitle("Digital Timing [ns]");

	TimeVsAmp1->GetXaxis()->SetTitle("Energy [keV]");
	TimeVsAmp1->GetYaxis()->SetTitle("Digital Timing [ns]");

	//Timing resolution as function of energy
	TH1F * Time_Gate_0 = new TH1F("Energy 0-200 keV"   ,"Energy 0-200 keV"     ,1000, -15, 15);
	TH1F * Time_Gate_1 = new TH1F("Energy 200-400 keV" ,"Energy 200-400 keV"   ,1000, -15, 15);
	TH1F * Time_Gate_2 = new TH1F("Energy 400-600 keV" ,"Energy 400-600 keV"   ,1000, -15, 15);
	TH1F * Time_Gate_3 = new TH1F("Energy 600-800 keV" ,"Energy 600-800 keV"   ,1000, -15, 15);
	TH1F * Time_Gate_4 = new TH1F("Energy 800-1000 keV","Energy 800-1000 keV"  ,1000, -15, 15);
	TH1F * Time_Gate_5 = new TH1F("Energy 1000-1200 keV","Energy 1000-1200 keV",1000, -15, 15);


	Time_Gate_0->GetXaxis()->SetTitle("Time [ns]");
	Time_Gate_0->GetYaxis()->SetTitle("Count");
	Time_Gate_1->GetXaxis()->SetTitle("Time [ns]");
	Time_Gate_1->GetYaxis()->SetTitle("Count");
	Time_Gate_2->GetXaxis()->SetTitle("Time [ns]");
	Time_Gate_2->GetYaxis()->SetTitle("Count");
	Time_Gate_3->GetXaxis()->SetTitle("Time [ns]");
	Time_Gate_3->GetYaxis()->SetTitle("Count");
	Time_Gate_4->GetXaxis()->SetTitle("Time [ns]");
	Time_Gate_4->GetYaxis()->SetTitle("Count");
	Time_Gate_5->GetXaxis()->SetTitle("Time [ns]");
	Time_Gate_5->GetYaxis()->SetTitle("Count");

	Time_Gate_0->SetStats(kFALSE);
	Time_Gate_1->SetStats(kFALSE);
	Time_Gate_2->SetStats(kFALSE);
	Time_Gate_3->SetStats(kFALSE);
	Time_Gate_4->SetStats(kFALSE);
	Time_Gate_5->SetStats(kFALSE);

  TH1F * d1_en_sp = new TH1F("d1_en_sp","Detector 1 Spectrum", 1000, 0, 1500);
  TH1F * d2_en_sp = new TH1F("d2_en_sp","Detector 2 Spectrum", 1000, 0, 1500);

  std::cout << "Loading Event from FILE\n";
  std::cout << "-----------------------\n";

  while(counter_A < nEventsA and counter_B < nEventsB)
  {

      // Display Progress

      if (counter_A % (nEventsA/100) == 0 or counter_B % (nEventsB/100) == 0)
      {
          system("clear");
          std::cout << "Loading Event from FILE\n";
          std::cout << "-----------------------\n";
          std::cout << "Load "
                    <<  (int)((counter_A*100.0)/nEventsA)
                    << "% in channel A"
                    << std::endl;
          std::cout << "Load "
                    <<  (int)((counter_B*100.0)/nEventsB)
                    << "% in channel B"
                    << std::endl;
      }

      // Events filling

      branch_ch_A->GetEntry(counter_A);
      branch_ch_B->GetEntry(counter_B);

      time_difference = event_ch_A.timetag-event_ch_B.timetag;

      if(TMath::Abs(time_difference) > 100) //  <--- scegli la finestra di allinamento in sample 1sample=1ns
      {
          if( time_difference > 0){ counter_B++; skipB++; } else { counter_A++; skipA++; }
      }
      else
			{

			//retrieve waveforms

				for(int i=0;i<Buffer;i++){
					DataVect_ch_A[i]=1000-event_ch_A.samples[i];
					Wave_ch_A->SetBinContent(i,DataVect_ch_A[i]);
					DataVect_ch_B[i]=1000-event_ch_B.samples[i];
					Wave_ch_B->SetBinContent(i,DataVect_ch_B[i]);
			  }

      	 //Initialise variables
				     bsvar_ch_A = 0.0;
				      Time_ch_A = 0.0;
				  BaseLine_ch_A = 0.0;
					  Thresh_ch_A = 0;

					   bsvar_ch_B = 0.0;
				      Time_ch_B = 0.0;
				  BaseLine_ch_B = 0.0;
					  Thresh_ch_B = 0;

			   // Baseline ch_A
  	     bs=0;
		     for(int i=0;i<Buffer;i++){
					 if((i>bsRange)&&(Thresh_ch_A==0)&&(DataVect_ch_A[i]>(BaseLine_ch_A)+Th))Thresh_ch_A=i;
		 		   if((i>bsmin)&&(i<bsRange)){BaseLine_ch_A+=DataVect_ch_A[i]; bs++;}
		       if(i==bsRange)BaseLine_ch_A=(double)BaseLine_ch_A/bs;
				 }

				 // Baseline ch_B
  	     bs=0;
		     for(int i=0;i<Buffer;i++){
					 if((i>bsRange)&&(Thresh_ch_B==0)&&(DataVect_ch_B[i]>(BaseLine_ch_B)+Th))Thresh_ch_B=i;
		 		   if((i>bsmin)&&(i<bsRange)){BaseLine_ch_B+=DataVect_ch_B[i]; bs++;}
		       if(i==bsRange)BaseLine_ch_B=(double)BaseLine_ch_B/bs;
				 }

				 // Digital Timing
		     if(Thresh_ch_A>0 && Thresh_ch_B>0){
					 Time_ch_A=TimingC2(DataVect_ch_A, Buffer, 0.22, 5, 3, BaseLine_ch_A, bsRange, bpsig_ch_A, Draw);
					 Time_ch_B=TimingC2(DataVect_ch_B, Buffer, 0.22, 5, 3, BaseLine_ch_B, bsRange, bpsig_ch_B, Draw);
				 }

				 if(Draw==1){
					show->cd(1);
					Wave_ch_A->Draw();
					bpsig_ch_A->Draw("SAME");
					show->cd(2);
					Wave_ch_B->Draw();
					bpsig_ch_B->Draw("SAME");
					show->Update();
					show->Draw();
					while (cin.get() != '\n'){}
				 }
        double Energy_0 = event_ch_B.qlong*0.0754713-54.25;     //calibrated
        double Energy_1 = event_ch_B.qlong*0.0824706-17.8629;   //calibrated

        d1_en_sp->Fill(Energy_0);
        d2_en_sp->Fill(Energy_1);

				TimeVsAmp0->Fill(Energy_0, Time_ch_B-Time_ch_A);
				TimeVsAmp1->Fill(Energy_1, Time_ch_B-Time_ch_A);

				if(Energy_0 > 0   && Energy_0 <= 200 )  Time_Gate_0->Fill(Time_ch_B-Time_ch_A);
	      if(Energy_0 > 200 && Energy_0 <= 400 )  Time_Gate_1->Fill(Time_ch_B-Time_ch_A);
	      if(Energy_0 > 400 && Energy_0 <= 600 )  Time_Gate_2->Fill(Time_ch_B-Time_ch_A);
	      if(Energy_0 > 600 && Energy_0 <= 800 )  Time_Gate_3->Fill(Time_ch_B-Time_ch_A);
	      if(Energy_0 > 800 && Energy_0 <= 1000)  Time_Gate_4->Fill(Time_ch_B-Time_ch_A);
				if(Energy_0 > 1000 && Energy_0 <= 1200) Time_Gate_5->Fill(Time_ch_B-Time_ch_A);

        // updating counters
        coincident_events++;
        id_A++;
        id_B++;
        counter_A++;
        counter_B++;

      }

  }

	TCanvas * c0 = new TCanvas("c0","c0", 500, 500);
	TCanvas * c1 = new TCanvas("c1","c1", 500, 500);
	TCanvas * c2 = new TCanvas("c2","c2", 500, 500);
	TCanvas * c3 = new TCanvas("c3","c3", 500, 500);
	TCanvas * c4 = new TCanvas("c4","c4", 500, 500);
	TCanvas * c5 = new TCanvas("c5","c5", 500, 500);

	std::vector<Double_t> my_vec_0 = Fitter(Time_Gate_0, 1.5, c0);
	std::vector<Double_t> my_vec_1 = Fitter(Time_Gate_1, 1.5, c1);
	std::vector<Double_t> my_vec_2 = Fitter(Time_Gate_2, 1.5, c2);
	std::vector<Double_t> my_vec_3 = Fitter(Time_Gate_3, 1.5, c3);
	std::vector<Double_t> my_vec_4 = Fitter(Time_Gate_4, 1.5, c4);
	std::vector<Double_t> my_vec_5 = Fitter(Time_Gate_5, 1.5, c5);

	ofstream temp("temp.dat");

	temp<<"100 "<<my_vec_0[0]<<" "<<my_vec_0[1]<<endl;
	temp<<"300 "<<my_vec_1[0]<<" "<<my_vec_1[1]<<endl;
	temp<<"500 "<<my_vec_2[0]<<" "<<my_vec_2[1]<<endl;
	temp<<"700 "<<my_vec_3[0]<<" "<<my_vec_3[1]<<endl;
	temp<<"900 "<<my_vec_4[0]<<" "<<my_vec_4[1]<<endl;
	temp<<"1100 "<<my_vec_5[0]<<" "<<my_vec_5[1]<<endl;

	temp.close();

	TCanvas *C = new TCanvas("c","c",1280,1080);

	TPad* upperPad = new TPad("upperPad", "upperPad", .005, .700, .995, .995);
	TPad* lowerPad = new TPad("lowerPad", "lowerPad", .005, .005, .995, .700);
	upperPad->Draw();
	lowerPad->Draw();
	Float_t small = 1e-5;
	lowerPad->SetTopMargin(small);
	upperPad->SetBottomMargin(small);

	lowerPad->SetGridx();
	upperPad->SetGridx();

	lowerPad->SetTickx(2);
	upperPad->cd();


			 TGraphErrors *Graph = new TGraphErrors("temp.dat","%lg%lg%lg");
			 Graph->GetXaxis()->SetTitle("Energy[KeV]");
			 Graph->GetYaxis()->SetTitle("FWHM[ns]");
			 Graph->GetXaxis()->SetLimits(0,1400);
			 Graph->GetYaxis()->SetLimits(0.53,2.5);

			 Graph->GetYaxis()->SetTitleSize(0.06);
			 Graph->GetYaxis()->SetTitleOffset(0.8);
			 Graph->GetYaxis()->SetLabelSize(0.05);
			 Graph->GetYaxis()->SetNdivisions(502);

			 Graph->GetXaxis()->SetTitleSize(0.06);
			 Graph->GetXaxis()->SetLabelSize(0.04);
			 Graph->GetXaxis()->SetTitleOffset(0.75);

			 Graph->SetLineWidth(1);
			 Graph->Draw();

			 lowerPad->cd();

			 gStyle->SetOptStat(false);
			 TimeVsAmp0->Draw("COLZ");

  d1_en_sp->Draw();

  return ;
}
