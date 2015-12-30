// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_structure.hpp"
// Member Functions 
bool mousse::meshStructure::structured() const
{
  return structured_;
}
const mousse::labelList& mousse::meshStructure::cellToPatchFaceAddressing() const
{
  return cellToPatchFaceAddressing_;
}
mousse::labelList& mousse::meshStructure::cellToPatchFaceAddressing()
{
  return cellToPatchFaceAddressing_;
}
const mousse::labelList& mousse::meshStructure::cellLayer() const
{
  return cellLayer_;
}
mousse::labelList& mousse::meshStructure::cellLayer()
{
  return cellLayer_;
}
const mousse::labelList& mousse::meshStructure::faceToPatchFaceAddressing() const
{
  return faceToPatchFaceAddressing_;
}
mousse::labelList& mousse::meshStructure::faceToPatchFaceAddressing()
{
  return faceToPatchFaceAddressing_;
}
const mousse::labelList& mousse::meshStructure::faceToPatchEdgeAddressing() const
{
  return faceToPatchEdgeAddressing_;
}
mousse::labelList& mousse::meshStructure::faceToPatchEdgeAddressing()
{
  return faceToPatchEdgeAddressing_;
}
const mousse::labelList& mousse::meshStructure::faceLayer() const
{
  return faceLayer_;
}
mousse::labelList& mousse::meshStructure::faceLayer()
{
  return faceLayer_;
}
const mousse::labelList& mousse::meshStructure::pointToPatchPointAddressing() const
{
  return pointToPatchPointAddressing_;
}
mousse::labelList& mousse::meshStructure::pointToPatchPointAddressing()
{
  return pointToPatchPointAddressing_;
}
const mousse::labelList& mousse::meshStructure::pointLayer() const
{
  return pointLayer_;
}
mousse::labelList& mousse::meshStructure::pointLayer()
{
  return pointLayer_;
}
