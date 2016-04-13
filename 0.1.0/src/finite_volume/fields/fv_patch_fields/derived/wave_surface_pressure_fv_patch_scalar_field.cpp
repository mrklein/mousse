// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wave_surface_pressure_fv_patch_scalar_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "uniform_dimensioned_fields.hpp"
#include "euler_ddt_scheme.hpp"
#include "crank_nicolson_ddt_scheme.hpp"
#include "backward_ddt_scheme.hpp"
#include "time.hpp"


// Static Data Members
namespace mousse {

template<>
const char* NamedEnum
<
  waveSurfacePressureFvPatchScalarField::ddtSchemeType,
  3
>::names[] =
{
  fv::EulerDdtScheme<scalar>::typeName_(),
  fv::CrankNicolsonDdtScheme<scalar>::typeName_(),
  fv::backwardDdtScheme<scalar>::typeName_()
};

}

const mousse::NamedEnum
<
  mousse::waveSurfacePressureFvPatchScalarField::ddtSchemeType,
  3
>   mousse::waveSurfacePressureFvPatchScalarField::ddtSchemeTypeNames_;


// Constructors 
mousse::waveSurfacePressureFvPatchScalarField::
waveSurfacePressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF},
  phiName_{"phi"},
  zetaName_{"zeta"},
  rhoName_{"rho"}
{}


mousse::waveSurfacePressureFvPatchScalarField::
waveSurfacePressureFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  zetaName_{dict.lookupOrDefault<word>("zeta", "zeta")},
  rhoName_{dict.lookupOrDefault<word>("rho", "rho")}
{
  fvPatchField<scalar>::operator=
  (
    scalarField{"value", dict, p.size()}
  );
}


mousse::waveSurfacePressureFvPatchScalarField::
waveSurfacePressureFvPatchScalarField
(
  const waveSurfacePressureFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper},
  phiName_{ptf.phiName_},
  zetaName_{ptf.zetaName_},
  rhoName_{ptf.rhoName_}
{}


mousse::waveSurfacePressureFvPatchScalarField::
waveSurfacePressureFvPatchScalarField
(
  const waveSurfacePressureFvPatchScalarField& wspsf
)
:
  fixedValueFvPatchScalarField{wspsf},
  phiName_{wspsf.phiName_},
  zetaName_{wspsf.zetaName_},
  rhoName_{wspsf.rhoName_}
{}


mousse::waveSurfacePressureFvPatchScalarField::
waveSurfacePressureFvPatchScalarField
(
  const waveSurfacePressureFvPatchScalarField& wspsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{wspsf, iF},
  phiName_{wspsf.phiName_},
  zetaName_{wspsf.zetaName_},
  rhoName_{wspsf.rhoName_}
{}


// Member Functions 
void mousse::waveSurfacePressureFvPatchScalarField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const label patchI = patch().index();
  const scalar dt = db().time().deltaTValue();
  // retrieve non-const access to zeta field from the database
  volVectorField& zeta =
    const_cast<volVectorField&>
    (
      db().lookupObject<volVectorField>(zetaName_)
    );
  vectorField& zetap = zeta.boundaryField()[patchI];
  // lookup d/dt scheme from database for zeta
  const word ddtSchemeName{zeta.mesh().ddtScheme(zeta.name())};
  ddtSchemeType ddtScheme{ddtSchemeTypeNames_[ddtSchemeName]};
  // retrieve the flux field from the database
  const surfaceScalarField& phi =
    db().lookupObject<surfaceScalarField>(phiName_);
  // cache the patch face-normal vectors
  tmp<vectorField> nf{patch().nf()};
  // change in zeta due to flux
  vectorField dZetap{dt*nf()*phi.boundaryField()[patchI]/patch().magSf()};
  if (phi.dimensions() == dimDensity*dimVelocity*dimArea) {
    const scalarField& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    dZetap /= rhop;
  }
  switch (ddtScheme) {
    case tsEuler:
    case tsCrankNicolson:
    {
      zetap = zeta.oldTime().boundaryField()[patchI] + dZetap;
      break;
    }
    case tsBackward:
    {
      scalar dt0 = db().time().deltaT0Value();
      scalar c = 1.0 + dt/(dt + dt0);
      scalar c00 = dt*dt/(dt0*(dt + dt0));
      scalar c0 = c + c00;
      zetap =
        (c0*zeta.oldTime().boundaryField()[patchI]
         - c00*zeta.oldTime().oldTime().boundaryField()[patchI]
         + dZetap)/c;
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "waveSurfacePressureFvPatchScalarField<Type>::updateCoeffs()"
      )
      << "    Unsupported temporal differencing scheme : "
      << ddtSchemeName << nl
      << "    on patch " << this->patch().name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << abort(FatalError);
    }
  }
  Info << "min/max zetap = " << gMin(zetap & nf()) << ", "
    << gMax(zetap & nf()) << endl;
  // update the surface pressure
  const uniformDimensionedVectorField& g =
    db().lookupObject<uniformDimensionedVectorField>("g");
  operator==(-g.value() & zetap);
  fixedValueFvPatchScalarField::updateCoeffs();
}


void mousse::waveSurfacePressureFvPatchScalarField::write(Ostream& os) const
{
  fvPatchScalarField::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "zeta", "zeta", zetaName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  writeEntry("value", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  waveSurfacePressureFvPatchScalarField
);

}

