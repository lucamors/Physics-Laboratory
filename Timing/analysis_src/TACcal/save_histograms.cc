/*
*	Root Macro for reading Timing Experiment Data.
*
*	Data Acquisition System: Ve.R.D.I.
*	Digitizer: CAEN 1Gs
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


void save_histograms(string infilename, string outfile_name)
{

	acqEventPSD_t inc_data_ch0;
	acqEventPSD_t inc_data_ch1;
	acqEventPSD_t inc_data_ch2;
	acqEventPSD_t inc_data_ch3;


	TFile * incfile = new TFile(infilename.c_str());

	TTree *inctree = (TTree*) incfile->Get("acq_tree_0");

	TBranch * incbranch_0;
	TBranch * incbranch_1;
	TBranch * incbranch_2;
	TBranch * incbranch_3;

	// Detector #1
	incbranch_0 = inctree->GetBranch("acq_ch0");
	incbranch_0->SetAddress(&inc_data_ch0.timetag);

	// Detector #2
	incbranch_1 = inctree->GetBranch("acq_ch1");
	incbranch_1->SetAddress(&inc_data_ch1.timetag);

	// TAC
	incbranch_2 = inctree->GetBranch("acq_ch2");
	incbranch_2->SetAddress(&inc_data_ch2.timetag);

	// CH3 - empty channel
	// incbranch_3 = inctree->GetBranch("acq_ch4");
	// incbranch_3->SetAddress(&inc_data_ch3.timetag);


	psd_params_t params;
	TBranch *config = inctree->GetBranch("psd_params");
	config->SetAddress(&params.channel);
	config->GetEntry(0);


	long int tot_event_ch0 = incbranch_0->GetEntries();
	long int tot_event_ch1 = incbranch_1->GetEntries();
	long int tot_event_ch2 = incbranch_2->GetEntries();
	// long int tot_event_ch3 = incbranch_3->GetEntries();

	long int counter_ch0 = 0;
	long int counter_ch1 = 0;
	long int counter_ch2 = 0;

	// CH0 SPECTRUM
	TH1F * ch0_spectrum = new TH1F("ch0", "Channel 0", 1000, 0, 20000);
	ch0_spectrum->GetXaxis()->SetTitle("Energy [a.u.]");
	ch0_spectrum->GetYaxis()->SetTitle("Counts #");

	// CH1 SPECTRUM
	TH1F * ch1_spectrum = new TH1F("ch1", "Channel 1", 1000, 0, 20000);
	ch1_spectrum->GetXaxis()->SetTitle("Energy [a.u.]");
	ch1_spectrum->GetYaxis()->SetTitle("Counts #");

	// CH2 SPECTRUM
	TH1F * ch2_spectrum = new TH1F("ch2", "Channel 2", 1000, 0, 100000);
	ch2_spectrum->GetXaxis()->SetTitle("Time [a.u.]");
	ch2_spectrum->GetYaxis()->SetTitle("Counts #");

	// Scanning over Branch ch0
	while( counter_ch0 < tot_event_ch0)
	{

		incbranch_0->GetEntry(counter_ch0);

		ch0_spectrum->Fill(inc_data_ch0.qlong);

		counter_ch0++;

	}

	// Scanning over Branch ch1
	while( counter_ch1 < tot_event_ch1)
	{

		incbranch_1->GetEntry(counter_ch1);

		ch1_spectrum->Fill(inc_data_ch1.qlong);

		counter_ch1++;

	}

	// Scanning over Branch ch2
	while( counter_ch2 < tot_event_ch2)
	{

		incbranch_2->GetEntry(counter_ch2);

		ch2_spectrum->Fill(inc_data_ch2.qlong);

		counter_ch2++;

	}

	// Saving Histograms into a root file.
	TFile * outfile = new TFile(outfile_name.c_str(), "RECREATE");

	ch0_spectrum->Write();
	ch1_spectrum->Write();
	ch2_spectrum->Write();

	outfile->Close();

	return ;
}
