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
  	histo_ch_2->GetYaxis()->SetTitle("Count [n.u.]");

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
		histo_0->GetXaxis()->SetRangeUser(10,30);
		histo_0->GetYaxis()->SetRangeUser(0,0.035);
    histo_0->SetLineColor(kRed);
		histo_0->Draw("SAME");
    histo_1->SetLineColor(kGreen);
    histo_1->Draw("SAME");
    histo_2->SetLineColor(kOrange);
    histo_2->Draw("SAME");
    histo_3->SetLineColor(kBlue);
    histo_3->Draw("SAME");

		TF1 *GausPeak_0 = new TF1("GPeak_0","gaus", 22.5, 26.5);
		histo_0->Fit(GausPeak_0,"RN");

		TF1 *GausPeak_1 = new TF1("GPeak_1","gaus", 19, 23);
		histo_1->Fit(GausPeak_1,"RN");

		TF1 *GausPeak_2 = new TF1("GPeak_2","gaus", 15.6, 19.6);
		histo_2->Fit(GausPeak_2,"RN");

		TF1 *GausPeak_3 = new TF1("GPeak_3","gaus", 12.6, 16.6);
		histo_3->Fit(GausPeak_3,"RN");

		GausPeak_0->SetLineColor(kRed+2);
		GausPeak_0->SetLineWidth(3);
		GausPeak_0->SetLineStyle(9);
		GausPeak_0->Draw("SAME");

		GausPeak_1->SetLineColor(kGreen+4);
		GausPeak_1->SetLineWidth(3);
		GausPeak_1->SetLineStyle(9);
		GausPeak_1->Draw("SAME");

		GausPeak_2->SetLineColor(kOrange+4);
		GausPeak_2->SetLineWidth(3);
		GausPeak_2->SetLineStyle(9);
		GausPeak_2->Draw("SAME");

		GausPeak_3->SetLineColor(kBlue+3);
		GausPeak_3->SetLineWidth(3);
		GausPeak_3->SetLineStyle(9);
		GausPeak_3->Draw("SAME");

		TArrow *ar_0 = new TArrow(GausPeak_0->GetParameter(1),0.025,GausPeak_1->GetParameter(1),0.025,0.005,"<|>");
		        ar_0->Draw();

		TArrow *ar_1 = new TArrow(GausPeak_1->GetParameter(1),0.025,GausPeak_2->GetParameter(1),0.025,0.005,"<|>");
		        ar_1->Draw();

		TArrow *ar_2 = new TArrow(GausPeak_2->GetParameter(1),0.025,GausPeak_3->GetParameter(1),0.025,0.005,"<|>");
		        ar_2->Draw();

		TLine  *ln_0 = new TLine(GausPeak_0->GetParameter(1),0,GausPeak_0->GetParameter(1),0.035);
            ln_0->SetLineStyle(7);
		        ln_0->Draw();
						TLine  *ln_1 = new TLine(GausPeak_1->GetParameter(1),0,GausPeak_1->GetParameter(1),0.035);
				            ln_1->SetLineStyle(7);
						        ln_1->Draw();
										TLine  *ln_2 = new TLine(GausPeak_2->GetParameter(1),0,GausPeak_2->GetParameter(1),0.035);
														ln_2->SetLineStyle(7);
														ln_2->Draw();
														TLine  *ln_3 = new TLine(GausPeak_3->GetParameter(1),0,GausPeak_3->GetParameter(1),0.035);
																		ln_3->SetLineStyle(7);
																		ln_3->Draw();

																ofstream temp("temp.dat");
																temp<<GausPeak_3->GetParameter(1)<<" 0 "<<GausPeak_3->GetParError(1)<<"  0.144338"<<endl;
																temp<<GausPeak_2->GetParameter(1)<<" 42.5 "<<GausPeak_2->GetParError(1)<<" 0.144338"<<endl;
																temp<<GausPeak_1->GetParameter(1)<<" 92.5 "<<GausPeak_1->GetParError(1)<<" 0.144338"<<endl;
																temp<<GausPeak_0->GetParameter(1)<<" 142.5 "<<GausPeak_0->GetParError(1)<<" 0.144338"<<endl;
																temp.close();

                                TCanvas *Fit = new TCanvas("fit","fit", 1920, 1080);
																TGraphErrors *Graph = new TGraphErrors("temp.dat","%lg%lg%lg%lg");
																Graph->GetXaxis()->SetTitle("TAC [ns]");
																Graph->GetYaxis()->SetTitle("Source position [cm]");
																Graph->GetXaxis()->SetLimits(10, 29);
																Graph->GetYaxis()->SetLimits(-5, 150);

																Graph->GetYaxis()->SetTitleSize(0.06);
																Graph->GetYaxis()->SetTitleOffset(0.8);
																Graph->GetYaxis()->SetLabelSize(0.05);
																Graph->GetYaxis()->SetNdivisions(502);

																Graph->GetXaxis()->SetTitleSize(0.06);
																Graph->GetXaxis()->SetLabelSize(0.04);
																Graph->GetXaxis()->SetTitleOffset(0.75);

																Graph->SetLineWidth(1);
																Graph->Draw();
  return ;
}
