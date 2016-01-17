// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::referredWallFace
// Description
//   Storage for referred wall faces.  Stores patch index, face and
//   associated points
// SourceFiles
//   referred_wall_face.cpp
#ifndef referred_wall_face_hpp_
#define referred_wall_face_hpp_
#include "face.hpp"
#include "point_field.hpp"
namespace mousse
{
// Forward declaration of classes
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class referredWallFace;
Istream& operator>>(Istream&, referredWallFace&);
Ostream& operator<<(Ostream&, const referredWallFace&);
class referredWallFace
:
  public face
{
  // Private data
    //- Points of face
    pointField pts_;
    //- Index of originating patch
    label patchI_;
public:
  // Constructors
    //- Construct null
    referredWallFace();
    //- Construct from components
    referredWallFace
    (
      const face& f,
      const pointField& pts,
      label patchI
    );
    //- Construct as copy
    referredWallFace(const referredWallFace&);
  //- Destructor
  ~referredWallFace();
  // Member Functions
    // Access
      //- Return access to the stored points
      inline const pointField& points() const;
      //- Return non-const access to the stored points
      inline pointField& points();
      //- Return access to the patch index
      inline label patchIndex() const;
      //- Return non-const access to the patch index
      inline label& patchIndex();
  // Member Operators
    bool operator==(const referredWallFace&) const;
    bool operator!=(const referredWallFace&) const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, referredWallFace&);
    friend Ostream& operator<<(Ostream&, const referredWallFace&);
};
}  // namespace mousse

// Member Functions 
const mousse::pointField& mousse::referredWallFace::points() const
{
  return pts_;
}
mousse::pointField& mousse::referredWallFace::points()
{
  return pts_;
}
mousse::label mousse::referredWallFace::patchIndex() const
{
  return patchI_;
}
mousse::label& mousse::referredWallFace::patchIndex()
{
  return patchI_;
}
#endif
