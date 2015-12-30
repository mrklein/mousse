// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ensight_ptraits.hpp"
// Static Data Members
const char* const mousse::ensightPTraits<mousse::scalar>::typeName =
  mousse::pTraits<mousse::scalar>::typeName;
const char* const mousse::ensightPTraits<mousse::vector>::typeName =
  mousse::pTraits<mousse::vector>::typeName;
const char* const mousse::ensightPTraits<mousse::sphericalTensor>::typeName =
  mousse::pTraits<mousse::scalar>::typeName;
const char* const mousse::ensightPTraits<mousse::symmTensor>::typeName =
  "tensor symm";
const char* const mousse::ensightPTraits<mousse::tensor>::typeName =
  "tensor asym";
