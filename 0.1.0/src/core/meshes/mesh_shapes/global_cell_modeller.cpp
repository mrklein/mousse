// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_modeller.hpp"
#include "os_specific.hpp"
#include "ifstream.hpp"


// Static data
// PtrList of models
mousse::PtrList<mousse::cellModel> mousse::cellModeller::models_
{
  IFstream{findEtcFile("cellModels", true)}()
};

// List of model pointers
mousse::List<mousse::cellModel*> mousse::cellModeller::modelPtrs_;

// HashTable of model pointers
mousse::HashTable<const mousse::cellModel*>
  mousse::cellModeller::modelDictionary_;


namespace mousse {

// Construct a dummy cellModeller which reads the models and fills
// the above tables
cellModeller globalCellModeller_;

}  // namespace mousse

