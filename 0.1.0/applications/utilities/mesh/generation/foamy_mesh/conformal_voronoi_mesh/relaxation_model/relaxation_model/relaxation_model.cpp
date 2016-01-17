// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "relaxation_model.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
defineTypeNameAndDebug(relaxationModel, 0);
defineRunTimeSelectionTable(relaxationModel, dictionary);
// Constructors 
relaxationModel::relaxationModel
(
  const word& type,
  const dictionary& relaxationDict,
  const Time& runTime
)
:
  dictionary(relaxationDict),
  runTime_(runTime),
  coeffDict_(subDict(type + "Coeffs"))
{}
// Selectors
autoPtr<relaxationModel> relaxationModel::New
(
  const dictionary& relaxationDict,
  const Time& runTime
)
{
  word relaxationModelTypeName
  (
    relaxationDict.lookup("relaxationModel")
  );
  Info<< nl << "Selecting relaxationModel "
    << relaxationModelTypeName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(relaxationModelTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "relaxationModel::New(const dictionary&, "
      "const conformalVoronoiMesh&)"
    )   << "Unknown relaxationModel type "
      << relaxationModelTypeName
      << endl << endl
      << "Valid relaxationModel types are :" << endl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return autoPtr<relaxationModel>(cstrIter()(relaxationDict, runTime));
}
// Destructor 
relaxationModel::~relaxationModel()
{}
}  // namespace mousse
