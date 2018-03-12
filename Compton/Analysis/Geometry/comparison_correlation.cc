// Comparison between event by event energy conservation
// and simpler software gate

void comparison(string input_filename)
{

  TFile * input_file = new TFile(input_filename.c_str() );

  // Detector Spectrum

  TH1F * software_detector = (TH1F *) input_file->Get("ch2");
  TH1F * conservation_detector = (TH1F *) input_file->Get("ch2_coinc");

  software_detector->SetTitle("Software Detector");
  conservation_detector->SetTitle("Conservation Detector");

  // Scatterer Spectrum

  TH1F * software_scatterer = (TH1F *) input_file->Get("ch1");
  TH1F * conservation_scatterer = (TH1F *) input_file->Get("ch1_coinc");

  // Setting Names

  software_scatterer->SetTitle("Software Scatterer");
  conservation_scatterer->SetTitle("Conservation Scatterer");

  // Setting Histograms Range
  software_detector->GetXaxis()->SetRangeUser(0, 500);
  conservation_detector->GetXaxis()->SetRangeUser(0, 500);
  software_scatterer->GetXaxis()->SetRangeUser(0, 500);
  conservation_scatterer->GetXaxis()->SetRangeUser(0, 500);

  // Avoiding useless stats

  software_detector->SetStats(kFALSE);
  conservation_detector->SetStats(kFALSE);
  software_scatterer->SetStats(kFALSE);
  conservation_scatterer->SetStats(kFALSE);

  // Drawing Canvas

  TCanvas * comparison_canvas = new TCanvas("Comparison Canvas", "Comparison", 1200, 720);

  comparison_canvas->Divide(2,2);

  comparison_canvas->cd(1);

  software_detector->Draw();

  comparison_canvas->cd(2);

  conservation_detector->Draw();

  comparison_canvas->cd(3);

  software_scatterer->Draw();

  comparison_canvas->cd(4);

  conservation_scatterer->Draw();

  TFile * outfile = new TFile("prova.root", "RECREATE");

  comparison_canvas->Write();
  outfile->Close();


  input_file->Close();
  return ;
}
