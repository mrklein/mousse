#ifndef CORE_MESHES_MESH_SHAPES_HEX_MATCHER_HPP_
#define CORE_MESHES_MESH_SHAPES_HEX_MATCHER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hexMatcher
// Description
//   A cellMatcher for hex cells
// See Also
//   cellMatcher
// SourceFiles
//   hex_matcher.cpp


#include "cell_matcher.hpp"

namespace mousse
{

class hexMatcher
:
  public cellMatcher
{

  // Static data members

    //- Constants for this shape

    static const label vertPerCell;

    static const label facePerCell;

    static const label maxVertPerFace;

public:

  // Constructors

    //- Construct null
    hexMatcher();

    //- Disallow default bitwise copy construct
    hexMatcher(const hexMatcher&) = delete;

    //- Disallow default bitwise assignment
    hexMatcher& operator=(const hexMatcher&) = delete;

  //- Destructor
  ~hexMatcher();

  // Member Functions

    virtual label nVertPerCell() const
    {
      return vertPerCell;
    }

    virtual label nFacePerCell() const
    {
      return facePerCell;
    }

    virtual label nMaxVertPerFace() const
    {
      return maxVertPerFace;
    }

    virtual label faceHashValue() const;

    virtual bool faceSizeMatch(const faceList&, const labelList&) const;

    virtual bool matchShape
    (
      const bool checkOnly,
      const faceList& faces,
      const labelList& faceOwner,
      const label cellI,
      const labelList& myFaces
    );

    virtual bool isA(const primitiveMesh& mesh, const label cellI);

    virtual bool isA(const faceList&);

    virtual bool matches
    (
      const primitiveMesh& mesh,
      const label cellI,
      cellShape& shape
    );

};

}  // namespace mousse
#endif
