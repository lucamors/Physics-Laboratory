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


void calibration(string infilename)
{

	acqEventPSD_t inc_data_ch0;
	acqEventPSD_t inc_data_ch1;
	acqEventPSD_t inc_data_ch2;
	acqEventPSD_t inc_data_ch3;


	TFile * incfile = new TFile(infilename.c_str());

	TTree *inctree = (TTree*) incfile->Get("acq_tree_0");

	TBranch * incbranch_1;
	TBranch * incbranch_2;
	TBranch * incbranch_3;
	TBranch * incbranch_4;


	incbranch_1 = inctree->GetBranch("acq_ch0");
	incbranch_1->SetAddress(&inc_data_ch0.timetag);
	//
	// incbranch_2 = inctree->GetBranch("acq_ch1");
	// incbranch_2->SetAddress(&inc_data_ch1.timetag);
	//
	// incbranch_3 = inctree->GetBranch("acq_ch2");
	// incbranch_3->SetAddress(&inc_data_ch2.timetag);
	//
	// incbranch_4 = inctree->GetBranch("acq_ch4");
	// incbranch_4->SetAddress(&inc_data_ch3.timetag);


	// psd_params_t params;
	// TBranch *config = inctree->GetBranch("psd_params");
	// config->SetAddress(&params.channel);
	// config->GetEntry(0);


	long int tot_event_ch0 = incbranch_1->GetEntries();

	long int counter_ch0 = 0;


	TH1F * ch0_spectrum = new TH1F("CH0", "CH0", 2048, 0, 30000);

	while( counter_ch0 < tot_event_ch0)
	{

		incbranch_1->GetEntry(counter_ch0);

		ch0_spectrum->Fill(inc_data_ch0.qlong);

		counter_ch0++;

	}

	TFile * outfile = new TFile("prova.root", "RECREATE");

	ch0_spectrum->Write();

	outfile->Close();



	return ;
}
