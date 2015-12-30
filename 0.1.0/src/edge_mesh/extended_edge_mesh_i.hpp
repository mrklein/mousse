// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline mousse::label mousse::extendedEdgeMesh::convexStart() const
{
  return convexStart_;
}
inline mousse::label mousse::extendedEdgeMesh::concaveStart() const
{
  return concaveStart_;
}
inline mousse::label mousse::extendedEdgeMesh::mixedStart() const
{
  return mixedStart_;
}
inline mousse::label mousse::extendedEdgeMesh::nonFeatureStart() const
{
  return nonFeatureStart_;
}
inline mousse::label mousse::extendedEdgeMesh::externalStart() const
{
  return externalStart_;
}
inline mousse::label mousse::extendedEdgeMesh::internalStart() const
{
  return internalStart_;
}
inline mousse::label mousse::extendedEdgeMesh::flatStart() const
{
  return flatStart_;
}
inline mousse::label mousse::extendedEdgeMesh::openStart() const
{
  return openStart_;
}
inline mousse::label mousse::extendedEdgeMesh::multipleStart() const
{
  return multipleStart_;
}
inline bool mousse::extendedEdgeMesh::featurePoint(label ptI) const
{
  return ptI < nonFeatureStart_;
}
inline const mousse::vectorField& mousse::extendedEdgeMesh::normals() const
{
  return normals_;
}
inline const mousse::List<mousse::extendedEdgeMesh::sideVolumeType>&
mousse::extendedEdgeMesh::normalVolumeTypes() const
{
  return normalVolumeTypes_;
}
inline const mousse::vectorField& mousse::extendedEdgeMesh::edgeDirections()
const
{
  return edgeDirections_;
}
inline const mousse::labelListList&
mousse::extendedEdgeMesh::normalDirections() const
{
  return normalDirections_;
}
inline mousse::vector mousse::extendedEdgeMesh::edgeDirection
(
  label edgeI,
  label ptI
) const
{
  const edge& e = edges()[edgeI];
  if (ptI == e.start())
  {
    return edgeDirections()[edgeI];
  }
  else if (ptI == e.end())
  {
    return -edgeDirections()[edgeI];
  }
  else
  {
    FatalErrorIn("mousse::extendedEdgeMesh::edgeDirection")
      << "Requested ptI " << ptI << " is not a point on the requested "
      << "edgeI " << edgeI << ". edgeI start and end: "
      << e.start() << " " << e.end()
      << exit(FatalError);
    return vector::zero;
  }
}
inline const mousse::labelListList& mousse::extendedEdgeMesh::edgeNormals()
const
{
  return edgeNormals_;
}
inline mousse::vectorField mousse::extendedEdgeMesh::edgeNormals
(
  const labelList& edgeNormIs
) const
{
  vectorField norms(edgeNormIs.size());
  forAll(edgeNormIs, i)
  {
    norms[i] = normals_[edgeNormIs[i]];
  }
  return norms;
}
inline mousse::vectorField mousse::extendedEdgeMesh::edgeNormals(label edgeI)
const
{
  return edgeNormals(edgeNormals_[edgeI]);
}
inline const mousse::labelListList&
mousse::extendedEdgeMesh::featurePointNormals() const
{
  return featurePointNormals_;
}
inline mousse::vectorField mousse::extendedEdgeMesh::featurePointNormals
(
  label ptI
) const
{
  if (!featurePoint(ptI))
  {
    WarningIn("vectorField extendedEdgeMesh::featurePointNormals")
      << "Requesting the normals of a non-feature point. "
      << "Returned zero length vectorField."
      << endl;
    return vectorField(0);
  }
  labelList featPtNormIs(featurePointNormals_[ptI]);
  vectorField norms(featPtNormIs.size());
  forAll(featPtNormIs, i)
  {
    norms[i] = normals_[featPtNormIs[i]];
  }
  return norms;
}
inline const mousse::labelListList&
mousse::extendedEdgeMesh::featurePointEdges() const
{
  return featurePointEdges_;
}
inline const mousse::labelList& mousse::extendedEdgeMesh::regionEdges() const
{
  return regionEdges_;
}
inline mousse::extendedEdgeMesh::pointStatus
mousse::extendedEdgeMesh::getPointStatus(label ptI) const
{
  if (ptI < concaveStart_)
  {
    return CONVEX;
  }
  else if (ptI < mixedStart_)
  {
    return CONCAVE;
  }
  else if (ptI < nonFeatureStart_)
  {
    return MIXED;
  }
  else
  {
    return NONFEATURE;
  }
}
inline mousse::extendedEdgeMesh::edgeStatus
mousse::extendedEdgeMesh::getEdgeStatus(label edgeI) const
{
  if (edgeI < internalStart_)
  {
    return EXTERNAL;
  }
  else if (edgeI < flatStart_)
  {
    return INTERNAL;
  }
  else if (edgeI < openStart_)
  {
    return FLAT;
  }
  else if (edgeI < multipleStart_)
  {
    return OPEN;
  }
  else
  {
    return MULTIPLE;
  }
}
inline mousse::PackedList<2> mousse::extendedEdgeMesh::edgeBaffles
(
  label edgeI
) const
{
  const labelList& eNormals = edgeNormals_[edgeI];
  DynamicList<label> edgeBaffles(eNormals.size());
  forAll(eNormals, enI)
  {
    const label normI = eNormals[enI];
    if (normalVolumeTypes_[normI])
    {
      edgeBaffles.append(normI);
    }
  }
  return PackedList<2>(edgeBaffles);
}
