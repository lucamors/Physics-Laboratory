struct Timing_par {

	Float_t		Fraction;
  Int_t	  Delay;
  Int_t   Zcline;
  std::vector<Double_t> Timing_A;
  std::vector<Double_t> Timing_B;
  std::vector<Double_t> Energy_A;

};


  void Read(string input_file_name)
  {
    TFile * input_file = new TFile(input_file_name.c_str());
  	TTree * tree = (TTree*) input_file->Get("Digi_Time");


  	Timing_par event;

    TBranch * branch ;

    branch = tree->GetBranch("Event");
    branch->SetAddress(&event);

    for(int i=0; i<branch->GetEntries(); i++){

        branch->GetEntry(i);
        cout<<event.Zcline<<endl;

    }
    return;
    }
