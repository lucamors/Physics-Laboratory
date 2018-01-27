#include <ctime>

struct acqEventPSD_t
{

	ULong64_t	timetag;
	UInt_t		baseline;
	UShort_t	qshort;
	UShort_t	qlong;
	UShort_t	pur;
	UShort_t	samples[4096];
};


class Opt_par : public TObject
{
	public:

		int delay;
		float factor;
		int zcl;

		std::vector<double> TimeA;
		std::vector<double> TimeB;
		std::vector<double> Energy;

		ClassDef(Opt_par, 1)

};

ClassImp(Opt_par)

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

void par_opt(string input_file_name)
{

  // Retrieving File
  TFile * input_file = new TFile(input_file_name.c_str());

	TTree * daq_tree;

  // Retrieving TTree from TFile

  daq_tree = (TTree*) input_file->Get("acq_tree_0");


	cleanEv * event_ch_A = new cleanEv;
	cleanEv * event_ch_B = new cleanEv;

	TBranch * branch_ch_A, * branch_ch_B;
	branch_ch_A = daq_tree->GetBranch("acq_ch0");
	branch_ch_A->SetAddress(&event_ch_A);
	branch_ch_B = daq_tree->GetBranch("acq_ch1");
	branch_ch_B->SetAddress(&event_ch_B);

  long int number_of_ev = (long int) branch_ch_A->GetEntries();

  // Declaring Saving ttree structure

	Opt_par * opt = new  Opt_par;

  TFile * outfile = new TFile("file_out_opt.root","RECREATE");

  TTree * out_tree = new TTree("out_opt_par", "risultati ottimizzazione");

	out_tree->Branch("parameters",&opt);

	TH1F * energy = new TH1F("Energy", "energy", 1400, 0, 1400);

	for (size_t i = 0; i < number_of_ev; i++)
	{
		branch_ch_B->GetEntry(i);

		energy->Fill(event_ch_B->qlong*0.083215-32.6856);

	}


	energy->Draw();


    return ;
}
