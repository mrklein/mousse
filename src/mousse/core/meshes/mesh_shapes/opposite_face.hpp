#ifndef CORE_MESHES_MESH_SHAPES_OPPOSITE_FACE_HPP_
#define CORE_MESHES_MESH_SHAPES_OPPOSITE_FACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::oppositeFace
// Description
//   Class containing opposite face for a prismatic cell with addressing
//   and a possibility of failure.

#include "face.hpp"


namespace mousse {

class oppositeFace
:
  public face
{
  // Private data
    //- Master face index
    const label masterIndex_;
    //- Opposite face index
    const label oppositeIndex_;
public:
  // Constructors
    //- Construct from components
    oppositeFace
    (
      const face& f,
      const label masterIndex,
      const label oppositeIndex
    )
    :
      face{f},
      masterIndex_{masterIndex},
      oppositeIndex_{oppositeIndex}
    {}
  // Member Functions
    //- Master face index
    inline label masterIndex() const
    {
      return masterIndex_;
    }
    //- Slave face index
    inline label oppositeIndex() const
    {
      return oppositeIndex_;
    }
    //- Does the opposite face exist?
    inline bool found() const
    {
      return oppositeIndex_ >= 0;
    }
};
}  // namespace mousse
#endif
