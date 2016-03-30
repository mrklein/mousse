// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_modeller.hpp"


// Constructors 
mousse::cellModeller::cellModeller()
{
  if (modelPtrs_.size()) {
    FATAL_ERROR_IN("cellModeller::cellModeller(const fileName&)")
      << "attempt to re-construct cellModeller when it already exists"
      << exit(FatalError);
  }
  label maxIndex = 0;
  FOR_ALL(models_, i) {
    if (models_[i].index() > maxIndex)
      maxIndex = models_[i].index();
  }
  modelPtrs_.setSize(maxIndex + 1);
  modelPtrs_ = NULL;
  // For all the words in the wordlist, set the details of the model
  // to those specified by the word name and the other parameters
  // given. This should result in an automatic 'read' of the model
  // from its File (see cellModel class).
  FOR_ALL(models_, i) {
    if (modelPtrs_[models_[i].index()]) {
      FATAL_ERROR_IN("cellModeller::cellModeller(const fileName&)")
        << "more than one model share the index "
        << models_[i].index()
        << exit(FatalError);
    }
    modelPtrs_[models_[i].index()] = &models_[i];
    if (modelDictionary_.found(models_[i].name())) {
      FATAL_ERROR_IN("cellModeller::cellModeller(const fileName&)")
        << "more than one model share the name "
        << models_[i].name()
        << exit(FatalError);
    }
    modelDictionary_.insert(models_[i].name(), &models_[i]);
  }
}


// Destructor 
mousse::cellModeller::~cellModeller()
{}


// Member Functions 
// Returns a pointer to a model which matches the string symbol
// supplied. A null pointer is returned if there is no suitable match.
const mousse::cellModel* mousse::cellModeller::lookup(const word& name)
{
  HashTable<const cellModel*>::iterator iter = modelDictionary_.find(name);
  if (iter != modelDictionary_.end()) {
    return iter();
  } else {
    return NULL;
  }
}

