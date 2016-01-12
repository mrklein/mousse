// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "external_coupled_temperature_mixed_fv_patch_scalar_field.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "ofstream.hpp"
// Protected Member Functions 
void mousse::externalCoupledTemperatureMixedFvPatchScalarField::writeHeader
(
  OFstream& os
) const
{
  os << "# Values: magSf value qDot htc" << endl;
}
// Constructors 
mousse::externalCoupledTemperatureMixedFvPatchScalarField::
externalCoupledTemperatureMixedFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  externalCoupledMixedFvPatchField<scalar>{p, iF}
{}
mousse::externalCoupledTemperatureMixedFvPatchScalarField::
externalCoupledTemperatureMixedFvPatchScalarField
(
  const externalCoupledTemperatureMixedFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  externalCoupledMixedFvPatchField<scalar>{ptf, p, iF, mapper}
{}
mousse::externalCoupledTemperatureMixedFvPatchScalarField::
externalCoupledTemperatureMixedFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  externalCoupledMixedFvPatchField<scalar>{p, iF, dict}
{}
mousse::externalCoupledTemperatureMixedFvPatchScalarField::
externalCoupledTemperatureMixedFvPatchScalarField
(
  const externalCoupledTemperatureMixedFvPatchScalarField& ecmpf
)
:
  externalCoupledMixedFvPatchField<scalar>{ecmpf}
{}
mousse::externalCoupledTemperatureMixedFvPatchScalarField::
externalCoupledTemperatureMixedFvPatchScalarField
(
  const externalCoupledTemperatureMixedFvPatchScalarField& ecmpf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  externalCoupledMixedFvPatchField<scalar>{ecmpf, iF}
{}
// Destructor 
mousse::externalCoupledTemperatureMixedFvPatchScalarField::
~externalCoupledTemperatureMixedFvPatchScalarField()
{}
// Member Functions 
void mousse::externalCoupledTemperatureMixedFvPatchScalarField::transferData
(
  OFstream& os
) const
{
  if (log())
  {
    Info
      << type() << ": " << this->patch().name()
      << ": writing data to " << os.name()
      << endl;
  }
  const label patchI = patch().index();
  // heat flux [W/m2]
  scalarField qDot(this->patch().size(), 0.0);
  typedef compressible::turbulenceModel cmpTurbModelType;
  static word turbName
  (
    IOobject::groupName
    (
      turbulenceModel::propertiesName,
      dimensionedInternalField().group()
    )
  );
  static word thermoName(basicThermo::dictName);
  if (db().foundObject<cmpTurbModelType>(turbName))
  {
    const cmpTurbModelType& turbModel =
      db().lookupObject<cmpTurbModelType>(turbName);
    const basicThermo& thermo = turbModel.transport();
    const fvPatchScalarField& hep = thermo.he().boundaryField()[patchI];
    qDot = turbModel.alphaEff(patchI)*hep.snGrad();
  }
  else if (db().foundObject<basicThermo>(thermoName))
  {
    const basicThermo& thermo = db().lookupObject<basicThermo>(thermoName);
    const fvPatchScalarField& hep = thermo.he().boundaryField()[patchI];
    qDot = thermo.alpha().boundaryField()[patchI]*hep.snGrad();
  }
  else
  {
    FATAL_ERROR_IN
    (
      "void mousse::externalCoupledTemperatureMixedFvPatchScalarField::"
      "transferData"
      "("
        "OFstream&"
      ") const"
    )
    << "Condition requires either compressible turbulence and/or "
    << "thermo model to be available" << exit(FatalError);
  }
  // patch temperature [K]
  const scalarField Tp(*this);
  // near wall cell temperature [K]
  const scalarField Tc(patchInternalField());
  // heat transfer coefficient [W/m2/K]
  const scalarField htc(qDot/(Tp - Tc + ROOTVSMALL));
  if (Pstream::parRun())
  {
    int tag = Pstream::msgType() + 1;
    List<Field<scalar> > magSfs(Pstream::nProcs());
    magSfs[Pstream::myProcNo()].setSize(this->patch().size());
    magSfs[Pstream::myProcNo()] = this->patch().magSf();
    Pstream::gatherList(magSfs, tag);
    List<Field<scalar> > values(Pstream::nProcs());
    values[Pstream::myProcNo()].setSize(this->patch().size());
    values[Pstream::myProcNo()] = Tp;
    Pstream::gatherList(values, tag);
    List<Field<scalar> > qDots(Pstream::nProcs());
    qDots[Pstream::myProcNo()].setSize(this->patch().size());
    qDots[Pstream::myProcNo()] = qDot;
    Pstream::gatherList(qDots, tag);
    List<Field<scalar> > htcs(Pstream::nProcs());
    htcs[Pstream::myProcNo()].setSize(this->patch().size());
    htcs[Pstream::myProcNo()] = htc;
    Pstream::gatherList(htcs, tag);
    if (Pstream::master())
    {
      FOR_ALL(values, procI)
      {
        const Field<scalar>& magSf = magSfs[procI];
        const Field<scalar>& value = values[procI];
        const Field<scalar>& qDot = qDots[procI];
        const Field<scalar>& htc = htcs[procI];
        FOR_ALL(magSf, faceI)
        {
          os  << magSf[faceI] << token::SPACE
            << value[faceI] << token::SPACE
            << qDot[faceI] << token::SPACE
            << htc[faceI] << token::SPACE
            << nl;
        }
      }
      os.flush();
    }
  }
  else
  {
    const Field<scalar>& magSf(this->patch().magSf());
    FOR_ALL(patch(), faceI)
    {
      os<< magSf[faceI] << token::SPACE
        << Tp[faceI] << token::SPACE
        << qDot[faceI] << token::SPACE
        << htc[faceI] << token::SPACE
        << nl;
    }
    os.flush();
  }
}
void mousse::externalCoupledTemperatureMixedFvPatchScalarField::evaluate
(
  const Pstream::commsTypes comms
)
{
  externalCoupledMixedFvPatchField<scalar>::evaluate(comms);
}
void mousse::externalCoupledTemperatureMixedFvPatchScalarField::write
(
  Ostream& os
) const
{
  externalCoupledMixedFvPatchField<scalar>::write(os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  externalCoupledTemperatureMixedFvPatchScalarField
);
}
