// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_model_function_object_list.hpp"


// Constructors 
mousse::regionModels::regionModelFunctionObjectList::regionModelFunctionObjectList
(
  regionModel& owner
)
:
  PtrList<regionModelFunctionObject>{},
  owner_{owner},
  dict_{dictionary::null}
{}


mousse::regionModels::regionModelFunctionObjectList::regionModelFunctionObjectList
(
  regionModel& owner,
  const dictionary& dict,
  const bool readFields
)
:
  PtrList<regionModelFunctionObject>{},
  owner_{owner},
  dict_{dict}
{
  if (readFields) {
    wordList modelNames{dict.toc()};
    Info << "    Selecting region model functions" << endl;
    if (modelNames.size() > 0) {
      this->setSize(modelNames.size());
      FOR_ALL(modelNames, i) {
        const word& modelName = modelNames[i];
        this->set
        (
          i,
          regionModelFunctionObject::New
          (
            dict,
            owner,
            modelName
          )
        );
      }
    } else {
      Info << "    none" << endl;
    }
  }
}


mousse::regionModels::regionModelFunctionObjectList::regionModelFunctionObjectList
(
  const regionModelFunctionObjectList& cfol
)
:
  PtrList<regionModelFunctionObject>{cfol},
  owner_{cfol.owner_},
  dict_{cfol.dict_}
{}


// Destructor 
mousse::regionModels::regionModelFunctionObjectList::
~regionModelFunctionObjectList()
{}


// Member Functions 
void mousse::regionModels::regionModelFunctionObjectList::preEvolveRegion()
{
  FOR_ALL(*this, i) {
    this->operator[](i).preEvolveRegion();
  }
}


void mousse::regionModels::regionModelFunctionObjectList::postEvolveRegion()
{
  FOR_ALL(*this, i) {
    this->operator[](i).postEvolveRegion();
  }
}

