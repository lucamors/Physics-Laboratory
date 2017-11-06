#include <string>
#include <vector>


double peak(double *x, double *par)
{
       double xx= x[0];
       //const double invsqrt2pi = 0.398942280401433;
       double gaus = par[0]*exp(-0.5*(xx-par[1])*(xx-par[1])/(par[2]*par[2]));
       double expl = par[0]*exp(((par[3]*par[3])/2)+(par[3]*((xx-par[1])/par[2])));
       double expr = par[0]*exp(((par[4]*par[4])/2)-(par[4]*((xx-par[1])/par[2])));
       if (xx<par[1]-par[3]*par[2]){return expl;}
       else if(xx>par[1]+par[4]*par[2]){return expr;}
       else {return gaus;}
}

void del_opt(string filename)
{
  TFile * input_file = new TFile(filename.c_str());

  TH1F * TAC_spectrum = (TH1F*) input_file->Get("ch2");

  TAC_spectrum->GetXaxis()->Set(1000,-3.18588, -3.18588+0.000735898*65536);

  // TSpectrum Peak Search

  TSpectrum *search_peaks = new TSpectrum(100);

  int found_peaks = search_peaks->Search(TAC_spectrum, 1, "", 0.05);

  // Retrieving X positions and print them out to std output

  Double_t * x_pos_peaks = search_peaks->GetPositionX();
  Double_t * y_peak = search_peaks->GetPositionY();

  // FWHM Parameters

  double peakmax=x_pos_peaks[0];
  double peakheight= y_peak[0];
  double par0;
  double par1;
  double par2;
  double par3;
  double par4;
  double par2sig;
  double par3sig;
  double par4sig;
  double sig23;
  double sig24;
  double sig43;
  const double sqrt2ln2 = 1.17741002251547;
  double hwhm1, hwhm2, FWHM;
  double FWHMsig=0;
  int checkR=0, checkL=0;

  TF1 *fpeak = new TF1("fpeak",peak, peakmax-1.5, peakmax+1.5, 5);
  fpeak->SetParameter(0,peakheight);
  fpeak->SetParLimits(0,peakheight-6*peakheight/100,peakheight+1*peakheight/100);
  fpeak->SetParameter(1,peakmax);
  //fpeak->SetParLimits(1,peakmax-0.4,peakmax+0.4);
  fpeak->SetParameter(2,0.25);
  //fpeak->SetParLimits(2,0.1,2);
  fpeak->SetParameter(3,1.4);
  //fpeak->SetParLimits(3,0,10);
  fpeak->SetParameter(4,0.5);
  //fpeak->SetParLimits(4,0,10);
  //TacCh1Ch3->Fit(fpeak,"QRLSN");
  TFitResultPtr res=TAC_spectrum->Fit(fpeak,"RSNME");
  //TFitResultPtr res=TacCh1Ch3->Fit(fpeak,"BRLSNI");
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
     else {hwhm1=par1-par2*sqrt2ln2;
       checkR=1;}

  if((par1+par2*sqrt2ln2)>par1+(par4*par2)){hwhm2=par1+par2*((0.5*par4)+(0.6931471806/par4));}
     else {hwhm2=par1+par2*sqrt2ln2;
       checkL=1;}

  fpeak->SetFillColor(19);
  fpeak->SetFillStyle(1);
  fpeak->SetLineWidth(2);
  fpeak->Draw("SAME");

  FWHM=hwhm2-hwhm1;
  if(checkR==0 && checkL==0) FWHMsig=sqrt((((FWHM*par2sig)/par2)*((FWHM*par2sig)/par2))+((par3sig*par2*(0.5-(0.6931471806/(par3*par3))))*(par3sig*par2*(0.5-(0.6931471806/(par3*par3)))))+((par4sig*par2*(0.5-(0.6931471806/(par4*par4))))*(par4sig*par2*(0.5-(0.6931471806/(par4*par4)))))+2*(FWHM/par2)*(par2*(0.5-(0.6931471806/(par3*par3))))*sig23+2*(FWHM/par2)*(par2*(0.5-(0.6931471806/(par4*par4))))*sig24+2*(par2*(0.5-(0.6931471806/(par4*par4))))*(par2*(0.5-(0.6931471806/(par3*par3))))*sig43);
  if(checkR==0 && checkL==1) FWHMsig=sqrt((((FWHM*par2sig)/par2)*((FWHM*par2sig)/par2))+((par3sig*par2*(0.5-(0.6931471806/(par3*par3))))*(par3sig*par2*(0.5-(0.6931471806/(par3*par3)))))+2*(FWHM/par2)*(par2*(0.5-(0.6931471806/(par3*par3))))*sig23);
  if(checkR==1 && checkL==0) FWHMsig=sqrt((((FWHM*par2sig)/par2)*((FWHM*par2sig)/par2))+((par4sig*par2*(0.5-(0.6931471806/(par4*par4))))*(par4sig*par2*(0.5-(0.6931471806/(par4*par4)))))+2*(FWHM/par2)*(par2*(0.5-(0.6931471806/(par4*par4))))*sig24);
  if(checkR==1 && checkL==1) FWHMsig=(FWHM*par2sig)/par2;
  std::cout << "FWHM [ns]--> " << FWHM << "\n"
            << "Error [ns]--> " << FWHMsig << endl;

  // Drawing fit result
  //
  // TCanvas * time_canvas = new TCanvas("time_canvas", "time_canvas", 500);
  //
  // TAC_spectrum->SetMarkerSize(2);
  // TAC_spectrum->SetMarkerStyle(2);
  // TAC_spectrum->GetXaxis()->SetTitleOffset(1.2);
  // TAC_spectrum->GetYaxis()->SetTitleOffset(1.2);
  // TAC_spectrum->GetXaxis()->SetTitle("TAC Channel [a.u.]");
  // TAC_spectrum->GetYaxis()->SetTitle("Time [ns]");
  // TAC_spectrum->SetTitle("TAC Calibration");
  // TAC_spectrum->Draw();


  return ;
}
