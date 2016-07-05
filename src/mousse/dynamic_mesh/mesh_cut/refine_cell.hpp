#ifndef DYNAMIC_MESH_MESH_CUT_REFINE_CELL_REFINE_CELL_HPP_
#define DYNAMIC_MESH_MESH_CUT_REFINE_CELL_REFINE_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::refineCell
// Description
//   Container with cells to refine. Refinement given as single direction.

#include "label.hpp"
#include "vector.hpp"


namespace mousse {

class Istream;
class Ostream;


class refineCell
{
  // Private data
  //- Cell label
  label cellNo_;
  //- Preferred refinement direction (always normalized).
  vector direction_;
public:
  // Constructors
    //- Null
    refineCell();
    //- From components. Vector will be normalized upon construction.
    refineCell(const label, const vector&);
    //- From Istream. Vector will be normalized upon construction.
    refineCell(Istream& is);
  // Member Functions
    label cellNo() const
    {
      return cellNo_;
    }
    const vector& direction() const
    {
      return direction_;
    }
  // Friend Operators
    inline friend bool operator==
    (
      const refineCell& rc1,
      const refineCell& rc2
    )
    {
      return (rc1.cellNo() == rc2.cellNo());
    }
    inline friend bool operator!=
    (
      const refineCell& rc1,
      const refineCell& rc2
    )
    {
      return !(rc1 == rc2);
    }
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const refineCell&);
};

}  // namespace mousse

#endif
