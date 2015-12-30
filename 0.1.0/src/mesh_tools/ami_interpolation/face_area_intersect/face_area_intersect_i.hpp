// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
inline void mousse::faceAreaIntersect::setTriPoints
(
  const point& a,
  const point& b,
  const point& c,
  label& count,
  FixedList<triPoints, 10>& tris
) const
{
  triPoints& tp = tris[count++];
  tp[0] = a;
  tp[1] = b;
  tp[2] = c;
}
inline mousse::faceAreaIntersect::triPoints mousse::faceAreaIntersect::getTriPoints
(
  const pointField& points,
  const face& f,
  const bool reverse
) const
{
  triPoints result;
  if (reverse)
  {
    result[2] = points[f[0]];
    result[1] = points[f[1]];
    result[0] = points[f[2]];
  }
  else
  {
    result[0] = points[f[0]];
    result[1] = points[f[1]];
    result[2] = points[f[2]];
  }
  return result;
}
inline void mousse::faceAreaIntersect::triangleFan
(
  const face& f,
  DynamicList<face>& faces
) const
{
  if (f.size() > 2)
  {
    const label v0 = 0;
    labelList indices(3);
    for (label i = 1; i < f.size() - 1; i++)
    {
      indices[0] = f[v0];
      indices[1] = f[i];
      indices[2] = f[i + 1];
      faces.append(face(indices));
    }
  }
}
inline mousse::point mousse::faceAreaIntersect::planeIntersection
(
  const FixedList<scalar, 3>& d,
  const triPoints& t,
  const label negI,
  const label posI
) const
{
  return (d[posI]*t[negI] - d[negI]*t[posI])/(-d[negI] + d[posI]);
}
inline mousse::scalar mousse::faceAreaIntersect::triArea(const triPoints& t) const
{
  return mag(0.5*((t[1] - t[0])^(t[2] - t[0])));
}
// Member Functions 
mousse::scalar& mousse::faceAreaIntersect::tolerance()
{
  return tol;
}
