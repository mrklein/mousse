// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
// Construct null
inline mousse::cell::cell()
{}
// Construct given size
inline mousse::cell::cell(label s)
:
  labelList(s, -1)
{}
// Construct from components
inline mousse::cell::cell(const labelUList& lst)
:
  labelList(lst)
{}
inline mousse::cell::cell(const Xfer<labelList>& lst)
:
  labelList(lst)
{}
// Construct from Istream
inline mousse::cell::cell(Istream& is)
:
  labelList(is)
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
