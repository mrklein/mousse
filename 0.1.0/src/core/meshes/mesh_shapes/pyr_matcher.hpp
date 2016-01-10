// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pyrMatcher
// Description
//   A cellMatcher for pyr cells
// See Also
//   cellMatcher
// SourceFiles
//   pyr_matcher.cpp

#ifndef pyr_matcher_hpp_
#define pyr_matcher_hpp_

#include "cell_matcher.hpp"

namespace mousse
{

class pyrMatcher
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
    pyrMatcher();

    //- Disallow default bitwise copy construct
    pyrMatcher(const pyrMatcher&) = delete;

    //- Disallow default bitwise assignment
    pyrMatcher& operator=(const pyrMatcher&) = delete;

  //- Destructor
  ~pyrMatcher();

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
