void ComptonSubInt(string filename)
{

  gStyle->SetTitleAlign(33);
  gStyle->SetTitleX(.99);

  TFile * infile = new TFile(filename.c_str());

  TCanvas * c1 = new TCanvas("c1");

  c1->Divide(2,1);

  c1->cd(1);

  // Setting up detector 1
  TH1F * ch0_raw = (TH1F*) infile->Get("ch0");

  int nbins = 1000;

  Double_t source[nbins];

  TH1F * ch0_bg = new TH1F("ch0_bg","",nbins, 0, 2000);
  TH1F * ch0_clean = new TH1F("ch0_clean","",nbins, 0, 2000);

  TSpectrum * s_ch0 = new TSpectrum();

  for (int i = 0; i < nbins; i++) source[i] = ch0_raw->GetBinContent(i + 1);

  s_ch0->Background(source,nbins,10,TSpectrum::kBackDecreasingWindow,
                TSpectrum::kBackOrder8,kTRUE,
                TSpectrum::kBackSmoothing5,kTRUE);

  for (int i = 0; i < nbins; i++) ch0_bg->SetBinContent(i + 1,source[i]);


  for (size_t i = 0; i < nbins; i++)
  {
    ch0_clean->SetBinContent(i+1, ch0_raw->GetBinContent(i+1)-ch0_bg->GetBinContent(i+1));
  }

  ch0_raw->SetTitle("Detector #1");
  ch0_raw->GetXaxis()->SetRangeUser(400,650);
  ch0_raw->SetStats(kFALSE);
  ch0_raw->Draw();
  ch0_bg->SetLineColor(kRed);
  ch0_bg->Draw("SAME");
  ch0_clean->SetLineColor(kOrange);
  ch0_clean->SetLineWidth(2);
  ch0_clean->Draw("SAME");


  // Setting up detector 2

  c1->cd(2);
  TH1F * ch1_raw = (TH1F*) infile->Get("ch1");

  Double_t source2[nbins];

  TH1F * ch1_bg = new TH1F("ch1_bg","",nbins, 0, 2000);
  TH1F * ch1_clean = new TH1F("ch1_clean","",nbins, 0, 2000);

  TSpectrum * s_ch1 = new TSpectrum();

  for (int i = 0; i < nbins; i++) source2[i] = ch1_raw->GetBinContent(i + 1);

  s_ch1->Background(source2,nbins,10,TSpectrum::kBackDecreasingWindow,
                TSpectrum::kBackOrder8,kTRUE,
                TSpectrum::kBackSmoothing5,kTRUE);

  for (int i = 0; i < nbins; i++) ch1_bg->SetBinContent(i + 1,source2[i]);


  for (size_t i = 0; i < nbins; i++)
  {
    ch1_clean->SetBinContent(i+1, ch1_raw->GetBinContent(i+1)-ch1_bg->GetBinContent(i+1));
  }

  ch1_raw->SetTitle("Detector #2");
  ch1_raw->GetXaxis()->SetRangeUser(400,650);
  ch1_raw->SetStats(kFALSE);
  ch1_raw->Draw();
  ch1_bg->SetLineColor(kRed);
  ch1_bg->Draw("SAME");
  ch1_clean->SetLineColor(kOrange);
  ch1_clean->SetLineWidth(2);
  ch1_clean->Draw("SAME");


  // Computing Integral

  int xmin1 = ch0_clean->GetXaxis()->FindBin(400);
  int xmax1 = ch0_clean->GetXaxis()->FindBin(650);
  int xmin2 = ch1_clean->GetXaxis()->FindBin(400);
  int xmax2 = ch1_clean->GetXaxis()->FindBin(650);

  int gamma_det1 = ch0_clean->Integral(xmin1,xmax1);
  int gamma_det2 = ch1_clean->Integral(xmin2,xmax2);


  // Output result

  std::cout << "Integral of photopeak detector #1 : " << gamma_det1 << '\n';
  std::cout << "Integral of photopeak detector #2 : " << gamma_det2 << '\n';



  return ;
}
