// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time_selector.hpp"
#include "time.hpp"
#include "fv_mesh.hpp"
#include "vector_io_field.hpp"
#include "vol_fields.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    // Check for new mesh
    mesh.readUpdate();
    Info << "Writing components of cellCentre positions to volScalarFields"
      << " ccx, ccy, ccz in " <<  runTime.timeName() << endl;
    for (direction i=0; i<vector::nComponents; i++) {
      volScalarField cci
      {
        {
          "cc" + word(vector::componentNames[i]),
          runTime.timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        mesh.C().component(i)
      };
      cci.write();
    }
    volScalarField V
    {
      {
        "V",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE,
        false
      },
      mesh,
      {"V", mesh.V().dimensions(), 0.0},
      calculatedFvPatchField<scalar>::typeName
    };
    V.dimensionedInternalField() = mesh.V();
    FOR_ALL(V.boundaryField(), patchI) {
      V.boundaryField()[patchI] = V.boundaryField()[patchI].patch().magSf();
    }
    Info << "Writing cellVolumes and patch faceAreas to " << V.name()
      << " in " << runTime.timeName() << endl;
    V.write();
  }
  Info << "\nEnd\n" << endl;
  return 0;
}

