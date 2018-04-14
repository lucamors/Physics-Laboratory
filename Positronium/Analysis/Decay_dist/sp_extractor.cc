

const int MAX_X = 35000;

void cal_sp(string filename, int channel, string outfilename)
{

  TFile * _inputfile = new TFile(filename.c_str());


  string histogram_name = "ch" + to_string(channel);
  string histogram_title = "Calibrated Spectra Channel " + to_string(channel);

  TH1F * calibrated_spectra;

  Int_t n_bins;

  switch (channel)
  {
    case 0: // Det_1   *0.0726917-23.9426

      calibrated_spectra = (TH1F*) _inputfile->Get("ch0");

      // -2 is due to underflow and overlow
      n_bins = calibrated_spectra->GetSize()-2;

      calibrated_spectra->GetXaxis()->Set(n_bins, -23.9426, -23.9426+ MAX_X*0.0726917 );

      break;

    case 1: // Det_2    *0.0735321-24.8858

      calibrated_spectra = (TH1F*) _inputfile->Get("ch1");

      // -2 is due to underflow and overlow
      n_bins = calibrated_spectra->GetSize()-2;

      calibrated_spectra->GetXaxis()->Set(n_bins, -24.8858 , -24.8858 + MAX_X*0.0735321 );

      break;

    case 2: // Det_3      *0.0714929-26.3991

      calibrated_spectra = (TH1F*) _inputfile->Get("ch2");

      // -2 is due to underflow and overlow
      n_bins = calibrated_spectra->GetSize()-2;

      calibrated_spectra->GetXaxis()->Set(n_bins, -26.3991 , -26.3991 + MAX_X*0.0714929 );

      break;

      case 3: // TAC

        calibrated_spectra = (TH1F*) _inputfile->Get("ch3");

        // -2 is due to underflow and overlow
        n_bins = calibrated_spectra->GetSize()-2;

        calibrated_spectra->GetXaxis()->Set(n_bins, -38.9 , -38.9 + MAX_X * 0.00507 );

        break;

    default:

      cout << "channel not valid\n";

  }

  // Saving to file

  TFile * outfile = new TFile(outfilename.c_str(), "RECREATE");

  calibrated_spectra->Write();

  outfile->Close();

  _inputfile->Close();


  return ;
}
