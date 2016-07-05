#ifndef CORE_MESHES_PRIMITIVE_MESH_PATCH_ZONES_HPP_
#define CORE_MESHES_PRIMITIVE_MESH_PATCH_ZONES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchZones
// Description
//   Calculates zone number for every face of patch.
//   Gets constructed from patch and is a labeList with zone number for
//   every patch face.

#include "label_list.hpp"
#include "point_field.hpp"
#include "poly_patch.hpp"


namespace mousse {

class patchZones
:
  public labelList
{
  // Private data
    //- Reference to patch
    const primitivePatch& pp_;
    //- Reference to protected edges
    const boolList& borderEdge_;
    //- Max number of zones
    label nZones_;
  // Private Member Functions
    //- Gets labels of changed faces and propagates them to the edges.
    //  Returns labels of edges changed. Does not cross edges marked in
    //  regionEdge
    labelList faceToEdge
    (
      const labelList& changedFaces,
      labelList& edgeRegion
    );
    //- Reverse of faceToEdge: gets edges and returns faces
    labelList edgeToFace(const labelList& changedEdges);
    //- Fill *this with current zone for every face reachable
    //  from faceI without crossing edge marked in borderEdge.
    void markZone(label faceI);
public:
  CLASS_NAME("patchZones");
  // Constructors
    //- Fills *this with zone of face. Zone is area
    //  reachable by edge crossing without crossing borderEdge
    //  (bool for every edge in patch).
    patchZones(const primitivePatch& pp, const boolList& borderEdge);
  // Member Functions
    //- Number of zones
    label nZones() const
    {
      return nZones_;
    }
};
}  // namespace mousse
#endif
