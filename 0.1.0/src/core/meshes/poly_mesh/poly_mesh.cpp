// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "time.hpp"
#include "cell_io_list.hpp"
#include "wedge_poly_patch.hpp"
#include "empty_poly_patch.hpp"
#include "global_mesh_data.hpp"
#include "processor_poly_patch.hpp"
#include "poly_mesh_tet_decomposition.hpp"
#include "indexed_octree.hpp"
#include "tree_data_cell.hpp"
#include "_mesh_object.hpp"
#include "point_mesh.hpp"
#include "sub_field.hpp"
#include "pstream_reduce_ops.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(polyMesh, 0);
word polyMesh::defaultRegion = "region0";
word polyMesh::meshSubDir = "polyMesh";

}


// Private Member Functions
void mousse::polyMesh::calcDirections() const
{
  for (direction cmpt=0; cmpt<vector::nComponents; cmpt++) {
    solutionD_[cmpt] = 1;
  }
  // Knock out empty and wedge directions. Note:they will be present on all
  // domains.
  label nEmptyPatches = 0;
  label nWedgePatches = 0;
  vector emptyDirVec = vector::zero;
  vector wedgeDirVec = vector::zero;
  FOR_ALL(boundaryMesh(), patchi) {
    if (boundaryMesh()[patchi].size()) {
      if (isA<emptyPolyPatch>(boundaryMesh()[patchi])) {
        nEmptyPatches++;
        emptyDirVec += sum(cmptMag(boundaryMesh()[patchi].faceAreas()));
      } else if (isA<wedgePolyPatch>(boundaryMesh()[patchi])) {
        const wedgePolyPatch& wpp =
          refCast<const wedgePolyPatch>
          (
            boundaryMesh()[patchi]
          );
        nWedgePatches++;
        wedgeDirVec += cmptMag(wpp.centreNormal());
      }
    }
  }
  reduce(nEmptyPatches, maxOp<label>());
  reduce(nWedgePatches, maxOp<label>());
  if (nEmptyPatches) {
    reduce(emptyDirVec, sumOp<vector>());
    emptyDirVec /= mag(emptyDirVec);
    for (direction cmpt=0; cmpt<vector::nComponents; cmpt++) {
      if (emptyDirVec[cmpt] > 1e-6) {
        solutionD_[cmpt] = -1;
      } else {
        solutionD_[cmpt] = 1;
      }
    }
  }
  // Knock out wedge directions
  geometricD_ = solutionD_;
  if (nWedgePatches) {
    reduce(wedgeDirVec, sumOp<vector>());
    wedgeDirVec /= mag(wedgeDirVec);
    for (direction cmpt=0; cmpt<vector::nComponents; cmpt++) {
      if (wedgeDirVec[cmpt] > 1e-6) {
        geometricD_[cmpt] = -1;
      } else {
        geometricD_[cmpt] = 1;
      }
    }
  }
}


// Constructors
mousse::polyMesh::polyMesh(const IOobject& io)
:
  objectRegistry{io},
  primitiveMesh{},
  points_
  {
    {
      "points",
      time().findInstance(meshDir(), "points"),
      meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    }
  },
  faces_
  {
    {
      "faces",
      time().findInstance(meshDir(), "faces"),
      meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    }
  },
  owner_
  {
    {
      "owner",
      faces_.instance(),
      meshSubDir,
      *this,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE
    }
  },
  neighbour_
  {
    {
      "neighbour",
      faces_.instance(),
      meshSubDir,
      *this,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE
    }
  },
  clearedPrimitives_{false},
  boundary_
  {
    {
      "boundary",
      time().findInstance(meshDir(), "boundary"),
      meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    },
    *this
  },
  bounds_{points_},
  comm_{UPstream::worldComm},
  geometricD_{Vector<label>::zero},
  solutionD_{Vector<label>::zero},
  tetBasePtIsPtr_{nullptr},
  cellTreePtr_{nullptr},
  pointZones_
  {
    {
      "pointZones",
      time().findInstance
      (
        meshDir(),
        "pointZones",
        IOobject::READ_IF_PRESENT
      ),
      meshSubDir,
      *this,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE
    },
    *this
  },
  faceZones_
  {
    {
      "faceZones",
      time().findInstance
      (
        meshDir(),
        "faceZones",
        IOobject::READ_IF_PRESENT
      ),
      meshSubDir,
      *this,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE
    },
    *this
  },
  cellZones_
  {
    {
      "cellZones",
      time().findInstance
      (
        meshDir(),
        "cellZones",
        IOobject::READ_IF_PRESENT
      ),
      meshSubDir,
      *this,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE
    },
    *this
  },
  globalMeshDataPtr_{nullptr},
  moving_{false},
  topoChanging_{false},
  curMotionTimeIndex_{time().timeIndex()},
  oldPointsPtr_{nullptr}
{
  if (exists(owner_.objectPath())) {
    initMesh();
  } else {
    cellCompactIOList cLst
    {
      {
        "cells",
        time().findInstance(meshDir(), "cells"),
        meshSubDir,
        *this,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      }
    };
    // Set the primitive mesh
    initMesh(cLst);
    owner_.write();
    neighbour_.write();
  }
  // Calculate topology for the patches (processor-processor comms etc.)
  boundary_.updateMesh();
  // Calculate the geometry for the patches (transformation tensors etc.)
  boundary_.calcGeometry();
  // Warn if global empty mesh
  if (returnReduce(nPoints(), sumOp<label>()) == 0) {
    WARNING_IN("polyMesh(const IOobject&)")
      << "no points in mesh" << endl;
  }
  if (returnReduce(nCells(), sumOp<label>()) == 0) {
    WARNING_IN("polyMesh(const IOobject&)")
      << "no cells in mesh" << endl;
  }
  // Initialise demand-driven data
  calcDirections();
}


mousse::polyMesh::polyMesh
(
  const IOobject& io,
  const Xfer<pointField>& points,
  const Xfer<faceList>& faces,
  const Xfer<labelList>& owner,
  const Xfer<labelList>& neighbour,
  const bool syncPar
)
:
  objectRegistry{io},
  primitiveMesh{},
  points_
  {
    {
      "points",
      instance(),
      meshSubDir,
      *this,
      io.readOpt(),
      IOobject::AUTO_WRITE
    },
    points
  },
  faces_
  {
    {
      "faces",
      instance(),
      meshSubDir,
      *this,
      io.readOpt(),
      IOobject::AUTO_WRITE
    },
    faces
  },
  owner_
  {
    {
      "owner",
      instance(),
      meshSubDir,
      *this,
      io.readOpt(),
      IOobject::AUTO_WRITE
    },
    owner
  },
  neighbour_
  {
    {
      "neighbour",
      instance(),
      meshSubDir,
      *this,
      io.readOpt(),
      IOobject::AUTO_WRITE
    },
    neighbour
  },
  clearedPrimitives_{false},
  boundary_
  {
    {
      "boundary",
      instance(),
      meshSubDir,
      *this,
      io.readOpt(),
      IOobject::AUTO_WRITE
    },
    *this,
    polyPatchList()
  },
  bounds_{points_, syncPar},
  comm_{UPstream::worldComm},
  geometricD_{Vector<label>::zero},
  solutionD_{Vector<label>::zero},
  tetBasePtIsPtr_{nullptr},
  cellTreePtr_{nullptr},
  pointZones_
  {
    {
      "pointZones",
      instance(),
      meshSubDir,
      *this,
      io.readOpt(),
      IOobject::NO_WRITE
    },
    *this,
    PtrList<pointZone>{}
  },
  faceZones_
  {
    {
      "faceZones",
      instance(),
      meshSubDir,
      *this,
      io.readOpt(),
      IOobject::NO_WRITE
    },
    *this,
    PtrList<faceZone>{}
  },
  cellZones_
  {
    {
      "cellZones",
      instance(),
      meshSubDir,
      *this,
      io.readOpt(),
      IOobject::NO_WRITE
    },
    *this,
    PtrList<cellZone>{}
  },
  globalMeshDataPtr_{nullptr},
  moving_{false},
  topoChanging_{false},
  curMotionTimeIndex_{time().timeIndex()},
  oldPointsPtr_{nullptr}
{
  // Check if the faces and cells are valid
  FOR_ALL(faces_, facei) {
    const face& curFace = faces_[facei];
    if (min(curFace) < 0 || max(curFace) > points_.size()) {
      FATAL_ERROR_IN
      (
        "polyMesh::polyMesh\n"
        "(\n"
        "    const IOobject& io,\n"
        "    const pointField& points,\n"
        "    const faceList& faces,\n"
        "    const cellList& cells\n"
        ")\n"
      )
      << "Face " << facei << "contains vertex labels out of range: "
      << curFace << " Max point index = " << points_.size()
      << abort(FatalError);
    }
  }
  // Set the primitive mesh
  initMesh();
}


mousse::polyMesh::polyMesh
(
  const IOobject& io,
  const Xfer<pointField>& points,
  const Xfer<faceList>& faces,
  const Xfer<cellList>& cells,
  const bool syncPar
)
:
  objectRegistry{io},
  primitiveMesh{},
  points_
  {
    {
      "points",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    points
  },
  faces_
  {
    {
      "faces",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    faces
  },
  owner_
  {
    {
      "owner",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    0
  },
  neighbour_
  {
    {
      "neighbour",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    0
  },
  clearedPrimitives_{false},
  boundary_
  {
    {
      "boundary",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    *this,
    0
  },
  bounds_{points_, syncPar},
  comm_{UPstream::worldComm},
  geometricD_{Vector<label>::zero},
  solutionD_{Vector<label>::zero},
  tetBasePtIsPtr_{nullptr},
  cellTreePtr_{nullptr},
  pointZones_
  {
    {
      "pointZones",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    *this,
    0
  },
  faceZones_
  {
    {
      "faceZones",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    *this,
    0
  },
  cellZones_
  {
    {
      "cellZones",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    *this,
    0
  },
  globalMeshDataPtr_{nullptr},
  moving_{false},
  topoChanging_{false},
  curMotionTimeIndex_{time().timeIndex()},
  oldPointsPtr_{nullptr}
{
  // Check if faces are valid
  FOR_ALL(faces_, facei) {
    const face& curFace = faces_[facei];
    if (min(curFace) < 0 || max(curFace) > points_.size()) {
      FATAL_ERROR_IN
      (
        "polyMesh::polyMesh\n"
        "(\n"
        "    const IOobject&,\n"
        "    const Xfer<pointField>&,\n"
        "    const Xfer<faceList>&,\n"
        "    const Xfer<cellList>&\n"
        ")\n"
      )
      << "Face " << facei << "contains vertex labels out of range: "
      << curFace << " Max point index = " << points_.size()
      << abort(FatalError);
    }
  }
  // transfer in cell list
  cellList cLst{cells};
  // Check if cells are valid
  FOR_ALL(cLst, celli) {
    const cell& curCell = cLst[celli];
    if (min(curCell) < 0 || max(curCell) > faces_.size()) {
      FATAL_ERROR_IN
      (
        "polyMesh::polyMesh\n"
        "(\n"
        "  const IOobject&,\n"
        "  const Xfer<pointField>&,\n"
        "  const Xfer<faceList>&,\n"
        "  const Xfer<cellList>&\n"
        ")\n"
      )
      << "Cell " << celli << "contains face labels out of range: "
      << curCell << " Max face index = " << faces_.size()
      << abort(FatalError);
    }
  }
  // Set the primitive mesh
  initMesh(cLst);
}


void mousse::polyMesh::resetPrimitives
(
  const Xfer<pointField>& points,
  const Xfer<faceList>& faces,
  const Xfer<labelList>& owner,
  const Xfer<labelList>& neighbour,
  const labelList& patchSizes,
  const labelList& patchStarts,
  const bool validBoundary
)
{
  // Clear addressing. Keep geometric props and updateable props for mapping.
  clearAddressing(true);
  // Take over new primitive data.
  // Optimized to avoid overwriting data at all
  if (notNull(points)) {
    points_.transfer(points());
    bounds_ = boundBox(points_, validBoundary);
  }
  if (notNull(faces)) {
    faces_.transfer(faces());
  }
  if (notNull(owner)) {
    owner_.transfer(owner());
  }
  if (notNull(neighbour)) {
    neighbour_.transfer(neighbour());
  }
  // Reset patch sizes and starts
  FOR_ALL(boundary_, patchI) {
    boundary_[patchI] = polyPatch
    {
      boundary_[patchI],
      boundary_,
      patchI,
      patchSizes[patchI],
      patchStarts[patchI]
    };
  }
  // Flags the mesh files as being changed
  setInstance(time().timeName());
  // Check if the faces and cells are valid
  FOR_ALL(faces_, facei) {
    const face& curFace = faces_[facei];
    if (min(curFace) < 0 || max(curFace) > points_.size()) {
      FATAL_ERROR_IN
      (
        "polyMesh::polyMesh::resetPrimitives\n"
        "(\n"
        "  const Xfer<pointField>&,\n"
        "  const Xfer<faceList>&,\n"
        "  const Xfer<labelList>& owner,\n"
        "  const Xfer<labelList>& neighbour,\n"
        "  const labelList& patchSizes,\n"
        "  const labelList& patchStarts\n"
        "  const bool validBoundary\n"
        ")\n"
      )
      << "Face " << facei << " contains vertex labels out of range: "
      << curFace << " Max point index = " << points_.size()
      << abort(FatalError);
    }
  }
  // Set the primitive mesh from the owner_, neighbour_.
  // Works out from patch end where the active faces stop.
  initMesh();
  if (validBoundary) {
    // Note that we assume that all the patches stay the same and are
    // correct etc. so we can already use the patches to do
    // processor-processor comms.
    // Calculate topology for the patches (processor-processor comms etc.)
    boundary_.updateMesh();
    // Calculate the geometry for the patches (transformation tensors etc.)
    boundary_.calcGeometry();
    // Warn if global empty mesh
    if ((returnReduce(nPoints(), sumOp<label>()) == 0)
        || (returnReduce(nCells(), sumOp<label>()) == 0)) {
      FATAL_ERROR_IN
      (
        "polyMesh::polyMesh::resetPrimitives\n"
        "(\n"
        "    const Xfer<pointField>&,\n"
        "    const Xfer<faceList>&,\n"
        "    const Xfer<labelList>& owner,\n"
        "    const Xfer<labelList>& neighbour,\n"
        "    const labelList& patchSizes,\n"
        "    const labelList& patchStarts\n"
        "    const bool validBoundary\n"
        ")\n"
      )
      << "no points or no cells in mesh" << endl;
    }
  }
}


// Destructor
mousse::polyMesh::~polyMesh()
{
  clearOut();
  resetMotion();
}


// Member Functions
const mousse::fileName& mousse::polyMesh::dbDir() const
{
  if (objectRegistry::dbDir() == defaultRegion) {
    return parent().dbDir();
  } else {
    return objectRegistry::dbDir();
  }
}


mousse::fileName mousse::polyMesh::meshDir() const
{
  return dbDir()/meshSubDir;
}


const mousse::fileName& mousse::polyMesh::pointsInstance() const
{
  return points_.instance();
}


const mousse::fileName& mousse::polyMesh::facesInstance() const
{
  return faces_.instance();
}


const mousse::Vector<mousse::label>& mousse::polyMesh::geometricD() const
{
  if (geometricD_.x() == 0) {
    calcDirections();
  }
  return geometricD_;
}


mousse::label mousse::polyMesh::nGeometricD() const
{
  return cmptSum(geometricD() + Vector<label>::one)/2;
}


const mousse::Vector<mousse::label>& mousse::polyMesh::solutionD() const
{
  if (solutionD_.x() == 0) {
    calcDirections();
  }
  return solutionD_;
}


mousse::label mousse::polyMesh::nSolutionD() const
{
  return cmptSum(solutionD() + Vector<label>::one)/2;
}


const mousse::labelList& mousse::polyMesh::tetBasePtIs() const
{
  if (tetBasePtIsPtr_.empty()) {
    if (debug) {
      WARNING_IN("const labelList& polyMesh::tetBasePtIs() const")
        << "Tet base point indices not available.  "
        << "Forcing storage of base points."
        << endl;
    }
    tetBasePtIsPtr_.reset
    (
      new labelList
      {
        polyMeshTetDecomposition::findFaceBasePts(*this)
      }
    );
  }
  return tetBasePtIsPtr_();
}


const mousse::indexedOctree<mousse::treeDataCell>&
mousse::polyMesh::cellTree() const
{
  if (cellTreePtr_.empty()) {
    treeBoundBox overallBb{points()};
    Random rndGen{261782};
    overallBb = overallBb.extend(rndGen, 1e-4);
    overallBb.min() -= point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
    overallBb.max() += point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
    cellTreePtr_.reset
    (
      new indexedOctree<treeDataCell>
      {
        treeDataCell
        {
          false,      // not cache bb
          *this,
          CELL_TETS   // use tet-decomposition for any inside test
        },
        overallBb,
        8,              // maxLevel
        10,             // leafsize
        5.0             // duplicity
      }
    );
  }
  return cellTreePtr_();
}


void mousse::polyMesh::addPatches
(
  const List<polyPatch*>& p,
  const bool validBoundary
)
{
  if (boundaryMesh().size()) {
    FATAL_ERROR_IN
    (
      "void polyMesh::addPatches(const List<polyPatch*>&, const bool)"
    )
    << "boundary already exists"
    << abort(FatalError);
  }
  // Reset valid directions
  geometricD_ = Vector<label>::zero;
  solutionD_ = Vector<label>::zero;
  boundary_.setSize(p.size());
  // Copy the patch pointers
  FOR_ALL(p, pI) {
    boundary_.set(pI, p[pI]);
  }
  // parallelData depends on the processorPatch ordering so force
  // recalculation. Problem: should really be done in removeBoundary but
  // there is some info in parallelData which might be interesting inbetween
  // removeBoundary and addPatches.
  globalMeshDataPtr_.clear();
  if (validBoundary) {
    // Calculate topology for the patches (processor-processor comms etc.)
    boundary_.updateMesh();
    // Calculate the geometry for the patches (transformation tensors etc.)
    boundary_.calcGeometry();
    boundary_.checkDefinition();
  }
}


void mousse::polyMesh::addZones
(
  const List<pointZone*>& pz,
  const List<faceZone*>& fz,
  const List<cellZone*>& cz
)
{
  if (pointZones().size() || faceZones().size() || cellZones().size()) {
    FATAL_ERROR_IN
    (
      "void addZones\n"
      "(\n"
      "  const List<pointZone*>&,\n"
      "  const List<faceZone*>&,\n"
      "  const List<cellZone*>&\n"
      ")"
    )
    << "point, face or cell zone already exists"
    << abort(FatalError);
  }
  // Point zones
  if (pz.size()) {
    pointZones_.setSize(pz.size());
    // Copy the zone pointers
    FOR_ALL(pz, pI) {
      pointZones_.set(pI, pz[pI]);
    }
    pointZones_.writeOpt() = IOobject::AUTO_WRITE;
  }
  // Face zones
  if (fz.size()) {
    faceZones_.setSize(fz.size());
    // Copy the zone pointers
    FOR_ALL(fz, fI) {
      faceZones_.set(fI, fz[fI]);
    }
    faceZones_.writeOpt() = IOobject::AUTO_WRITE;
  }
  // Cell zones
  if (cz.size()) {
    cellZones_.setSize(cz.size());
    // Copy the zone pointers
    FOR_ALL(cz, cI) {
      cellZones_.set(cI, cz[cI]);
    }
    cellZones_.writeOpt() = IOobject::AUTO_WRITE;
  }
}


const mousse::pointField& mousse::polyMesh::points() const
{
  if (clearedPrimitives_) {
    FATAL_ERROR_IN("const pointField& polyMesh::points() const")
      << "points deallocated"
      << abort(FatalError);
  }
  return points_;
}


bool mousse::polyMesh::upToDatePoints(const regIOobject& io) const
{
  return io.upToDate(points_);
}


void mousse::polyMesh::setUpToDatePoints(regIOobject& io) const
{
  io.eventNo() = points_.eventNo();
}


const mousse::faceList& mousse::polyMesh::faces() const
{
  if (clearedPrimitives_) {
    FATAL_ERROR_IN("const faceList& polyMesh::faces() const")
      << "faces deallocated"
      << abort(FatalError);
  }
  return faces_;
}


const mousse::labelList& mousse::polyMesh::faceOwner() const
{
  return owner_;
}


const mousse::labelList& mousse::polyMesh::faceNeighbour() const
{
  return neighbour_;
}


const mousse::pointField& mousse::polyMesh::oldPoints() const
{
  if (oldPointsPtr_.empty()) {
    if (debug) {
      WARNING_IN("const pointField& polyMesh::oldPoints() const")
        << "Old points not available.  Forcing storage of old points"
        << endl;
    }
    oldPointsPtr_.reset(new pointField(points_));
    curMotionTimeIndex_ = time().timeIndex();
  }
  return oldPointsPtr_();
}


mousse::tmp<mousse::scalarField> mousse::polyMesh::movePoints
(
  const pointField& newPoints
)
{
  if (debug) {
    Info << "tmp<scalarField> polyMesh::movePoints(const pointField&) : "
      << " Moving points for time " << time().value()
      << " index " << time().timeIndex() << endl;
  }
  moving(true);
  // Pick up old points
  if (curMotionTimeIndex_ != time().timeIndex()) {
    // Mesh motion in the new time step
    oldPointsPtr_.clear();
    oldPointsPtr_.reset(new pointField(points_));
    curMotionTimeIndex_ = time().timeIndex();
  }
  points_ = newPoints;
  bool moveError = false;
  if (debug) {
    // Check mesh motion
    if (checkMeshMotion(points_, true)) {
      moveError = true;
      Info << "tmp<scalarField> polyMesh::movePoints"
        << "(const pointField&) : "
        << "Moving the mesh with given points will "
        << "invalidate the mesh." << nl
        << "Mesh motion should not be executed." << endl;
    }
  }
  points_.writeOpt() = IOobject::AUTO_WRITE;
  points_.instance() = time().timeName();
  points_.eventNo() = getEvent();
  tmp<scalarField> sweptVols = primitiveMesh::movePoints
  (
    points_,
    oldPoints()
  );
  // Adjust parallel shared points
  if (globalMeshDataPtr_.valid()) {
    globalMeshDataPtr_().movePoints(points_);
  }
  // Force recalculation of all geometric data with new points
  bounds_ = boundBox(points_);
  boundary_.movePoints(points_);
  pointZones_.movePoints(points_);
  faceZones_.movePoints(points_);
  cellZones_.movePoints(points_);
  // Reset valid directions (could change with rotation)
  geometricD_ = Vector<label>::zero;
  solutionD_ = Vector<label>::zero;
  meshObject::movePoints<polyMesh>(*this);
  meshObject::movePoints<pointMesh>(*this);
  const_cast<Time&>(time()).functionObjects().movePoints(*this);
  if (debug && moveError) {
    // Write mesh to ease debugging. Note we want to avoid calling
    // e.g. fvMesh::write since meshPhi not yet complete.
    polyMesh::write();
  }
  return sweptVols;
}


// Reset motion by deleting old points
void mousse::polyMesh::resetMotion() const
{
  curMotionTimeIndex_ = 0;
  oldPointsPtr_.clear();
}


// Return parallel info
const mousse::globalMeshData& mousse::polyMesh::globalData() const
{
  if (globalMeshDataPtr_.empty()) {
    if (debug) {
      Pout << "polyMesh::globalData() const : "
        << "Constructing parallelData from processor topology"
        << endl;
    }
    // Construct globalMeshData using processorPatch information only.
    globalMeshDataPtr_.reset(new globalMeshData(*this));
  }
  return globalMeshDataPtr_();
}


mousse::label mousse::polyMesh::comm() const
{
  return comm_;
}


mousse::label& mousse::polyMesh::comm()
{
  return comm_;
}


void mousse::polyMesh::removeFiles(const fileName& instanceDir) const
{
  fileName meshFilesPath = thisDb().time().path()/instanceDir/meshDir();
  rm(meshFilesPath/"points");
  rm(meshFilesPath/"faces");
  rm(meshFilesPath/"owner");
  rm(meshFilesPath/"neighbour");
  rm(meshFilesPath/"cells");
  rm(meshFilesPath/"boundary");
  rm(meshFilesPath/"pointZones");
  rm(meshFilesPath/"faceZones");
  rm(meshFilesPath/"cellZones");
  rm(meshFilesPath/"meshModifiers");
  rm(meshFilesPath/"parallelData");
  // remove subdirectories
  if (isDir(meshFilesPath/"sets"))
  {
    rmDir(meshFilesPath/"sets");
  }
}


void mousse::polyMesh::removeFiles() const
{
  removeFiles(instance());
}


void mousse::polyMesh::findCellFacePt
(
  const point& p,
  label& celli,
  label& tetFacei,
  label& tetPti
) const
{
  celli = -1;
  tetFacei = -1;
  tetPti = -1;
  const indexedOctree<treeDataCell>& tree = cellTree();
  // Find nearest cell to the point
  pointIndexHit info = tree.findNearest(p, sqr(GREAT));
  if (info.hit()) {
    label nearestCellI = tree.shapes().cellLabels()[info.index()];
    // Check the nearest cell to see if the point is inside.
    findTetFacePt(nearestCellI, p, tetFacei, tetPti);
    if (tetFacei != -1) {
      // Point was in the nearest cell
      celli = nearestCellI;
      return;
    } else {
      // Check the other possible cells that the point may be in
      labelList testCells = tree.findIndices(p);
      FOR_ALL(testCells, pCI) {
        label testCellI = tree.shapes().cellLabels()[testCells[pCI]];
        if (testCellI == nearestCellI) {
          // Don't retest the nearest cell
          continue;
        }
        // Check the test cell to see if the point is inside.
        findTetFacePt(testCellI, p, tetFacei, tetPti);
        if (tetFacei != -1) {
          // Point was in the test cell
          celli = testCellI;
          return;
        }
      }
    }
  } else {
    FATAL_ERROR_IN
    (
      "void mousse::polyMesh::findCellFacePt"
      "("
      "  const point& p, "
      "  label& celli, "
      "  label& tetFacei, "
      "  label& tetPti"
      ") const"
    )
    << "Did not find nearest cell in search tree."
    << abort(FatalError);
  }
}


void mousse::polyMesh::findTetFacePt
(
  const label celli,
  const point& p,
  label& tetFacei,
  label& tetPti
) const
{
  const polyMesh& mesh = *this;
  tetIndices tet(polyMeshTetDecomposition::findTet(mesh, celli, p));
  tetFacei = tet.face();
  tetPti = tet.tetPt();
}


bool mousse::polyMesh::pointInCell
(
  const point& p,
  label celli,
  const cellDecomposition decompMode
) const
{
  switch (decompMode) {
    case FACE_PLANES:
      {
        return primitiveMesh::pointInCell(p, celli);
      }
      break;
    case FACE_CENTRE_TRIS:
      {
        // only test that point is on inside of plane defined by cell face
        // triangles
        const cell& cFaces = cells()[celli];
        FOR_ALL(cFaces, cFacei) {
          label facei = cFaces[cFacei];
          const face& f = faces_[facei];
          const point& fc = faceCentres()[facei];
          bool isOwn = (owner_[facei] == celli);
          FOR_ALL(f, fp) {
            label pointI;
            label nextPointI;
            if (isOwn) {
              pointI = f[fp];
              nextPointI = f.nextLabel(fp);
            } else {
              pointI = f.nextLabel(fp);
              nextPointI = f[fp];
            }
            triPointRef faceTri
            {
              points()[pointI],
              points()[nextPointI],
              fc
            };
            vector proj = p - faceTri.centre();
            if ((faceTri.normal() & proj) > 0) {
              return false;
            }
          }
        }
        return true;
      }
      break;
    case FACE_DIAG_TRIS:
      {
        // only test that point is on inside of plane defined by cell face
        // triangles
        const cell& cFaces = cells()[celli];
        FOR_ALL(cFaces, cFacei) {
          label facei = cFaces[cFacei];
          const face& f = faces_[facei];
          for (label tetPti = 1; tetPti < f.size() - 1; tetPti++) {
            // Get tetIndices of face triangle
            tetIndices faceTetIs
            {
              polyMeshTetDecomposition::triangleTetIndices
                (
                  *this,
                  facei,
                  celli,
                  tetPti
                )
            };
            triPointRef faceTri = faceTetIs.faceTri(*this);
            vector proj = p - faceTri.centre();
            if ((faceTri.normal() & proj) > 0) {
              return false;
            }
          }
        }
        return true;
      }
      break;
    case CELL_TETS:
      {
        label tetFacei;
        label tetPti;
        findTetFacePt(celli, p, tetFacei, tetPti);
        return tetFacei != -1;
      }
      break;
  }
  return false;
}


mousse::label mousse::polyMesh::findCell
(
  const point& p,
  const cellDecomposition decompMode
) const
{
  if (Pstream::parRun()
      && (decompMode == FACE_DIAG_TRIS || decompMode == CELL_TETS))
  {
    // Force construction of face-diagonal decomposition before testing
    // for zero cells.
    //
    // If parallel running a local domain might have zero cells so never
    // construct the face-diagonal decomposition which uses parallel
    // transfers.
    (void)tetBasePtIs();
  }
  if (nCells() == 0) {
    return -1;
  }
  if (decompMode == CELL_TETS) {
    // Advanced search method utilizing an octree
    // and tet-decomposition of the cells
    label celli;
    label tetFacei;
    label tetPti;
    findCellFacePt(p, celli, tetFacei, tetPti);
    return celli;
  } else {
    // Approximate search avoiding the construction of an octree
    // and cell decomposition
    // Find the nearest cell centre to this location
    label celli = findNearestCell(p);
    // If point is in the nearest cell return
    if (pointInCell(p, celli, decompMode)) {
      return celli;
    } else {
      // Point is not in the nearest cell so search all cells
      for (label celli = 0; celli < nCells(); celli++) {
        if (pointInCell(p, celli, decompMode)) {
          return celli;
        }
      }
      return -1;
    }
  }
}

