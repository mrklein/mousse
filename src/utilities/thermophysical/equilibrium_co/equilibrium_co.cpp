// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "dictionary.hpp"
#include "ifstream.hpp"
#include "os_specific.hpp"
#include "iomanip.hpp"
#include "specie.hpp"
#include "perfect_gas.hpp"
#include "thermo.hpp"
#include "janaf_thermo.hpp"
#include "absolute_enthalpy.hpp"
#include "sl_ptr_list.hpp"


using namespace mousse;

typedef species::thermo<janafThermo<perfectGas<specie> >, absoluteEnthalpy>
  thermo;


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  Info << nl << "Reading thermodynamic data IOdictionary" << endl;
  IOdictionary thermoData
  {
    {
      "thermoData",
      runTime.constant(),
      runTime,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE,
      false
    }
  };
  scalar P = 1e5;
  scalar T = 3000.0;
  SLPtrList<thermo> EQreactions;
  EQreactions.append
  (
    new thermo
    {
      thermo(thermoData.subDict("CO2"))
      ==
      thermo(thermoData.subDict("CO")) + 0.5*thermo(thermoData.subDict("O2"))
    }
  );
  EQreactions.append
  (
    new thermo
    {
      thermo(thermoData.subDict("O2"))
      ==
      2.0*thermo(thermoData.subDict("O"))
    }
  );
  EQreactions.append
  (
    new thermo
    {
      thermo(thermoData.subDict("H2O"))
      ==
      thermo(thermoData.subDict("H2")) + 0.5*thermo(thermoData.subDict("O2"))
    }
  );
  EQreactions.append
  (
    new thermo
    {
      thermo(thermoData.subDict("H2O"))
      ==
      thermo(thermoData.subDict("H")) + thermo(thermoData.subDict("OH"))
    }
  );
  FOR_ALL_CONST_ITER(SLPtrList<thermo>, EQreactions, iter) {
    Info << "Kc(EQreactions) = " << iter().Kc(P, T) << endl;
  }
  Info << nl << "end" << endl;
  return 0;
}

