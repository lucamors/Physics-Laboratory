void calibrazione(char *infilename, char* outfile_name)
{
	// canali non calibrati
	float	ch0_sp;
	float	ch1_sp;
	float	ch2_sp;
	float ch3_sp;

	// canali calibrati
	float	cal_ch0_sp;
	float	cal_ch1_sp;
	float	cal_ch2_sp;
	float cal_ch3_sp;

	// dichiaro un file in input
	TFile *input_file = new TFile(infilename);

	// recupero la n-tupla
	TNtuple *input_ntuple = (TNtuple*)input_file->Get("pjmca");
	int nEntries = (int)input_ntuple->GetEntries();

	// link variabili con rami dell'n-tupla non calibrata
	input_ntuple->SetBranchAddress("ch0", &ch0_sp);
	input_ntuple->SetBranchAddress("ch1", &ch1_sp);
	input_ntuple->SetBranchAddress("ch2", &ch2_sp);
	input_ntuple->SetBranchAddress("ch3", &ch3_sp);

	TH1F* h_ch0_spectrum = new TH1F("ch1_spectrum", "ch1_spectrum", 2048, 0, 2048);
	TH1F* h_ch1_spectrum = new TH1F("ch2_spectrum", "ch2_spectrum",2048,0,2048);
	TH1F* h_ch2_spectrum = new TH1F("ch3_spectrum", "ch3_spectrum", 2048, 0, 2048);
	TH1F* h_ch3_spectrum = new TH1F("ch4_spectrum", "ch4_spectrum", 2048, 0, 2048);

	for (size_t i = 0; i < nEntries; i++)
	{
		input_ntuple->GetEntry(i);
		h_ch0_spectrum->Fill(ch0_sp);
		h_ch1_spectrum->Fill(ch1_sp);
		h_ch2_spectrum->Fill(ch2_sp);
		h_ch3_spectrum->Fill(ch3_sp);
	}

	TFile * outfile = new TFile(outfile_name);

	h_ch0_spectrum->Write();
	h_ch1_spectrum->Write();
	h_ch2_spectrum->Write();
	h_ch3_spectrum->Write();

	outfile->Close();

	// h_ch2_spectrum->GetXaxis()->Set(2048,20.8612,20.8612+2048*1.26869);
	// h_ch2_spectrum->Draw();

	return ;
}

