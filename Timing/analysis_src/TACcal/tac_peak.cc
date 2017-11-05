#include <string>
#include <vector>

void find_peaks(string filename)
{
  TFile * input_file = new TFile(filename.c_str());

  TH1F * TAC_spectrum = (TH1F*) input_file->Get("ch2");

  // TSpectrum Peak Search

  TSpectrum *search_peaks = new TSpectrum(100);

  int found_peaks = search_peaks->Search(TAC_spectrum, 2, "", 0.05);

  // Retrieving X positions and print them out to std output

  Double_t * x_pos_peaks = search_peaks->GetPositionX();
  Double_t * delay_unit_presets = new Double_t[16];

  for (size_t i = 0; i < found_peaks; i++) { delay_unit_presets[i] = 2*(i+1); }

  // Refactorizable:
  // Vector instead of dynamic array of Double_t in order to use sorting algs

  std::vector<Double_t> V_x_pos_peaks;

  for (size_t i = 0; i < found_peaks; i++) { V_x_pos_peaks.push_back(x_pos_peaks[i]); }

  sort(V_x_pos_peaks.begin(),V_x_pos_peaks.end());

  for (size_t i = 0; i < found_peaks; i++) { x_pos_peaks[i] = V_x_pos_peaks[i]; }

  // Calibration of the TAC spectrum

  std::vector<Double_t> V_delay_unit_presets;

  for (size_t i = 0; i < 16; i++) { V_delay_unit_presets.push_back(2*(i+1)); }

  TGraph * time_vs_channel = new TGraph(found_peaks, x_pos_peaks, delay_unit_presets);

  TF1 * f_linear = new TF1("f_linear", "pol1", 0, 100000);

  time_vs_channel->Fit(f_linear);

  TCanvas * time_canvas = new TCanvas("time_canvas", "time_canvas", 500);

  time_vs_channel->SetMarkerSize(2);
  time_vs_channel->SetMarkerStyle(2);
  time_vs_channel->GetXaxis()->SetTitleOffset(1.2);
  time_vs_channel->GetYaxis()->SetTitleOffset(1.2);
  time_vs_channel->GetXaxis()->SetTitle("TAC Channel [a.u.]");
  time_vs_channel->GetYaxis()->SetTitle("Time [ns]");
  time_vs_channel->SetTitle("TAC Calibration");
  time_vs_channel->Draw();
  f_linear->Draw("SAME");


  return ;
}
