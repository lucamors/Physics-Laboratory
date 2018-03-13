void plot_corr_line(string corr_filename)
{

  TFile * corr_file = new TFile(corr_filename.c_str());

  TH2F * corr = (TH2F*) corr_file->Get("correlation_coinc");

  TLine * corr_fit = new TLine(0, 508, 516, 0);
  corr_fit->SetLineColor(kRed);

  TCanvas * c1 = new TCanvas();

  corr->Draw();
  corr_fit->Draw("SAME");

  return ;
}
