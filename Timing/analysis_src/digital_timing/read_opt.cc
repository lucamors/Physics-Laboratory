
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

	std::vector<TCanvas * > Canvas;
	//TCanvas * c = 0;

	for (size_t i = 0; i < n_ev; i++)
  {
    branch->GetEntry(i);

		//get parameters
		float Fraction = opt_sett->factor;
		int   Delay    = opt_sett->delay;
		int   ZCL      = opt_sett->zcl;

		TCanvas * c = new TCanvas(Form("Canvas %i", i),Form("Frac%.2f_Del%i_Zcl%i", Fraction, Delay, ZCL), 1280,800);

		Canvas.push_back(c);
	}

	TH1F * Time = 0;
	TH2F * TimeVsAmp = 0;

  for (size_t i = 0; i < n_ev; i++)
  {
    branch->GetEntry(i);

		//get parameters
		float Fraction = opt_sett->factor;
		int   Delay    = opt_sett->delay;
		int   ZCL      = opt_sett->zcl;

		Time      = new TH1F("Time"       , Form("Frac%.2f_Del%i_Zcl%i_Timing", Fraction, Delay, ZCL), 1000, -10, 10);
		TimeVsAmp = new TH2F("Time Vs Amp", Form("Frac%.2f_Del%i_Zcl%i_TimVsAmp", Fraction, Delay, ZCL), 1000, 0, 500, 1000, -10, 10);

		Time->GetXaxis()->SetTitle("Timing [ns]");
  	Time->GetYaxis()->SetTitle("Count");

		TimeVsAmp->GetXaxis()->SetTitle("Energy [keV]");
  	TimeVsAmp->GetYaxis()->SetTitle("Timing [ns]");


    for (size_t j = 0; j < opt_sett->TimeA.size() ; j++)
		{
			Time->Fill(opt_sett->TimeA[j]-opt_sett->TimeB[j]);
			TimeVsAmp->Fill(opt_sett->Energy[j],opt_sett->TimeA[j]-opt_sett->TimeB[j]);
		}

		Canvas[i]->Divide(2,1);

		Canvas[i]->cd(1);
		Time->Draw();

		Canvas[i]->cd(2);
		TimeVsAmp->Draw("COLZ");

		Canvas[i]->Update();

  }


  return ;
}
