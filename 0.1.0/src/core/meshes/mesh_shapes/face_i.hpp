// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
// Edge to the right of face vertex i
inline mousse::label mousse::face::right(const label i) const
{
  return i;
}
// Edge to the left of face vertex i
inline mousse::label mousse::face::left(const label i) const
{
  return rcIndex(i);
}
// Constructors 
inline mousse::face::face()
{}
inline mousse::face::face(label s)
:
  labelList(s, -1)
{}
inline mousse::face::face(const labelUList& lst)
:
  labelList(lst)
{}
inline mousse::face::face(const labelList& lst)
:
  labelList(lst)
{}
inline mousse::face::face(const Xfer<labelList>& lst)
:
  labelList(lst)
{}
inline mousse::face::face(Istream& is)
{
  is >> *this;
}
// Member Functions 
inline mousse::pointField mousse::face::points(const pointField& meshPoints) const
{
  // There are as many points as there labels for them
  pointField p(size());
  // For each point in list, set it to the point in 'pnts' addressed
  // by 'labs'
  FOR_ALL(p, i)
  {
    p[i] = meshPoints[operator[](i)];
  }
  // Return list
  return p;
}
inline mousse::scalar mousse::face::mag(const pointField& p) const
{
  return ::mousse::mag(normal(p));
}
inline mousse::label mousse::face::nEdges() const
{
  // for a closed polygon a number of edges is the same as number of points
  return size();
}
inline mousse::edge mousse::face::faceEdge(const label n) const
{
  return edge(operator[](n), operator[](fcIndex(n)));
}
// Next vertex on face
inline mousse::label mousse::face::nextLabel(const label i) const
{
  return operator[](fcIndex(i));
}
// Previous vertex on face
inline mousse::label mousse::face::prevLabel(const label i) const
{
  return operator[](rcIndex(i));
}
// Number of triangles directly known from number of vertices
inline mousse::label mousse::face::nTriangles() const
{
  return size() - 2;
}
// Friend Operators 
inline bool mousse::operator==(const face& a, const face& b)
{
  return face::compare(a,b) != 0;
}
inline bool mousse::operator!=(const face& a, const face& b)
{
  return face::compare(a,b) == 0;
}
// IOstream Operators 
inline mousse::Istream& mousse::operator>>(Istream& is, face& f)
{
  if (is.version() == IOstream::originalVersion)
  {
    // Read starting (
    is.readBegin("face");
    // Read the 'name' token for the face
    token t(is);
    // Read labels
    is >> static_cast<labelList&>(f);
    // Read end)
    is.readEnd("face");
  }
  else
  {
    is >> static_cast<labelList&>(f);
  }
  // Check state of Ostream
  is.check("Istream& operator>>(Istream&, face&)");
  return is;
}
