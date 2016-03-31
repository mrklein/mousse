#ifndef TRI_SURFACE_TOOLS_SORT_LABELLED_TRI_HPP_
#define TRI_SURFACE_TOOLS_SORT_LABELLED_TRI_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sortLabelledTri
// Description
//   Helper class which when constructed with a triSurface
//   sorts the faces according to region number (or rather constructs a
//   mapping).

#include "label_list.hpp"


namespace mousse {

class sortLabelledTri;
class triSurface;


//- Hold surface and label
class surfAndLabel
{
  const triSurface* surfPtr_;
  label index_;
  // Private Classes
    //- Scalar comparison function used for sorting
    class less
    {
    public:
      inline bool operator()
      (
        const surfAndLabel& one,
        const surfAndLabel& two
      ) const;
    };
public:
  friend class sortLabelledTri;
  // Constructors
    //- Construct null
    surfAndLabel()
    :
      surfPtr_{NULL},
      index_{-1}
    {}
    //- Construct from surface and index
    surfAndLabel(const triSurface& surf, const label index)
    :
      surfPtr_{&surf},
      index_{index}
    {}
};


class sortLabelledTri
:
  public List<surfAndLabel>
{
public:
  // Constructors
    //- Construct from surface, sorting the faces according to patch
    sortLabelledTri(const triSurface&);
  // Member Functions
    // Access
      //- Set the labelList to those of sorted point indices
      void indices(labelList&) const;
      //- Return the list of sorted point indices
      labelList indices() const;
};

}  // namespace mousse

#endif
