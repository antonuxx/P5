#ifndef FM1
#define FM1

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class Fm1: public upc::Instrument {
    EnvelopeADSR adsr;
    unsigned int index;
    float I;
    float fm;
    int N;
    float phase; 
    float step; 
	float A;
    std::vector<float> tbl;
  public:
    Fm1(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif