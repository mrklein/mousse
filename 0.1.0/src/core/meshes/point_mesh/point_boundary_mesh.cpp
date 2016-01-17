// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_boundary_mesh.hpp"
#include "poly_boundary_mesh.hpp"
#include "face_point_patch.hpp"
#include "point_mesh.hpp"
#include "pstream_buffers.hpp"
#include "ldu_schedule.hpp"
#include "global_mesh_data.hpp"

// Constructors
mousse::pointBoundaryMesh::pointBoundaryMesh
(
  const pointMesh& m,
  const polyBoundaryMesh& basicBdry
)
:
  pointPatchList{basicBdry.size()},
  mesh_{m}
{
  // Set boundary patches
  pointPatchList& Patches = *this;
  FOR_ALL(Patches, patchI)
  {
    Patches.set
    (
      patchI,
      facePointPatch::New(basicBdry[patchI], *this).ptr()
    );
  }
}

// Member Functions
mousse::label mousse::pointBoundaryMesh::findPatchID(const word& patchName) const
{
  return mesh()().boundaryMesh().findPatchID(patchName);
}
mousse::labelList mousse::pointBoundaryMesh::findIndices
(
  const keyType& key,
  const bool usePatchGroups
) const
{
  return mesh()().boundaryMesh().findIndices(key, usePatchGroups);
}
void mousse::pointBoundaryMesh::calcGeometry()
{
  PstreamBuffers pBufs(Pstream::defaultCommsType);
  if
  (
    Pstream::defaultCommsType == Pstream::blocking
  || Pstream::defaultCommsType == Pstream::nonBlocking
  )
  {
    FOR_ALL(*this, patchi)
    {
      operator[](patchi).initGeometry(pBufs);
    }
    pBufs.finishedSends();
    FOR_ALL(*this, patchi)
    {
      operator[](patchi).calcGeometry(pBufs);
    }
  }
  else if (Pstream::defaultCommsType == Pstream::scheduled)
  {
    const lduSchedule& patchSchedule = mesh().globalData().patchSchedule();
    // Dummy.
    pBufs.finishedSends();
    FOR_ALL(patchSchedule, patchEvali)
    {
      label patchi = patchSchedule[patchEvali].patch;
      if (patchSchedule[patchEvali].init)
      {
        operator[](patchi).initGeometry(pBufs);
      }
      else
      {
        operator[](patchi).calcGeometry(pBufs);
      }
    }
  }
}
void mousse::pointBoundaryMesh::movePoints(const pointField& p)
{
  PstreamBuffers pBufs(Pstream::defaultCommsType);
  if
  (
    Pstream::defaultCommsType == Pstream::blocking
  || Pstream::defaultCommsType == Pstream::nonBlocking
  )
  {
    FOR_ALL(*this, patchi)
    {
      operator[](patchi).initMovePoints(pBufs, p);
    }
    pBufs.finishedSends();
    FOR_ALL(*this, patchi)
    {
      operator[](patchi).movePoints(pBufs, p);
    }
  }
  else if (Pstream::defaultCommsType == Pstream::scheduled)
  {
    const lduSchedule& patchSchedule = mesh().globalData().patchSchedule();
    // Dummy.
    pBufs.finishedSends();
    FOR_ALL(patchSchedule, patchEvali)
    {
      label patchi = patchSchedule[patchEvali].patch;
      if (patchSchedule[patchEvali].init)
      {
        operator[](patchi).initMovePoints(pBufs, p);
      }
      else
      {
        operator[](patchi).movePoints(pBufs, p);
      }
    }
  }
}
void mousse::pointBoundaryMesh::updateMesh()
{
  PstreamBuffers pBufs(Pstream::defaultCommsType);
  if
  (
    Pstream::defaultCommsType == Pstream::blocking
  || Pstream::defaultCommsType == Pstream::nonBlocking
  )
  {
    FOR_ALL(*this, patchi)
    {
      operator[](patchi).initUpdateMesh(pBufs);
    }
    pBufs.finishedSends();
    FOR_ALL(*this, patchi)
    {
      operator[](patchi).updateMesh(pBufs);
    }
  }
  else if (Pstream::defaultCommsType == Pstream::scheduled)
  {
    const lduSchedule& patchSchedule = mesh().globalData().patchSchedule();
    // Dummy.
    pBufs.finishedSends();
    FOR_ALL(patchSchedule, patchEvali)
    {
      label patchi = patchSchedule[patchEvali].patch;
      if (patchSchedule[patchEvali].init)
      {
        operator[](patchi).initUpdateMesh(pBufs);
      }
      else
      {
        operator[](patchi).updateMesh(pBufs);
      }
    }
  }
}
