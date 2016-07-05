// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "adjust_phi.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "inlet_outlet_fv_patch_fields.hpp"
#include "pstream_reduce_ops.hpp"


// Global Functions 
bool mousse::adjustPhi
(
  surfaceScalarField& phi,
  const volVectorField& U,
  volScalarField& p
)
{
  if (p.needReference()) {
    scalar massIn = 0.0;
    scalar fixedMassOut = 0.0;
    scalar adjustableMassOut = 0.0;
    surfaceScalarField::GeometricBoundaryField& bphi = phi.boundaryField();

    FOR_ALL(bphi, patchi) {
      const fvPatchVectorField& Up = U.boundaryField()[patchi];
      const fvsPatchScalarField& phip = bphi[patchi];
      if (!phip.coupled()) {
        if (Up.fixesValue() && !isA<inletOutletFvPatchVectorField>(Up)) {
          FOR_ALL(phip, i) {
            if (phip[i] < 0.0) {
              massIn -= phip[i];
            } else {
              fixedMassOut += phip[i];
            }
          }
        } else {
          FOR_ALL(phip, i) {
            if (phip[i] < 0.0) {
              massIn -= phip[i];
            } else {
              adjustableMassOut += phip[i];
            }
          }
        }
      }
    }

    // Calculate the total flux in the domain, used for normalisation
    scalar totalFlux = VSMALL + sum(mag(phi)).value();
    reduce(massIn, sumOp<scalar>());
    reduce(fixedMassOut, sumOp<scalar>());
    reduce(adjustableMassOut, sumOp<scalar>());
    scalar massCorr = 1.0;
    scalar magAdjustableMassOut = mag(adjustableMassOut);
    if (magAdjustableMassOut > VSMALL
        && magAdjustableMassOut/totalFlux > SMALL)
    {
      massCorr = (massIn - fixedMassOut)/adjustableMassOut;
    } else if (mag(fixedMassOut - massIn)/totalFlux > 1e-8) {
      FATAL_ERROR_IN
      (
        "adjustPhi"
        "("
          "surfaceScalarField&, "
          "const volVectorField&,"
          "volScalarField&"
        ")"
      )
      << "Continuity error cannot be removed by adjusting the"
         " outflow.\nPlease check the velocity boundary conditions"
         " and/or run potentialFoam to initialise the outflow." << nl
      << "Total flux              : " << totalFlux << nl
      << "Specified mass inflow   : " << massIn << nl
      << "Specified mass outflow  : " << fixedMassOut << nl
      << "Adjustable mass outflow : " << adjustableMassOut << nl
      << exit(FatalError);
    }
    FOR_ALL(bphi, patchi) {
      const fvPatchVectorField& Up = U.boundaryField()[patchi];
      fvsPatchScalarField& phip = bphi[patchi];
      if (!phip.coupled()) {
        if (!Up.fixesValue() || isA<inletOutletFvPatchVectorField>(Up)) {
          FOR_ALL(phip, i) {
            if (phip[i] > 0.0) {
              phip[i] *= massCorr;
            }
          }
        }
      }
    }
    return mag(massIn)/totalFlux < SMALL
      && mag(fixedMassOut)/totalFlux < SMALL
      && mag(adjustableMassOut)/totalFlux < SMALL;
  } else {
    return false;
  }
}
