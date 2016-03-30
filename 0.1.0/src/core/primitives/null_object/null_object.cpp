// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "null_object.hpp"


// Static Data Members
const mousse::NullObject mousse::NullObject::nullObject;
const mousse::NullObject* mousse::nullObjectPtr = &mousse::NullObject::nullObject;
