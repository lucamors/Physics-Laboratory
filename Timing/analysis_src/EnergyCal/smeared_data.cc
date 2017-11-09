/*
* ROOT MACRO:
*
* Energy Calibration of organic scintillators
*
* Gaussian Smearing Method.
*
*/

#include <iostream>
#include <string>
#include <vector>

#include "f_profile_class.h"
#include "TCanvas.h"
#include "TSpectrum.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TF1.h"
#include "TRandom.h"
#include "TApplication.h"
#include "TFile.h"

const int n_bins = 1000;
const int max_bins = 2000;
const int n_smearings = 50;
std::string filename = "Na22_d1_calen.root";

// Custom methods
TH1F * Get_klein_nishina_spectrum(Double_t energy_peak, Double_t E_compton);
std::vector<TH1F*> Gaussian_smearing(TH1F * spectrum, Double_t energy_peak, Double_t E_compton, Int_t N);
Double_t * rough_sort(Double_t * array, Int_t n_found);
Double_t f_profile(Double_t *x, Double_t *par);


// Main function
void smearing_data(int argc, char **argv)
{

  Double_t E_compton_511 = 2*511.0*511.0/(511+2*511);
  Double_t E_compton_1275 = 2*1275.0*1275.0/(511+2*1275.0);

  // Creating Theoretical Compton Spectrum

  TH1F * ideal_spectrum_511 = new TH1F("ideal_spectrum_511","ideal_spectrum_511", n_bins, 0, max_bins);
  TH1F * ideal_spectrum_1275 = new TH1F("ideal_spectrum_1275","ideal_spectrum_1275", n_bins, 0, max_bins);

  // Retrieving ideal spectrum

  ideal_spectrum_511 = Get_klein_nishina_spectrum(511.0, E_compton_511);
  ideal_spectrum_1275 = Get_klein_nishina_spectrum(1275.0, E_compton_1275);


  // Smearing Spectrum

  std::vector<TH1F*> smeared_spectra_511 = Gaussian_smearing(ideal_spectrum_511, 511.0, E_compton_511, n_smearings);
  std::vector<TH1F*> smeared_spectra_1275 = Gaussian_smearing(ideal_spectrum_1275, 1275.0, E_compton_1275, n_smearings);

  std::vector<std::vector<TH1F * >> smeared_spectra;

  smeared_spectra.push_back(smeared_spectra_511);
  smeared_spectra.push_back(smeared_spectra_1275);

  // Saving smeared spectra into a TFile

  TFile * f_smearings = new TFile("smearings.root","RECREATE");

  for (size_t i = 0; i < smeared_spectra[0].size(); i++) { smeared_spectra[0][i]->Write(); }
  for (size_t i = 0; i < smeared_spectra[1].size(); i++) { smeared_spectra[1][i]->Write(); }

  f_smearings->Close();

  // Retrieving file
  TFile * input_file = new TFile(filename.c_str());

  //Retrieving spectrum
  TH1F * spectrum = (TH1F*) input_file->Get("ch0");

  // Finding peaks

  TSpectrum * sp_pk = new TSpectrum(10);

  Int_t n_found = sp_pk->Search(spectrum, 20,"");

  Double_t * x_peaks = sp_pk->GetPositionX();

  x_peaks = rough_sort(x_peaks, n_found);


  // Compton Background subctraction

  Double_t  * background_bins = new Double_t[n_bins];
  TH1F * filtered_spectrum = new TH1F("h_filtered","Filtered Spectrum",n_bins,0,20000);

  for (size_t i = 0; i < n_bins; i++) { background_bins[i] = spectrum->GetBinContent(i+1); }

  sp_pk->Background(background_bins, n_bins, 50,
                    TSpectrum::kBackDecreasingWindow, TSpectrum::kBackOrder8, kTRUE,
                    TSpectrum::kBackSmoothing5, kTRUE);

  for (size_t i = 0; i < n_bins; i++){ filtered_spectrum->SetBinContent(i+1, (spectrum->GetBinContent(i+1)-background_bins[i])); }

  // Showing Spectrum & Filtered Spectrum

  TCanvas * c3 = new TCanvas("c3");
  spectrum->Draw();
  filtered_spectrum->SetLineColor(kRed);
  filtered_spectrum->Draw("SAME");


  Int_t nPeaks = sp_pk->Search(filtered_spectrum,20, "nobackground",0.05);

  x_peaks = sp_pk->GetPositionX();

  x_peaks = rough_sort(x_peaks, nPeaks);

  Double_t compton_edges_peak[2] = { x_peaks[1], x_peaks[2]};

  // Energy Calibration

  TF1 * fit_measured_peaks;

  Double_t energies[2] = { E_compton_511, E_compton_1275}; // Compton Edge Peak energies
  Double_t shift[2]; // Shifting of the Compton Edge
  Double_t sigma_fall[2];
  Double_t a, b, bPeak, cPeak, nrgPeak, chi2, background, xmin, xmax, chi2min, t_shift, max_calib, max_smooth;

  for (int i = 0; i < 2; i++)
  {
    // Defining fitting window
    xmin = compton_edges_peak [i] - (0.25*compton_edges_peak [i]);
    xmax = compton_edges_peak [i] + (0.25*compton_edges_peak [i]);

    // Fitting measured peaks

    fit_measured_peaks = new TF1("fit_measured_peaks", "gaus", xmin, xmax);
    fit_measured_peaks->SetParameters(1, compton_edges_peak [i]);
    filtered_spectrum->Fit("fit_measured_peaks", "QNO", "", xmin, xmax);
    sigma_fall[i] = fit_measured_peaks->GetParameter(2);
    shift[i] = 0;

  }

  // Copying uncalibrated spectra
  TH1F * calibrated_spectrum = (TH1F*) spectrum->Clone();

  // declaring a smoothed spectra
  TH1F * h_smooth;

  calibrated_spectrum->SetNameTitle("h_calibrated", "Calibrated Spectra");

  std::cout << "-------------------------------------------------------------\n";
  std::cout << "                     Compton Edge peak                       \n";
  std::cout << "-------------------------------------------------------------\n";

  for (int e = 0; e < 2; e++)
  {
    energies[e] = energies[e] - shift[e];
    printf("compton_edges_peak [%d] = %f\tshift[%d] = %f\tnrg[%d] = %f\n",e,compton_edges_peak [e],e,shift[e],e,energies[e]);
    std::cout << std::endl;
  }

  // Linear function ax+b
  TF1 * linear_fitfun = new TF1("calibration_linear_fitfun", "pol1", compton_edges_peak [0], compton_edges_peak [1]);

  linear_fitfun->SetParLimits(0,-200,200);
  linear_fitfun->SetParLimits(1,0.,1.);

  TGraphErrors * graphErr;
  graphErr = new TGraphErrors(2, compton_edges_peak , energies);
  graphErr->Fit(linear_fitfun,"RN QON");

  a = linear_fitfun->GetParameter(1);
  b = linear_fitfun->GetParameter(0);

  // std::cout << "intercepts = " << b << "  angular coefficient = " << a << std::endl;

  calibrated_spectrum->GetXaxis()->SetLimits(b, spectrum->GetBinCenter(n_bins)*a+b);


  // To be refactored
  std::vector<Double_t> h_511_best_par = {0, 0};
  std::vector<Double_t> h_1275_best_par = {0, 0};
  std::vector<std::vector<Double_t>> best_par = { h_511_best_par, h_1275_best_par };

  std::cout << " . . .looping on energy . . ." << std::endl;

  for (int e = 0; e < 2; e++)
  {
    // Setting High chi2 to minimize;
    chi2min = 9999999;

    for (size_t i_smearing = 0; i_smearing < n_smearings; i_smearing++)
    {
      h_smooth = smeared_spectra[e][i_smearing];
      bPeak = h_smooth->GetMaximumBin();
      nrgPeak = ( bPeak * max_bins ) / n_bins;
      t_shift = energies[e] - nrgPeak;
      cPeak = calibrated_spectrum->FindBin( a*compton_edges_peak [e]+b);
      max_calib = calibrated_spectrum->GetBinContent(cPeak);
      max_smooth = h_smooth->GetBinContent(bPeak);

      // defining fit function

      f_profile_class f_profile(h_smooth, max_calib, max_smooth, t_shift);

      TF1 * f_smear_profile = new TF1("f_smear_profile", f_profile, nrgPeak-a*sigma_fall[e], nrgPeak+a*sigma_fall[e]*3,1);
      f_smear_profile->SetParameters(0,600);
      calibrated_spectrum->Fit("f_smear_profile", "QON", "", a*compton_edges_peak [e]+b-a*sigma_fall[e],a*compton_edges_peak [e]+b+a*sigma_fall[e]*3);
      chi2 = f_smear_profile->GetChisquare();
      background = f_smear_profile->GetParameter(0);

      if(chi2 < chi2min)
      {
        chi2min = chi2;
        best_par[e][0] = i_smearing;
        best_par[e][1] = t_shift;
      }

      delete f_smear_profile;

    }

  }

  energies[0] = energies[0] - best_par[0][1];
  energies[1] = energies[1] - best_par[1][1];

  std::cout << compton_edges_peak[0] << " " << energies[0] << std::endl;
  std::cout << compton_edges_peak[1] << " " << energies[1] << std::endl;

  graphErr = new TGraphErrors(2, compton_edges_peak , energies);
  graphErr->Fit(linear_fitfun,"RN QON");

  a = linear_fitfun->GetParameter(1);
  b = linear_fitfun->GetParameter(0);

  calibrated_spectrum->GetXaxis()->SetLimits(b, spectrum->GetBinCenter(n_bins)*a+b);

  TCanvas * c5 = new TCanvas("calibrated_spectrum", "Calibrated Spectrum", 500);
  calibrated_spectrum->Draw();

  // Print out fitting result

  std::cout << "--------------------------------\n";
  std::cout << "          Fitting result        \n";
  std::cout << "--------------------------------\n";
  std::cout << "511 Photon -> 340 keV electron: \n";
  std::cout << std::endl;
  std::cout << "Sigma [keV]: " << best_par[0][0] << std::endl;
  std::cout << "Shifting [keV]: " << best_par[0][1] << std::endl;
  std::cout << std::endl;
  std::cout << "1275 Photon -> 1062 keV electron: \n";
  std::cout << std::endl;
  std::cout << "Sigma [keV]: " << best_par[1][0] << std::endl;
  std::cout << "Shifting [keV]: " << best_par[1][1] << std::endl;

  std::cout << "--------------------------------\n";
  std::cout << "     Calibration Parameters     \n";
  std::cout << "--------------------------------\n";
  std::cout << std::endl << std::endl;
  std::cout << "a [keV/channel] = " << a << std::endl;
  std::cout << "b [keV] = " << b << std::endl;
  std::cout << std::endl;

  std::cout << "All done! Press Ctrl+Alt+C";



  return ;

}

int main(int argc, char **argv)
{
    TApplication app("ROOT Application", &argc, argv);
    smearing_data(app.Argc(), app.Argv());
    app.Run();

    return 0;
}




TH1F * Get_klein_nishina_spectrum(Double_t energy_peak, Double_t E_compton)
{

  // declaring spectrum parameters

  const int KN_NORM = 5000;

  TH1F * h_ideal = new TH1F("h_ideal", "h_ideal", n_bins, 0, max_bins);

  Double_t alpha; // hv/m_e c^2
  Double_t r; //
  Double_t energy; //

  for (size_t j=0 ; j < n_bins ; j++)
  {
    if (j>h_ideal->FindBin(50) && j<h_ideal->FindBin(E_compton))
    {
      r = h_ideal->GetBinCenter(j)/energy_peak;
      alpha = energy_peak/511.0;

      // Klein-Nishina dsigma/dT Differential cross-section
      energy = KN_NORM * (2+r*r/(alpha*alpha*(1-r)*(1-r))+r/(1-r) * (r-2/alpha));
      h_ideal->SetBinContent(j,energy);
    }
    else
    {
        h_ideal->SetBinContent(j,0);
    }
  }

  return h_ideal;
}

std::vector<TH1F*> Gaussian_smearing(TH1F * spectrum, Double_t energy_peak, Double_t E_compton, Int_t N)
{

  std::vector<TH1F*> smeared_spectra;

  std::string smoothName;
  Double_t height;

  for (size_t i_smearing = 1; i_smearing <= N; i_smearing++)
  {
    smoothName = "smooth_"+std::to_string((Int_t)energy_peak)+"_"+std::to_string(i_smearing);
    TH1F * h_smooth = new TH1F(smoothName.c_str(),smoothName.c_str(),n_bins,0,max_bins);
    std::cout << "Creating: " << smoothName << std::endl;

    for (size_t j = 0; j <  spectrum->FindBin(E_compton) ; j++)
    {
      height = spectrum->GetBinContent(j);
      for (size_t k = 0; k < height; k++)
      {
        h_smooth->Fill(gRandom->Gaus(spectrum->GetBinCenter(j), i_smearing));
      }
    }

    smeared_spectra.push_back(h_smooth);

  }

  return smeared_spectra;
}


Double_t * rough_sort(Double_t * array, Int_t n)
{

  std::vector<Double_t> array_cp;

  for (size_t i = 0; i < n; i++) { array_cp.push_back(array[i]); }

  // sorting through std vector inner algs

  sort(array_cp.begin(),array_cp.end());

  for (size_t i = 0; i < n; i++) { array[i] = array_cp[i]; }

  return array;
}
