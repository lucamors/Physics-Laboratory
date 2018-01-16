#include <string>
#include <vector>


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


TH1F * ReadData(string infile_name , int ch){

    // Retrieving data
  	acqEventPSD_t inc_data_ch0;
  	acqEventPSD_t inc_data_ch1;
  	acqEventPSD_t inc_data_ch2;
  	acqEventPSD_t inc_data_ch3;

  	TFile * incfile = new TFile(infile_name.c_str());

  	TTree *inctree = (TTree*) incfile->Get("acq_tree_0");

  	TBranch * incbranch_0;
  	TBranch * incbranch_1;
  	TBranch * incbranch_2;


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


  	long int tot_event = incbranch_2->GetEntries();

  	long int counter = 0;

    TH1F * histo_ch_0 = new TH1F((infile_name+"_ch0").c_str() , "Ch0" , 1000, 0, 1400);
    TH1F * histo_ch_1 = new TH1F((infile_name+"_ch1").c_str() , "Ch1" , 1000, 0, 1400);
    TH1F * histo_ch_2 = new TH1F((infile_name+"_ch2").c_str() , "Ch2" , 1000, 0, 30);

    histo_ch_0->GetXaxis()->SetTitle("Energy [keV]");
  	histo_ch_0->GetYaxis()->SetTitle("Count");
    histo_ch_1->GetXaxis()->SetTitle("Energy [keV]");
  	histo_ch_1->GetYaxis()->SetTitle("Count");
    histo_ch_2->GetXaxis()->SetTitle("TAC [ns]");
  	histo_ch_2->GetYaxis()->SetTitle("Count");

	  histo_ch_0->SetStats(kFALSE);
    histo_ch_1->SetStats(kFALSE);
    histo_ch_2->SetStats(kFALSE);

		Double_t TAC      = 0;
    Double_t Energy_0 = 0;
    Double_t Energy_1 = 0;
  	// Scanning over Branches
  	while( counter < tot_event)
  	{

  		incbranch_0->GetEntry(counter);
      incbranch_1->GetEntry(counter);
      incbranch_2->GetEntry(counter);

      TAC      = inc_data_ch2.qlong*0.000735898;         //calibrated
      Energy_0 = inc_data_ch0.qlong*0.0754713-49.1682;   //calibrated
      Energy_1 = inc_data_ch1.qlong*0.0824706-17.8629;   //calibrated

  		histo_ch_0->Fill(Energy_0);
      histo_ch_1->Fill(Energy_1);
      histo_ch_2->Fill(TAC);

  		counter++;

  	}

    return histo_ch_2;


}


void Overlay(string infile_name_0, string infile_name_1, string infile_name_2, string infile_name_3)
{

    TH1F * histo_0 = ReadData(infile_name_0,2);
    TH1F * histo_1 = ReadData(infile_name_1,2);
    TH1F * histo_2 = ReadData(infile_name_2,2);
    TH1F * histo_3 = ReadData(infile_name_3,2);


    
    histo_0->Scale(1/histo_0->GetEntries());
		histo_1->Scale(1/histo_1->GetEntries());
		histo_2->Scale(1/histo_2->GetEntries());

		histo_3->Scale(1/histo_3->GetEntries());
    

    TCanvas * C = new TCanvas("c0","c0", 1920, 1080);

    histo_0->SetLineColor(kRed);
		histo_0->Draw("SAME");
    histo_1->SetLineColor(kGreen);
    histo_1->Draw("SAME");
    histo_2->SetLineColor(kOrange);
    histo_2->Draw("SAME");
    histo_3->SetLineColor(kBlue);\
    histo_3->Draw("SAME");

  return ;
}
