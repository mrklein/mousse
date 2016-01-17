// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_edge_face_wave.hpp"
#include "poly_mesh.hpp"
#include "global_mesh_data.hpp"
#include "patch_tools.hpp"

// Static Data Members
template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
mousse::scalar mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
propagationTol_ = 0.01;

template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
mousse::label
mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
dummyTrackData_ = 12345;

// Private Member Functions 
// Update info for edgeI, at position pt, with information from
// neighbouring face.
// Updates:
//      - changedEdge_, changedEdges_,
//      - statistics: nEvals_, nUnvisitedEdges_
template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
bool mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
updateEdge
(
  const label edgeI,
  const label neighbourFaceI,
  const Type& neighbourInfo,
  Type& edgeInfo
)
{
  nEvals_++;
  bool wasValid = edgeInfo.valid(td_);
  bool propagate =
    edgeInfo.updateEdge
    (
      mesh_,
      patch_,
      edgeI,
      neighbourFaceI,
      neighbourInfo,
      propagationTol_,
      td_
    );
  if (propagate)
  {
    if (!changedEdge_[edgeI])
    {
      changedEdge_[edgeI] = true;
      changedEdges_.append(edgeI);
    }
  }
  if (!wasValid && edgeInfo.valid(td_))
  {
    --nUnvisitedEdges_;
  }
  return propagate;
}


// Update info for faceI, at position pt, with information from
// neighbouring edge.
// Updates:
//      - changedFace_, changedFaces_,
//      - statistics: nEvals_, nUnvisitedFace_
template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
bool mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
updateFace
(
  const label faceI,
  const label neighbourEdgeI,
  const Type& neighbourInfo,
  Type& faceInfo
)
{
  nEvals_++;
  bool wasValid = faceInfo.valid(td_);
  bool propagate =
    faceInfo.updateFace
    (
      mesh_,
      patch_,
      faceI,
      neighbourEdgeI,
      neighbourInfo,
      propagationTol_,
      td_
    );
  if (propagate)
  {
    if (!changedFace_[faceI])
    {
      changedFace_[faceI] = true;
      changedFaces_.append(faceI);
    }
  }
  if (!wasValid && faceInfo.valid(td_))
  {
    --nUnvisitedFaces_;
  }
  return propagate;
}


template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
void mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
syncEdges()
{
  const globalMeshData& globalData = mesh_.globalData();
  const mapDistribute& map = globalData.globalEdgeSlavesMap();
  const PackedBoolList& cppOrientation = globalData.globalEdgeOrientation();
  // Convert patch-edge data into cpp-edge data
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //- Construct with all data in consistent orientation
  List<Type> cppEdgeData(map.constructSize());
  FOR_ALL(patchEdges_, i)
  {
    label patchEdgeI = patchEdges_[i];
    label coupledEdgeI = coupledEdges_[i];
    if (changedEdge_[patchEdgeI])
    {
      const Type& data = allEdgeInfo_[patchEdgeI];
      // Patch-edge data needs to be converted into coupled-edge data
      // (optionally flipped) and consistent in orientation with
      // master of coupled edge (optionally flipped)
      bool sameOrientation =
      (
        sameEdgeOrientation_[i]
      == cppOrientation[coupledEdgeI]
      );
      cppEdgeData[coupledEdgeI].updateEdge
      (
        mesh_,
        patch_,
        data,
        sameOrientation,
        propagationTol_,
        td_
      );
    }
  }
  // Synchronise
  // ~~~~~~~~~~~
  globalData.syncData
  (
    cppEdgeData,
    globalData.globalEdgeSlaves(),
    globalData.globalEdgeTransformedSlaves(),
    map,
    globalData.globalTransforms(),
    updateOp<PrimitivePatchType, Type, TrackingData>
    (
      mesh_,
      patch_,
      propagationTol_,
      td_
    ),
    transformOp<PrimitivePatchType, Type, TrackingData>
    (
      mesh_,
      patch_,
      propagationTol_,
      td_
    )
  );
  // Back from cpp-edge to patch-edge data
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  FOR_ALL(patchEdges_, i)
  {
    label patchEdgeI = patchEdges_[i];
    label coupledEdgeI = coupledEdges_[i];
    const Type& data = cppEdgeData[coupledEdgeI];
    if (data.valid(td_))
    {
      bool sameOrientation =
      (
        sameEdgeOrientation_[i]
      == cppOrientation[coupledEdgeI]
      );
      allEdgeInfo_[patchEdgeI].updateEdge
      (
        mesh_,
        patch_,
        data,
        sameOrientation,
        propagationTol_,
        td_
      );
      if (!changedEdge_[patchEdgeI])
      {
        changedEdges_.append(patchEdgeI);
        changedEdge_[patchEdgeI] = true;
      }
    }
  }
}


// Constructors 
// Iterate, propagating changedEdgesInfo across patch, until no change (or
// maxIter reached). Initial edge values specified.
template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
PatchEdgeFaceWave
(
  const polyMesh& mesh,
  const PrimitivePatchType& patch,
  const labelList& changedEdges,
  const List<Type>& changedEdgesInfo,
  UList<Type>& allEdgeInfo,
  UList<Type>& allFaceInfo,
  const label maxIter,
  TrackingData& td
)
:
  mesh_(mesh),
  patch_(patch),
  allEdgeInfo_(allEdgeInfo),
  allFaceInfo_(allFaceInfo),
  td_(td),
  changedEdge_(patch_.nEdges()),
  changedEdges_(patch_.size()),
  changedFace_(patch_.size()),
  changedFaces_(patch_.size()),
  nEvals_(0),
  nUnvisitedEdges_(patch_.nEdges()),
  nUnvisitedFaces_(patch_.size())
{
  // Calculate addressing between patch_ and mesh.globalData().coupledPatch()
  // for ease of synchronisation
  PatchTools::matchEdges
  (
    patch_,
    mesh_.globalData().coupledPatch(),
    patchEdges_,
    coupledEdges_,
    sameEdgeOrientation_
  );
  if (allEdgeInfo_.size() != patch_.nEdges())
  {
    FATAL_ERROR_IN
    (
      "PatchEdgeFaceWave<Type, TrackingData>::PatchEdgeFaceWave"
      "(const polyMesh&, const labelList&, const List<Type>,"
      " List<Type>&, List<Type>&, const label maxIter)"
    )
    << "size of edgeInfo work array is not equal to the number"
    << " of edges in the patch" << endl
    << "    edgeInfo   :" << allEdgeInfo_.size() << endl
    << "    patch.nEdges:" << patch_.nEdges()
    << exit(FatalError);
  }
  if (allFaceInfo_.size() != patch_.size())
  {
    FATAL_ERROR_IN
    (
      "PatchEdgeFaceWave<Type, TrackingData>::PatchEdgeFaceWave"
      "(const polyMesh&, const labelList&, const List<Type>,"
      " List<Type>&, List<Type>&, const label maxIter)"
    )
    << "size of edgeInfo work array is not equal to the number"
    << " of faces in the patch" << endl
    << "    faceInfo   :" << allFaceInfo_.size() << endl
    << "    patch.size:" << patch_.size()
    << exit(FatalError);
  }
  // Set from initial changed edges data
  setEdgeInfo(changedEdges, changedEdgesInfo);
  if (debug)
  {
    Pout<< "Seed edges                : " << changedEdges_.size() << endl;
  }
  // Iterate until nothing changes
  label iter = iterate(maxIter);
  if ((maxIter > 0) && (iter >= maxIter))
  {
    FATAL_ERROR_IN
    (
      "PatchEdgeFaceWave<Type, TrackingData>::PatchEdgeFaceWave"
      "(const polyMesh&, const labelList&, const List<Type>,"
      " List<Type>&, List<Type>&, const label maxIter)"
    )
    << "Maximum number of iterations reached. Increase maxIter." << endl
    << "    maxIter:" << maxIter << endl
    << "    changedEdges:" << changedEdges_.size() << endl
    << "    changedFaces:" << changedFaces_.size() << endl
    << exit(FatalError);
  }
}


template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
PatchEdgeFaceWave
(
  const polyMesh& mesh,
  const PrimitivePatchType& patch,
  UList<Type>& allEdgeInfo,
  UList<Type>& allFaceInfo,
  TrackingData& td
)
:
  mesh_(mesh),
  patch_(patch),
  allEdgeInfo_(allEdgeInfo),
  allFaceInfo_(allFaceInfo),
  td_(td),
  changedEdge_(patch_.nEdges()),
  changedEdges_(patch_.nEdges()),
  changedFace_(patch_.size()),
  changedFaces_(patch_.size()),
  nEvals_(0),
  nUnvisitedEdges_(patch_.nEdges()),
  nUnvisitedFaces_(patch_.size())
{
  // Calculate addressing between patch_ and mesh.globalData().coupledPatch()
  // for ease of synchronisation
  PatchTools::matchEdges
  (
    patch_,
    mesh_.globalData().coupledPatch(),
    patchEdges_,
    coupledEdges_,
    sameEdgeOrientation_
  );
}


// Member Functions 
template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
mousse::label mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
getUnsetEdges() const
{
  return nUnvisitedEdges_;
}


template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
mousse::label mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
getUnsetFaces() const
{
  return nUnvisitedFaces_;
}


// Copy edge information into member data
template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
void mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
setEdgeInfo
(
  const labelList& changedEdges,
  const List<Type>& changedEdgesInfo
)
{
  FOR_ALL(changedEdges, changedEdgeI)
  {
    label edgeI = changedEdges[changedEdgeI];
    bool wasValid = allEdgeInfo_[edgeI].valid(td_);
    // Copy info for edgeI
    allEdgeInfo_[edgeI] = changedEdgesInfo[changedEdgeI];
    // Maintain count of unset edges
    if (!wasValid && allEdgeInfo_[edgeI].valid(td_))
    {
      --nUnvisitedEdges_;
    }
    // Mark edgeI as changed, both on list and on edge itself.
    if (!changedEdge_[edgeI])
    {
      changedEdge_[edgeI] = true;
      changedEdges_.append(edgeI);
    }
  }
}


// Propagate information from face to edge. Return number of edges changed.
template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
mousse::label mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
faceToEdge()
{
  changedEdges_.clear();
  changedEdge_ = false;
  FOR_ALL(changedFaces_, changedFaceI)
  {
    label faceI = changedFaces_[changedFaceI];
    if (!changedFace_[faceI])
    {
      FATAL_ERROR_IN("PatchEdgeFaceWave<Type, TrackingData>::faceToEdge()")
        << "face " << faceI
        << " not marked as having been changed" << nl
        << "This might be caused by multiple occurences of the same"
        << " seed edge." << abort(FatalError);
    }
    const Type& neighbourWallInfo = allFaceInfo_[faceI];
    // Evaluate all connected edges
    const labelList& fEdges = patch_.faceEdges()[faceI];
    FOR_ALL(fEdges, fEdgeI)
    {
      label edgeI = fEdges[fEdgeI];
      Type& currentWallInfo = allEdgeInfo_[edgeI];
      if (!currentWallInfo.equal(neighbourWallInfo, td_))
      {
        updateEdge
        (
          edgeI,
          faceI,
          neighbourWallInfo,
          currentWallInfo
        );
      }
    }
  }
  syncEdges();
  if (debug)
  {
    Pout<< "Changed edges             : " << changedEdges_.size() << endl;
  }
  return returnReduce(changedEdges_.size(), sumOp<label>());
}


// Propagate information from edge to face. Return number of faces changed.
template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
mousse::label mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
edgeToFace()
{
  changedFaces_.clear();
  changedFace_ = false;
  const labelListList& edgeFaces = patch_.edgeFaces();
  FOR_ALL(changedEdges_, changedEdgeI)
  {
    label edgeI = changedEdges_[changedEdgeI];
    if (!changedEdge_[edgeI])
    {
      FATAL_ERROR_IN("PatchEdgeFaceWave<Type, TrackingData>::edgeToFace()")
        << "edge " << edgeI
        << " not marked as having been changed" << nl
        << "This might be caused by multiple occurences of the same"
        << " seed edge." << abort(FatalError);
    }
    const Type& neighbourWallInfo = allEdgeInfo_[edgeI];
    // Evaluate all connected faces
    const labelList& eFaces = edgeFaces[edgeI];
    FOR_ALL(eFaces, eFaceI)
    {
      label faceI = eFaces[eFaceI];
      Type& currentWallInfo = allFaceInfo_[faceI];
      if (!currentWallInfo.equal(neighbourWallInfo, td_))
      {
        updateFace
        (
          faceI,
          edgeI,
          neighbourWallInfo,
          currentWallInfo
        );
      }
    }
  }
  if (debug)
  {
    Pout<< "Changed faces             : " << changedFaces_.size() << endl;
  }
  return returnReduce(changedFaces_.size(), sumOp<label>());
}


// Iterate
template
<
  class PrimitivePatchType,
  class Type,
  class TrackingData
>
mousse::label mousse::PatchEdgeFaceWave<PrimitivePatchType, Type, TrackingData>::
iterate
(
  const label maxIter
)
{
  // Make sure coupled edges contain same info
  syncEdges();
  nEvals_ = 0;
  label iter = 0;
  while (iter < maxIter)
  {
    if (debug)
    {
      Pout<< "Iteration " << iter << endl;
    }
    label nFaces = edgeToFace();
    if (debug)
    {
      Pout<< "Total changed faces       : " << nFaces << endl;
    }
    if (nFaces == 0)
    {
      break;
    }
    label nEdges = faceToEdge();
    if (debug)
    {
      Pout<< "Total changed edges       : " << nEdges << nl
        << "Total evaluations         : " << nEvals_ << nl
        << "Remaining unvisited edges : " << nUnvisitedEdges_ << nl
        << "Remaining unvisited faces : " << nUnvisitedFaces_ << nl
        << endl;
    }
    if (nEdges == 0)
    {
      break;
    }
    iter++;
  }
  return iter;
}

