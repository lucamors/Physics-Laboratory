
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

void read_opt(string filename)
{

  // Retrieving File
  TFile * input_file = new TFile(filename.c_str());

  TTree * daq_tree;

  // Retrieving TTree from TFile

  daq_tree = (TTree*) input_file->Get("out_opt_par");


  TBranch * branch = daq_tree->GetBranch("parameters");

  long int n_ev = branch->GetEntries();

  Opt_par * opt_sett = new Opt_par;

  branch->SetAddress(&opt_sett);

  for (size_t i = 0; i < n_ev; i++)
  {
    branch->GetEntry(i);

    cout << opt_sett->zcl << endl;
    

  }


  return ;
}
