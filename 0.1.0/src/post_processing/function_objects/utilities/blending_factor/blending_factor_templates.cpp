// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gauss_convection_scheme.hpp"
#include "blended_scheme_base.hpp"
#include "fvc_cell_reduce.hpp"
template<class Type>
mousse::volScalarField& mousse::blendingFactor::factor
(
  const GeometricField<Type, fvPatchField, volMesh>& field
)
{
  const word fieldName = "blendingFactor:" + field.name();
  if (!obr_.foundObject<volScalarField>(fieldName))
  {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    volScalarField* factorPtr =
      new volScalarField
      (
        IOobject
        (
          fieldName,
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("0", dimless, 0.0),
        zeroGradientFvPatchScalarField::typeName
      );
    obr_.store(factorPtr);
  }
  return
    const_cast<volScalarField&>
    (
      obr_.lookupObject<volScalarField>(fieldName)
    );
}
template<class Type>
void mousse::blendingFactor::calc()
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  if (!obr_.foundObject<fieldType>(fieldName_))
  {
    return;
  }
  const fvMesh& mesh = refCast<const fvMesh>(obr_);
  const fieldType& field = mesh.lookupObject<fieldType>(fieldName_);
  const word divScheme("div(" + phiName_ + ',' + fieldName_ + ')');
  ITstream& its = mesh.divScheme(divScheme);
  const surfaceScalarField& phi =
    mesh.lookupObject<surfaceScalarField>(phiName_);
  tmp<fv::convectionScheme<Type> > cs =
    fv::convectionScheme<Type>::New(mesh, phi, its);
  const fv::gaussConvectionScheme<Type>& gcs =
    refCast<const fv::gaussConvectionScheme<Type> >(cs());
  const surfaceInterpolationScheme<Type>& interpScheme =
    gcs.interpScheme();
  if (!isA<blendedSchemeBase<Type> >(interpScheme))
  {
    FATAL_ERROR_IN("void mousse::blendingFactor::execute()")
      << interpScheme.typeName << " is not a blended scheme"
      << exit(FatalError);
  }
  // retrieve the face-based blending factor
  const blendedSchemeBase<Type>& blendedScheme =
    refCast<const blendedSchemeBase<Type> >(interpScheme);
  const surfaceScalarField factorf(blendedScheme.blendingFactor(field));
  // convert into vol field whose values represent the local face maxima
  volScalarField& factor = this->factor(field);
  factor = fvc::cellReduce(factorf, maxEqOp<scalar>());
  factor.correctBoundaryConditions();
}
