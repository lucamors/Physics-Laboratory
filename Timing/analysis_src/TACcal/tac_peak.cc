#include <string>

void find_peaks(string filename)
{
  TFile * input_file = new TFile(filename.c_str());

  TH1F * TAC_spectrum = (TH1F*) input_file->Get("ch2");

  // TSpectrum Peak Search

  TSpectrum *search_peaks = new TSpectrum(100);

  int found_peaks = search_peaks->Search(TAC_spectrum, 2, "", 1);

  TAC_spectrum->Draw();

  return ;
}
