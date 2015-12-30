// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wave_transmissive_fv_patch_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "euler_ddt_scheme.hpp"
#include "crank_nicolson_ddt_scheme.hpp"
#include "backward_ddt_scheme.hpp"
// Constructors 
template<class Type>
mousse::waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  advectiveFvPatchField<Type>(p, iF),
  psiName_("thermo:psi"),
  gamma_(0.0)
{}
template<class Type>
mousse::waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
  const waveTransmissiveFvPatchField& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  advectiveFvPatchField<Type>(ptf, p, iF, mapper),
  psiName_(ptf.psiName_),
  gamma_(ptf.gamma_)
{}
template<class Type>
mousse::waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  advectiveFvPatchField<Type>(p, iF, dict),
  psiName_(dict.lookupOrDefault<word>("psi", "thermo:psi")),
  gamma_(readScalar(dict.lookup("gamma")))
{}
template<class Type>
mousse::waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
  const waveTransmissiveFvPatchField& ptpsf
)
:
  advectiveFvPatchField<Type>(ptpsf),
  psiName_(ptpsf.psiName_),
  gamma_(ptpsf.gamma_)
{}
template<class Type>
mousse::waveTransmissiveFvPatchField<Type>::waveTransmissiveFvPatchField
(
  const waveTransmissiveFvPatchField& ptpsf,
  const DimensionedField<Type, volMesh>& iF
)
:
  advectiveFvPatchField<Type>(ptpsf, iF),
  psiName_(ptpsf.psiName_),
  gamma_(ptpsf.gamma_)
{}
// Member Functions 
template<class Type>
mousse::tmp<mousse::scalarField>
mousse::waveTransmissiveFvPatchField<Type>::advectionSpeed() const
{
  // Lookup the velocity and compressibility of the patch
  const fvPatchField<scalar>& psip =
    this->patch().template
      lookupPatchField<volScalarField, scalar>(psiName_);
  const surfaceScalarField& phi =
    this->db().template lookupObject<surfaceScalarField>(this->phiName_);
  fvsPatchField<scalar> phip =
    this->patch().template
      lookupPatchField<surfaceScalarField, scalar>(this->phiName_);
  if (phi.dimensions() == dimDensity*dimVelocity*dimArea)
  {
    const fvPatchScalarField& rhop =
      this->patch().template
        lookupPatchField<volScalarField, scalar>(this->rhoName_);
    phip /= rhop;
  }
  // Calculate the speed of the field wave w
  // by summing the component of the velocity normal to the boundary
  // and the speed of sound (sqrt(gamma_/psi)).
  return phip/this->patch().magSf() + sqrt(gamma_/psip);
}
template<class Type>
void mousse::waveTransmissiveFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  this->template
    writeEntryIfDifferent<word>(os, "phi", "phi", this->phiName_);
  this->template
    writeEntryIfDifferent<word>(os, "rho", "rho", this->rhoName_);
  this->template
    writeEntryIfDifferent<word>(os, "psi", "thermo:psi", psiName_);
  os.writeKeyword("gamma") << gamma_ << token::END_STATEMENT << nl;
  if (this->lInf_ > SMALL)
  {
    os.writeKeyword("fieldInf") << this->fieldInf_
      << token::END_STATEMENT << nl;
    os.writeKeyword("lInf") << this->lInf_
      << token::END_STATEMENT << nl;
  }
  this->writeEntry("value", os);
}
