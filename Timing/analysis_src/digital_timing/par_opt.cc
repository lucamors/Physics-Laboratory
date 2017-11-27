#include "opt_par.h"

int counter_world = 0;

struct acqEventPSD_t {

	ULong64_t	timetag;
	UInt_t		baseline;
	UShort_t	qshort;
	UShort_t	qlong;
	UShort_t	pur;
	UShort_t	samples[4096];
};
// Defining Digital Timing algorithm
double TimingC2(double *data, int Buffer, double Fact, int delay, double Zcline, double BaseLine, int bsRange){

	double zcof[Buffer];
	for(int i=0;i<Buffer;i++){zcof[i]=0.0;}
    int Go20=0;

    // take now a proportional of the standard deviation
    double zcoth;
    double zcosigma=0;
    double shapth = 150;
    double shapsize = 0.0;
    int bs=0;

            for(int i=delay;i<Buffer;i++){

                zcof[i]=(Fact*(data[i]-BaseLine)-(data[i-delay]-BaseLine));
                if((i>10)&&(i<bsRange)){zcosigma+=zcof[i]*zcof[i];bs++;}
                if(i==bsRange) zcosigma=sqrt(zcosigma/bs);
            }

                  zcoth=4.0*zcosigma;
            int HalfPassBin=10;

            for(int i=bsRange ;i<Buffer;i++){
				if(Go20==1){
                    shapsize=zcof[i-3]+2.*zcof[i-2]+3.*zcof[i-1]-3.*zcof[i+1]-2.*zcof[i+2]-zcof[i+3];
                    if((zcof[i]<Zcline)&&(shapsize>shapth)){HalfPassBin=i; break;}
                    }
                if(Go20==0 && zcof[i]>zcoth){Go20=1;}
            }
            //***********************************************************************
              double IntData[4];

            IntData[0]= (1.*zcof[HalfPassBin-3] - 8.*zcof[HalfPassBin-2] + 19.*zcof[HalfPassBin-1] - 19.*zcof[HalfPassBin] + 8.*zcof[HalfPassBin+1] - 1.*zcof[HalfPassBin+2])/18.0;
            IntData[1]= (-4.*zcof[HalfPassBin-3] + 32.*zcof[HalfPassBin-2] - 58.*zcof[HalfPassBin-1] + 37.*zcof[HalfPassBin] - 8.*zcof[HalfPassBin+1] + 1.*zcof[HalfPassBin+2])/30.0;
            IntData[2]= (7.*zcof[HalfPassBin-3] - 56.*zcof[HalfPassBin-2] - 11.*zcof[HalfPassBin-1] + 74.*zcof[HalfPassBin] - 16.*zcof[HalfPassBin+1] + 2.*zcof[HalfPassBin+2])/90.0;
            IntData[3]= zcof[HalfPassBin-1];
            char MyPlo3[200];
            sprintf(MyPlo3,"[0]*(x-%i)**3+[1]*(x-%i)**2+[2]*(x-%i)+[3]+%4.10f",HalfPassBin-1,HalfPassBin-1,HalfPassBin-1,BaseLine);

            TF1 *f2 = new TF1("f2",MyPlo3, HalfPassBin-1, HalfPassBin);
            f2->SetParameter(0,IntData[0]);
            f2->SetParameter(1,IntData[1]);
            f2->SetParameter(2,IntData[2]);
            f2->SetParameter(3,IntData[3]);
            f2->SetFillColor(19);
            f2->SetFillStyle(1);

            f2->SetLineWidth(1);
            //*************************************************************************

            double Time=f2->GetX(BaseLine+Zcline,(double)(HalfPassBin-1),(double)HalfPassBin);

            delete f2;
            return Time;
}

void par_opt(string input_file_name)
{
  // Retrieving File
  TFile * input_file = new TFile(input_file_name.c_str());

	TTree * daq_tree;

  // Retrieving TTree from TFile

  daq_tree = (TTree*) input_file->Get("cleandata");

  // Linking

  acqEventPSD_t event_ch_A, event_ch_B;

  TBranch * branch_ch_A, * branch_ch_B;
  branch_ch_A = daq_tree->GetBranch(Form("acq_ch%i",0));
  branch_ch_A->SetAddress(&event_ch_A.timetag);
  branch_ch_B = daq_tree->GetBranch(Form("acq_ch%i",1));
  branch_ch_B->SetAddress(&event_ch_B.timetag);

  long int number_of_ev = (long int) branch_ch_A->GetEntries();


  // Declaring Saving ttree structure

  TFile * outfile = new TFile("file_out_opt.root","RECREATE");

  TTree * out_tree = new TTree("out_opt_par", "risultati ottimizzazione");


  Opt_params * opt = new  Opt_params(number_of_ev);

  string tleaf = "id_ev/i:TimeA["+std::to_string(number_of_ev)+"]/d:TimeB["+std::to_string(number_of_ev)+"]/d:Energy["+std::to_string(number_of_ev)+"]/d:delay/f:factor/f";

  TBranch * branch = out_tree->Branch("opt", opt->get_id(), tleaf.c_str());

  // Defining waveform array and timing variables
	int Buffer = 350;
	int     Th = 10;
	int     bs = 0;
	int Thresh_ch_A, Thresh_ch_B;

	int bsmin   = 5;
	int bsRange = 75;

	double    bsvar_ch_A = 0.0;
	double    Time_ch_A = 0.0;
	double BaseLine_ch_A = 0.0;

	double    bsvar_ch_B = 0.0;
	double    Time_ch_B = 0.0;
	double BaseLine_ch_B = 0.0;

	Double_t * DataVect_ch_A=new Double_t[Buffer];
  Double_t * DataVect_ch_B=new Double_t[Buffer];

    for(float Frac = 0.2; Frac<0.25; Frac+=0.05)
    {
      for(int Del=3; Del<4; Del++)
      {
        for(int ZCL=-10; ZCL<11; ZCL+=2)
        {

          opt->set_id(counter_world);
          counter_world++;

          for (int counter = 0; counter < number_of_ev; counter++)
          {
            if(counter > 10) break;

            if ( counter % (number_of_ev/100) == 0)
            {

               system("clear");
               std::cout << "Loading Event " << counter << endl;
               std::cout << "Frac: " << Frac << " Del:" << Del << " ZCL: " << ZCL << std::endl;
               std::cout << "-----------------------\n";
               std::cout << "Load "
                         <<  (int)((counter*100.0)/number_of_ev)
                         << "%"
                         << std::endl;
              }

              branch_ch_A->GetEntry(counter);
              branch_ch_B->GetEntry(counter);

              cout << "here\n";
              //Initialise variables
              bsvar_ch_A = 0.0;
              Time_ch_A = 0.0;
              BaseLine_ch_A = 0.0;
              Thresh_ch_A = 0;

              bsvar_ch_B = 0.0;
              Time_ch_B = 0.0;
              BaseLine_ch_B = 0.0;
              Thresh_ch_B = 0;

              // Baseline ch_A
              bs=0;
              for(int i=0;i<Buffer;i++)
              {
                if((i>bsRange)&&(Thresh_ch_A==0)&&(DataVect_ch_A[i]>(BaseLine_ch_A)+Th))Thresh_ch_A=i;
                if((i>bsmin)&&(i<bsRange)){BaseLine_ch_A+=DataVect_ch_A[i]; bs++;}
                if(i==bsRange)BaseLine_ch_A=(double)BaseLine_ch_A/bs;
              }

              // Baseline ch_B
              bs=0;
              for(int i=0;i<Buffer;i++)
              {
                if((i>bsRange)&&(Thresh_ch_B==0)&&(DataVect_ch_B[i]>(BaseLine_ch_B)+Th))Thresh_ch_B=i;
                if((i>bsmin)&&(i<bsRange)){BaseLine_ch_B+=DataVect_ch_B[i]; bs++;}
                if(i==bsRange)BaseLine_ch_B=(double)BaseLine_ch_B/bs;
              }

              // Digital Timing
              if(Thresh_ch_A>0 && Thresh_ch_B>0)
              {
                opt->get_timeA()[counter] = TimingC2(DataVect_ch_A, Buffer, Frac, Del, ZCL, BaseLine_ch_A, bsRange);
                opt->get_timeB()[counter] = TimingC2(DataVect_ch_B, Buffer, Frac, Del, ZCL, BaseLine_ch_B, bsRange);
                opt->get_energy()[counter] = event_ch_B.qlong*0.0748945-54.25;
              }


          } // events

          branch->Fill();
        }// ZCL
      }// Del
    }// FRAC

    out_tree->Write();
  	outfile->Close();

    return ;
}
