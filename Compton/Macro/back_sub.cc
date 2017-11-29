void back_sub(string input_filename){
  TFile * incfile = new TFile(input_filename.c_str());

	TH1F * spectrum = (TH1F*) incfile->Get("ch0");

  TSpectrum * s = new TSpectrum(3);

  Int_t n_found = s->Search(spectrum, 20,"",0.7);

  TCanvas * c1 = new TCanvas("c1");
  // c1->Divide(2);
  // c1->cd(1);
  spectrum->Draw();

  Int_t n_bins=1000;

  Double_t  * background_bins = new Double_t[n_bins];

  TH1F * bg_spectrum = new TH1F("h_bg","Background Spectrum",n_bins,0,1500);

  for (size_t i = 0; i < n_bins; i++) { background_bins[i] = spectrum->GetBinContent(i+1); }

  s->Background(background_bins, n_bins, 100,
                    TSpectrum::kBackDecreasingWindow, TSpectrum::kBackOrder8, kTRUE,
                    TSpectrum::kBackSmoothing5, kTRUE);

  for (Int_t i = 0; i < n_bins; i++){bg_spectrum->SetBinContent(i,background_bins[i]);}

  bg_spectrum->SetLineColor(kAzure+2);
  bg_spectrum->SetLineWidth(2);
  bg_spectrum->Draw("SAME");

  TH1F * filtered_spectrum = new TH1F("h_filtered","Filtered Spectrum",n_bins,0,1500);
  for (Int_t i = 0; i < n_bins; i++){filtered_spectrum->SetBinContent(i,spectrum->GetBinContent(i)-bg_spectrum->GetBinContent(i));}

  // c1->cd(2);

  filtered_spectrum->SetFillColor(kRed);
  filtered_spectrum->SetLineColor(kRed+1);
  filtered_spectrum->Draw("SAME");


}
