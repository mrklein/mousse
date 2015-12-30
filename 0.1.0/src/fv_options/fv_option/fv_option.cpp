// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_option.hpp"
#include "vol_fields.hpp"
// Static Data Members
namespace mousse
{
  namespace fv
  {
    defineTypeNameAndDebug(option, 0);
    defineRunTimeSelectionTable(option, dictionary);
  }
}
// Constructors 
mousse::fv::option::option
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  name_(name),
  modelType_(modelType),
  mesh_(mesh),
  dict_(dict),
  coeffs_(dict.subDict(modelType + "Coeffs")),
  active_(dict_.lookupOrDefault<Switch>("active", true)),
  fieldNames_(),
  applied_()
{
  Info<< incrIndent << indent << "Source: " << name_ << endl << decrIndent;
}
// Selectors
mousse::autoPtr<mousse::fv::option> mousse::fv::option::New
(
  const word& name,
  const dictionary& coeffs,
  const fvMesh& mesh
)
{
  word modelType(coeffs.lookup("type"));
  Info<< indent
    << "Selecting finite volume options model type " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "option::New(const word&, const dictionary&, const fvMesh&)"
    )   << "Unknown Model type " << modelType << nl << nl
      << "Valid model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<option>(cstrIter()(name, modelType, coeffs, mesh));
}
mousse::fv::option::~option()
{}
// Member Functions 
bool mousse::fv::option::isActive()
{
  return active_;
}
mousse::label mousse::fv::option::applyToField(const word& fieldName) const
{
  return findIndex(fieldNames_, fieldName);
}
void mousse::fv::option::checkApplied() const
{
  forAll(applied_, i)
  {
    if (!applied_[i])
    {
      WarningIn("void option::checkApplied() const")
        << "Source " << name_ << " defined for field "
        << fieldNames_[i] << " but never used" << endl;
    }
  }
}
void mousse::fv::option::addSup
(
  fvMatrix<scalar>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::addSup
(
  fvMatrix<vector>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::addSup
(
  fvMatrix<sphericalTensor>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::addSup
(
  fvMatrix<symmTensor>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::addSup
(
  fvMatrix<tensor>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::addSup
(
  const volScalarField& rho,
  fvMatrix<scalar>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::addSup
(
  const volScalarField& rho,
  fvMatrix<vector>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::addSup
(
  const volScalarField& rho,
  fvMatrix<sphericalTensor>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::addSup
(
  const volScalarField& rho,
  fvMatrix<symmTensor>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::addSup
(
  const volScalarField& rho,
  fvMatrix<tensor>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::addSup
(
  const volScalarField& alpha,
  const volScalarField& rho,
  fvMatrix<scalar>& eqn,
  const label fieldI
)
{
  addSup(alpha*rho, eqn, fieldI);
}
void mousse::fv::option::addSup
(
  const volScalarField& alpha,
  const volScalarField& rho,
  fvMatrix<vector>& eqn,
  const label fieldI
)
{
  addSup(alpha*rho, eqn, fieldI);
}
void mousse::fv::option::addSup
(
  const volScalarField& alpha,
  const volScalarField& rho,
  fvMatrix<sphericalTensor>& eqn,
  const label fieldI
)
{
  addSup(alpha*rho, eqn, fieldI);
}
void mousse::fv::option::addSup
(
  const volScalarField& alpha,
  const volScalarField& rho,
  fvMatrix<symmTensor>& eqn,
  const label fieldI
)
{
  addSup(alpha*rho, eqn, fieldI);
}
void mousse::fv::option::addSup
(
  const volScalarField& alpha,
  const volScalarField& rho,
  fvMatrix<tensor>& eqn,
  const label fieldI
)
{
  addSup(alpha*rho, eqn, fieldI);
}
void mousse::fv::option::constrain(fvMatrix<scalar>& eqn, const label fieldI)
{}
void mousse::fv::option::constrain(fvMatrix<vector>& eqn, const label fieldI)
{}
void mousse::fv::option::constrain
(
  fvMatrix<sphericalTensor>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::constrain
(
  fvMatrix<symmTensor>& eqn,
  const label fieldI
)
{}
void mousse::fv::option::constrain(fvMatrix<tensor>& eqn, const label fieldI)
{}
void mousse::fv::option::correct(volScalarField& field)
{}
void mousse::fv::option::correct(volVectorField& field)
{}
void mousse::fv::option::correct(volSphericalTensorField& field)
{}
void mousse::fv::option::correct(volSymmTensorField& field)
{}
void mousse::fv::option::correct(volTensorField& field)
{}
