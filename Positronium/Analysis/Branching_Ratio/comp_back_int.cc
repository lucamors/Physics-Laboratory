void ComptonSubInt(string filename, int channel)
{

  gStyle->SetTitleAlign(33);
  gStyle->SetTitleX(.99);

  TFile * infile = new TFile(filename.c_str());

  TCanvas * c1 = new TCanvas("c1");

  string channel_name = "ch" + to_string(channel);
  // Setting up detector 1
  TH1F * ch0_raw = (TH1F*) infile->Get(channel_name.c_str());

  int nbins = ch0_raw->GetSize();

  Double_t source[nbins];

//    TH1F * ch0_bg = new TH1F("ch0_bg","",nbins, 0, );
//    TH1F * ch0_clean = new TH1F("ch0_clean","",nbins, 0, 2000);

      TH1F * ch0_bg = (TH1F*)ch0_raw->Clone("ch0_bg");
      TH1F * ch0_clean = (TH1F*)ch0_raw->Clone("ch0_clean");


  TSpectrum * s_ch0 = new TSpectrum();

  for (int i = 0; i < nbins; i++) source[i] = ch0_raw->GetBinContent(i + 1);

  s_ch0->Background(source,nbins,100,TSpectrum::kBackDecreasingWindow,
                TSpectrum::kBackOrder4,kTRUE,
                TSpectrum::kBackSmoothing3,kTRUE);

  for (int i = 0; i < nbins; i++) ch0_bg->SetBinContent(i + 1,source[i]);


  for (size_t i = 0; i < nbins; i++)
  {
    ch0_clean->SetBinContent(i+1, ch0_raw->GetBinContent(i+1)-ch0_bg->GetBinContent(i+1));
  }
/*
  ch0_raw->SetTitle("");
  ch0_raw->GetXaxis()->SetRangeUser(0,800);
  ch0_raw->GetXaxis()->SetTitle("Energy [keV]");
  ch0_raw->SetStats(kFALSE);
  ch0_raw->Draw();
  ch0_bg->SetLineColor(kRed);
  ch0_bg->Draw("SAME");*/
  ch0_clean->SetLineColor(kOrange);
  ch0_clean->GetXaxis()->SetRangeUser(0,800);
  ch0_clean->GetYaxis()->SetRangeUser(0,60);
  ch0_clean->GetXaxis()->SetTitle("Energy [keV]");
  ch0_clean->SetStats(kFALSE);
  ch0_clean->SetTitle("");
  ch0_clean->SetLineWidth(2);
  ch0_clean->Draw();

  TH1F *h1c = (TH1F*)ch0_clean->Clone();
  h1c->SetFillColorAlpha(kRed,0.80);
  h1c->SetFillStyle(3003);
  const double sqrt2ln2 = 1.17741002251547;

  TF1 *GausPeak = new TF1("GPeak","gaus", 200, 420);
  ch0_clean->Fit(GausPeak,"RNME");
  Double_t Center = GausPeak->GetParameter(1);
  Double_t Sigma  = GausPeak->GetParameter(2);
  Double_t FWHM     = 2*sqrt2ln2*Sigma;
// Computing Integral

  h1c->GetXaxis()->SetRangeUser(Center-1.5*FWHM,Center+1.5*FWHM);
  h1c->Draw("SAME");
  c1->Update();
  int xmin1 = ch0_clean->GetXaxis()->FindBin(Center-1.5*FWHM);
  int xmax1 = ch0_clean->GetXaxis()->FindBin(Center+1.5*FWHM);
  int gamma_det1 = ch0_clean->Integral(xmin1,xmax1);

  // Output result

  std::cout << "Integral of photopeak detector #1 : " << gamma_det1 << '\n';

  return ;
}
