

const int MAX_X = 35000;

void sum(string filename, int kind, string outfilename)
{

  TFile * _inputfile = new TFile(filename.c_str());

  TH1F * calibrated_spectra_1;
  TH1F * calibrated_spectra_2;
  TH1F * calibrated_spectra_3;
  TH1F * Sum_spectrum;
  Int_t n_bins;

  switch (kind)
  {
    case 2: // Det_1   *0.0726917-23.9426

      calibrated_spectra_1 = (TH1F*) _inputfile->Get("ch0");
      calibrated_spectra_2 = (TH1F*) _inputfile->Get("ch1");

      // -2 is due to underflow and overlow
      n_bins = calibrated_spectra_1->GetSize()-2;

      calibrated_spectra_1->GetXaxis()->Set(n_bins, -23.9426, -23.9426+ MAX_X*0.0726917 );
      calibrated_spectra_2->GetXaxis()->Set(n_bins, -24.8858 , -24.8858 + MAX_X*0.0735321 );

      Sum_spectrum = (TH1F*)calibrated_spectra_1->Clone();

      for (size_t i = 0; i < n_bins; i++) {
        Sum_spectrum->SetBinContent(i, calibrated_spectra_1->GetBinContent(i)+calibrated_spectra_2->GetBinContent(i));
      }
      break;

    case 3: // Det_2    *0.0735321-24.8858

          calibrated_spectra_1 = (TH1F*) _inputfile->Get("ch0");
          calibrated_spectra_2 = (TH1F*) _inputfile->Get("ch1");
          calibrated_spectra_3 = (TH1F*) _inputfile->Get("ch2");

          // -2 is due to underflow and overlow
          n_bins = calibrated_spectra_1->GetSize()-2;

          calibrated_spectra_1->GetXaxis()->Set(n_bins, -23.9426, -23.9426+ MAX_X*0.0726917 );
          calibrated_spectra_2->GetXaxis()->Set(n_bins, -24.8858 , -24.8858 + MAX_X*0.0735321 );
          calibrated_spectra_3->GetXaxis()->Set(n_bins, -26.3991 , -26.3991 + MAX_X*0.0714929 );

          Sum_spectrum = (TH1F*)calibrated_spectra_1->Clone();

          for (size_t i = 0; i < n_bins; i++) {
            Sum_spectrum->SetBinContent(i, calibrated_spectra_1->GetBinContent(i)+calibrated_spectra_2->GetBinContent(i)+calibrated_spectra_3->GetBinContent(i));
          }
          break;


    default:

      cout << "channel not valid\n";

  }

  // Saving to file

  TFile * outfile = new TFile(outfilename.c_str(), "RECREATE");

  Sum_spectrum->Write();


  outfile->Close();

  _inputfile->Close();


  return ;
}
