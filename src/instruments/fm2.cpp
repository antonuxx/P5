#include <iostream>
#include <math.h>
#include "fm2.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

Fm2::Fm2(const std::string &param) 
  : adsr(SamplingRate, param), adsr2(SamplingRate) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);
  if (!kv.to_int("N1",(Fm2::N1)))
    N1=1; //default value
  if (!kv.to_int("N2",(Fm2::N2)))
    N2=1; //default value

    float a,d,s,r; 
    kv.to_float("ADSR2_A",a); 
    kv.to_float("ADSR2_D",d); 
    kv.to_float("ADSR2_S",s); 
    kv.to_float("ADSR2_R",r); 
    adsr2.set(a,d,s,r); 

}


void Fm2::command(long cmd, long note, long vel) {
  float playedNote;
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start(); adsr2.start(); 
   // index = 0;
    Fm2::phase1 = 0; Fm2::phase2 = 0;
    float f0=440.0*pow(2,(((float)note-69.0)/12.0));
    playedNote = f0/SamplingRate;
  	A = vel / 127.;
    (Fm2::I) = N2/(Fm2::step2);

    Fm2::step1 = 2 * M_PI * playedNote;
    Fm2::step2 = 2 * M_PI *N1* playedNote;
  }
  else if (cmd == 8 || cmd == 0) {	//'Key' released: sustain ends, release begins
    adsr.stop(); adsr2.stop();
  }
}


const vector<float> & Fm2::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  vector <float> a2(x.size()); 
    for(int j=0;j<a2.size();j++) 
        a2[j]= 1; 
    adsr2(a2); 

    for (int i=0; i < x.size() ; ++i) { 
        x[i] = 0.3*(Fm2::A)*sin(phase1+(I+a2[i])*sin(phase2)); 
        phase1 += step1; 
        phase2 += step2; 
        while (phase1 > M_PI) phase1 -= 2*M_PI; 
        while (phase2 > M_PI) phase2 -= 2*M_PI; 
    }    

  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
