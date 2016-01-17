// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
const mousse::searchableSurfaces& mousse::conformationSurfaces::geometry() const
{
  return allGeometry_;
}
const mousse::PtrList<mousse::extendedFeatureEdgeMesh>&
mousse::conformationSurfaces::features() const
{
  return features_;
}
const mousse::point& mousse::conformationSurfaces::locationInMesh() const
{
  return locationInMesh_;
}
const mousse::labelList& mousse::conformationSurfaces::surfaces() const
{
  return surfaces_;
}
const mousse::List<mousse::word>& mousse::conformationSurfaces::patchNames() const
{
  return patchNames_;
}
const mousse::PtrList<mousse::surfaceZonesInfo>&
mousse::conformationSurfaces::surfZones() const
{
  return surfZones_;
}
const mousse::PtrList<mousse::dictionary>&
mousse::conformationSurfaces::patchInfo() const
{
  return patchInfo_;
}
const mousse::treeBoundBox& mousse::conformationSurfaces::globalBounds() const
{
  return globalBounds_;
}
