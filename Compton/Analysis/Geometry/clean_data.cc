/*
*
* Reading TTree from CAEN Digitizer
* Channel aligning
*/

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

void clean_data(string input_file_name, string outfile_name)
{

	// Retrieving File
  TFile * input_file = new TFile(input_file_name.c_str());

	TTree * daq_tree;

  // Retrieving TTree from TFile

  daq_tree = (TTree*) input_file->Get("acq_tree_0");

  // Linking

  acqEventPSD_t event_ch_A, event_ch_B, event_ch_C;


  TBranch * branch_ch_A, * branch_ch_B, *branch_ch_C;
  branch_ch_A = daq_tree->GetBranch(Form("acq_ch%i",0));
  branch_ch_A->SetAddress(&event_ch_A.timetag);
  branch_ch_B = daq_tree->GetBranch(Form("acq_ch%i",1));
  branch_ch_B->SetAddress(&event_ch_B.timetag);
	branch_ch_C = daq_tree->GetBranch(Form("acq_ch%i",2));
	branch_ch_C->SetAddress(&event_ch_C.timetag);

  // Align channels

  long int nEventsA = (long int) branch_ch_A->GetEntries();
  long int nEventsB = (long int) branch_ch_B->GetEntries();
	long int nEventsC = (long int) branch_ch_C->GetEntries();

	// Declaring Saving ttree structure

	cleanEv * event_ch_A_clean = new  cleanEv;
	cleanEv * event_ch_B_clean = new  cleanEv;
	cleanEv * event_ch_C_clean = new  cleanEv;

	TFile * outfile = new TFile(outfile_name.c_str(),"RECREATE");

	TTree * out_tree = new TTree("acq_tree_0", "Dati Allineati");

	out_tree->Branch("acq_ch0",&event_ch_A_clean);
	out_tree->Branch("acq_ch1",&event_ch_B_clean);
	out_tree->Branch("acq_ch2",&event_ch_C_clean);

	// aligning channels

	int counter_A = 0;
	int skipA = 0;
	int counter_B = 0;
	int skipB = 0;
	int counter_C = 0;
	int skipC = 0;
	int coincident_events = 0;

	int id_A = 0;
	int id_B = 0;
	int id_C = 0;

	long int time_difference = 0;


	while(counter_A < nEventsA and counter_B < nEventsB and counter_C < nEventsC)
	{

	    // Display Progress

	    if (counter_A % (nEventsA/100) == 0 or counter_B % (nEventsB/100) == 0 or counter_C % (nEventsC/100) == 0)
	    {
	        system("clear");
	        std::cout << "Loading Event " << counter_A << endl;
	        std::cout << "-----------------------\n";
	        std::cout << "Load "
	                  <<  (int)((counter_A*100.0)/nEventsA)
	                  << "% in channel A"
	                  << std::endl;
	        std::cout << "Load "
	                  <<  (int)((counter_B*100.0)/nEventsB)
	                  << "% in channel B"
	                  << std::endl;
	        std::cout << "Load "
	                  <<  (int)((counter_C*100.0)/nEventsC)
	                  << "% in channel C"
	                  << std::endl;
	    }

	    // Events filling

	    branch_ch_A->GetEntry(counter_A);
	    branch_ch_B->GetEntry(counter_B);
	    branch_ch_C->GetEntry(counter_C);
      std::map<ULong64_t, int> Times;
		  Times[event_ch_A.timetag]=0;
		  Times[event_ch_B.timetag]=1;
		  Times[event_ch_C.timetag]=2;

			std::vector<ULong64_t> Timetag = {event_ch_A.timetag,event_ch_B.timetag,event_ch_C.timetag};
			std::sort(Timetag.begin(),Timetag.end());

			ULong64_t TimeDiff=Timetag[2]-Timetag[0];


	    if(TimeDiff > 12.5) //  <--- scegli la finestra di allinamento in sample 1sample=4ns
	    {
				  switch(Times[Timetag[0]]){
						case 0:
						counter_A++; skipA++; break;
						case 1:
						counter_B++; skipB++; break;
						case 2:
						counter_C++; skipC++; break;
					}
	    }
	    else
	    {

				event_ch_A_clean->timetag = event_ch_A.timetag;
				event_ch_A_clean->qlong = event_ch_A.qlong;

				event_ch_B_clean->timetag = event_ch_B.timetag;
				event_ch_B_clean->qlong = event_ch_B.qlong;

				event_ch_C_clean->timetag = event_ch_C.timetag;
				event_ch_C_clean->qlong = event_ch_C.qlong;


				out_tree->Fill();

	      // updating counters

				coincident_events++;
	      id_A++;
	      id_B++;
				id_C++;
	      counter_A++;
	      counter_B++;
        counter_C++;
    }
	}
  cout<<"coincidences "<<coincident_events<<endl<<"tot events "<<nEventsA<<endl;
	out_tree->Write();
	outfile->Close();

  return ;
}
