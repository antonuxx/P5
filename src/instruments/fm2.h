#ifndef FM2
#define FM2

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class Fm2: public upc::Instrument {
    EnvelopeADSR adsr;
    EnvelopeADSR adsr2;
    unsigned int index;
	float A;
    int N1;
    int N2;
    double I;
    float phase1;
    float phase2;
    float step1;
    float step2;
    std::vector<float> tbl;
  public:
    Fm2(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif