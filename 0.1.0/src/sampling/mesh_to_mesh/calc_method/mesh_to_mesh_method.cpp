// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_to_mesh_method.hpp"
#include "tet_overlap_volume.hpp"
#include "ofstream.hpp"
#include "time.hpp"
#include "tree_bound_box.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(meshToMeshMethod, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(meshToMeshMethod, components);

}

mousse::scalar mousse::meshToMeshMethod::tolerance_ = 1e-6;


// Private Member Functions 
mousse::labelList mousse::meshToMeshMethod::maskCells() const
{
  boundBox intersectBb
  {
    max(src_.bounds().min(), tgt_.bounds().min()),
    min(src_.bounds().max(), tgt_.bounds().max())
  };
  intersectBb.inflate(0.01);
  const cellList& srcCells = src_.cells();
  const faceList& srcFaces = src_.faces();
  const pointField& srcPts = src_.points();
  DynamicList<label> cells{src_.nCells()};
  FOR_ALL(srcCells, srcI) {
    boundBox cellBb{srcCells[srcI].points(srcFaces, srcPts), false};
    if (intersectBb.overlaps(cellBb)) {
      cells.append(srcI);
    }
  }
  if (debug) {
    Pout << "participating source mesh cells: " << cells.size() << endl;
  }
  return cells;
}


bool mousse::meshToMeshMethod::intersect
(
  const label srcCellI,
  const label tgtCellI
) const
{
  scalar threshold = tolerance_*src_.cellVolumes()[srcCellI];
  tetOverlapVolume overlapEngine;
  treeBoundBox bbTgtCell{tgt_.points(), tgt_.cellPoints()[tgtCellI]};
  return
    overlapEngine.cellCellOverlapMinDecomp(src_, srcCellI, tgt_, tgtCellI,
                                           bbTgtCell, threshold);
}


mousse::scalar mousse::meshToMeshMethod::interVol
(
  const label srcCellI,
  const label tgtCellI
) const
{
  tetOverlapVolume overlapEngine;
  treeBoundBox bbTgtCell{tgt_.points(), tgt_.cellPoints()[tgtCellI]};
  scalar vol =
    overlapEngine.cellCellOverlapVolumeMinDecomp(src_, srcCellI, tgt_,
                                                 tgtCellI, bbTgtCell);
  return vol;
}


void mousse::meshToMeshMethod::appendNbrCells
(
  const label cellI,
  const polyMesh& mesh,
  const DynamicList<label>& visitedCells,
  DynamicList<label>& nbrCellIDs
) const
{
  const labelList& nbrCells = mesh.cellCells()[cellI];
  // filter out cells already visited from cell neighbours
  FOR_ALL(nbrCells, i) {
    label nbrCellI = nbrCells[i];
    if ((findIndex(visitedCells, nbrCellI) == -1)
        && (findIndex(nbrCellIDs, nbrCellI) == -1)) {
      nbrCellIDs.append(nbrCellI);
    }
  }
}


bool mousse::meshToMeshMethod::initialise
(
  labelListList& srcToTgtAddr,
  scalarListList& srcToTgtWght,
  labelListList& tgtToSrcAddr,
  scalarListList& tgtToSrcWght
) const
{
  srcToTgtAddr.setSize(src_.nCells());
  srcToTgtWght.setSize(src_.nCells());
  tgtToSrcAddr.setSize(tgt_.nCells());
  tgtToSrcWght.setSize(tgt_.nCells());
  if (!src_.nCells()) {
    return false;
  } else if (!tgt_.nCells()) {
    if (debug) {
      Pout << "mesh interpolation: hhave " << src_.nCells() << " source "
        << " cells but no target cells" << endl;
    }
    return false;
  }
  return true;
}


// Constructors 
mousse::meshToMeshMethod::meshToMeshMethod
(
  const polyMesh& src,
  const polyMesh& tgt
)
:
  src_{src},
  tgt_{tgt},
  V_{0.0}
{
  if (!src_.nCells() || !tgt_.nCells()) {
    if (debug) {
      Pout << "mesh interpolation: cells not on processor: Source cells = "
        << src_.nCells() << ", target cells = " << tgt_.nCells()
        << endl;
    }
  }
}


// Destructor 
mousse::meshToMeshMethod::~meshToMeshMethod()
{}


// Member Functions 
void mousse::meshToMeshMethod::writeConnectivity
(
  const polyMesh& mesh1,
  const polyMesh& mesh2,
  const labelListList& mesh1ToMesh2Addr
) const
{
  Pout << "Source size = " << mesh1.nCells() << endl;
  Pout << "Target size = " << mesh2.nCells() << endl;
  word fName{"addressing_" + mesh1.name() + "_to_" + mesh2.name()};
  if (Pstream::parRun()) {
    fName = fName +  "_proc" + mousse::name(Pstream::myProcNo());
  }
  OFstream os{src_.time().path()/fName + ".obj"};
  label vertI = 0;
  FOR_ALL(mesh1ToMesh2Addr, i) {
    const labelList& addr = mesh1ToMesh2Addr[i];
    FOR_ALL(addr, j) {
      label cellI = addr[j];
      const vector& c0 = mesh1.cellCentres()[i];
      const cell& c = mesh2.cells()[cellI];
      const pointField pts(c.points(mesh2.faces(), mesh2.points()));
      FOR_ALL(pts, j) {
        const point& p = pts[j];
        os << "v " << p.x() << ' ' << p.y() << ' ' << p.z() << nl;
        vertI++;
        os << "v " << c0.x() << ' ' << c0.y() << ' ' << c0.z() << nl;
        vertI++;
        os << "l " << vertI - 1 << ' ' << vertI << nl;
      }
    }
  }
}

