// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fan_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "tuple2.hpp"
#include "polynomial_entry.hpp"
// Static Data Members
namespace mousse
{
  makeTemplatePatchTypeField
  (
    fvPatchScalarField,
    fanFvPatchScalarField
  );
}
// Private Member Functions 
template<>
void mousse::fanFvPatchField<mousse::scalar>::calcFanJump()
{
  if (this->cyclicPatch().owner())
  {
    const surfaceScalarField& phi =
      db().lookupObject<surfaceScalarField>(phiName_);
    const fvsPatchField<scalar>& phip =
      patch().patchField<surfaceScalarField, scalar>(phi);
    scalarField Un(max(phip/patch().magSf(), scalar(0)));
    if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
    {
      Un /= patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    }
    this->jump_ = max(this->jumpTable_->value(Un), scalar(0));
  }
}
// Constructors 
template<>
mousse::fanFvPatchField<mousse::scalar>::fanFvPatchField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  uniformJumpFvPatchField<scalar>(p, iF),
  phiName_(dict.lookupOrDefault<word>("phi", "phi")),
  rhoName_(dict.lookupOrDefault<word>("rho", "rho"))
{
  if (this->cyclicPatch().owner())
  {
    if (dict.found("f"))
    {
      // Backwards compatibility
      Istream& is = dict.lookup("f");
      is.format(IOstream::ASCII);
      scalarList f(is);
      label nPows = 0;
      forAll(f, powI)
      {
        if (mag(f[powI]) > VSMALL)
        {
          nPows++;
        }
      }
      List<Tuple2<scalar, scalar> > coeffs(nPows);
      nPows = 0;
      forAll(f, powI)
      {
        if (mag(f[powI]) > VSMALL)
        {
          coeffs[nPows++] = Tuple2<scalar, scalar>(f[powI], powI);
        }
      }
      this->jumpTable_.reset
      (
        new PolynomialEntry<scalar>("jumpTable", coeffs)
      );
    }
    else
    {
      // Generic input constructed from dictionary
      this->jumpTable_ = DataEntry<scalar>::New("jumpTable", dict);
    }
  }
  if (dict.found("value"))
  {
    fvPatchScalarField::operator=
    (
      scalarField("value", dict, p.size())
    );
  }
  else
  {
    this->evaluate(Pstream::blocking);
  }
}
