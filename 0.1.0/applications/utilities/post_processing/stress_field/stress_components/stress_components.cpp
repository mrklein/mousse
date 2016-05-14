// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "incompressible/single_phase_transport_model.hpp"
#include "zero_gradient_fv_patch_fields.hpp"


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
    IOobject Uheader
    {
      "U",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ
    };
    // Check U exists
    if (Uheader.headerOk()) {
      mesh.readUpdate();
      Info << "    Reading U" << endl;
      volVectorField U{Uheader, mesh};
      #include "create_phi.inc"
      singlePhaseTransportModel laminarTransport{U, phi};
      volSymmTensorField sigma
      {
        {
          "sigma",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        laminarTransport.nu()*2*dev(symm(fvc::grad(U)))
      };
      volScalarField sigmaxx
      {
        {
          "sigmaxx",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ
        },
        sigma.component(symmTensor::XX)
      };
      sigmaxx.write();
      volScalarField sigmayy
      {
        {
          "sigmayy",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ
        },
        sigma.component(symmTensor::YY)
      };
      sigmayy.write();
      volScalarField sigmazz
      {
        {
          "sigmazz",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ
        },
        sigma.component(symmTensor::ZZ)
      };
      sigmazz.write();
      volScalarField sigmaxy
      {
        {
          "sigmaxy",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ
        },
        sigma.component(symmTensor::XY)
      };
      sigmaxy.write();
      volScalarField sigmaxz
      {
        {
          "sigmaxz",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ
        },
        sigma.component(symmTensor::XZ)
      };
      sigmaxz.write();
      volScalarField sigmayz
      {
        {
          "sigmayz",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ
        },
        sigma.component(symmTensor::YZ)
      };
      sigmayz.write();
      volVectorField Ub
      {
        {
          "Ub",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ
        },
        U,
        zeroGradientFvPatchVectorField::typeName
      };
      Ub.correctBoundaryConditions();
      Ub.write();
      volScalarField sigmaUn
      {
        {
          "sigmaUn",
          runTime.timeName(),
          mesh,
          IOobject::NO_READ
        },
        0.0*sigma.component(symmTensor::YZ)
      };
      FOR_ALL(sigmaUn.boundaryField(), patchI) {
        sigmaUn.boundaryField()[patchI] =
        (
          mesh.boundary()[patchI].nf() & sigma.boundaryField()[patchI]
        )().component(vector::X);
      }
      sigmaUn.write();
    } else {
      Info << "    No U" << endl;
    }
    Info << endl;
  }
  Info << "End" << endl;
  return 0;
}

