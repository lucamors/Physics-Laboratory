
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

  for(int i = 0; i < 50; i++)
  {
    if( i < (percent/2))
    {
      bar.replace(i,1,"=");
    }
    else if( i == (percent/2))
    {
      bar.replace(i,1,">");
    }
    else
    {
      bar.replace(i,1," ");
    }
  }

  cout<< "\r" "[" << bar << "] ";
  cout.width( 3 );
  cout<< percent << "%     " << std::flush;
}


void counter(string data_filename, string output_file_name)
{
  // Retrieving File
  TFile * data_file = new TFile(data_filename.c_str());

	TTree * daq_tree;

  // Retrieving TTree from TFile

  daq_tree = (TTree*) data_file->Get("acq_tree_0");

  // Linking

	cleanEv * event_ch_A = new cleanEv;
	cleanEv * event_ch_B = new cleanEv;

	TBranch * branch_ch_A, * branch_ch_B;
	branch_ch_A = daq_tree->GetBranch("acq_ch0");
	branch_ch_A->SetAddress(&event_ch_A);

	branch_ch_B = daq_tree->GetBranch("acq_ch2");
	branch_ch_B->SetAddress(&event_ch_B);

  // Tagger
	TH1F * Energy_spectrum_ch0 = new TH1F("ch0","ch0", 2000, 0, 2000);

  // Detector
	TH1F * Energy_spectrum_ch2 = new TH1F("ch2","ch2", 2000, 0, 2000);

	// coincidences Spectra
	// ---------------------------------------------------------------------------
	// Tagger
	TH1F * Energy_spectrum_ch0_coinc = new TH1F("ch0_coinc","ch0", 2000, 0, 2000);

  // Detector
	TH1F * Energy_spectrum_ch2_coinc = new TH1F("ch2_coinc","ch2", 2000, 0, 2000);
	// ---------------------------------------------------------------------------


  // Correlation graph
  TH2F * Correlation = new TH2F("correlation", "Correlation", 2000,0,2000,2000,0,2000);
	TH2F * Correlation_coinc = new TH2F("correlation_coinc", "Correlation_coinc", 2000,0,2000,2000,0,2000);

	long int number_of_ev = (long int) branch_ch_A->GetEntries();

  // Declaring Saving ttree structure
  TFile * outfile = new TFile(output_file_name.c_str(),"RECREATE");

	// Declaring array for fitting

	std::vector<double> scatterer_x;
	std::vector<double> detector_y;

	// energy vars

	double tagger_energy, scatterer_energy, detector_energy;


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



			tagger_energy = event_ch_A->qlong*0.0544615-7.32807;
			detector_energy = event_ch_B->qlong*0.0531775-6.78006;

			//Filling Spectra
			Energy_spectrum_ch0->Fill(tagger_energy);
			Energy_spectrum_ch2->Fill(detector_energy);

			Correlation->Fill(tagger_energy, detector_energy);

			if ( tagger_energy > 490 and tagger_energy < 520 )
			{

				if( 1 )
				{
					Energy_spectrum_ch0_coinc->Fill(tagger_energy);
					Energy_spectrum_ch2_coinc->Fill(detector_energy);
					Correlation_coinc->Fill(tagger_energy, detector_energy);
				}

			}
  }

	// Saving whole Spectra
	Energy_spectrum_ch0->Write();
	Energy_spectrum_ch2->Write();
	Correlation->Write();

	// Saving coincidences Spectra
	Energy_spectrum_ch0_coinc->Write();
	Energy_spectrum_ch2_coinc->Write();
	Correlation_coinc->Write();


	outfile->Close();


  return ;
}
