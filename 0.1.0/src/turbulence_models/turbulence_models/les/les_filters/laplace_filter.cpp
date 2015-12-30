// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "laplace_filter.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "calculated_fv_patch_fields.hpp"
#include "fvm.hpp"
#include "fvc.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(laplaceFilter, 0);
  addToRunTimeSelectionTable(LESfilter, laplaceFilter, dictionary);
}
// Constructors 
mousse::laplaceFilter::laplaceFilter(const fvMesh& mesh, scalar widthCoeff)
:
  LESfilter(mesh),
  widthCoeff_(widthCoeff),
  coeff_
  (
    IOobject
    (
      "laplaceFilterCoeff",
      mesh.time().timeName(),
      mesh
    ),
    mesh,
    dimensionedScalar("zero", dimLength*dimLength, 0),
    calculatedFvPatchScalarField::typeName
  )
{
  coeff_.dimensionedInternalField() = pow(mesh.V(), 2.0/3.0)/widthCoeff_;
}
mousse::laplaceFilter::laplaceFilter(const fvMesh& mesh, const dictionary& bd)
:
  LESfilter(mesh),
  widthCoeff_(readScalar(bd.subDict(type() + "Coeffs").lookup("widthCoeff"))),
  coeff_
  (
    IOobject
    (
      "laplaceFilterCoeff",
      mesh.time().timeName(),
      mesh
    ),
    mesh,
    dimensionedScalar("zero", dimLength*dimLength, 0),
    calculatedFvPatchScalarField::typeName
  )
{
  coeff_.dimensionedInternalField() = pow(mesh.V(), 2.0/3.0)/widthCoeff_;
}
// Member Functions 
void mousse::laplaceFilter::read(const dictionary& bd)
{
  bd.subDict(type() + "Coeffs").lookup("widthCoeff") >> widthCoeff_;
}
// Member Operators 
mousse::tmp<mousse::volScalarField> mousse::laplaceFilter::operator()
(
  const tmp<volScalarField>& unFilteredField
) const
{
  tmp<volScalarField> filteredField =
    unFilteredField() + fvc::laplacian(coeff_, unFilteredField());
  unFilteredField.clear();
  return filteredField;
}
mousse::tmp<mousse::volVectorField> mousse::laplaceFilter::operator()
(
  const tmp<volVectorField>& unFilteredField
) const
{
  tmp<volVectorField> filteredField =
    unFilteredField() + fvc::laplacian(coeff_, unFilteredField());
  unFilteredField.clear();
  return filteredField;
}
mousse::tmp<mousse::volSymmTensorField> mousse::laplaceFilter::operator()
(
  const tmp<volSymmTensorField>& unFilteredField
) const
{
  tmp<volSymmTensorField> filteredField =
    unFilteredField() + fvc::laplacian(coeff_, unFilteredField());
  unFilteredField.clear();
  return filteredField;
}
mousse::tmp<mousse::volTensorField> mousse::laplaceFilter::operator()
(
  const tmp<volTensorField>& unFilteredField
) const
{
  tmp<volTensorField> filteredField =
    unFilteredField() + fvc::laplacian(coeff_, unFilteredField());
  unFilteredField.clear();
  return filteredField;
}
