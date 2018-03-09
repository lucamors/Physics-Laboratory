

const int MAX_X = 35000;

void cal_sp(string filename, int channel, string outfilename)
{

  TFile * _inputfile = new TFile(filename.c_str());


  string histogram_name = "h_calib_ch_" + to_string(channel);
  string histogram_title = "Calibrated Spectra Channel " + to_string(channel);

  TH1F * calibrated_spectra;

  Int_t n_bins;

  switch (channel)
  {
    case 0: // Tagger

      calibrated_spectra = (TH1F*) _inputfile->Get("ch0");

      // -2 is due to underflow and overlow
      n_bins = calibrated_spectra->GetSize()-2;

      calibrated_spectra->GetXaxis()->Set(n_bins, -7.32807, -7.32807+ MAX_X*0.0544615 );

      break;

    case 1: // Scatterer

      calibrated_spectra = (TH1F*) _inputfile->Get("ch1");

      // -2 is due to underflow and overlow
      n_bins = calibrated_spectra->GetSize()-2;

      calibrated_spectra->GetXaxis()->Set(n_bins, -7.81986 , -7.81986 + MAX_X*0.0577517 );

      break;

    case 2: // Scatterer

      calibrated_spectra = (TH1F*) _inputfile->Get("ch2");

      // -2 is due to underflow and overlow
      n_bins = calibrated_spectra->GetSize()-2;

      calibrated_spectra->GetXaxis()->Set(n_bins, -6.78006 , -6.78006 + MAX_X * 0.0531775 );

      break;

    default:

      cout << "channel not valid\n";

  }

  // Saving to file

  TFile * outfile = new TFile((outfilename+".root").c_str(), "RECREATE");

  calibrated_spectra->Write();

  outfile->Close();

  _inputfile->Close();


  return ;
}
