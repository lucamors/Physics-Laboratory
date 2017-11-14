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

void read_CAEN(string input_file_name)
{
  TFile * input_file = new TFile(input_file_name.c_str());
  TTree * daq_tree;
  // Retrieving TTree from TFile
  daq_tree = (TTree*) input_file->Get("acq_tree_0");

  // Linking

  acqEventPSD_t event_ch_A, event_ch_B;

  TBranch * branch_ch_A, * branch_ch_B;
  branch_ch_A = daq_tree->GetBranch(Form("acq_ch%i",0));
  branch_ch_A->SetAddress(&event_ch_A.timetag);
  branch_ch_B = daq_tree->GetBranch(Form("acq_ch%i",1));
  branch_ch_B->SetAddress(&event_ch_B.timetag);


  // Align channels

  long int nEventsA = (long int) branch_ch_A->GetEntries();
  long int nEventsB = (long int) branch_ch_B->GetEntries();

  int counter_A = 0;
  int skipA = 0;
  int counter_B = 0;
  int skipB = 0;
  int coincident_events = 0;

  int id_A = 0;
  int id_B = 0;

  long int time_difference = 0;


  // Defining Histograms

  TH1F * d1_en_sp = new TH1F("d1_en_sp","Detector 1 Spectrum", 1000, 0, 1500);
  TH1F * d2_en_sp = new TH1F("d2_en_sp","Detector 2 Spectrum", 1000, 0, 1500);

  std::cout << "Loading Event from FILE\n";
  std::cout << "-----------------------\n";

  while(counter_A < nEventsA and counter_B < nEventsB)
  {

      // Display Progress

      if (counter_A % (nEventsA/100) == 0 or counter_B % (nEventsB/100) == 0)
      {
          system("clear");
          std::cout << "Loading Event from FILE\n";
          std::cout << "-----------------------\n";
          std::cout << "Load "
                    <<  (int)((counter_A*100.0)/nEventsA)
                    << "% in channel A"
                    << std::endl;
          std::cout << "Load "
                    <<  (int)((counter_B*100.0)/nEventsB)
                    << "% in channel B"
                    << std::endl;
      }

      // Events filling

      branch_ch_A->GetEntry(counter_A);
      branch_ch_B->GetEntry(counter_B);

      time_difference = event_ch_A.timetag-event_ch_B.timetag;

      if(TMath::Abs(time_difference) > 10000000) //  <--- scegli la finestra di allinamento in sample 1sample=1ns
      {
          if( time_difference > 0){ counter_B++; skipB++; } else { counter_A++; skipA++; }
      }
      else
      {

        // code here

        double Energy_0 = event_ch_A.qlong*0.0754713-54.25;     //calibrated
        double Energy_1 = event_ch_B.qlong*0.0824706-17.8629;   //calibrated

        d1_en_sp->Fill(Energy_0);
        d2_en_sp->Fill(Energy_1);



        // updating counters
        coincident_events++;
        id_A++;
        id_B++;
        counter_A++;
        counter_B++;

      }

  }

  d1_en_sp->Draw();

  return ;
}
