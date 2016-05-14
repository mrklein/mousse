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


using namespace mousse;

typedef species::thermo<janafThermo<perfectGas<specie>>, absoluteEnthalpy>
  thermo;


int main(int argc, char *argv[])
{
  argList::validArgs.append("controlFile");
  argList args(argc, argv);
  const fileName controlFileName = args[1];
  // Construct control dictionary
  IFstream controlFile{controlFileName};
  // Check controlFile stream is OK
  if (!controlFile.good()) {
    FATAL_ERROR_IN(args.executable())
      << "Cannot read file " << controlFileName
      << abort(FatalError);
  }
  dictionary control{controlFile};
  scalar P{readScalar(control.lookup("P"))};
  const word fuelName{control.lookup("fuel")};
  scalar n{readScalar(control.lookup("n"))};
  scalar m{readScalar(control.lookup("m"))};
  Info << nl << "Reading thermodynamic data dictionary" << endl;
  fileName thermoDataFileName{findEtcFile("thermoData/thermoData")};
  // Construct control dictionary
  IFstream thermoDataFile{thermoDataFileName};
  // Check thermoData stream is OK
  if (!thermoDataFile.good()) {
    FATAL_ERROR_IN(args.executable())
      << "Cannot read file " << thermoDataFileName
      << abort(FatalError);
  }
  dictionary thermoData{thermoDataFile};
  Info << nl << "Reading thermodynamic data for relevant species"
    << nl << endl;
  // Reactants
  thermo FUEL{thermoData.subDict(fuelName)};
  thermo O2{thermoData.subDict("O2")};
  thermo N2{thermoData.subDict("N2")};
  // Products
  thermo CO2{thermoData.subDict("CO2")};
  thermo H2O{thermoData.subDict("H2O")};
  // Product fragments
  thermo CO{thermoData.subDict("CO")};
  thermo H2{thermoData.subDict("H2")};
  // Product dissociation reactions
  thermo CO2BreakUp
  {
    CO2 == CO + 0.5* O2
  };
  thermo H2OBreakUp
  {
    H2O == H2 + 0.5*O2
  };
  // Stoiciometric number of moles of species for one mole of fuel
  scalar stoicO2 = n + m/4.0;
  scalar stoicN2 = (0.79/0.21)*(n + m/4.0);
  scalar stoicCO2 = n;
  scalar stoicH2O = m/2.0;
  // Oxidant
  thermo oxidant
  {
    "oxidant",
    stoicO2*O2 + stoicN2*N2
  };
  dimensionedScalar stoichiometricAirFuelMassRatio
  {
    "stoichiometricAirFuelMassRatio",
    dimless,
    (oxidant.W()*oxidant.nMoles())/FUEL.W()
  };
  Info << "stoichiometricAirFuelMassRatio "
    << stoichiometricAirFuelMassRatio << ';' << endl;
  Info << "Equilibrium flame temperature data ("
    << P/1e5 << " bar)" << nl << nl
    << setw(3) << "Phi"
    << setw(12) << "ft"
    << setw(7) << "T0"
    << setw(12) << "Tad"
    << setw(12) << "Teq"
    << setw(12) << "Terror"
    << setw(20) << "O2res (mole frac)" << nl
    << endl;
  // Loop over equivalence ratios
  for (int i=0; i<16; i++) {
    scalar equiv = 0.6 + i*0.05;
    scalar ft = 1/(1 + stoichiometricAirFuelMassRatio.value()/equiv);
    // Loop over initial temperatures
    for (int j=0; j<28; j++) {
      scalar T0 = 300.0 + j*100.0;
      // Number of moles of species for one mole of fuel
      scalar o2 = (1.0/equiv)*stoicO2;
      scalar n2 = (0.79/0.21)*o2;
      scalar fres = max(1.0 - 1.0/equiv, 0.0);
      scalar fburnt = 1.0 - fres;
      // Initial guess for number of moles of product species
      // ignoring product dissociation
      scalar oresInit = max(1.0/equiv - 1.0, 0.0)*stoicO2;
      scalar co2Init = fburnt*stoicCO2;
      scalar h2oInit = fburnt*stoicH2O;
      scalar ores = oresInit;
      scalar co2 = co2Init;
      scalar h2o = h2oInit;
      scalar co = 0.0;
      scalar h2 = 0.0;
      // Total number of moles in system
      scalar N = fres + n2 + co2 + h2o + ores;
      // Initial guess for adiabatic flame temperature
      scalar adiabaticFlameTemperature =
        T0
        + (fburnt/(1.0 + o2 + n2))/(1.0/(1.0 + (1.0 + 0.79/0.21)*stoicO2))
        *2000.0;
      scalar equilibriumFlameTemperature = adiabaticFlameTemperature;
      // Iteration loop for adiabatic flame temperature
      for (int j=0; j<20; j++) {
        if (j > 0) {
          co = co2*
            min
            (
              CO2BreakUp.Kn(P, equilibriumFlameTemperature, N)
              /::sqrt(max(ores, 0.001)),
              1.0
            );
          h2 = h2o*
            min
            (
              H2OBreakUp.Kn(P, equilibriumFlameTemperature, N)
              /::sqrt(max(ores, 0.001)),
              1.0
            );
          co2 = co2Init - co;
          h2o = h2oInit - h2;
          ores = oresInit + 0.5*co + 0.5*h2;
        }
        thermo reactants
        {
          FUEL + o2*O2 + n2*N2
        };
        thermo products
        {
          fres*FUEL + ores*O2 + n2*N2 + co2*CO2 + h2o*H2O + co*CO + h2*H2
        };
        scalar equilibriumFlameTemperatureNew =
          products.THa(reactants.Ha(P, T0), P, adiabaticFlameTemperature);
        if (j==0) {
          adiabaticFlameTemperature = equilibriumFlameTemperatureNew;
        } else {
          equilibriumFlameTemperature =
            0.5*(equilibriumFlameTemperature + equilibriumFlameTemperatureNew);
        }
      }
      Info << setw(3) << equiv
        << setw(12) << ft
        << setw(7) << T0
        << setw(12) << adiabaticFlameTemperature
        << setw(12) << equilibriumFlameTemperature
        << setw(12)
        << adiabaticFlameTemperature - equilibriumFlameTemperature
        << setw(12) << ores/N
        << endl;
    }
  }
  Info << nl << "end" << endl;
  return 0;
}

