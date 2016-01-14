// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inverse_face_distance_diffusivity.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "hash_set.hpp"
#include "wall_point.hpp"
#include "mesh_wave.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(inverseFaceDistanceDiffusivity, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    motionDiffusivity,
    inverseFaceDistanceDiffusivity,
    Istream
  );
}
// Constructors 
mousse::inverseFaceDistanceDiffusivity::inverseFaceDistanceDiffusivity
(
  const fvMesh& mesh,
  Istream& mdData
)
:
  uniformDiffusivity(mesh, mdData),
  patchNames_(mdData)
{
  correct();
}
// Destructor 
mousse::inverseFaceDistanceDiffusivity::~inverseFaceDistanceDiffusivity()
{}
// Member Functions 
void mousse::inverseFaceDistanceDiffusivity::correct()
{
  const polyBoundaryMesh& bdry = mesh().boundaryMesh();
  labelHashSet patchSet(bdry.size());
  label nPatchFaces = 0;
  FOR_ALL(patchNames_, i)
  {
    const label pID = bdry.findPatchID(patchNames_[i]);
    if (pID > -1)
    {
      patchSet.insert(pID);
      nPatchFaces += bdry[pID].size();
    }
  }
  List<wallPoint> faceDist(nPatchFaces);
  labelList changedFaces(nPatchFaces);
  nPatchFaces = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchSet, iter)
  {
    const polyPatch& patch = bdry[iter.key()];
    const vectorField::subField fc(patch.faceCentres());
    FOR_ALL(fc, patchFaceI)
    {
      changedFaces[nPatchFaces] = patch.start() + patchFaceI;
      faceDist[nPatchFaces] = wallPoint(fc[patchFaceI], 0);
      nPatchFaces++;
    }
  }
  faceDist.setSize(nPatchFaces);
  changedFaces.setSize(nPatchFaces);
  MeshWave<wallPoint> waveInfo
  (
    mesh(),
    changedFaces,
    faceDist,
    mesh().globalData().nTotalCells()+1   // max iterations
  );
  const List<wallPoint>& faceInfo = waveInfo.allFaceInfo();
  const List<wallPoint>& cellInfo = waveInfo.allCellInfo();
  for (label faceI=0; faceI<mesh().nInternalFaces(); faceI++)
  {
    scalar dist = faceInfo[faceI].distSqr();
    faceDiffusivity_[faceI] = 1.0/sqrt(dist);
  }
  FOR_ALL(faceDiffusivity_.boundaryField(), patchI)
  {
    fvsPatchScalarField& bfld = faceDiffusivity_.boundaryField()[patchI];
    const labelUList& faceCells = bfld.patch().faceCells();
    if (patchSet.found(patchI))
    {
      FOR_ALL(bfld, i)
      {
        scalar dist = cellInfo[faceCells[i]].distSqr();
        bfld[i] = 1.0/sqrt(dist);
      }
    }
    else
    {
      const label start = bfld.patch().start();
      FOR_ALL(bfld, i)
      {
        scalar dist = faceInfo[start+i].distSqr();
        bfld[i] = 1.0/sqrt(dist);
      }
    }
  }
}
