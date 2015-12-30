// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "profile_model.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  defineTypeNameAndDebug(profileModel, 0);
  defineRunTimeSelectionTable(profileModel, dictionary);
}
// Protected Member Functions 
bool mousse::profileModel::readFromFile() const
{
  return fName_ != fileName::null;
}
// Constructors 
mousse::profileModel::profileModel(const dictionary& dict, const word& name)
:
  dict_(dict),
  name_(name),
  fName_(fileName::null)
{
  dict.readIfPresent("fileName", fName_);
}
// Destructor 
mousse::profileModel::~profileModel()
{}
// Member Functions 
const mousse::word& mousse::profileModel::name() const
{
  return name_;
}
mousse::autoPtr<mousse::profileModel> mousse::profileModel::New
(
  const dictionary& dict
)
{
  const word& modelName(dict.dictName());
  const word modelType(dict.lookup("type"));
  Info<< "    - creating " << modelType << " profile " << modelName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn("profileModel::New(const dictionary&)")
      << "Unknown profile model type " << modelType
      << nl << nl
      << "Valid model types are :" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<profileModel>(cstrIter()(dict, modelName));
}
