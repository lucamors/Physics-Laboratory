/*
* Simple ROOT macro that plot simulation result
*/

void plot_result()
{
  TFile * infile = new TFile("risultati.root");

  TGraph2D * d1 = (TGraph2D*) infile->Get("d1_det");
  TGraph2D * d2 = (TGraph2D*) infile->Get("d2_det");
  TGraph2D * d3 = (TGraph2D*) infile->Get("d3_det");
  TGraph2D * not_det = (TGraph2D*) infile->Get("not_det");


  // Plotting

  d1->SetMarkerColor(kGreen-3);
  d1->SetMarkerStyle(3);

  d2->SetMarkerColor(kGreen-3);
  d2->SetMarkerStyle(3);

  d3->SetMarkerColor(kGreen-3);
  d3->SetMarkerStyle(3);

  not_det->SetMarkerColor(kBlue);
  not_det->SetMarkerStyle(20);

  not_det->SetTitle("Decay Simulation");
  not_det->Draw("p");
  d1->Draw("p SAME");
  d2->Draw("p SAME");
  d3->Draw("p SAME");

  return;
}
