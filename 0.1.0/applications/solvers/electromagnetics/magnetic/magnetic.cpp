// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "os_specific.hpp"
#include "magnet.hpp"
#include "electromagnetic_constants.hpp"
#include "simple_control.hpp"

int main(int argc, char *argv[])
{
  argList::addBoolOption
  (
    "noH",
    "do not write the magnetic field intensity field"
  );
  argList::addBoolOption
  (
    "noB",
    "do not write the magnetic flux density field"
  );
  argList::addBoolOption
  (
    "HdotGradH",
    "write the paramagnetic particle force field"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  simpleControl simple{mesh};
  #include "create_fields.inc"

  Info<< "Calculating the magnetic field potential" << endl;
  runTime++;
  while (simple.correctNonOrthogonal())
  {
    solve(fvm::laplacian(murf, psi) + fvc::div(murf*Mrf));
  }
  psi.write();
  if (!args.optionFound("noH") || args.optionFound("HdotGradH"))
  {
    volVectorField H
    (
      IOobject
      (
        "H",
        runTime.timeName(),
        mesh
      ),
      fvc::reconstruct(fvc::snGrad(psi)*mesh.magSf())
    );
    if (!args.optionFound("noH"))
    {
      Info<< nl
        << "Creating field H for time "
        << runTime.timeName() << endl;
      H.write();
    }
    if (args.optionFound("HdotGradH"))
    {
      Info<< nl
        << "Creating field HdotGradH for time "
        << runTime.timeName() << endl;
      volVectorField HdotGradH
      (
        IOobject
        (
          "HdotGradH",
          runTime.timeName(),
          mesh
        ),
        H & fvc::grad(H)
      );
      HdotGradH.write();
    }
  }
  if (!args.optionFound("noB"))
  {
    Info<< nl
      << "Creating field B for time "
      << runTime.timeName() << endl;
    volVectorField B
    (
      IOobject
      (
        "B",
        runTime.timeName(),
        mesh
      ),
      constant::electromagnetic::mu0
     *fvc::reconstruct(murf*fvc::snGrad(psi)*mesh.magSf() + murf*Mrf)
    );
    B.write();
  }
  Info<< "\nEnd\n" << endl;
  return 0;
}
