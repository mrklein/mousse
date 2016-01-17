// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "single_phase_transport_model.hpp"
#include "turbulent_transport_model.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "nut_wall_function_fv_patch_scalar_field.hpp"
#include "near_wall_dist.hpp"
#include "wall_fv_patch.hpp"
template<class TurbulenceModel>
void calcYPlus
(
  const TurbulenceModel& turbulenceModel,
  const fvMesh& mesh,
  const volVectorField& U,
  volScalarField& yPlus
)
{
  volScalarField::GeometricBoundaryField d = nearWallDist(mesh).y();
  const volScalarField::GeometricBoundaryField nutBf =
    turbulenceModel->nut()().boundaryField();
  const volScalarField::GeometricBoundaryField nuEffBf =
    turbulenceModel->nuEff()().boundaryField();
  const volScalarField::GeometricBoundaryField nuBf =
    turbulenceModel->nu()().boundaryField();
  const fvPatchList& patches = mesh.boundary();
  FOR_ALL(patches, patchi)
  {
    const fvPatch& patch = patches[patchi];
    if (isA<nutWallFunctionFvPatchScalarField>(nutBf[patchi]))
    {
      const nutWallFunctionFvPatchScalarField& nutPf =
        dynamic_cast<const nutWallFunctionFvPatchScalarField&>
        (
          nutBf[patchi]
        );
      yPlus.boundaryField()[patchi] = nutPf.yPlus();
      const scalarField& Yp = yPlus.boundaryField()[patchi];
      Info<< "Patch " << patchi
        << " named " << nutPf.patch().name()
        << ", wall-function " << nutPf.type()
        << ", y+ : min: " << gMin(Yp) << " max: " << gMax(Yp)
        << " average: " << gAverage(Yp) << nl << endl;
    }
    else if (isA<wallFvPatch>(patch))
    {
      yPlus.boundaryField()[patchi] =
        d[patchi]
       *sqrt
        (
          nuEffBf[patchi]
         *mag(U.boundaryField()[patchi].snGrad())
        )/nuBf[patchi];
      const scalarField& Yp = yPlus.boundaryField()[patchi];
      Info<< "Patch " << patchi
        << " named " << patch.name()
        << " y+ : min: " << gMin(Yp) << " max: " << gMax(Yp)
        << " average: " << gAverage(Yp) << nl << endl;
    }
  }
}
void calcIncompressibleYPlus
(
  const fvMesh& mesh,
  const Time& runTime,
  const volVectorField& U,
  volScalarField& yPlus
)
{
  #include "create_phi.inc"
  singlePhaseTransportModel laminarTransport(U, phi);
  autoPtr<incompressible::turbulenceModel> turbulenceModel
  (
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
  );
  calcYPlus(turbulenceModel, mesh, U, yPlus);
}
void calcCompressibleYPlus
(
  const fvMesh& mesh,
  const Time& runTime,
  const volVectorField& U,
  volScalarField& yPlus
)
{
  IOobject rhoHeader
  {
    "rho",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::NO_WRITE
  };
  if (!rhoHeader.headerOk())
  {
    Info<< "    no rho field" << endl;
    return;
  }
  Info<< "Reading field rho\n" << endl;
  volScalarField rho{rhoHeader, mesh};
  #include "compressible_create_phi.inc"
  autoPtr<fluidThermo> pThermo{fluidThermo::New(mesh)};
  fluidThermo& thermo = pThermo();
  autoPtr<compressible::turbulenceModel> turbulenceModel
  {
    compressible::turbulenceModel::New
    (
      rho,
      U,
      phi,
      thermo
    )
  };
  calcYPlus(turbulenceModel, mesh, U, yPlus);
}
int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select(runTime, args, "yPlus");
  #include "create_named_mesh.inc"
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info<< "Time = " << runTime.timeName() << endl;
    mesh.readUpdate();
    volScalarField yPlus
    {
      // IOobject
      {
        "yPlus",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      {"yPlus", dimless, 0.0}
    };
    IOobject UHeader
    {
      "U",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    };
    if (UHeader.headerOk())
    {
      Info<< "Reading field U\n" << endl;
      volVectorField U{UHeader, mesh};
      if
      (
        IOobject
        {
          basicThermo::dictName,
          runTime.constant(),
          mesh
        }.headerOk()
      )
      {
        calcCompressibleYPlus(mesh, runTime, U, yPlus);
      }
      else
      {
        calcIncompressibleYPlus(mesh, runTime, U, yPlus);
      }
    }
    else
    {
      Info<< "    no U field" << endl;
    }
    Info<< "Writing yPlus to field " << yPlus.name() << nl << endl;
    yPlus.write();
  }
  Info<< "End\n" << endl;
  return 0;
}
