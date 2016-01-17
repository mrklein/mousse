// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetMatcher
// Description
//   A cellMatcher for tet cells
// See Also
//   cellMatcher
// SourceFiles
//   tet_matcher.cpp

#ifndef tet_matcher_hpp_
#define tet_matcher_hpp_

#include "cell_matcher.hpp"

namespace mousse
{

class tetMatcher
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
    tetMatcher();

    //- Disallow default bitwise copy construct
    tetMatcher(const tetMatcher&) = delete;

    //- Disallow default bitwise assignment
    tetMatcher& operator=(const tetMatcher&) = delete;

  //- Destructor
  ~tetMatcher();

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
