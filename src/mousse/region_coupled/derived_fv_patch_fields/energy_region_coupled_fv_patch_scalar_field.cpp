// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#include "energy_region_coupled_fv_patch_scalar_field.hpp"
#include "time.hpp"
#include "turbulent_fluid_thermo_model.hpp"


// Static Member Data 
namespace mousse {

template<>
const char* mousse::NamedEnum
<
  mousse::energyRegionCoupledFvPatchScalarField::kappaMethodType,
  3
>::names[] =
{
  "solid",
  "fluid",
  "undefined"
};

}

const mousse::NamedEnum
<
  mousse::energyRegionCoupledFvPatchScalarField::kappaMethodType,
  3
> mousse::energyRegionCoupledFvPatchScalarField::methodTypeNames_;


// Private members
void mousse::energyRegionCoupledFvPatchScalarField::setMethod() const
{
  if (method_ == UNDEFINED) {
    if (this->db().foundObject<compressible::turbulenceModel>
        (
          turbulenceModel::propertiesName
        )) {
      method_ = FLUID;
    } else {
      method_ = SOLID;
    }
  }
  if (!nbrThermoPtr_) {
    nbrThermoPtr_ =
      (
        &regionCoupledPatch_.nbrMesh().lookupObject<basicThermo>
        (
          basicThermo::dictName
        )
      );
  }
  if (!thermoPtr_) {
    thermoPtr_ =
      (
        &this->db().lookupObject<basicThermo>
        (
          basicThermo::dictName
        )
      );
  }
}


mousse::tmp<mousse::scalarField> mousse::energyRegionCoupledFvPatchScalarField::
kappa() const
{
  switch (method_) {
    case FLUID:
      {
        const compressible::turbulenceModel& turbModel =
          this->db().lookupObject<compressible::turbulenceModel>
          (
            turbulenceModel::propertiesName
          );
        return turbModel.kappaEff(patch().index());
      }
      break;
    case SOLID:
      {
        const basicThermo& thermo =
          this->db().lookupObject<basicThermo>
          (
            basicThermo::dictName
          );
        return thermo.kappa(patch().index());
      }
      break;
    case UNDEFINED:
      {
        FATAL_ERROR_IN("energyRegionCoupledFvPatchScalarField::kappa() const")
          << " on mesh " << this->db().name() << " patch "
          << patch().name()
          << " could not find a method in. Methods are:  "
          << methodTypeNames_.toc()
          << " Not turbulenceModel or thermophysicalProperties"
          << " were found"
          << exit(FatalError);
      }
      break;
  }
  return scalarField{0};
}


mousse::tmp<mousse::scalarField> mousse::energyRegionCoupledFvPatchScalarField::
weights() const
{
  const fvPatch& patch = regionCoupledPatch_.patch();
  const scalarField deltas{patch.nf() & patch.delta()};
  const scalarField alphaDelta{kappa()/deltas};
  const fvPatch& nbrPatch = regionCoupledPatch_.neighbFvPatch();
  const energyRegionCoupledFvPatchScalarField& nbrField =
    refCast
    <
      const energyRegionCoupledFvPatchScalarField
    >
    (
      nbrPatch.lookupPatchField<volScalarField, scalar>("T")
    );
  // Needed in the first calculation of weights
  nbrField.setMethod();
  const scalarField nbrAlpha
  {
    regionCoupledPatch_.regionCoupledPatch().interpolate(nbrField.kappa())
  };
  const scalarField nbrDeltas
  {
    regionCoupledPatch_.regionCoupledPatch().interpolate
    (
      nbrPatch.nf() & nbrPatch.delta()
    )
  };
  const scalarField nbrAlphaDelta{nbrAlpha/nbrDeltas};
  tmp<scalarField> tw{new scalarField{deltas.size()}};
  scalarField& w = tw();
  FOR_ALL(alphaDelta, faceI) {
    scalar di = alphaDelta[faceI];
    scalar dni = nbrAlphaDelta[faceI];
    w[faceI] = di/(di + dni);
  }
  return tw;
}


// Constructors 
mousse::energyRegionCoupledFvPatchScalarField::
energyRegionCoupledFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  coupledFvPatchField<scalar>{p, iF},
  regionCoupledPatch_{refCast<const regionCoupledBaseFvPatch>(p)},
  method_{UNDEFINED},
  nbrThermoPtr_{nullptr},
  thermoPtr_{nullptr}
{}


mousse::energyRegionCoupledFvPatchScalarField::
energyRegionCoupledFvPatchScalarField
(
  const energyRegionCoupledFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  coupledFvPatchField<scalar>{ptf, p, iF, mapper},
  regionCoupledPatch_{refCast<const regionCoupledBaseFvPatch>(p)},
  method_{ptf.method_},
  nbrThermoPtr_{nullptr},
  thermoPtr_{nullptr}
{}


mousse::energyRegionCoupledFvPatchScalarField::
energyRegionCoupledFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  coupledFvPatchField<scalar>{p, iF, dict},
  regionCoupledPatch_{refCast<const regionCoupledBaseFvPatch>(p)},
  method_{UNDEFINED},
  nbrThermoPtr_{nullptr},
  thermoPtr_{nullptr}
{
  if (!isA<regionCoupledBase>(this->patch().patch())) {
    FATAL_ERROR_IN
    (
      "energyRegionCoupledFvPatchScalarField::"
      "energyRegionCoupledFvPatchScalarField\n"
      "(\n"
      "  const fvPatch& p,\n"
      "  const DimensionedField<scalar, volMesh>& iF,\n"
      "  const dictionary& dict\n"
      ")\n"
    )
    << "\n    patch type '" << p.type()
    << "' not type '" << regionCoupledBase::typeName << "'"
    << "\n    for patch " << p.name()
    << " of field " << dimensionedInternalField().name()
    << " in file " << dimensionedInternalField().objectPath()
    << exit(FatalError);
  }
}


mousse::energyRegionCoupledFvPatchScalarField::
energyRegionCoupledFvPatchScalarField
(
  const energyRegionCoupledFvPatchScalarField& ptf
)
:
  coupledFvPatchField<scalar>{ptf},
  regionCoupledPatch_{ptf.regionCoupledPatch_},
  method_{ptf.method_},
  nbrThermoPtr_{nullptr},
  thermoPtr_{nullptr}
{}


mousse::energyRegionCoupledFvPatchScalarField::
energyRegionCoupledFvPatchScalarField
(
  const energyRegionCoupledFvPatchScalarField& ptf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  coupledFvPatchField<scalar>{ptf, iF},
  regionCoupledPatch_{ptf.regionCoupledPatch_},
  method_{ptf.method_},
  nbrThermoPtr_{nullptr},
  thermoPtr_{nullptr}
{}


// Member Functions 
mousse::tmp<mousse::scalarField> mousse::energyRegionCoupledFvPatchScalarField::
snGrad() const
{
  DEBUG("snGrad");
  return
    regionCoupledPatch_.patch().deltaCoeffs()*(*this - patchInternalField());
}


mousse::tmp<mousse::scalarField> mousse::energyRegionCoupledFvPatchScalarField::
snGrad(const scalarField&) const
{
  DEBUG("snGrad");
  return snGrad();
}


void mousse::energyRegionCoupledFvPatchScalarField::evaluate
(
  const Pstream::commsTypes
)
{
  if (!updated()) {
    updateCoeffs();
  }
  label patchi = patch().index();
  const scalarField& pp =  thermoPtr_->p().boundaryField()[patchi];
  const scalarField lWeights(weights());
  scalarField::operator=
  (
    thermoPtr_->he(pp,
                   lWeights*patchInternalTemperatureField()
                   + (1.0 - lWeights)*patchNeighbourTemperatureField(),
                   patchi)
  );
  fvPatchScalarField::evaluate();
}


mousse::tmp<mousse::Field<mousse::scalar> >
mousse::energyRegionCoupledFvPatchScalarField::
patchNeighbourField() const
{
  const fvPatch& nbrPatch = regionCoupledPatch_.neighbFvPatch();
  const labelUList& nbrFaceCells = nbrPatch.faceCells();
  setMethod();
  const scalarField nbrIntT{nbrThermoPtr_->T().internalField(), nbrFaceCells};
  scalarField intNbrT
  {
    regionCoupledPatch_.regionCoupledPatch().interpolate(nbrIntT)
  };
  label patchi = patch().index();
  const scalarField& pp =  thermoPtr_->p().boundaryField()[patchi];
  tmp<scalarField> tmyHE = thermoPtr_->he(pp, intNbrT, patchi);
  return tmyHE;
}


mousse::tmp<mousse::scalarField> mousse::energyRegionCoupledFvPatchScalarField::
patchNeighbourTemperatureField() const
{
  const fvPatch& nbrPatch = regionCoupledPatch_.neighbFvPatch();
  const labelUList& nbrFaceCells = nbrPatch.faceCells();
  const scalarField nbrIntT
  {
    nbrThermoPtr_->T().internalField(), nbrFaceCells
  };
  tmp<scalarField> tintNbrT =
    regionCoupledPatch_.regionCoupledPatch().interpolate(nbrIntT);
  return tintNbrT;
}


mousse::tmp<mousse::scalarField> mousse::energyRegionCoupledFvPatchScalarField::
patchInternalTemperatureField() const
{
  const labelUList& faceCells = regionCoupledPatch_.faceCells();
  tmp<scalarField> tintT
  {
    new scalarField{thermoPtr_->T().internalField(), faceCells}
  };
  return tintT;
}


void mousse::energyRegionCoupledFvPatchScalarField::updateInterfaceMatrix
(
  Field<scalar>& result,
  const scalarField& psiInternal,
  const scalarField& coeffs,
  const direction /*cmpt*/,
  const Pstream::commsTypes
) const
{
  setMethod();
  scalarField myHE{this->size()};
  if (&psiInternal == &internalField()) {
    label patchi = this->patch().index();
    const scalarField& pp =  thermoPtr_->p().boundaryField()[patchi];
    const scalarField& Tp =  thermoPtr_->T().boundaryField()[patchi];
    myHE = thermoPtr_->he(pp, Tp, patchi);
  } else {
    //NOTE: This is not correct for preconditioned solvers
    // psiInternal is not the information needed of the slave
    FOR_ALL(*this, facei) {
      myHE[facei] = psiInternal[regionCoupledPatch_.faceCells()[facei]];
    }
  }
  // Multiply the field by coefficients and add into the result
  const labelUList& faceCells = regionCoupledPatch_.faceCells();
  FOR_ALL(faceCells, elemI) {
    result[faceCells[elemI]] -= coeffs[elemI]*myHE[elemI];
  }
}


void mousse::energyRegionCoupledFvPatchScalarField::updateInterfaceMatrix
(
  Field<scalar>& /*result*/,
  const Field<scalar>& /*psiInternal*/,
  const scalarField& /*coeffs*/,
  const Pstream::commsTypes
) const
{
  NOT_IMPLEMENTED
  (
    "energyRegionCoupledFvPatchScalarField::updateInterfaceMatrix"
    "("
    "  Field<scalar>&"
    "  const Field<scalar>&"
    "  const scalarField&"
    "  const Pstream::commsTypes"
    ") const"
  );
}


void mousse::energyRegionCoupledFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  this->writeEntry("value", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  energyRegionCoupledFvPatchScalarField
);

};

