#ifndef CORE_MESHES_MESH_SHAPES_CELL_HPP_
#define CORE_MESHES_MESH_SHAPES_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cell
// Description
//   A cell is defined as a list of faces with extra functionality
#include "face_list.hpp"
#include "opposite_face.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class cell;
bool operator==(const cell&, const cell&);
inline bool operator!=(const cell&, const cell&);
class cell
:
  public labelList
{
public:
  // Static data members
    static const char* const typeName;
  // Constructors
    //- Construct null
    inline cell();
    //- Construct given size
    explicit inline cell(label);
    //- Construct from list of labels
    explicit inline cell(const labelUList&);
    //- Construct by transferring the parameter contents
    explicit inline cell(const Xfer<labelList>&);
    //- Construct from Istream
    inline cell(Istream&);
  // Member Functions
    //- Return number of faces
    inline label nFaces() const;
    //- Return labels of cell vertices
    labelList labels(const faceUList&) const;
    //- Return the cell vertices
    pointField points(const faceUList&, const pointField&) const;
    //- Return cell edges
    edgeList edges(const faceUList&) const;
    //- Return index of opposite face
    label opposingFaceLabel
    (
      const label masterFaceLabel,
      const faceUList& meshFaces
    ) const;
    //- Return opposite face oriented the same way as the master face
    oppositeFace opposingFace
    (
      const label masterFaceLabel,
      const faceUList& meshFaces
    ) const;
    // Instructions for centre and mag:
    // When one wants to access the cell centre and magnitude, the
    // functionality on the mesh level should be used in preference to the
    // functions provided here. They do not rely to the functionality
    // implemented here, provide additional checking and are more efficient.
    // The cell::centre and cell::mag functions may be removed in the
    // future.
    //- Returns cell centre
    point centre(const pointField&, const faceUList&) const;
    //- Returns cell volume
    scalar mag(const pointField&, const faceUList&) const;
  // Friend Operators
    friend bool operator==(const cell&, const cell&);
    friend bool operator!=(const cell&, const cell&);
};
}  // namespace mousse

// Constructors 
// Construct null
inline mousse::cell::cell()
{}
// Construct given size
inline mousse::cell::cell(label s)
:
  labelList{s, -1}
{}
// Construct from components
inline mousse::cell::cell(const labelUList& lst)
:
  labelList{lst}
{}
inline mousse::cell::cell(const Xfer<labelList>& lst)
:
  labelList{lst}
{}
// Construct from Istream
inline mousse::cell::cell(Istream& is)
:
  labelList{is}
{}
// Member Functions 
// Number of faces
inline mousse::label mousse::cell::nFaces() const
{
  return size();
}
inline bool mousse::operator!=(const cell& a, const cell& b)
{
  return (!(a == b));
}
#endif
