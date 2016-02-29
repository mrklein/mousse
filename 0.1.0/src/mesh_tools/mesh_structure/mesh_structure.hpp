#ifndef MESH_TOOLS_MESH_STRUCTURE_MESH_STRUCTURE_HPP_
#define MESH_TOOLS_MESH_STRUCTURE_MESH_STRUCTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshStructure
// Description
//   Detect extruded mesh structure given a set of patch faces.
// SourceFiles
//   mesh_structure.cpp
#include "label_list.hpp"
#include "uindirect_primitive_patch.hpp"
#include "class_name.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class meshStructure
{
  // Private data
    //- Cell to patch face
    labelList cellToPatchFaceAddressing_;
    //- Cell to layer
    labelList cellLayer_;
    //- Face to patch face
    labelList faceToPatchFaceAddressing_;
    //- Face to patch edge
    labelList faceToPatchEdgeAddressing_;
    //- Face to layer
    labelList faceLayer_;
    //- Point to patch point
    labelList pointToPatchPointAddressing_;
    //- Point to layer
    labelList pointLayer_;
    //- Is mesh structured?
    bool structured_;
 // Private Member Functions
    //- Is cell structured
    bool isStructuredCell
    (
      const polyMesh& mesh,
      const label layerI,
      const label cellI
    ) const;
    //- Calculate all maps.
    void correct
    (
      const polyMesh& mesh,
      const uindirectPrimitivePatch& pp
    );
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("meshStructure");
  // Constructors
    //- Construct null
    meshStructure(const polyMesh& mesh, const uindirectPrimitivePatch&);
  // Member Functions
    //- Is mesh structured?
    inline bool structured() const;
    //- Cell to patch face
    inline const labelList& cellToPatchFaceAddressing() const;
    //- Cell to patch face
    inline labelList& cellToPatchFaceAddressing();
    //- Cell to layer
    inline const labelList& cellLayer() const;
    //- Cell to layer
    inline labelList& cellLayer();
    //- Face to patch face
    inline const labelList& faceToPatchFaceAddressing() const;
    //- Face to patch face
    inline labelList& faceToPatchFaceAddressing();
    //- Face to patch edge
    inline const labelList& faceToPatchEdgeAddressing() const;
    //- Face to patch edge
    inline labelList& faceToPatchEdgeAddressing();
    //- Face to layer
    inline const labelList& faceLayer() const;
    //- Face to layer
    inline labelList& faceLayer();
    //- Point to patch point
    inline const labelList& pointToPatchPointAddressing() const;
    //- Point to patch point
    inline labelList& pointToPatchPointAddressing();
    //- Point to layer
    inline const labelList& pointLayer() const;
    //- Point to layer
    inline labelList& pointLayer();
};
}  // namespace mousse

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
#endif
