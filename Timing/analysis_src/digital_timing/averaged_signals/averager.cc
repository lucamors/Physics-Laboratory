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

void average(string input_file_name)
{
  // Retrieving File
  TFile * input_file = new TFile(input_file_name.c_str());

	TTree * daq_tree;
	daq_tree = (TTree*) input_file->Get("acq_tree_0");

  // Retrieving TTree from TFile

	cleanEv * event_ch_B = new cleanEv;

	TBranch * branch_ch_B;

	branch_ch_B = daq_tree->GetBranch("acq_ch1");
	branch_ch_B->SetAddress(&event_ch_B);

  long int number_of_ev = (long int) branch_ch_B->GetEntries();

	int Buffer = 350;
	int     Th = 10;
	int     bs = 0;
	int Thresh_ch_A, Thresh_ch_B;

	int bsmin   = 5;
	int bsRange = 15;

	double    bsvar_ch_B = 0.0;
	double    Time_ch_B = 0.0;
	double BaseLine_ch_B = 0.0;
	int processed_events=0;

	Double_t * Pulse=new Double_t[Buffer];
  Double_t * Del_Pulse=new Double_t[Buffer];
	Double_t * Frac_Pulse=new Double_t[Buffer];
	Double_t * Bp_Pulse=new Double_t[Buffer];
	Double_t * Time_axis=new Double_t[Buffer];

	   for (size_t i = 0; i < Buffer; i++) {
	    Pulse[i]=0;
			Frac_Pulse[i]=0;
			Time_axis[i]=0;
			Del_Pulse[i]=0;
			Bp_Pulse[i]=0;
	   }
  float Frac = 0.15;
  int Del=4;

	for (int counter = 0; counter < 10000; counter++){

              branch_ch_B->GetEntry(counter);

							double Energy=event_ch_B->qlong*0.0748945-54.25;
							Double_t * Pulse_temp=new Double_t[Buffer];
							for(int i=0;i<Buffer;i++)
							{
				        Pulse_temp[i]=1000-event_ch_B->samples[i];
				      }

              bsvar_ch_B = 0.0;
              BaseLine_ch_B = 0.0;
              Thresh_ch_B = 0;



              // Baseline ch_B
              bs=0;
              for(int i=0;i<Buffer;i++)
              {
                if((i>bsRange)&&(Thresh_ch_B==0)&&(Pulse_temp[i]>(BaseLine_ch_B)+Th))Thresh_ch_B=i;
                if((i>bsmin)&&(i<bsRange)){BaseLine_ch_B+=Pulse_temp[i]; bs++;}
                if(i==bsRange)BaseLine_ch_B=(double)BaseLine_ch_B/bs;
              }

              if(Thresh_ch_B>0)
              {
								for (size_t i = 0; i < Buffer; i++) {
									Pulse[i]+=(Pulse_temp[i]-BaseLine_ch_B)/Energy;
								}
                processed_events++;
              }
							delete [] Pulse_temp;

          } //

 int periods=3;
	for(int i=periods+1; i<(Buffer-periods); i++){
	int tot=0;
	Pulse[i]*=(periods+1);
	for(int j=1; j<=periods; j++){
		Pulse[i]+=(Pulse[i+j]+Pulse[i-j])*(periods-j+1);
		tot+=2*(periods-j+1);
	}
	Pulse[i]=Pulse[i]/(tot+periods+1);
}

   for (size_t i = 0; i < Buffer; i++) {
    Pulse[i]=Pulse[i]/double(processed_events);
		Frac_Pulse[i]=Frac*Pulse[i];
		Time_axis[i]=i;
		if(i>Del)Del_Pulse[i]=-Pulse[i-Del];
		if(i<=Del)Del_Pulse[i]=0;
		Bp_Pulse[i]=Frac_Pulse[i]+Del_Pulse[i];
   }
	 TGraph* W   = new TGraph(Buffer,Time_axis,Pulse);
	 TGraph* FW  = new TGraph(Buffer,Time_axis,Frac_Pulse);
	 TGraph* DW  = new TGraph(Buffer,Time_axis,Del_Pulse);
	 TGraph* BPW = new TGraph(Buffer,Time_axis,Bp_Pulse);


		W->GetYaxis()->SetRangeUser(-1,1);
		W->SetLineColor(kRed);
    W->SetTitle("Original Pulse");
		W->Draw();

		DW->SetLineColor(kBlue);
		DW->SetTitle("Delayed Pulse");
	  DW->Draw("SAME");

		FW->SetLineColor(kOrange);
		FW->SetTitle("Reduced Pulse");
		FW->Draw("SAME");

		BPW->SetLineColor(kGreen);
    BPW->SetTitle("Bipolar Pulse");
    BPW->Draw("SAME");


    return ;
}
