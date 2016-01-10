// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointHitSort
// Description
//   Container for sorting intersections
// SourceFiles
#ifndef point_hit_sort_hpp_
#define point_hit_sort_hpp_
#include "point_hit.hpp"
#include "label.hpp"
namespace mousse
{
// Forward declaration of classes
class pointHitSort
{
  // Private data
    //- intersection
    pointHit inter_;
    //- Original index
    label index_;
public:
  // Constructors
    //- Construct null
    pointHitSort()
    :
      inter_(false, vector::zero, GREAT, false),
      index_(-1)
    {}
    //- Construct from intersection, index
    pointHitSort(const pointHit& inter, const label index)
    :
      inter_(inter),
      index_(index)
    {}
  // Member Functions
    const pointHit& inter() const
    {
      return inter_;
    }
    label index() const
    {
      return index_;
    }
  // Member Operators
    bool operator==(const pointHitSort& rhs) const
    {
      return inter_.distance() == rhs.inter().distance();
    }
    bool operator<(const pointHitSort& rhs) const
    {
      return inter_.distance() < rhs.inter().distance();
    }
};
}  // namespace mousse
#endif
