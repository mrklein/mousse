// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "les_model.hpp"
// Protected Member Functions 
template<class BasicTurbulenceModel>
void mousse::LESModel<BasicTurbulenceModel>::printCoeffs(const word& type)
{
  if (printCoeffs_)
  {
    Info<< type << "Coeffs" << coeffDict_ << endl;
  }
}
// Constructors 
template<class BasicTurbulenceModel>
mousse::LESModel<BasicTurbulenceModel>::LESModel
(
  const word& type,
  const alphaField& alpha,
  const rhoField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
:
  BasicTurbulenceModel
  (
    type,
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName
  ),
  LESDict_(this->subOrEmptyDict("LES")),
  turbulence_(LESDict_.lookup("turbulence")),
  printCoeffs_(LESDict_.lookupOrDefault<Switch>("printCoeffs", false)),
  coeffDict_(LESDict_.subOrEmptyDict(type + "Coeffs")),
  kMin_
  (
    dimensioned<scalar>::lookupOrAddToDict
    (
      "kMin",
      LESDict_,
      sqr(dimVelocity),
      SMALL
    )
  ),
  delta_
  (
    LESdelta::New
    (
      IOobject::groupName("delta", U.group()),
      *this,
      LESDict_
    )
  )
{
  // Force the construction of the mesh deltaCoeffs which may be needed
  // for the construction of the derived models and BCs
  this->mesh_.deltaCoeffs();
}
// Selectors
template<class BasicTurbulenceModel>
mousse::autoPtr<mousse::LESModel<BasicTurbulenceModel> >
mousse::LESModel<BasicTurbulenceModel>::New
(
  const alphaField& alpha,
  const rhoField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
{
  // get model name, but do not register the dictionary
  // otherwise it is registered in the database twice
  const word modelType
  (
    IOdictionary
    (
      IOobject
      (
        IOobject::groupName(propertiesName, U.group()),
        U.time().constant(),
        U.db(),
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      )
    ).subDict("LES").lookup("LESModel")
  );
  Info<< "Selecting LES turbulence model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "LESModel::New"
      "("
        "const volScalarField&, "
        "const volVectorField&, "
        "const surfaceScalarField&, "
        "transportModel&, "
        "const word&"
      ")"
    )   << "Unknown LESModel type "
      << modelType << nl << nl
      << "Valid LESModel types:" << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<LESModel>
  (
    cstrIter()(alpha, rho, U, alphaRhoPhi, phi, transport, propertiesName)
  );
}
// Member Functions 
template<class BasicTurbulenceModel>
bool mousse::LESModel<BasicTurbulenceModel>::read()
{
  if (BasicTurbulenceModel::read())
  {
    LESDict_ <<= this->subDict("LES");
    LESDict_.lookup("turbulence") >> turbulence_;
    if (const dictionary* dictPtr = LESDict_.subDictPtr(type() + "Coeffs"))
    {
      coeffDict_ <<= *dictPtr;
    }
    delta_().read(LESDict_);
    kMin_.readIfPresent(LESDict_);
    return true;
  }
  else
  {
    return false;
  }
}
template<class BasicTurbulenceModel>
void mousse::LESModel<BasicTurbulenceModel>::correct()
{
  delta_().correct();
  BasicTurbulenceModel::correct();
}
