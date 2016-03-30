// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "word.hpp"
#include "debug.hpp"


// Static Data Members
const char* const mousse::word::typeName = "word";
int mousse::word::debug(mousse::debug::debugSwitch(word::typeName, 0));
const mousse::word mousse::word::null;
