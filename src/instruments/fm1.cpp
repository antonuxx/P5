#include <iostream>
#include <math.h>
#include "fm1.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

Fm1::Fm1(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);

  if (!kv.to_float("I",Fm1::I))
    I = 1; //default value


  if (!kv.to_float("fm",Fm1::fm))
     fm = 10; //default value

  if (!kv.to_int("N",N))
    N = 40; //default value

     tbl.resize(N);
  float phase = 0, step = 2 * M_PI /(float) N;
  index = 0;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(phase);
    phase += step;
  }
}


void Fm1::command(long cmd, long note, long vel) {
  float playedNote;
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    index = 0;
    Fm1::phase=0;
    float f0=440.0*pow(2,(((float)note-69.0)/12.0));
    playedNote = f0/SamplingRate;
  	A = vel / 127.;
    Fm1::step = 2 * M_PI * playedNote * I;
    I = fm/step;
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & Fm1::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    x[i]=0.3*(Fm1::A)*sin((Fm1::phase));
    Fm1::phase += Fm1::step;
    while((Fm1::phase) > 2*M_PI) (Fm1::phase) -= 2*M_PI;
    if (index == tbl.size())
      index = 0;
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
