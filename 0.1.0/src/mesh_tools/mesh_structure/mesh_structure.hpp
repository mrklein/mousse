// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshStructure
// Description
//   Detect extruded mesh structure given a set of patch faces.
// SourceFiles
//   mesh_structure.cpp
#ifndef mesh_structure_hpp_
#define mesh_structure_hpp_
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
#include "mesh_structure_i.hpp"
#endif
