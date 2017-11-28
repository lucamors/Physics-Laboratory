struct acqEventPSD_t {

	ULong64_t	timetag;
	UInt_t		baseline;
	UShort_t	qshort;
	UShort_t	qlong;
	UShort_t	pur;
	UShort_t	samples[4096];
};

class cleanEv : public TObject
{
	public:

		long int timetag;
		short qshort;
		short qlong;
		short samples[4096];


		ClassDef(cleanEv, 1)

};

ClassImp(cleanEv)

void read_clean(string filename)
{
		// Retrieving File
		TFile * input_file = new TFile(filename.c_str());

		TTree * daq_tree;

	  daq_tree = (TTree*) input_file->Get("acq_tree_0");

	  // Linking

	  cleanEv * event_ch_A = new cleanEv;
		cleanEv * event_ch_B = new cleanEv;

	  TBranch * branch_ch_A, * branch_ch_B;
	  branch_ch_A = daq_tree->GetBranch("acq_ch0");
	  branch_ch_A->SetAddress(&event_ch_A);
	  branch_ch_B = daq_tree->GetBranch("acq_ch1");
	  branch_ch_B->SetAddress(&event_ch_B);

		for (size_t i = 0; i < 100; i++)
		{

			branch_ch_A->GetEntry(i);
			branch_ch_B->GetEntry(i);

			

		}


  return ;
}
