#include <string>
#include <fstream>

void grapher(string filename)
{

  TGraphErrors * my_graph = new TGraphErrors(filename.c_str(), "%lg %lg %lg");

  my_graph->Draw();

  return ;
}
