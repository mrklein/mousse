// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "turbulent_transport_model.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "incompressible/single_phase_transport_model.hpp"
void calcIncompressible
(
  const fvMesh& mesh,
  const Time& runTime,
  const volVectorField& U,
  volVectorField& wallShearStress
)
{
  #include "create_phi.hpp"
  singlePhaseTransportModel laminarTransport(U, phi);
  autoPtr<incompressible::turbulenceModel> model
  (
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
  );
  const volSymmTensorField Reff(model->devReff());
  forAll(wallShearStress.boundaryField(), patchI)
  {
    wallShearStress.boundaryField()[patchI] =
    (
     -mesh.Sf().boundaryField()[patchI]
     /mesh.magSf().boundaryField()[patchI]
    ) & Reff.boundaryField()[patchI];
  }
}
void calcCompressible
(
  const fvMesh& mesh,
  const Time& runTime,
  const volVectorField& U,
  volVectorField& wallShearStress
)
{
  IOobject rhoHeader
  (
    "rho",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::NO_WRITE
  );
  if (!rhoHeader.headerOk())
  {
    Info<< "    no rho field" << endl;
    return;
  }
  Info<< "Reading field rho\n" << endl;
  volScalarField rho(rhoHeader, mesh);
  #include "compressible_create_phi.hpp"
  autoPtr<fluidThermo> pThermo(fluidThermo::New(mesh));
  fluidThermo& thermo = pThermo();
  autoPtr<compressible::turbulenceModel> model
  (
    compressible::turbulenceModel::New(rho, U, phi, thermo)
  );
  const volSymmTensorField Reff(model->devRhoReff());
  forAll(wallShearStress.boundaryField(), patchI)
  {
    wallShearStress.boundaryField()[patchI] =
    (
     -mesh.Sf().boundaryField()[patchI]
     /mesh.magSf().boundaryField()[patchI]
    ) & Reff.boundaryField()[patchI];
  }
}
int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.hpp"
  #include "set_root_case.hpp"
  #include "create_time.hpp"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.hpp"
  forAll(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info<< "Time = " << runTime.timeName() << endl;
    mesh.readUpdate();
    volVectorField wallShearStress
    (
      IOobject
      (
        "wallShearStress",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
      ),
      mesh,
      dimensionedVector
      (
        "wallShearStress",
        sqr(dimLength)/sqr(dimTime),
        vector::zero
      )
    );
    IOobject UHeader
    (
      "U",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    );
    if (UHeader.headerOk())
    {
      Info<< "Reading field U\n" << endl;
      volVectorField U(UHeader, mesh);
      if
      (
        IOobject
        (
          basicThermo::dictName,
          runTime.constant(),
          mesh
        ).headerOk()
      )
      {
        calcCompressible(mesh, runTime, U, wallShearStress);
      }
      else
      {
        calcIncompressible(mesh, runTime, U, wallShearStress);
      }
    }
    else
    {
      Info<< "    no U field" << endl;
    }
    Info<< "Writing wall shear stress to field " << wallShearStress.name()
      << nl << endl;
    wallShearStress.write();
  }
  Info<< "End" << endl;
  return 0;
}
