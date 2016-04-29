// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_model_function_object.hpp"
#include "region_model.hpp"


// Static Data Members
namespace mousse {
namespace regionModels {

DEFINE_TYPE_NAME_AND_DEBUG(regionModelFunctionObject, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(regionModelFunctionObject, dictionary);

}
}


// Constructors 
mousse::regionModels::regionModelFunctionObject::regionModelFunctionObject
(
  regionModel& owner
)
:
  dict_{dictionary::null},
  owner_{owner},
  modelType_{"modelType"}
{}


mousse::regionModels::regionModelFunctionObject::regionModelFunctionObject
(
  const dictionary& dict,
  regionModel& owner,
  const word& type
)
:
  dict_{dict},
  owner_{owner},
  modelType_{type}
{}


mousse::regionModels::regionModelFunctionObject::regionModelFunctionObject
(
  const regionModelFunctionObject& rmfo
)
:
  dict_{rmfo.dict_},
  owner_{rmfo.owner_},
  modelType_{rmfo.modelType_}
{}


// Destructor 
mousse::regionModels::regionModelFunctionObject::~regionModelFunctionObject()
{}


// Member Functions 
void mousse::regionModels::regionModelFunctionObject::preEvolveRegion()
{
  // do nothing
}


void mousse::regionModels::regionModelFunctionObject::postEvolveRegion()
{
  if (owner_.regionMesh().time().outputTime()) {
    write();
  }
}


void mousse::regionModels::regionModelFunctionObject::write() const
{
  // do nothing
}

