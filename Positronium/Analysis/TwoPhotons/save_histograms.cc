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


void save_histograms(string infilename, string outfile_name)
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

	// CH0 SPECTRUM
	TH1F * ch0_spectrum = new TH1F("ch0", "Channel 0", 1000, 0, 35000);  //3500 bin for 2 coinc
	ch0_spectrum->GetXaxis()->SetTitle("Energy [a.u.]");
	ch0_spectrum->GetYaxis()->SetTitle("Counts #");

	// CH1 SPECTRUM
	TH1F * ch1_spectrum = new TH1F("ch1", "Channel 1", 1000, 0, 35000);
	ch1_spectrum->GetXaxis()->SetTitle("Energy [a.u.]");
	ch1_spectrum->GetYaxis()->SetTitle("Counts #");

	// CH2 SPECTRUM
	TH1F * ch2_spectrum = new TH1F("ch2", "Channel 2", 1000, 0, 35000);
	ch2_spectrum->GetXaxis()->SetTitle("Energy [a.u.]");
	ch2_spectrum->GetYaxis()->SetTitle("Counts #");

	// CH3 SPECTRUM
	TH1F * ch3_spectrum = new TH1F("ch3", "Channel 3", 3500, 0, 35000);
	ch3_spectrum->GetXaxis()->SetTitle("TAC [a.u.]");
	ch3_spectrum->GetYaxis()->SetTitle("Counts #");

	// Scanning over Branch ch0
	while( counter_ch0 < tot_event_ch0)
	{

		branch_ch_A->GetEntry(counter_ch0);

		ch0_spectrum->Fill(event_ch_A->qlong);

		counter_ch0++;

	}

	// Scanning over Branch ch1
	while( counter_ch1 < tot_event_ch1)
	{

		branch_ch_B->GetEntry(counter_ch1);

		ch1_spectrum->Fill(event_ch_B->qlong);

		counter_ch1++;

	}

	// Scanning over Branch ch2
	while( counter_ch2 < tot_event_ch2)
	{

		branch_ch_C->GetEntry(counter_ch2);

		ch2_spectrum->Fill(event_ch_C->qlong);

		counter_ch2++;

	}

	// Scanning over Branch ch0
	while( counter_ch3 < tot_event_ch3)
	{

		branch_ch_C->GetEntry(counter_ch3);

		ch3_spectrum->Fill(event_ch_D->qlong);

		counter_ch3++;

	}

	// Saving Histograms into a root file.
	TFile * outfile = new TFile(outfile_name.c_str(), "RECREATE");

	ch0_spectrum->Write();
	ch1_spectrum->Write();
	ch2_spectrum->Write();
	ch3_spectrum->Write();

	outfile->Close();

	return ;
}
