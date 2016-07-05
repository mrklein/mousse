// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_zones.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(patchZones, 0);

}


// Global Functions 
// Gets labels of changed faces and propagates them to the edges. Returns
// labels of edges changed.
mousse::labelList mousse::patchZones::faceToEdge
(
  const labelList& changedFaces,
  labelList& edgeRegion
)
{
  labelList changedEdges{pp_.nEdges(), -1};
  label changedI = 0;
  FOR_ALL(changedFaces, i) {
    label faceI = changedFaces[i];
    const labelList& fEdges = pp_.faceEdges()[faceI];
    FOR_ALL(fEdges, fEdgeI) {
      label edgeI = fEdges[fEdgeI];
      if (!borderEdge_[edgeI] && (edgeRegion[edgeI] == -1)) {
        edgeRegion[edgeI] = nZones_;
        changedEdges[changedI++] = edgeI;
      }
    }
  }
  changedEdges.setSize(changedI);
  return changedEdges;
}


// Reverse of faceToEdge: gets edges and returns faces
mousse::labelList mousse::patchZones::edgeToFace(const labelList& changedEdges)
{
  labelList changedFaces(pp_.size(), -1);
  label changedI = 0;
  FOR_ALL(changedEdges, i) {
    label edgeI = changedEdges[i];
    const labelList& eFaces = pp_.edgeFaces()[edgeI];
    FOR_ALL(eFaces, eFaceI) {
      label faceI = eFaces[eFaceI];
      if (operator[](faceI) == -1) {
        operator[](faceI) = nZones_;
        changedFaces[changedI++] = faceI;
      }
    }
  }
  changedFaces.setSize(changedI);
  return changedFaces;
}


// Finds area, starting at faceI, delimited by borderEdge
void mousse::patchZones::markZone(label faceI)
{
  // List of faces whose faceZone has been set.
  labelList changedFaces{1, faceI};
  // List of edges whose faceZone has been set.
  labelList changedEdges;
  // Zones on all edges.
  labelList edgeZone{pp_.nEdges(), -1};
  while (true) {
    changedEdges = faceToEdge(changedFaces, edgeZone);
    if (debug) {
      Info
        << "From changedFaces:" << changedFaces.size()
        << " to changedEdges:" << changedEdges.size()
        << endl;
    }
    if (changedEdges.empty()) {
      break;
    }
    changedFaces = edgeToFace(changedEdges);
    if (debug) {
      Info
        << "From changedEdges:" << changedEdges.size()
        << " to changedFaces:" << changedFaces.size()
        << endl;
    }
    if (changedEdges.empty()) {
      break;
    }
  }
}


// Constructors 
// Construct from components
mousse::patchZones::patchZones
(
  const primitivePatch& pp,
  const boolList& borderEdge
)
:
  labelList{pp.size(), -1},
  pp_{pp},
  borderEdge_{borderEdge},
  nZones_{0}
{
  // Finds areas delimited by borderEdge (or 'real' edges).
  // Fills *this with zone number accordingly.
  if (borderEdge.size() != pp_.nEdges()) {
    FATAL_ERROR_IN
    (
      "patchZones::patchZones(const primitivePatch&, const boolList&)"
    )
    << "borderEdge boolList not same size as number of edges" << endl
    << "borderEdge:" << borderEdge.size() << endl
    << "nEdges    :" << pp_.nEdges()
    << abort(FatalError);
  }
  label faceI = 0;
  while (true) {
    // Find first non-visited face
    for (; faceI < pp_.size(); faceI++) {
      if (operator[](faceI) == -1) {
        operator[](faceI) = nZones_;
        markZone(faceI);
        break;
      }
    }
    if (faceI == pp_.size()) {
      // Finished.
      break;
    }
    nZones_++;
  }
}

