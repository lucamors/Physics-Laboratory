#include <run_manager.h>
#include <random>
#include <chrono>
#include <ctime>
#include <iostream>

int main(int argc, char const *argv[])
{
  RunManger * prova = new RunManger(false); // Generate fake event set to false

  int num_ev = strtol(argv[1], NULL, 10);
  
  prova->run(num_ev);

  return 0;
}
