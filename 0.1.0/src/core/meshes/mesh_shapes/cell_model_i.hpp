// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "cell_model.hpp"
namespace mousse
{
// Member Functions 
inline const word& cellModel::name() const
{
  return name_;
}
inline label cellModel::index() const
{
  return index_;
}
inline label cellModel::nPoints() const
{
  return nPoints_;
}
inline label cellModel::nEdges() const
{
  return edges_.size();
}
inline label cellModel::nFaces() const
{
  return faces_.size();
}
//  Return the faces of a cellModel by untangling the geometry
//  supplied in terms of the face labels
inline edgeList cellModel::edges(const labelList& pointLabels) const
{
  edgeList e(edges_.size());
  // Translate model lebels into global labels
  FOR_ALL(edges_, edgeI)
  {
    e[edgeI] =
      edge
      (
        pointLabels[edges_[edgeI].start()],
        pointLabels[edges_[edgeI].end()]
      );
  }
  return e;
}
// Return a raw list of model faces
inline const faceList& cellModel::modelFaces() const
{
  return faces_;
}
//  Return the faces of a cellModel by untangling the geometry
//  supplied in terms of the face labels
inline faceList cellModel::faces(const labelList& pointLabels) const
{
  faceList f(faces_.size());
  // Translate model lebels into global labels
  FOR_ALL(faces_, faceI)
  {
    const labelList& curModelLabels = faces_[faceI];
    face& curFace = f[faceI];
    curFace.setSize(curModelLabels.size());
    FOR_ALL(curModelLabels, labelI)
    {
      curFace[labelI] = pointLabels[curModelLabels[labelI]];
    }
  }
  return f;
}
// Friend Operators 
// Equality operator: true => ptr to models are equal !
inline bool operator==(const cellModel& m1, const cellModel& m2)
{
  return (&m1 == &m2);
}
// Inequality operator: true => ptr to models are not equal !
inline bool operator!=(const cellModel& m1, const cellModel& m2)
{
  return (&m1 != &m2);
}
}  // namespace mousse
