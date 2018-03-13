void photon_energy_dist(int n_points, double * x_points, double * y_points )
{
  double photon_energy = 511.0;

  double dx = 90.0/n_points;

  for (size_t i = 0; i < n_points; i++)
  {
    x_points[i] = i*dx;
    y_points[i] = (photon_energy)/( 1 + (1-cos((i*dx)*M_PI/180.0) ) );
  }

  return ;
}

void electron_energy_dist(int n_points, double * x_points , double * y_points)
{
  double photon_energy = 511.0;

  double dx = 90.0/n_points;

  for (size_t i = 0; i < n_points; i++)
  {
    x_points[i] = i*dx;
    y_points[i] = 511 - (photon_energy)/( 1 + (1-cos((i*dx)*M_PI/180.0) ) );
  }

  return ;
}


void plot(string electron_filename, string photon_filename)
{

  // Retrieving data file

  TGraphErrors * electron_graph = new TGraphErrors(electron_filename.c_str());
  TGraphErrors * photon_graph = new TGraphErrors(photon_filename.c_str());

  // Defining Compton Function

  int n_points = 100;

  double x_value_p[n_points];
  double y_value_p[n_points];

  photon_energy_dist(n_points, x_value_p, y_value_p);

  double x_value_e[n_points];
  double y_value_e[n_points];

  electron_energy_dist(n_points, x_value_e, y_value_e);

  TGraph * theo_photon = new TGraph(n_points, x_value_p, y_value_p);
  TGraph * theo_electron = new TGraph(n_points, x_value_e, y_value_e);


  // Getting Graph

  TCanvas * test = new TCanvas();

  electron_graph->SetMarkerColor(kBlue);
  electron_graph->SetMarkerStyle(20);
  electron_graph->Draw();

  theo_electron->Draw("SAME");
  theo_photon->Draw("SAME");

  photon_graph->SetMarkerColor(kRed);
  photon_graph->SetMarkerStyle(20);
  photon_graph->Draw("SAME");



  return ;
}
