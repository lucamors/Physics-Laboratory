void PlotCal(string filename_tag, string filename_scat, string filename_det)
{
  TFile * infile_tag = new TFile(filename_tag.c_str());

  TH1F * tagger_sp = (TH1F * ) infile_tag->Get("ch0");

  TFile * infile_scat = new TFile(filename_scat.c_str());

  TH1F * scat_sp = (TH1F * ) infile_scat->Get("ch1");

  TFile * infile_det = new TFile(filename_det.c_str());

  TH1F * det_sp = (TH1F * ) infile_det->Get("ch2");

  TCanvas * c1 = new TCanvas("c1");
  c1->Divide(1,3);
  c1->cd(1);

  tagger_sp->SetStats(kFALSE);
  tagger_sp->GetXaxis()->SetRangeUser(0,1400);
  tagger_sp->SetTitle("Tagger ^{22}Na Spectrum");
  tagger_sp->Draw();

  c1->cd(2);

  scat_sp->SetStats(kFALSE);
  scat_sp->GetXaxis()->SetRangeUser(0,1400);
  scat_sp->SetTitle("Scatterer ^{22}Na Spectrum");
  scat_sp->Draw();

  c1->cd(3);

  det_sp->SetStats(kFALSE);
  det_sp->GetXaxis()->SetRangeUser(0,1400);
  det_sp->SetTitle("Detector ^{22}Na Spectrum");
  det_sp->Draw();

  return ;
}
