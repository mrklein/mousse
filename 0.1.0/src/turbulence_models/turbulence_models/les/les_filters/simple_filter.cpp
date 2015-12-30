// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "simple_filter.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fvc.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(simpleFilter, 0);
  addToRunTimeSelectionTable(LESfilter, simpleFilter, dictionary);
}
// Constructors 
mousse::simpleFilter::simpleFilter
(
  const fvMesh& mesh
)
:
  LESfilter(mesh)
{}
mousse::simpleFilter::simpleFilter(const fvMesh& mesh, const dictionary&)
:
  LESfilter(mesh)
{}
// Member Functions 
void mousse::simpleFilter::read(const dictionary&)
{}
// Member Operators 
mousse::tmp<mousse::volScalarField> mousse::simpleFilter::operator()
(
  const tmp<volScalarField>& unFilteredField
) const
{
  tmp<volScalarField> filteredField = fvc::surfaceSum
  (
    mesh().magSf()*fvc::interpolate(unFilteredField)
  )/fvc::surfaceSum(mesh().magSf());
  unFilteredField.clear();
  return filteredField;
}
mousse::tmp<mousse::volVectorField> mousse::simpleFilter::operator()
(
  const tmp<volVectorField>& unFilteredField
) const
{
  tmp<volVectorField> filteredField = fvc::surfaceSum
  (
    mesh().magSf()*fvc::interpolate(unFilteredField)
  )/fvc::surfaceSum(mesh().magSf());
  unFilteredField.clear();
  return filteredField;
}
mousse::tmp<mousse::volSymmTensorField> mousse::simpleFilter::operator()
(
  const tmp<volSymmTensorField>& unFilteredField
) const
{
  tmp<volSymmTensorField> filteredField = fvc::surfaceSum
  (
    mesh().magSf()*fvc::interpolate(unFilteredField)
  )/fvc::surfaceSum(mesh().magSf());
  unFilteredField.clear();
  return filteredField;
}
mousse::tmp<mousse::volTensorField> mousse::simpleFilter::operator()
(
  const tmp<volTensorField>& unFilteredField
) const
{
  tmp<volTensorField> filteredField = fvc::surfaceSum
  (
    mesh().magSf()*fvc::interpolate(unFilteredField)
  )/fvc::surfaceSum(mesh().magSf());
  unFilteredField.clear();
  return filteredField;
}
