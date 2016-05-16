// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "dictionary.hpp"
#include "ifstream.hpp"
#include "os_specific.hpp"
#include "specie.hpp"
#include "perfect_gas.hpp"
#include "thermo.hpp"
#include "janaf_thermo.hpp"
#include "absolute_enthalpy.hpp"


using namespace mousse;

typedef species::thermo<janafThermo<perfectGas<specie> >, absoluteEnthalpy>
  thermo;


int main(int argc, char *argv[])
{
  argList::validArgs.append("controlFile");
  argList args{argc, argv};
  const fileName controlFileName = args[1];
  // Construct control dictionary
  IFstream controlFile{controlFileName};
  // Check controlFile stream is OK
  if (!controlFile.good()) {
    FATAL_ERROR_IN(args.executable())
      << "Cannot read file " << controlFileName
      << exit(FatalError);
  }
  dictionary control{controlFile};
  scalar P{readScalar(control.lookup("P"))};
  scalar T0{readScalar(control.lookup("T0"))};
  const word fuelName{control.lookup("fuel")};
  scalar n{readScalar(control.lookup("n"))};
  scalar m{readScalar(control.lookup("m"))};
  Info<< nl << "Reading thermodynamic data dictionary" << endl;
  fileName thermoDataFileName{findEtcFile("thermoData/thermoData")};
  // Construct control dictionary
  IFstream thermoDataFile{thermoDataFileName};
  // Check thermoData stream is OK
  if (!thermoDataFile.good()) {
    FATAL_ERROR_IN(args.executable())
      << "Cannot read file " << thermoDataFileName
      << exit(FatalError);
  }
  dictionary thermoData{thermoDataFile};
  const dictionary& O2dict = thermoData.subDict("O2");
  const dictionary& N2dict = thermoData.subDict("N2");
  const dictionary& fuel_dict = thermoData.subDict(fuelName);
  scalar stoicO2 = n + m/4.0;
  scalar stoicN2 = (0.79/0.21)*(n + m/4.0);
  scalar stoicCO2 = n;
  scalar stoicH2O = m/2.0;
  thermo fuel{"fuel", thermo(fuel_dict)};
  thermo oxidant
  {
    "oxidant",
    stoicO2*thermo(O2dict) + stoicN2*thermo(N2dict)
  };
  dimensionedScalar stoichiometricAirFuelMassRatio
  {
    "stoichiometricAirFuelMassRatio",
    dimless,
    (oxidant.W()*oxidant.nMoles())/fuel.W()
  };
  Info << "stoichiometricAirFuelMassRatio "
    << stoichiometricAirFuelMassRatio << ';' << endl;
  for (int i=0; i<300; i++) {
    scalar equiv = (i + 1)*0.01;
    scalar ft = 1/(1 + stoichiometricAirFuelMassRatio.value()/equiv);
    Info << "phi = " << equiv << nl
      << "ft = " << ft << endl;
    scalar o2 = (1.0/equiv)*stoicO2;
    scalar n2 = (0.79/0.21)*o2;
    scalar fres = max(1.0 - 1.0/equiv, 0.0);
    scalar ores = max(1.0/equiv - 1.0, 0.0);
    scalar fburnt = 1.0 - fres;
    thermo fuel{"fuel", thermo(fuel_dict)};
    Info << "fuel " << fuel << ';' << endl;
    thermo oxidant{"oxidant", o2*thermo(O2dict) + n2*thermo(N2dict)};
    Info<< "oxidant " << (1/oxidant.nMoles())*oxidant << ';' << endl;
    thermo reactants
    (
      "reactants",
      fuel + oxidant
    );
    Info<< "reactants " << (1/reactants.nMoles())*reactants << ';' << endl;
    thermo burntProducts
    {
      "burntProducts",
      + (n2 - (0.79/0.21)*ores*stoicO2)*thermo(N2dict)
      + fburnt*stoicCO2*thermo(thermoData.subDict("CO2"))
      + fburnt*stoicH2O*thermo(thermoData.subDict("H2O"))
    };
    Info << "burntProducts "
      << (1/burntProducts.nMoles())*burntProducts << ';' << endl;
    thermo products
    {
      "products",
      fres*fuel
      + n2*thermo(N2dict)
      + fburnt*stoicCO2*thermo(thermoData.subDict("CO2"))
      + fburnt*stoicH2O*thermo(thermoData.subDict("H2O"))
      + ores*stoicO2*thermo(O2dict)
    };
    Info << "products " << (1/products.nMoles())*products << ';' << endl;
    scalar Tad = products.THa(reactants.Ha(P, T0), P, 1000.0);
    Info << "Tad = " << Tad << nl << endl;
  }
  Info << nl << "end" << endl;
  return 0;
}

