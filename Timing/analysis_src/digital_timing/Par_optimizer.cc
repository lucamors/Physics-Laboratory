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

void Optimizer(string input_file_name)
{
	Int_t Id_counter = 1;

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

	// TTREE

	for(float Frac = 0.2; Frac<=0.45; Frac+=0.05){
	  for(int Del=3; Del<9; Del++){
	    for(int ZCL=-10; ZCL<11; ZCL+=2){


				int counter_A = 0;
				int skipA = 0;
				int counter_B = 0;
				int skipB = 0;
				int coincident_events = 0;

				int id_A = 0;
				int id_B = 0;

				long int time_difference = 0;

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

				std::vector<Double_t> TimeA;
				std::vector<Double_t> TimeB;
				std::vector<Double_t> Energy;

				while(counter_A < nEventsA and counter_B < nEventsB)
				{

             if(counter_A>100) break;   //////////////////////////////////////////////////////////////
				    // Display Progress

				    if (counter_A % (nEventsA/100) == 0 or counter_B % (nEventsB/100) == 0)
				    {
				        system("clear");
				        std::cout << "Loading Event " << counter_A << endl;
								std::cout<<"Frac = "<<Frac<<" Del = "<<Del<<" ZCL = "<<ZCL<<endl;
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

				    if(TMath::Abs(time_difference) > 100) //  <--- scegli la finestra di allinamento in sample 1sample=1ns
				    {
				        if( time_difference > 0){ counter_B++; skipB++; } else { counter_A++; skipA++; }
				    }
				    else
				    {

				    //retrieve waveforms

			      for(int i=0;i<Buffer;i++){
			        DataVect_ch_A[i]=1000-event_ch_A.samples[i];
			        DataVect_ch_B[i]=1000-event_ch_B.samples[i];
						}

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

			         TimeA.push_back(TimingC2(DataVect_ch_A, Buffer, Frac, Del, ZCL, BaseLine_ch_A, bsRange));
			         TimeB.push_back(TimingC2(DataVect_ch_B, Buffer, Frac, Del, ZCL, BaseLine_ch_B, bsRange));
							 Energy.push_back(event_ch_B.qlong*0.0748945-54.25);

			       }

			      // updating counters

						coincident_events++;
			      id_A++;
			      id_B++;
			      counter_A++;
			      counter_B++;

			    }

				}

				delete[] DataVect_ch_A;
				delete[] DataVect_ch_B;

				// SAVING

				std::string name = "opt_frac_"+std::to_string(Frac)+"__del_"+std::to_string(Del)+"__ZCL_"+std::to_string(ZCL)+".txt";

				std::ofstream outfile(name.c_str());

				for (size_t o = 0; o < TimeA.size(); o++)
				{
					outfile << TimeA[o] << "	" << TimeB[o] << "	" << Energy[o] << endl;
				}

				outfile.close();

	    }
	  }
	}

  return ;
}
