#include <run_manager.h>
#include <exp_setup.h>
#include <event.h>
#include <detector.h>

#include "TGraph2D.h"
#include "TFile.h"
#include "TCanvas.h"

RunManger::RunManger(bool flag )
{
  generate_fake_event = flag;

}

void RunManger::run(long int n_events)
{

  // Generating Simulated Events
  for (size_t i = 1; i <= n_events; i++)
  {
    std::cout << "Generating event #" << i+1 << ". . ." << '\n';
    Event * sample_ev = new Event(i,generate_fake_event);
    event_list.push_back(sample_ev);
    system("clear");

  }

  // Defining OutpuT Graph
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

  arma::vec event_momentum;
  for (size_t i = 0; i < event_list.size(); i++)
  {
    std::vector<Photon *> photon_list = event_list[i]->get_gamma_configuration();

    for (size_t j = 0; j < photon_list.size(); j++)
    {

      arma::vec photon_momentum = photon_list[j]->get_momentum();

      if(d1->check_detection(photon_momentum))
      {
        d1_detected->SetPoint(ev_det_1, photon_momentum[0],photon_momentum[1], photon_momentum[2]);
        ev_det_1++;
        d1_c++;
      }
      else if(d2->check_detection(photon_momentum))
      {
        d2_detected->SetPoint(ev_det_2, photon_momentum[0],photon_momentum[1], photon_momentum[2]);
        ev_det_2++;
        d2_c++;
      }
      else if(d3->check_detection(photon_momentum))
      {
        d3_detected->SetPoint(ev_det_3, photon_momentum[0],photon_momentum[1], photon_momentum[2]);
        ev_det_3++;
        d3_c++;
      }
      else
      {
        not_det->SetPoint(ev_not_det, photon_momentum[0],photon_momentum[1], photon_momentum[2]);
        ev_not_det++;
        dnot_c++;
      }
    }
  }

  // Outputting result via iostream
  std::cout << "Detected photon in #1 :" << d1_c << '\n';
  std::cout << "Detected photon in #2 :" << d2_c << '\n';
  std::cout << "Detected photon in #3 :" << d3_c << '\n';
  std::cout << "Not detected :" << dnot_c << '\n';


  // Output Result
  TFile * outfile = new TFile("risultati.root","RECREATE");

  d1_detected->Write();
  d2_detected->Write();
  d3_detected->Write();
  not_det->Write();

  outfile->Close();



  return ;
}
