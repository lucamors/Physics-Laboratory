/*
*	Root Macro for reading Compton Experiment Data.
*
*	Data Acquisition System: Ve.R.D.I.
*	Digitizer: CAEN
*
*/

#include <iostream>
#include <string>


struct acqEventPSD_t {

	ULong64_t	timetag;
	UInt_t		baseline;
	UShort_t	qshort;
	UShort_t	qlong;
	UShort_t	pur;
	UShort_t	samples[4096];
};


struct psd_params_t {
	UInt_t		channel;
	UInt_t		threshold;
	UInt_t		pretrigger;
	UInt_t		pregate;
	UInt_t		shortgate;
	UInt_t		longgate;
	UInt_t		numsamples;
};


class cleanEv : public TObject
{
	public:

		long int timetag;
		short qshort;
		short qlong;
		short samples[4096];


		ClassDef(cleanEv, 2)

};

ClassImp(cleanEv)


void sum(string infilename, string outfile_name)
{

	// Retrieving File
	TFile * data_file = new TFile(infilename.c_str());

	TTree * daq_tree;

	// Retrieving TTree from TFile

	daq_tree = (TTree*) data_file->Get("acq_tree_0");

	// Linking

	cleanEv * event_ch_A = new cleanEv;
	cleanEv * event_ch_B = new cleanEv;
	cleanEv * event_ch_C = new cleanEv;
	cleanEv * event_ch_D = new cleanEv;

	TBranch * branch_ch_A, * branch_ch_B,* branch_ch_C, * branch_ch_D ;

	branch_ch_A = daq_tree->GetBranch("acq_ch0");
	branch_ch_A->SetAddress(&event_ch_A);

	branch_ch_B = daq_tree->GetBranch("acq_ch1");
	branch_ch_B->SetAddress(&event_ch_B);

	branch_ch_C = daq_tree->GetBranch("acq_ch2");
	branch_ch_C->SetAddress(&event_ch_C);

	branch_ch_D = daq_tree->GetBranch("acq_ch3");
	branch_ch_D->SetAddress(&event_ch_D);


	long int tot_event_ch0 = branch_ch_A->GetEntries();
	long int tot_event_ch1 = branch_ch_B->GetEntries();
	long int tot_event_ch2 = branch_ch_C->GetEntries();
	long int tot_event_ch3 = branch_ch_D->GetEntries();

	long int counter_ch0 = 0;
	long int counter_ch1 = 0;
	long int counter_ch2 = 0;
	long int counter_ch3 = 0;


	// SUM SPECTRUM

	TH1F * sum_spectrum_2 = new TH1F("ch_sum", "sum", 1000, 0, 2000);
	TH1F * sum_spectrum_3 = new TH1F("ch_sum", "sum", 1000, 0, 2000);
	sum_spectrum_2->GetXaxis()->SetTitle("Energy [keV]");
	sum_spectrum_2->GetYaxis()->SetTitle("Counts #");
	sum_spectrum_3->GetXaxis()->SetTitle("Energy [keV]");
	sum_spectrum_3->GetYaxis()->SetTitle("Counts #");

  double q1, q2, q3;
	// Scanning over Branch ch0
	while( counter_ch0 < tot_event_ch0)
	{

		branch_ch_A->GetEntry(counter_ch0);
		branch_ch_B->GetEntry(counter_ch1);
		branch_ch_C->GetEntry(counter_ch2);

		q1=event_ch_A->qlong*0.0726917-23.9426;
		q2=event_ch_B->qlong*0.0735321-24.8858;
		q3=event_ch_C->qlong*0.0714929-26.3991;

		sum_spectrum_2->Fill(q1+q2);
		sum_spectrum_2->Fill(q1+q2+q3);

		counter_ch0++;
		counter_ch1++;
		counter_ch2++;
  }


	// Saving Histograms into a root file.
	TFile * outfile = new TFile(outfile_name.c_str(), "RECREATE");

	sum_spectrum_2->Write();
	sum_spectrum_3->Write();

	outfile->Close();

	return ;
}
