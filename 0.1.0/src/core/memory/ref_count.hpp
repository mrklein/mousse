#ifndef CORE_MEMORY_REF_COUNT_HPP_
#define CORE_MEMORY_REF_COUNT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::refCount
// Description
//   Reference counter for various OpenFOAM components.

#include "bool.hpp"


namespace mousse {

class refCount
{
  // Private data
    int count_;
public:
  // Constructors
    //- Construct null with zero count
    refCount()
    :
      count_{0}
    {}
    //- Dissallow copy
    refCount(const refCount&) = delete;
    //- Dissallow bitwise assignment
    void operator=(const refCount&) = delete;
  // Member Functions
    //- Return the reference count
    int count() const
    {
      return count_;
    }
    //- Return true if the reference count is zero
    bool okToDelete() const
    {
      return !count_;
    }
    //- Reset the reference count to zero
    void resetRefCount()
    {
      count_ = 0;
    }
  // Member Operators
    //- Increment the reference count
    void operator++()
    {
      count_++;
    }
    //- Increment the reference count
    void operator++(int)
    {
      count_++;
    }
    //- Decrement the reference count
    void operator--()
    {
      count_--;
    }
    //- Decrement the reference count
    void operator--(int)
    {
      count_--;
    }
};
}  // namespace mousse
#endif
