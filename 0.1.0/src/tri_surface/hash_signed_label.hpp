#ifndef TRI_SURFACE_HASH_SIGNED_LABEL_HPP_
#define TRI_SURFACE_HASH_SIGNED_LABEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hashSignedLabel
// Description
//   hash for signed integers (Hash\<label\> only works for unsigned ints)

#include "list.hpp"
#include "word.hpp"


namespace mousse {

class hashSignedLabel
{
public:
  hashSignedLabel()
  {}
  label operator()(const label key, const label tableSize) const
  {
    return mag(key)%tableSize;
  }
};

}  // namespace mousse

#endif
