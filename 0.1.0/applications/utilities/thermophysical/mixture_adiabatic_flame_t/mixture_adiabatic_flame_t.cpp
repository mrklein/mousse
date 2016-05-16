// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "dictionary.hpp"
#include "ifstream.hpp"
#include "os_specific.hpp"
#include "specie.hpp"
#include "perfect_gas.hpp"
#include "thermo.hpp"
#include "janaf_thermo.hpp"
#include "absolute_enthalpy.hpp"
#include "mixture.hpp"


using namespace mousse;


typedef species::thermo<janafThermo<perfectGas<specie>>, absoluteEnthalpy>
  thermo;

int main(int argc, char *argv[])
{
  argList::validArgs.append("controlFile");
  argList args(argc, argv);
  const fileName controlFileName(args[1]);
  // Construct control dictionary
  IFstream controlFile{controlFileName};
  // Check controlFile stream is OK
  if (!controlFile.good()) {
    FatalErrorIn(args.executable())
      << "Cannot read file " << controlFileName
      << abort(FatalError);
  }
  dictionary control{controlFile};
  scalar P{readScalar(control.lookup("P"))};
  scalar T0{readScalar(control.lookup("T0"))};
  mixture rMix{control.lookup("reactants")};
  mixture pMix{control.lookup("products")};
  Info << nl << "Reading thermodynamic data dictionary" << endl;
  fileName thermoDataFileName{findEtcFile("thermoData/thermoData")};
  // Construct control dictionary
  IFstream thermoDataFile{thermoDataFileName};
  // Check thermoData stream is OK
  if (!thermoDataFile.good()) {
    FatalErrorIn(args.executable())
      << "Cannot read file " << thermoDataFileName
      << abort(FatalError);
  }
  dictionary thermoData{thermoDataFile};
  thermo reactants
  {
    rMix[0].volFrac()*thermo(thermoData.subDict(rMix[0].name()))
  };
  for (label i = 1; i < rMix.size(); i++) {
    reactants = reactants
      + rMix[i].volFrac()*thermo(thermoData.subDict(rMix[i].name()));
  }
  thermo products
  {
    2*pMix[0].volFrac()*thermo(thermoData.subDict(pMix[0].name()))
  };
  for (label i = 1; i < pMix.size(); i++) {
    products = products
      + 2*pMix[i].volFrac()*thermo(thermoData.subDict(pMix[i].name()));
  }
  Info << "Adiabatic flame temperature of mixture " << rMix.name() << " = "
    << products.THa(reactants.Ha(P, T0), P, 1000.0) << " K" << endl;
  return 0;
}

