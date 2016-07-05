// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sliding_interface.hpp"
#include "poly_topo_change.hpp"
#include "poly_mesh.hpp"
#include "poly_topo_changer.hpp"
#include "poly_remove_point.hpp"
#include "poly_remove_face.hpp"


// Private Member Functions 
void mousse::slidingInterface::clearCouple
(
  polyTopoChange& ref
) const
{
  if (debug) {
    Pout << "void slidingInterface::clearCouple("
      << "polyTopoChange& ref) const for object " << name() << " : "
      << "Clearing old couple points and faces." << endl;
  }
  // Remove all points from the point zone
  const polyMesh& mesh = topoChanger().mesh();
  const labelList& cutPointZoneLabels =
    mesh.pointZones()[cutPointZoneID_.index()];
  FOR_ALL(cutPointZoneLabels, pointI) {
    ref.setAction(polyRemovePoint{cutPointZoneLabels[pointI]});
  }
  // Remove all faces from the face zone
  const labelList& cutFaceZoneLabels =
    mesh.faceZones()[cutFaceZoneID_.index()];
  FOR_ALL(cutFaceZoneLabels, faceI) {
    ref.setAction(polyRemoveFace{cutFaceZoneLabels[faceI]});
  }
  if (debug) {
    Pout << "void slidingInterface::clearCouple("
      << "polyTopoChange& ref) const for object " << name() << " : "
      << "Finished clearing old couple points and faces." << endl;
  }
}
