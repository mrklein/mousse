// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nutk_atm_rough_wall_function_fv_patch_scalar_field.hpp"
#include "turbulence_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Protected Member Functions 
tmp<scalarField> nutkAtmRoughWallFunctionFvPatchScalarField::calcNut() const
{
  const label patchi = patch().index();
  const turbulenceModel& turbModel =
    db().lookupObject<turbulenceModel>
    (
      IOobject::groupName
      (
        turbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  const scalarField& y = turbModel.y()[patchi];
  const tmp<volScalarField> tk = turbModel.k();
  const volScalarField& k = tk();
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  const scalar Cmu25 = pow025(Cmu_);
  tmp<scalarField> tnutw{new scalarField{*this}};
  scalarField& nutw = tnutw();
  FOR_ALL(nutw, faceI) {
    label faceCellI = patch().faceCells()[faceI];
    scalar uStar = Cmu25*sqrt(k[faceCellI]);
    scalar yPlus = uStar*y[faceI]/nuw[faceI];
    scalar Edash = (y[faceI] + z0_[faceI])/z0_[faceI];
    nutw[faceI] = nuw[faceI]*(yPlus*kappa_/log(max(Edash, 1+1e-4)) - 1);
    if (debug) {
      Info << "yPlus = " << yPlus
        << ", Edash = " << Edash
        << ", nutw = " << nutw[faceI]
        << endl;
    }
  }
  return tnutw;
}


// Constructors 
nutkAtmRoughWallFunctionFvPatchScalarField::
nutkAtmRoughWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutkWallFunctionFvPatchScalarField{p, iF},
  z0_{p.size(), 0.0}
{}


nutkAtmRoughWallFunctionFvPatchScalarField::
nutkAtmRoughWallFunctionFvPatchScalarField
(
  const nutkAtmRoughWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  nutkWallFunctionFvPatchScalarField{ptf, p, iF, mapper},
  z0_{ptf.z0_, mapper}
{}


nutkAtmRoughWallFunctionFvPatchScalarField::
nutkAtmRoughWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  nutkWallFunctionFvPatchScalarField{p, iF, dict},
  z0_{"z0", dict, p.size()}
{}


nutkAtmRoughWallFunctionFvPatchScalarField::
nutkAtmRoughWallFunctionFvPatchScalarField
(
  const nutkAtmRoughWallFunctionFvPatchScalarField& rwfpsf
)
:
  nutkWallFunctionFvPatchScalarField{rwfpsf},
  z0_{rwfpsf.z0_}
{}


nutkAtmRoughWallFunctionFvPatchScalarField::
nutkAtmRoughWallFunctionFvPatchScalarField
(
  const nutkAtmRoughWallFunctionFvPatchScalarField& rwfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutkWallFunctionFvPatchScalarField{rwfpsf, iF},
  z0_{rwfpsf.z0_}
{}


// Member Functions 
void nutkAtmRoughWallFunctionFvPatchScalarField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  nutkWallFunctionFvPatchScalarField::autoMap(m);
  z0_.autoMap(m);
}


void nutkAtmRoughWallFunctionFvPatchScalarField::rmap
(
  const fvPatchScalarField& ptf,
  const labelList& addr
)
{
  nutkWallFunctionFvPatchScalarField::rmap(ptf, addr);
  const nutkAtmRoughWallFunctionFvPatchScalarField& nrwfpsf =
    refCast<const nutkAtmRoughWallFunctionFvPatchScalarField>(ptf);
  z0_.rmap(nrwfpsf.z0_, addr);
}


void nutkAtmRoughWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  writeLocalEntries(os);
  z0_.writeEntry("z0", os);
  writeEntry("value", os);
}

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  nutkAtmRoughWallFunctionFvPatchScalarField
);

}  // namespace mousse

