// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "layer_addition_removal.hpp"
#include "poly_mesh.hpp"
#include "primitive_mesh.hpp"
#include "poly_topo_change.hpp"
#include "opposite_face.hpp"
#include "poly_topo_changer.hpp"
// Private Member Functions 
bool mousse::layerAdditionRemoval::setLayerPairing() const
{
  // Note:
  // This is also the most complex part of the topological change.
  // Therefore it will be calculated here and stored as temporary
  // data until the actual topological change, after which it will
  // be cleared.
  // Algorithm for point collapse
  // 1)  Go through the master cell layer and for every face of
  //     the face zone find the opposite face in the master cell.
  //     Check the direction of the opposite face and adjust as
  //     necessary.  Check other faces to find an edge defining
  //     relative orientation of the two faces and adjust the face
  //     as necessary.  Once the face is adjusted, record the
  //     addressing between the master and slave vertex layer.
  const polyMesh& mesh = topoChanger().mesh();
  const labelList& mc =
    mesh.faceZones()[faceZoneID_.index()].masterCells();
  const labelList& mf = mesh.faceZones()[faceZoneID_.index()];
  const boolList& mfFlip =
    mesh.faceZones()[faceZoneID_.index()].flipMap();
  const faceList& faces = mesh.faces();
  const cellList& cells = mesh.cells();
  // Grab the local faces from the master zone
  const faceList& mlf =
    mesh.faceZones()[faceZoneID_.index()]().localFaces();
  const labelList& meshPoints =
    mesh.faceZones()[faceZoneID_.index()]().meshPoints();
  // Create a list of points to collapse for every point of
  // the master patch
  if (pointsPairingPtr_ || facesPairingPtr_)
  {
    FATAL_ERROR_IN
    (
      "void mousse::layerAdditionRemoval::setLayerPairing() const"
    )
    << "Problem with layer pairing data"
    << abort(FatalError);
  }
  pointsPairingPtr_ = new labelList(meshPoints.size(), -1);
  labelList& ptc = *pointsPairingPtr_;
  facesPairingPtr_ = new labelList(mf.size(), -1);
  labelList& ftc = *facesPairingPtr_;
  if (debug > 1)
  {
    Pout<< "meshPoints: " << meshPoints << nl
      << "localPoints: "
      << mesh.faceZones()[faceZoneID_.index()]().localPoints()
      << endl;
  }
  // For all faces, create the mapping
  label nPointErrors = 0;
  label nFaceErrors = 0;
  FOR_ALL(mf, faceI)
  {
    // Get the local master face
    face curLocalFace = mlf[faceI];
    // Flip face based on flip index to recover original orientation
    if (mfFlip[faceI])
    {
      curLocalFace.flip();
    }
    // Get the opposing face from the master cell
    oppositeFace lidFace =
      cells[mc[faceI]].opposingFace(mf[faceI], faces);
    if (!lidFace.found())
    {
      // This is not a valid layer; cannot continue
      nFaceErrors++;
      continue;
    }
    if (debug > 1)
    {
      Pout<< "curMasterFace: " << faces[mf[faceI]] << nl
        << "cell shape: " << mesh.cellShapes()[mc[faceI]] << nl
        << "curLocalFace: " << curLocalFace << nl
        << "lidFace: " << lidFace
        << " master index: " << lidFace.masterIndex()
        << " oppositeIndex: " << lidFace.oppositeIndex() << endl;
    }
    // Grab the opposite face for face collapse addressing
    ftc[faceI] = lidFace.oppositeIndex();
    // Using the local face insert the points into the lid list
    FOR_ALL(curLocalFace, pointI)
    {
      const label clp = curLocalFace[pointI];
      if (ptc[clp] == -1)
      {
        // Point not mapped yet.  Insert the label
        ptc[clp] = lidFace[pointI];
      }
      else
      {
        // Point mapped from some other face.  Check the label
        if (ptc[clp] != lidFace[pointI])
        {
          nPointErrors++;
          if (debug > 1)
          {
            Pout<< "Topological error in cell layer pairing.  "
              << "This mesh is either topologically incorrect "
              << "or the master face layer is not defined "
              << "consistently.  Please check the "
              << "face zone flip map." << nl
              << "First index: " << ptc[clp]
              << " new index: " << lidFace[pointI] << endl;
          }
        }
      }
    }
  }
  reduce(nPointErrors, sumOp<label>());
  reduce(nFaceErrors, sumOp<label>());
  if (nPointErrors > 0 || nFaceErrors > 0)
  {
    clearAddressing();
    return false;
  }
  else
  {
    // Valid layer
    return true;
  }
}
const mousse::labelList& mousse::layerAdditionRemoval::pointsPairing() const
{
  if (!pointsPairingPtr_)
  {
    FATAL_ERROR_IN
    (
      "const labelList& layerAdditionRemoval::pointsPairing() const"
    )
    << "Problem with layer pairing data for object " << name()
    << abort(FatalError);
  }
  return *pointsPairingPtr_;
}
const mousse::labelList& mousse::layerAdditionRemoval::facesPairing() const
{
  if (!facesPairingPtr_)
  {
    FATAL_ERROR_IN
    (
      "const labelList& layerAdditionRemoval::facesPairing() const"
    )   << "Problem with layer pairing data for object " << name()
      << abort(FatalError);
  }
  return *facesPairingPtr_;
}
// Member Functions 
void mousse::layerAdditionRemoval::modifyMotionPoints
(
  pointField& /*motionPoints*/
) const
{
  if (debug)
  {
    Pout<< "void layerAdditionRemoval::modifyMotionPoints("
      << "pointField& motionPoints) const for object "
      << name() << " : ";
  }
  if (debug)
  {
    Pout<< "No motion point adjustment" << endl;
  }
}
