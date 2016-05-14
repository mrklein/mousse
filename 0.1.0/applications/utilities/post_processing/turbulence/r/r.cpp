// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "turbulent_transport_model.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "incompressible/single_phase_transport_model.hpp"


void calcIncompressibleR
(
  const fvMesh& mesh,
  const Time& runTime,
  const volVectorField& U
)
{
  #include "create_phi.inc"
  singlePhaseTransportModel laminarTransport{U, phi};
  autoPtr<incompressible::turbulenceModel> model
  {
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
  };
  Info << "Writing R field" << nl << endl;
  model->R()().write();
}


void calcCompressibleR
(
  const fvMesh& mesh,
  const Time& runTime,
  const volVectorField& U
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
  if (!rhoHeader.headerOk()) {
    Info << "    no " << rhoHeader.name() <<" field" << endl;
    return;
  }
  Info << "Reading field rho\n" << endl;
  volScalarField rho{rhoHeader, mesh};
  #include "compressible_create_phi.inc"
  autoPtr<fluidThermo> pThermo{fluidThermo::New(mesh)};
  fluidThermo& thermo = pThermo();
  autoPtr<compressible::turbulenceModel> model
  {
    compressible::turbulenceModel::New
    (
      rho,
      U,
      phi,
      thermo
    )
  };
  Info << "Writing R field" << nl << endl;
  model->R()().write();
}


inline bool hasThermo(const Time& time, const fvMesh& mesh)
{
  IOobject thermo
  {
    basicThermo::dictName,
    time.constant(),
    mesh
  };
  return thermo.headerOk();
}


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
    IOobject UHeader
    {
      "U",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ
    };
    if (UHeader.headerOk()) {
      Info << "Reading field " << UHeader.name() << nl << endl;
      volVectorField U{UHeader, mesh};
      if (hasThermo(runTime, mesh)) {
        calcCompressibleR(mesh, runTime, U);
      } else {
        calcIncompressibleR(mesh, runTime, U);
      }
    } else {
      Info << "    no " << UHeader.name() << " field" << endl;
    }
  }
  Info << "End\n" << endl;
  return 0;
}

