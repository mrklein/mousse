// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "intersection.hpp"
// Static Data Members
mousse::scalar mousse::intersection::planarTol_ = 0.2;
namespace mousse
{
  template<>
  const char* mousse::NamedEnum
  <
    mousse::intersection::direction,
    2
  >::names[] =
  {
    "vector",
    "contactSphere"
  };
  template<>
  const char* mousse::NamedEnum
  <
    mousse::intersection::algorithm,
    3
  >::names[] =
  {
    "fullRay",
    "halfRay",
    "visible"
  };
}
const mousse::NamedEnum<mousse::intersection::direction, 2>
mousse::intersection::directionNames_;
const mousse::NamedEnum<mousse::intersection::algorithm, 3>
mousse::intersection::algorithmNames_;
