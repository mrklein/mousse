// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//
// Application
//   mousse-pe
// Description
//   Calculates the Peclet number Pe from the flux phi and writes the maximum
//   value, the surfaceScalarField Pef and volScalarField Pe.
//   With the -noWrite option just outputs the max value without writing
//   the fields.

#include "calc.hpp"
#include "surface_interpolate.hpp"
#include "fvc_average.hpp"
#include "single_phase_transport_model.hpp"
#include "turbulent_transport_model.hpp"
#include "turbulent_fluid_thermo_model.hpp"


void mousse::calc(const argList& args, const Time& runTime, const fvMesh& mesh)
{
  bool writeResults = !args.optionFound("noWrite");
  IOobject phiHeader
  {
    "phi",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ
  };
  if (phiHeader.headerOk()) {
    autoPtr<surfaceScalarField> PePtr;
    Info << "    Reading phi" << endl;
    surfaceScalarField phi{phiHeader, mesh};
    volVectorField U
    {
      {
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
      },
      mesh
    };
    IOobject turbulencePropertiesHeader
    {
      "turbulenceProperties",
      runTime.constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED
    };
    Info << "    Calculating Pe" << endl;
    const auto& deltaCoeffs = mesh.surfaceInterpolation::deltaCoeffs();
    if (phi.dimensions() == dimensionSet{0, 3, -1, 0, 0}) {
      if (turbulencePropertiesHeader.headerOk()) {
        singlePhaseTransportModel laminarTransport(U, phi);
        autoPtr<incompressible::turbulenceModel> turbulenceModel
        {
          incompressible::turbulenceModel::New
          (
            U,
            phi,
            laminarTransport
          )
        };
        const auto nuEff = fvc::interpolate(turbulenceModel->nuEff());
        PePtr.set
        (
          new surfaceScalarField
          {
            {
              "Pef",
              runTime.timeName(),
              mesh
            },
            mag(phi)/(mesh.magSf()*deltaCoeffs*nuEff)
          }
        );
      } else {
        IOdictionary transportProperties
        {
          {
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED
          }
        };
        dimensionedScalar nu{transportProperties.lookup("nu")};
        PePtr.set
        (
          new surfaceScalarField
          {
            {
              "Pef",
              runTime.timeName(),
              mesh
            },
            mag(phi)/(mesh.magSf()*deltaCoeffs*nu)
          }
        );
      }
    } else if (phi.dimensions() == dimensionSet{1, 0, -1, 0, 0}) {
      if (turbulencePropertiesHeader.headerOk()) {
        autoPtr<fluidThermo> thermo{fluidThermo::New(mesh)};
        volScalarField rho
        {
          {
            "rho",
            runTime.timeName(),
            mesh
          },
          thermo->rho()
        };
        autoPtr<compressible::turbulenceModel> turbulenceModel
        {
          compressible::turbulenceModel::New
          (
            rho,
            U,
            phi,
            thermo()
          )
        };
        const auto muEff = fvc::interpolate(turbulenceModel->muEff()); 
        PePtr.set
        (
          new surfaceScalarField
          {
            {
              "Pef",
              runTime.timeName(),
              mesh
            },
            mag(phi)/(mesh.magSf()*deltaCoeffs*muEff)
          }
        );
      } else {
        IOdictionary transportProperties
        {
          {
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED
          }
        };
        dimensionedScalar mu{transportProperties.lookup("mu")};
        PePtr.set
        (
          new surfaceScalarField
          {
            {
              "Pef",
              runTime.timeName(),
              mesh
            },
            mag(phi)/(mesh.magSf()*deltaCoeffs*mu)
          }
        );
      }
    } else {
      FATAL_ERROR_IN(args.executable())
        << "Incorrect dimensions of phi: " << phi.dimensions()
          << abort(FatalError);
    }
    Info << "    Pe max : " << max(PePtr()).value() << endl;
    if (writeResults) {
      Info << "    Writing surfaceScalarField : "
        << PePtr().name() << endl;
      PePtr().write();
      volScalarField Pe
      {
        {
          "Pe",
          runTime.timeName(),
          mesh
        },
        fvc::average(PePtr())
      };
      Info << "    Writing volScalarField : "
        << Pe.name() << endl;
      Pe.write();
    }
  } else {
    Info << "    No phi" << endl;
  }
}

