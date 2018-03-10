
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

void printProgBar( int percent )
{
  string bar;

  for(int i = 0; i < 50; i++){
    if( i < (percent/2)){
      bar.replace(i,1,"=");
    }else if( i == (percent/2)){
      bar.replace(i,1,">");
    }else{
      bar.replace(i,1," ");
    }
  }

  cout<< "\r" "[" << bar << "] ";
  cout.width( 3 );
  cout<< percent << "%     " << std::flush;
}


void plotter(string input_file_name, string output_file_name)
{
  // Retrieving File
  TFile * input_file = new TFile(input_file_name.c_str());

	TTree * daq_tree;

  // Retrieving TTree from TFile

  daq_tree = (TTree*) input_file->Get("acq_tree_0");

  // Linking

	cleanEv * event_ch_A = new cleanEv;
	cleanEv * event_ch_B = new cleanEv;
	cleanEv * event_ch_C = new cleanEv;


	TBranch * branch_ch_A, * branch_ch_B, *branch_ch_C;
	branch_ch_A = daq_tree->GetBranch("acq_ch0");
	branch_ch_A->SetAddress(&event_ch_A);
	branch_ch_B = daq_tree->GetBranch("acq_ch1");
	branch_ch_B->SetAddress(&event_ch_B);
	branch_ch_C = daq_tree->GetBranch("acq_ch2");
	branch_ch_C->SetAddress(&event_ch_C);

  // Tagger
	TH1F * Energy_spectrum_ch0 = new TH1F("ch0","ch0", 2000, 0, 2000);
  // Scatterer
	TH1F * Energy_spectrum_ch1 = new TH1F("ch1","ch1", 2000, 0, 2000);
  // Detector
	TH1F * Energy_spectrum_ch2 = new TH1F("ch2","ch2", 2000, 0, 2000);


  // Correlation graph
  TH2F * Correlation = new TH2F("correlation", "Correlation", 2000,0,2000,2000,0,2000);

	long int number_of_ev = (long int) branch_ch_A->GetEntries();

  // Declaring Saving ttree structure
  TFile * outfile = new TFile(output_file_name.c_str(),"RECREATE");

  for (int counter = 0; counter < number_of_ev; counter++)
  {
			if ( counter % (number_of_ev/100) == 0)
      {

         system("clear");
         std::cout << "Loading Event " << counter << endl;
         printProgBar ((int)((counter*100.0)/number_of_ev));
      }

      branch_ch_A->GetEntry(counter);
      branch_ch_B->GetEntry(counter);
			branch_ch_C->GetEntry(counter);


			Energy_spectrum_ch0->Fill(event_ch_A->qlong*0.0544615-7.32807);
      Energy_spectrum_ch1->Fill(event_ch_B->qlong*0.0577517-7.81986);
			Energy_spectrum_ch2->Fill(event_ch_C->qlong*0.0531775-6.78006);

      if (event_ch_A->qlong*0.0544615-7.32807 < 540 and event_ch_A->qlong*0.0544615-7.32807 > 480)
      {
        Correlation->Fill(event_ch_B->qlong*0.0577517-7.81986, event_ch_C->qlong*0.0531775-6.78006);
      }

    }

    Correlation->Write();

		outfile->Close();

    return ;
}
