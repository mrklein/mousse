// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "single_phase_transport_model.hpp"
#include "turbulent_transport_model.hpp"


int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_mesh.inc"

  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    #include "create_fields.inc"
    Info << "\nRetrieving field k from turbulence model" << endl;
    const volScalarField k{RASModel->k()};
    Info << "\nRetrieving field epsilon from turbulence model" << endl;
    const volScalarField epsilon{RASModel->epsilon()};
    Info << "\nRetrieving field R from turbulence model" << endl;
    const volSymmTensorField R{RASModel->R()};
    // Check availability of tubulence fields
    if (!IOobject{"k", runTime.timeName(), mesh}.headerOk()) {
      Info << "\nWriting turbulence field k" << endl;
      k.write();
    } else {
      Info << "\nTurbulence k field already exists" << endl;
    }
    if (!IOobject{"epsilon", runTime.timeName(), mesh}.headerOk()) {
      Info << "\nWriting turbulence field epsilon" << endl;
      epsilon.write();
    } else {
      Info << "\nTurbulence epsilon field already exists" << endl;
    }
    if (!IOobject{"R", runTime.timeName(), mesh}.headerOk()) {
      Info << "\nWriting turbulence field R" << endl;
      R.write();
    } else {
      Info << "\nTurbulence R field already exists" << endl;
    }
    if (!IOobject{"omega", runTime.timeName(), mesh}.headerOk()) {
      const scalar Cmu = 0.09;
      Info << "creating omega" << endl;
      volScalarField omega
      {
        {
          "omega",
          runTime.timeName(),
          mesh
        },
        epsilon/(Cmu*k),
        epsilon.boundaryField().types()
      };
      Info << "\nWriting turbulence field omega" << endl;
      omega.write();
    } else {
      Info << "\nTurbulence omega field already exists" << endl;
    }
  }
  Info << "\nEnd\n" << endl;
  return 0;
}

