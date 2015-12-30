// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline mousse::label mousse::curvedEdge::start() const
{
  return start_;
}
inline mousse::label mousse::curvedEdge::end() const
{
  return end_;
}
inline int mousse::curvedEdge::compare(const label start, const label end) const
{
  if (start_ == start && end_ == end)
  {
    return 1;
  }
  else if (start_ == end && end_ == start)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}
inline int mousse::curvedEdge::compare(const curvedEdge& e) const
{
  return mousse::curvedEdge::compare(e.start(), e.end());
}
inline int mousse::curvedEdge::compare(const edge& e) const
{
  return mousse::curvedEdge::compare(e.start(), e.end());
}
