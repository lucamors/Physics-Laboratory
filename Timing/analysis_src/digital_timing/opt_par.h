#ifndef OPT_PARAMS_H
#define OPT_PARAMS_H

class Opt_params
{
	private:

    ULong64_t id_ev;
		Float_t delay;
		Float_t factor;

		Double_t * TimeA;
		Double_t * TimeB;
		Double_t * Energy;


	public:

		Opt_params(ULong64_t );
		

    // getter methods

    ULong64_t * get_id();
    Double_t * get_timeA();
    Double_t * get_timeB();
    Double_t * get_energy();

    void set_id(ULong64_t id);


};

#endif
