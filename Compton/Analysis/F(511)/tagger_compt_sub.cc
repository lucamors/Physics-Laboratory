
const string branch_name  = "cal_sp";

void subcompt(string input_filename,
              double fit_min, double fit_max,
              double sub_min, double sub_max,
              string output_filename)
{

  // Retrieving energy spectrum histogram

  TFile * infile = new TFile(input_filename.c_str());
  TH1F * energy_spectrum = ( TH1F * ) infile->Get(branch_name.c_str());

  // Fitting the Compton continuum

  TF1 * linear_poly = new TF1("linpoly", "[0]+[1]*x", fit_min, fit_max);

  energy_spectrum->Fit("linpoly", "R0");

  double q = linear_poly->GetParameter(0);
  double m = linear_poly->GetParameter(1);

  // Subtracting the background

  Int_t bin_min = energy_spectrum->GetXaxis()->FindBin(sub_min);
  Int_t bin_max = energy_spectrum->GetXaxis()->FindBin(sub_max);

  for (size_t i = bin_min; i < bin_max; i++)
  {
    double current_counter = energy_spectrum->GetBinContent(i);

    energy_spectrum->SetBinContent(i,  current_counter - m*energy_spectrum->GetBinCenter(i)-q );
  }

  double A511=  energy_spectrum->Integral(energy_spectrum->GetXaxis()->FindBin(470),energy_spectrum->GetXaxis()->FindBin(550));
  cout<<A511<<endl;
  // Saving to file

  TFile * outfile = new TFile((output_filename+".root").c_str(),"RECREATE");
  energy_spectrum->Write();
  outfile->Close();

  return ;
}
