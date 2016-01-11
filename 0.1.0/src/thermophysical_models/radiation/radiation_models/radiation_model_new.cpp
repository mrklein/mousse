// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "radiation_model.hpp"
#include "vol_fields.hpp"
// Constructors 
mousse::autoPtr<mousse::radiation::radiationModel>
mousse::radiation::radiationModel::New
(
  const volScalarField& T
)
{
  IOobject radIO
  (
    "radiationProperties",
    T.time().constant(),
    T.mesh(),
    IOobject::MUST_READ_IF_MODIFIED,
    IOobject::NO_WRITE,
    false
  );
  word modelType("none");
  if (radIO.headerOk())
  {
    IOdictionary(radIO).lookup("radiationModel") >> modelType;
  }
  else
  {
    Info<< "Radiation model not active: radiationProperties not found"
      << endl;
  }
  Info<< "Selecting radiationModel " << modelType << endl;
  TConstructorTable::iterator cstrIter =
    TConstructorTablePtr_->find(modelType);
  if (cstrIter == TConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "radiationModel::New(const volScalarField&)"
    )   << "Unknown radiationModel type "
      << modelType << nl << nl
      << "Valid radiationModel types are:" << nl
      << TConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<radiationModel>(cstrIter()(T));
}
mousse::autoPtr<mousse::radiation::radiationModel>
mousse::radiation::radiationModel::New
(
  const dictionary& dict,
  const volScalarField& T
)
{
  const word modelType(dict.lookup("radiationModel"));
  Info<< "Selecting radiationModel " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "radiationModel::New(const dictionary&, const volScalarField&)"
    )   << "Unknown radiationModel type "
      << modelType << nl << nl
      << "Valid radiationModel types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<radiationModel>(cstrIter()(dict, T));
}
