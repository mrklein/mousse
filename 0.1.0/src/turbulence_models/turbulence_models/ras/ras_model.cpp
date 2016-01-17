// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ras_model.hpp"
// Protected Member Functions 
template<class BasicTurbulenceModel>
void mousse::RASModel<BasicTurbulenceModel>::printCoeffs(const word& type)
{
  if (printCoeffs_)
  {
    Info<< type << "Coeffs" << coeffDict_ << endl;
  }
}
// Constructors 
template<class BasicTurbulenceModel>
mousse::RASModel<BasicTurbulenceModel>::RASModel
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
  RASDict_(this->subOrEmptyDict("RAS")),
  turbulence_(RASDict_.lookup("turbulence")),
  printCoeffs_(RASDict_.lookupOrDefault<Switch>("printCoeffs", false)),
  coeffDict_(RASDict_.subOrEmptyDict(type + "Coeffs")),
  kMin_
  (
    dimensioned<scalar>::lookupOrAddToDict
    (
      "kMin",
      RASDict_,
      sqr(dimVelocity),
      SMALL
    )
  ),
  epsilonMin_
  (
    dimensioned<scalar>::lookupOrAddToDict
    (
      "epsilonMin",
      RASDict_,
      kMin_.dimensions()/dimTime,
      SMALL
    )
  ),
  omegaMin_
  (
    dimensioned<scalar>::lookupOrAddToDict
    (
      "omegaMin",
      RASDict_,
      dimless/dimTime,
      SMALL
    )
  )
{
  // Force the construction of the mesh deltaCoeffs which may be needed
  // for the construction of the derived models and BCs
  this->mesh_.deltaCoeffs();
}
// Selectors
template<class BasicTurbulenceModel>
mousse::autoPtr<mousse::RASModel<BasicTurbulenceModel> >
mousse::RASModel<BasicTurbulenceModel>::New
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
    ).subDict("RAS").lookup("RASModel")
  );
  Info<< "Selecting RAS turbulence model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "RASModel::New"
      "("
        "const volScalarField&, "
        "const volVectorField&, "
        "const surfaceScalarField&, "
        "transportModel&, "
        "const word&"
      ")"
    )   << "Unknown RASModel type "
      << modelType << nl << nl
      << "Valid RASModel types:" << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<RASModel>
  (
    cstrIter()(alpha, rho, U, alphaRhoPhi, phi, transport, propertiesName)
  );
}
// Member Functions 
template<class BasicTurbulenceModel>
bool mousse::RASModel<BasicTurbulenceModel>::read()
{
  if (BasicTurbulenceModel::read())
  {
    RASDict_ <<= this->subDict("RAS");
    RASDict_.lookup("turbulence") >> turbulence_;
    if (const dictionary* dictPtr = RASDict_.subDictPtr(type() + "Coeffs"))
    {
      coeffDict_ <<= *dictPtr;
    }
    kMin_.readIfPresent(RASDict_);
    epsilonMin_.readIfPresent(RASDict_);
    omegaMin_.readIfPresent(RASDict_);
    return true;
  }
  else
  {
    return false;
  }
}
template<class BasicTurbulenceModel>
void mousse::RASModel<BasicTurbulenceModel>::correct()
{
  BasicTurbulenceModel::correct();
}
