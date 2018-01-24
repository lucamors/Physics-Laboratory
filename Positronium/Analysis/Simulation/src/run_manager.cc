#include <run_manager.h>
#include <exp_setup.h>
#include <event.h>
#include <detector.h>
#include <stdio.h>
#include <fstream>

#include "TGraph2D.h"
#include "TFile.h"
#include "TCanvas.h"


//---------------------------------------------------------------
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60
// From https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf
void printProgress (double percentage)
{
    setbuf(stdout, NULL);
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}
//---------------------------------------------------------------

RunManger::RunManger(bool flag )
{
  generate_fake_event = flag;

}

void RunManger::run(long int n_events)
{

  // Defining output Graph
  TGraph2D * d1_detected = new TGraph2D();
  d1_detected->SetName("d1_det");
  TGraph2D * d2_detected = new TGraph2D();
  d2_detected->SetName("d2_det");
  TGraph2D * d3_detected = new TGraph2D();
  d3_detected->SetName("d3_det");

  TGraph2D * not_det = new TGraph2D();
  not_det->SetName("not_det");

  int ev_det_1 = 0;
  int ev_det_2 = 0;
  int ev_det_3 = 0;
  int ev_not_det = 0;

  int d1_c = 0;
  int d2_c = 0;
  int d3_c = 0;
  int dnot_c = 0;

  // Retrieving Detectors
  std::vector<CylindricalDetector*> det_list = setup->get_detector_list();

  CylindricalDetector * d1 = det_list[0];
  CylindricalDetector * d2 = det_list[1];
  CylindricalDetector * d3 = det_list[2];

  // Checking Detection

  bool d1_check_det,d2_check_det,d3_check_det;
  long int coincidences = 0;

  std::ofstream out_test("pr.txt");

  // Generating Simulated Events
  for (size_t i = 1; i <= n_events; i++)
  {

    // Displaying information
    if (i % (n_events/100) == 0 )
    {
             system("clear");
             std::cout << "Positronium Decay Simulation\n";
             std::cout << "----------------------------\n";
             std::cout << "Simulation at "
                       <<  (int)((i*100.0)/n_events)
                       << "%"
                       << std::endl;
    }

    Event * sample_ev = new Event(i,generate_fake_event);

    std::vector<Photon *> photon_list = sample_ev->get_gamma_configuration();

    // Setting to off detection status

    d1_check_det = false;
    d2_check_det = false;
    d3_check_det = false;

    for (size_t j = 0; j < photon_list.size(); j++)
    {

      arma::vec photon_momentum = normalise(photon_list[j]->get_momentum());
      double photon_energy = photon_list[j]->get_energy();

      if( j == 0 ) det_1_sp->Fill(photon_energy);
      if( j == 1 ) det_2_sp->Fill(photon_energy);
      if( j == 2 ) det_3_sp->Fill(photon_energy);

      if(d1->check_detection(photon_momentum))
      {
        d1_detected->SetPoint(ev_det_1, photon_momentum[0],photon_momentum[1], photon_momentum[2]);
        ev_det_1++;
        d1_c++;
        d1_check_det = true;
      }
      else if(d2->check_detection(photon_momentum))
      {
        d2_detected->SetPoint(ev_det_2, photon_momentum[0],photon_momentum[1], photon_momentum[2]);
        ev_det_2++;
        d2_c++;
        d2_check_det = true;
      }
      else if(d3->check_detection(photon_momentum))
      {
        d3_detected->SetPoint(ev_det_3, photon_momentum[0],photon_momentum[1], photon_momentum[2]);
        ev_det_3++;
        d3_c++;
        d3_check_det = true;

      }
      else
      {
        not_det->SetPoint(ev_not_det, photon_momentum[0],photon_momentum[1], photon_momentum[2]);
        ev_not_det++;
        dnot_c++;
      }
    }

    if (d1_check_det and d2_check_det and d3_check_det )
    {
      coincidences++;
      coincidences_sp_1->Fill(photon_list[0]->get_energy());
      coincidences_sp_2->Fill(photon_list[1]->get_energy());
      coincidences_sp_3->Fill(photon_list[2]->get_energy());

      // For heatmap plotting

      arma::vec x_axis = {1,0,0};
      arma::vec z_axis = {0,0,1};

      arma::vec photon_momentum1 = photon_list[0]->get_momentum();
      arma::vec photon_momentum2 = photon_list[1]->get_momentum();
      arma::vec photon_momentum3 = photon_list[2]->get_momentum();

      double theta1 = acos(photon_momentum1[2]/norm(photon_momentum1));
      double theta2 = acos(photon_momentum2[2]/norm(photon_momentum2));
      double theta3 = acos(photon_momentum3[2]/norm(photon_momentum3));

      theta1 -= M_PI/2.0;
      theta2 -= M_PI/2.0;
      theta3 -= M_PI/2.0;

      double phi1,phi2,phi3;

      // double s1 = sqrt(photon_momentum1[0]*photon_momentum1[0]+photon_momentum1[1]*photon_momentum1[1]);
      // double s2 = sqrt(photon_momentum2[0]*photon_momentum2[0]+photon_momentum2[1]*photon_momentum2[1]);
      // double s3 = sqrt(photon_momentum3[0]*photon_momentum3[0]+photon_momentum3[1]*photon_momentum3[1]);
      //
      // if(photon_momentum1[0] > 0){ phi1 = asin(photon_momentum1[1]/s1); }
      // else{ phi1 = M_PI - asin(photon_momentum1[1]/s1); }
      //
      // if(photon_momentum2[0] > 0){ phi2 = asin(photon_momentum2[1]/s2); }
      // else{ phi2 = M_PI - asin(photon_momentum2[1]/s2); }
      //
      // if(photon_momentum3[0] > 0){ phi3 = asin(photon_momentum3[1]/s3); }
      // else{ phi3 = M_PI - asin(photon_momentum3[1]/s3); }

      // if (photon_momentum1[0] > 0 && photon_momentum1[1] <0 ) phi1 = M_PI - phi1;
      // if (photon_momentum2[0] > 0 && photon_momentum2[1] <0 ) phi2 = M_PI - phi2;
      // if (photon_momentum3[0] > 0 && photon_momentum3[1] <0 ) phi3 = M_PI - phi3;

      phi1 = atan2(photon_momentum1[1],photon_momentum1[0]);
      phi2 = atan2(photon_momentum2[1],photon_momentum2[0]);
      phi3 = atan2(photon_momentum3[1],photon_momentum3[0]);


      if( phi1 < 0 ) phi1 = 2*M_PI + phi1;
      if( phi2 < 0 ) phi2 = 2*M_PI + phi2;
      if( phi3 < 0 ) phi3 = 2*M_PI + phi3;


      // For Debugging

      out_test << "theta1 ->" << theta1*180.0/M_PI << '\n';
      out_test << "theta2 ->" << theta2*180.0/M_PI << '\n';
      out_test << "theta3 ->" << theta3*180.0/M_PI << '\n';

      out_test << "p1 ->" << phi1*180.0/M_PI << '\n';
      out_test << "p2 ->" << phi2*180.0/M_PI << '\n';
      out_test << "p3 ->" << phi3*180.0/M_PI << '\n';
      out_test << "------------------------------\n";

      heat_map->Fill(phi1*180.0/M_PI, 1);
      heat_map->Fill(phi2*180.0/M_PI, 1);
      heat_map->Fill(phi3*180.0/M_PI, 1);

      // heat_map->Fill(phi1, theta1);
      // heat_map->Fill(phi2, theta2);
      // heat_map->Fill(phi3, theta3);

      double angle12 = acos(dot(photon_list[0]->get_momentum(), photon_list[1]->get_momentum())/(norm(photon_list[0]->get_momentum())*norm(photon_list[1]->get_momentum())));
      double angle23 = acos(dot(photon_list[1]->get_momentum(), photon_list[2]->get_momentum())/(norm(photon_list[1]->get_momentum())*norm(photon_list[2]->get_momentum())));

      angular_distribution_coinc->Fill(angle12*180.0/M_PI, angle23*180.0/M_PI);
    }

    double angle12 = acos(dot(photon_list[0]->get_momentum(), photon_list[1]->get_momentum())/(norm(photon_list[0]->get_momentum())*norm(photon_list[1]->get_momentum())));
    double angle23 = acos(dot(photon_list[1]->get_momentum(), photon_list[2]->get_momentum())/(norm(photon_list[1]->get_momentum())*norm(photon_list[2]->get_momentum())));

    // Plotting angular distribution in degrees
    angular_distribution->Fill(angle12*180.0/M_PI, angle23*180.0/M_PI);


    delete sample_ev;

  }

  out_test.close();

  // Outputting result via iostream
  std::cout << "Detected photon in #1 :" << d1_c << '\n';
  std::cout << "Detected photon in #2 :" << d2_c << '\n';
  std::cout << "Detected photon in #3 :" << d3_c << '\n';
  std::cout << "Coincidences :" << coincidences << '\n';
  std::cout << "Not detected :" << dnot_c << '\n';

  // Output Result
  TFile * outfile = new TFile("analysis_macros/risultati.root","RECREATE");

  d1_detected->Write();
  d2_detected->Write();
  d3_detected->Write();
  coincidences_sp_1->Write();
  coincidences_sp_2->Write();
  coincidences_sp_3->Write();
  not_det->Write();
  det_1_sp->Write();
  det_2_sp->Write();
  det_3_sp->Write();
  angular_distribution->Write();
  angular_distribution_coinc->Write();
  heat_map->Write();


  outfile->Close();



  return ;
}
