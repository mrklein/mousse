// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_source.hpp"
// Member Functions 
inline const mousse::fieldValues::cellSource::sourceType&
mousse::fieldValues::cellSource::source() const
{
  return source_;
}
inline const mousse::labelList&
mousse::fieldValues::cellSource::cellId() const
{
  return cellId_;
}
