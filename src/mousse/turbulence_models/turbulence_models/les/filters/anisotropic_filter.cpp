// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "anisotropic_filter.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "wall_fv_patch.hpp"
#include "fvc.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(anisotropicFilter, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(LESfilter, anisotropicFilter, dictionary);

}


// Constructors 
mousse::anisotropicFilter::anisotropicFilter
(
  const fvMesh& mesh,
  scalar widthCoeff
)
:
  LESfilter{mesh},
  widthCoeff_{widthCoeff},
  coeff_
  {
    IOobject
    (
      "anisotropicFilterCoeff",
      mesh.time().timeName(),
      mesh
    ),
    mesh,
    {"zero", dimLength*dimLength, vector::zero},
    calculatedFvPatchVectorField::typeName
  }
{
  for (direction d=0; d<vector::nComponents; d++) {
    coeff_.internalField().replace
    (
      d,
      (1/widthCoeff_)*
      sqr
      (
        2.0*mesh.V()
       /fvc::surfaceSum(mag(mesh.Sf().component(d)))().internalField()
      )
    );
  }
}


mousse::anisotropicFilter::anisotropicFilter
(
  const fvMesh& mesh,
  const dictionary& bd
)
:
  LESfilter{mesh},
  widthCoeff_{readScalar(bd.subDict(type() + "Coeffs").lookup("widthCoeff"))},
  coeff_
  {
    IOobject
    (
      "anisotropicFilterCoeff",
      mesh.time().timeName(),
      mesh
    ),
    mesh,
    {"zero", dimLength*dimLength, vector::zero},
    calculatedFvPatchScalarField::typeName
  }
{
  for (direction d=0; d<vector::nComponents; d++) {
    coeff_.internalField().replace
    (
      d,
      (1/widthCoeff_)*
      sqr
      (
        2.0*mesh.V()
       /fvc::surfaceSum(mag(mesh.Sf().component(d)))().internalField()
      )
    );
  }
}


// Member Functions 
void mousse::anisotropicFilter::read(const dictionary& bd)
{
  bd.subDict(type() + "Coeffs").lookup("widthCoeff") >> widthCoeff_;
}


// Member Operators 
mousse::tmp<mousse::volScalarField> mousse::anisotropicFilter::operator()
(
  const tmp<volScalarField>& unFilteredField
) const
{
  tmp<volScalarField> tmpFilteredField =
    unFilteredField
    + (coeff_
       & fvc::surfaceIntegrate(mesh().Sf() *fvc::snGrad(unFilteredField())));
  unFilteredField.clear();
  return tmpFilteredField;
}


mousse::tmp<mousse::volVectorField> mousse::anisotropicFilter::operator()
(
  const tmp<volVectorField>& unFilteredField
) const
{
  tmp<volVectorField> tmpFilteredField =
    unFilteredField
    + (coeff_
       & fvc::surfaceIntegrate(mesh().Sf()*fvc::snGrad(unFilteredField())));
  unFilteredField.clear();
  return tmpFilteredField;
}


mousse::tmp<mousse::volSymmTensorField> mousse::anisotropicFilter::operator()
(
  const tmp<volSymmTensorField>& unFilteredField
) const
{
  tmp<volSymmTensorField> tmpFilteredField
  {
    new volSymmTensorField
    {
      {
        "anisotropicFilteredSymmTensorField",
        mesh().time().timeName(),
        mesh()
      },
      mesh(),
      unFilteredField().dimensions()
    }
  };
  for (direction d=0; d<symmTensor::nComponents; d++) {
    tmpFilteredField().replace
    (
      d, anisotropicFilter::operator()(unFilteredField().component(d))
    );
  }
  unFilteredField.clear();
  return tmpFilteredField;
}


mousse::tmp<mousse::volTensorField> mousse::anisotropicFilter::operator()
(
  const tmp<volTensorField>& unFilteredField
) const
{
  tmp<volTensorField> tmpFilteredField
  {
    new volTensorField
    {
      {
        "anisotropicFilteredTensorField",
        mesh().time().timeName(),
        mesh()
      },
      mesh(),
      unFilteredField().dimensions()
    }
  };
  for (direction d=0; d<tensor::nComponents; d++) {
    tmpFilteredField().replace
    (
      d, anisotropicFilter::operator()(unFilteredField().component(d))
    );
  }
  unFilteredField.clear();
  return tmpFilteredField;
}

