// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "istream.hpp"
#include "cell.hpp"
// Constructors 
inline mousse::cellShape::cellShape()
:
  m(NULL)
{}
inline mousse::cellShape::cellShape
(
  const cellModel& M,
  const labelList& l,
  const bool doCollapse
)
:
  labelList(l),
  m(&M)
{
  if (doCollapse)
  {
    collapse();
  }
}
inline mousse::cellShape::cellShape(Istream& is)
{
  is >> *this;
}
inline mousse::autoPtr<mousse::cellShape> mousse::cellShape::clone() const
{
  return autoPtr<cellShape>(new cellShape(*this));
}
// Member Functions 
inline mousse::pointField mousse::cellShape::points
(
  const pointField& meshPoints
) const
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
inline const mousse::cellModel& mousse::cellShape::model() const
{
  return *m;
}
inline mousse::labelList mousse::cellShape::meshFaces
(
  const faceList& allFaces,
  const cell& cFaces
) const
{
  // Faces in model order
  faceList localFaces(faces());
  // Do linear match (usually cell shape is low complexity)
  labelList modelToMesh(localFaces.size(), -1);
  FOR_ALL(localFaces, i)
  {
    const face& localF = localFaces[i];
    FOR_ALL(cFaces, j)
    {
      label meshFaceI = cFaces[j];
      if (allFaces[meshFaceI] == localF)
      {
        modelToMesh[i] = meshFaceI;
        break;
      }
    }
  }
  return modelToMesh;
}
inline mousse::labelList mousse::cellShape::meshEdges
(
  const edgeList& allEdges,
  const labelList& cEdges
) const
{
  // Edges in model order
  edgeList localEdges(edges());
  // Do linear match (usually cell shape is low complexity)
  labelList modelToMesh(localEdges.size(), -1);
  FOR_ALL(localEdges, i)
  {
    const edge& e = localEdges[i];
    FOR_ALL(cEdges, j)
    {
      label edgeI = cEdges[j];
      if (allEdges[edgeI] == e)
      {
        modelToMesh[i] = edgeI;
        break;
      }
    }
  }
  return modelToMesh;
}
inline mousse::faceList mousse::cellShape::faces() const
{
  return m->faces(*this);
}
inline mousse::faceList mousse::cellShape::collapsedFaces() const
{
  faceList oldFaces(faces());
  faceList newFaces(oldFaces.size());
  label newFaceI = 0;
  FOR_ALL(oldFaces, oldFaceI)
  {
    const face& f = oldFaces[oldFaceI];
    face& newF = newFaces[newFaceI];
    newF.setSize(f.size());
    label newFp = 0;
    label prevVertI = -1;
    FOR_ALL(f, fp)
    {
      label vertI = f[fp];
      if (vertI != prevVertI)
      {
        newF[newFp++] = vertI;
        prevVertI = vertI;
      }
    }
    if ((newFp > 1) && (newF[newFp-1] == newF[0]))
    {
      --newFp;
    }
    if (newFp > 2)
    {
      // Size face and go to next one
      newF.setSize(newFp);
      newFaceI++;
    }
  }
  newFaces.setSize(newFaceI);
  return newFaces;
}
inline mousse::label mousse::cellShape::nFaces() const
{
  return m->nFaces();
}
inline mousse::edgeList mousse::cellShape::edges() const
{
  return m->edges(*this);
}
inline mousse::label mousse::cellShape::nEdges() const
{
  return m->nEdges();
}
inline mousse::label mousse::cellShape::nPoints() const
{
  return size();
}
inline mousse::point mousse::cellShape::centre(const pointField& points) const
{
  return m->centre(*this, points);
}
inline mousse::scalar mousse::cellShape::mag(const pointField& points) const
{
  return m->mag(*this, points);
}
