void fit(string input, string name, string output)
{

	TGraph * calibration_params = new TGraph(input.c_str());

	// Calibration Points

	calibration_params->GetXaxis()->SetTitle("Channels [a.u.]");
	calibration_params->GetYaxis()->SetTitle("Energy [keV]");
	calibration_params->SetTitle(name.c_str());
	calibration_params->SetMarkerStyle(20);
	calibration_params->SetMarkerColor(kRed);

	// Fit Function

	TF1 * linear = new TF1("f1","[0]+[1]*x");
	linear->SetLineColor(kBlue);

	// Fit & Drawing

	calibration_params->Fit("f1");
	calibration_params->Draw();

	// Saving to file

	TFile * _outfile = new TFile((output+".root").c_str(), "RECREATE");

	calibration_params->Write();

	_outfile->Close();


	return 0;
}
