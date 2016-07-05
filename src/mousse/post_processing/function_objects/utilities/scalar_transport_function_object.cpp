// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar_transport_function_object.hpp"


// Static Data Members
namespace mousse {

DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(scalarTransportFunctionObject, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  functionObject,
  scalarTransportFunctionObject,
  dictionary
);

}

