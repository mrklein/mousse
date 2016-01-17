// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_area_weight_model.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
defineTypeNameAndDebug(faceAreaWeightModel, 0);
defineRunTimeSelectionTable(faceAreaWeightModel, dictionary);
// Constructors 
faceAreaWeightModel::faceAreaWeightModel
(
  const word& type,
  const dictionary& relaxationDict
)
:
  dictionary(relaxationDict),
  coeffDict_(subDict(type + "Coeffs"))
{}
// Selectors
autoPtr<faceAreaWeightModel> faceAreaWeightModel::New
(
  const dictionary& relaxationDict
)
{
  word faceAreaWeightModelTypeName
  (
    relaxationDict.lookup("faceAreaWeightModel")
  );
  Info<< nl << "Selecting faceAreaWeightModel "
    << faceAreaWeightModelTypeName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(faceAreaWeightModelTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "faceAreaWeightModel::New(const dictionary&, "
      "const conformalVoronoiMesh&)"
    )   << "Unknown faceAreaWeightModel type "
      << faceAreaWeightModelTypeName
      << endl << endl
      << "Valid faceAreaWeightModel types are :" << endl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return autoPtr<faceAreaWeightModel>(cstrIter()(relaxationDict));
}
// Destructor 
faceAreaWeightModel::~faceAreaWeightModel()
{}
}  // namespace mousse
