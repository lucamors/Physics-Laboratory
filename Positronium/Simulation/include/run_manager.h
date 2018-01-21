#ifndef RUN_MANAGER_H
#define RUN_MANAGER_H

#include <vector>
#include <event.h>
#include <exp_setup.h>

class RunManger
{
  private:

    std::vector<Event*> event_list;
    bool generate_fake_event; // Debugging Purpouse
    ExperimentalSetup * setup = new ExperimentalSetup();

  public:

    RunManger(bool );
    void run(long int );

};

#endif
