// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "couple_group_identifier.hpp"
// Member Functions 
const mousse::word& mousse::coupleGroupIdentifier::name() const
{
  return name_;
}
bool mousse::coupleGroupIdentifier::valid() const
{
  return !name_.empty();
}
