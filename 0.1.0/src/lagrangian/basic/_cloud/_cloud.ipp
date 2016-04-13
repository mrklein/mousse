// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_cloud.hpp"
#include "processor_poly_patch.hpp"
#include "global_mesh_data.hpp"
#include "pstream_combine_reduce_ops.hpp"
#include "map_poly_mesh.hpp"
#include "time.hpp"
#include "ofstream.hpp"
#include "wall_poly_patch.hpp"
#include "cyclic_ami_poly_patch.hpp"


// Private Member Functions 
template<class ParticleType>
void mousse::Cloud<ParticleType>::checkPatches() const
{
  const polyBoundaryMesh& pbm = polyMesh_.boundaryMesh();
  bool ok = true;
  FOR_ALL(pbm, patchI) {
    if (isA<cyclicAMIPolyPatch>(pbm[patchI])) {
      const cyclicAMIPolyPatch& cami =
        refCast<const cyclicAMIPolyPatch>(pbm[patchI]);
      if (cami.owner()) {
        ok = ok && (cami.AMI().singlePatchProc() != -1);
      }
    }
  }
  if (!ok) {
    FATAL_ERROR_IN("void mousse::Cloud<ParticleType>::initCloud(const bool)")
      << "Particle tracking across AMI patches is only currently "
      << "supported for cases where the AMI patches reside on a "
      << "single processor" << abort(FatalError);
  }
}


template<class ParticleType>
void mousse::Cloud<ParticleType>::calcCellWallFaces() const
{
  cellWallFacesPtr_.reset(new PackedBoolList(pMesh().nCells(), false));
  PackedBoolList& cellWallFaces = cellWallFacesPtr_();
  const polyBoundaryMesh& patches = polyMesh_.boundaryMesh();
  FOR_ALL(patches, patchI) {
    if (isA<wallPolyPatch>(patches[patchI])) {
      const polyPatch& patch = patches[patchI];
      const labelList& pFaceCells = patch.faceCells();
      FOR_ALL(pFaceCells, pFCI) {
        cellWallFaces[pFaceCells[pFCI]] = true;
      }
    }
  }
}


// Constructors 
template<class ParticleType>
mousse::Cloud<ParticleType>::Cloud
(
  const polyMesh& pMesh,
  const IDLList<ParticleType>& particles
)
:
  cloud{pMesh},
  IDLList<ParticleType>{},
  polyMesh_{pMesh},
  labels_{},
  nTrackingRescues_{},
  cellWallFacesPtr_{}
{
  checkPatches();
  // Ask for the tetBasePtIs to trigger all processors to build
  // them, otherwise, if some processors have no particles then
  // there is a comms mismatch.
  polyMesh_.tetBasePtIs();
  IDLList<ParticleType>::operator=(particles);
}


template<class ParticleType>
mousse::Cloud<ParticleType>::Cloud
(
  const polyMesh& pMesh,
  const word& cloudName,
  const IDLList<ParticleType>& particles
)
:
  cloud{pMesh, cloudName},
  IDLList<ParticleType>{},
  polyMesh_{pMesh},
  labels_{},
  nTrackingRescues_{},
  cellWallFacesPtr_{}
{
  checkPatches();
  // Ask for the tetBasePtIs to trigger all processors to build
  // them, otherwise, if some processors have no particles then
  // there is a comms mismatch.
  polyMesh_.tetBasePtIs();
  IDLList<ParticleType>::operator=(particles);
}


// Member Functions 
template<class ParticleType>
const mousse::PackedBoolList& mousse::Cloud<ParticleType>::cellHasWallFaces()
const
{
  if (!cellWallFacesPtr_.valid()) {
    calcCellWallFaces();
  }
  return cellWallFacesPtr_();
}


template<class ParticleType>
void mousse::Cloud<ParticleType>::addParticle(ParticleType* pPtr)
{
  this->append(pPtr);
}


template<class ParticleType>
void mousse::Cloud<ParticleType>::deleteParticle(ParticleType& p)
{
  delete(this->remove(&p));
}


template<class ParticleType>
void mousse::Cloud<ParticleType>::cloudReset(const Cloud<ParticleType>& c)
{
  // Reset particle cound and particles only
  // - not changing the cloud object registry or reference to the polyMesh
  ParticleType::particleCount_ = 0;
  IDLList<ParticleType>::operator=(c);
}


template<class ParticleType>
template<class TrackData>
void mousse::Cloud<ParticleType>::move(TrackData& td, const scalar trackTime)
{
  const polyBoundaryMesh& pbm = pMesh().boundaryMesh();
  const globalMeshData& pData = polyMesh_.globalData();
  // Which patches are processor patches
  const labelList& procPatches = pData.processorPatches();
  // Indexing of patches into the procPatches list
  const labelList& procPatchIndices = pData.processorPatchIndices();
  // Indexing of equivalent patch on neighbour processor into the
  // procPatches list on the neighbour
  const labelList& procPatchNeighbours = pData.processorPatchNeighbours();
  // Which processors this processor is connected to
  const labelList& neighbourProcs = pData[Pstream::myProcNo()];
  // Indexing from the processor number into the neighbourProcs list
  labelList neighbourProcIndices{Pstream::nProcs(), -1};
  FOR_ALL(neighbourProcs, i) {
    neighbourProcIndices[neighbourProcs[i]] = i;
  }
  // Initialise the stepFraction moved for the particles
  FOR_ALL_ITER(typename Cloud<ParticleType>, *this, pIter) {
    pIter().stepFraction() = 0;
  }
  // Reset nTrackingRescues
  nTrackingRescues_ = 0;
  // List of lists of particles to be transfered for all of the
  // neighbour processors
  List<IDLList<ParticleType>> particleTransferLists{neighbourProcs.size()};
  // List of destination processorPatches indices for all of the
  // neighbour processors
  List<DynamicList<label>> patchIndexTransferLists{neighbourProcs.size()};
  // Allocate transfer buffers
  PstreamBuffers pBufs{Pstream::nonBlocking};
  // While there are particles to transfer
  while (true) {
    particleTransferLists = IDLList<ParticleType>();
    FOR_ALL(patchIndexTransferLists, i) {
      patchIndexTransferLists[i].clear();
    }
    // Loop over all particles
    FOR_ALL_ITER(typename Cloud<ParticleType>, *this, pIter) {
      ParticleType& p = pIter();
      // Move the particle
      bool keepParticle = p.move(td, trackTime);
      // If the particle is to be kept
      // (i.e. it hasn't passed through an inlet or outlet)
      if (keepParticle) {
        // If we are running in parallel and the particle is on a
        // boundary face
        if (Pstream::parRun() && p.face() >= pMesh().nInternalFaces()) {
          label patchI = pbm.whichPatch(p.face());
          // ... and the face is on a processor patch
          // prepare it for transfer
          if (procPatchIndices[patchI] != -1) {
            label n = neighbourProcIndices
            [
              refCast<const processorPolyPatch>
              (
                pbm[patchI]
              ).neighbProcNo()
            ];
            p.prepareForParallelTransfer(patchI, td);
            particleTransferLists[n].append(this->remove(&p));
            patchIndexTransferLists[n].append
            (
              procPatchNeighbours[patchI]
            );
          }
        }
      } else {
        deleteParticle(p);
      }
    }
    if (!Pstream::parRun()) {
      break;
    }
    // Clear transfer buffers
    pBufs.clear();
    // Stream into send buffers
    FOR_ALL(particleTransferLists, i) {
      if (particleTransferLists[i].size()) {
        UOPstream particleStream{neighbourProcs[i], pBufs};
        particleStream
          << patchIndexTransferLists[i]
          << particleTransferLists[i];
      }
    }
    // Start sending. Sets number of bytes transferred
    labelListList allNTrans{Pstream::nProcs()};
    pBufs.finishedSends(allNTrans);
    bool transfered = false;
    FOR_ALL(allNTrans, i) {
      FOR_ALL(allNTrans[i], j) {
        if (allNTrans[i][j]) {
          transfered = true;
          break;
        }
      }
    }
    if (!transfered) {
      break;
    }
    // Retrieve from receive buffers
    FOR_ALL(neighbourProcs, i) {
      label neighbProci = neighbourProcs[i];
      label nRec = allNTrans[neighbProci][Pstream::myProcNo()];
      if (nRec) {
        UIPstream particleStream{static_cast<int>(neighbProci), pBufs};
        labelList receivePatchIndex{particleStream};
        IDLList<ParticleType> newParticles
        {
          particleStream,
          typename ParticleType::iNew(polyMesh_)
        };
        label pI = 0;
        FOR_ALL_ITER(typename Cloud<ParticleType>, newParticles, newpIter) {
          ParticleType& newp = newpIter();
          label patchI = procPatches[receivePatchIndex[pI++]];
          newp.correctAfterParallelTransfer(patchI, td);
          addParticle(newParticles.remove(&newp));
        }
      }
    }
  }
  if (cloud::debug) {
    reduce(nTrackingRescues_, sumOp<label>());
    if (nTrackingRescues_ > 0) {
      Info << nTrackingRescues_ << " tracking rescue corrections" << endl;
    }
  }
}


template<class ParticleType>
template<class TrackData>
void mousse::Cloud<ParticleType>::autoMap
(
  TrackData& td,
  const mapPolyMesh& mapper
)
{
  if (cloud::debug) {
    Info << "Cloud<ParticleType>::autoMap(TrackData&, const mapPolyMesh&) "
      << "for lagrangian cloud " << cloud::name() << endl;
  }
  const labelList& reverseCellMap = mapper.reverseCellMap();
  const labelList& reverseFaceMap = mapper.reverseFaceMap();
  // Reset stored data that relies on the mesh
  //   polyMesh_.clearCellTree();
  cellWallFacesPtr_.clear();
  // Ask for the tetBasePtIs to trigger all processors to build
  // them, otherwise, if some processors have no particles then
  // there is a comms mismatch.
  polyMesh_.tetBasePtIs();
  FOR_ALL_ITER(typename Cloud<ParticleType>, *this, pIter) {
    ParticleType& p = pIter();
    if (reverseCellMap[p.cell()] >= 0) {
      p.cell() = reverseCellMap[p.cell()];
      if (p.face() >= 0 && reverseFaceMap[p.face()] >= 0) {
        p.face() = reverseFaceMap[p.face()];
      } else {
        p.face() = -1;
      }
      p.initCellFacePt();
    } else {
      label trackStartCell = mapper.mergedCell(p.cell());
      if (trackStartCell < 0) {
        trackStartCell = 0;
        p.cell() = 0;
      } else {
        p.cell() = trackStartCell;
      }
      vector pos = p.position();
      const_cast<vector&>(p.position()) =
        polyMesh_.cellCentres()[trackStartCell];
      p.stepFraction() = 0;
      p.initCellFacePt();
      p.track(pos, td);
    }
  }
}


template<class ParticleType>
void mousse::Cloud<ParticleType>::writePositions() const
{
  OFstream pObj
  {
    this->db().time().path()/this->name() + "_positions.obj"
  };
  FOR_ALL_CONST_ITER(typename Cloud<ParticleType>, *this, pIter) {
    const ParticleType& p = pIter();
    pObj << "v " << p.position().x() << " " << p.position().y() << " "
      << p.position().z() << nl;
  }
  pObj.flush();
}

//  IOStream operators
#include "_cloud_io.ipp"

