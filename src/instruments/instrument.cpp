#include <iostream>
#include "instrument_dumb.h"
#include "seno.h"
#include "fm1.h"
#include "fm2.h"

/*
  For each new instrument:
  - Add the header in this file
  - Add the call to the constructor in get_instrument() (also in this file)
  - Add the source file to src/meson.build
*/

using namespace std;

namespace upc {
  Instrument * get_instrument(const string &name,
			      const string &parameters) {
    Instrument * pInst = 0;
    //    cout << name << ": " << parameters << endl;
    if (name == "InstrumentDumb") {
      pInst = (Instrument *) new InstrumentDumb(parameters);
    }

    else if (name == "Seno") {
      pInst = (Instrument *) new Seno(parameters);
    }

    else if(name =="Fm1"){
      pInst = (Instrument *) new Fm1(parameters);
    }

     else if(name =="Fm2"){
      pInst = (Instrument *) new Fm2(parameters);
    }

    return pInst;
  }
}
