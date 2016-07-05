// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tree_data_cell.hpp"
#include "indexed_octree.hpp"
#include "poly_mesh.hpp"
// Static Data Members
namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(treeDataCell, 0);

}
// Private Member Functions 
mousse::treeBoundBox mousse::treeDataCell::calcCellBb(const label cellI) const
{
  const cellList& cells = mesh_.cells();
  const faceList& faces = mesh_.faces();
  const pointField& points = mesh_.points();
  treeBoundBox cellBb
  (
    vector(GREAT, GREAT, GREAT),
    vector(-GREAT, -GREAT, -GREAT)
  );
  const cell& cFaces = cells[cellI];
  FOR_ALL(cFaces, cFaceI)
  {
    const face& f = faces[cFaces[cFaceI]];
    FOR_ALL(f, fp)
    {
      const point& p = points[f[fp]];
      cellBb.min() = min(cellBb.min(), p);
      cellBb.max() = max(cellBb.max(), p);
    }
  }
  return cellBb;
}
void mousse::treeDataCell::update()
{
  if (cacheBb_)
  {
    bbs_.setSize(cellLabels_.size());
    FOR_ALL(cellLabels_, i)
    {
      bbs_[i] = calcCellBb(cellLabels_[i]);
    }
  }
}
// Constructors 
mousse::treeDataCell::treeDataCell
(
  const bool cacheBb,
  const polyMesh& mesh,
  const labelUList& cellLabels,
  const polyMesh::cellDecomposition decompMode
)
:
  mesh_(mesh),
  cellLabels_(cellLabels),
  cacheBb_(cacheBb),
  decompMode_(decompMode)
{
  update();
}
mousse::treeDataCell::treeDataCell
(
  const bool cacheBb,
  const polyMesh& mesh,
  const Xfer<labelList>& cellLabels,
  const polyMesh::cellDecomposition decompMode
)
:
  mesh_(mesh),
  cellLabels_(cellLabels),
  cacheBb_(cacheBb),
  decompMode_(decompMode)
{
  update();
}
mousse::treeDataCell::treeDataCell
(
  const bool cacheBb,
  const polyMesh& mesh,
  const polyMesh::cellDecomposition decompMode
)
:
  mesh_(mesh),
  cellLabels_(identity(mesh_.nCells())),
  cacheBb_(cacheBb),
  decompMode_(decompMode)
{
  update();
}
mousse::treeDataCell::findNearestOp::findNearestOp
(
  const indexedOctree<treeDataCell>& tree
)
:
  tree_(tree)
{}
mousse::treeDataCell::findIntersectOp::findIntersectOp
(
  const indexedOctree<treeDataCell>& tree
)
:
  tree_(tree)
{}
// Member Functions 
mousse::pointField mousse::treeDataCell::shapePoints() const
{
  pointField cc(cellLabels_.size());
  FOR_ALL(cellLabels_, i)
  {
    cc[i] = mesh_.cellCentres()[cellLabels_[i]];
  }
  return cc;
}
bool mousse::treeDataCell::overlaps
(
  const label index,
  const treeBoundBox& cubeBb
) const
{
  if (cacheBb_)
  {
    return cubeBb.overlaps(bbs_[index]);
  }
  else
  {
    return cubeBb.overlaps(calcCellBb(cellLabels_[index]));
  }
}
bool mousse::treeDataCell::contains
(
  const label index,
  const point& sample
) const
{
  return mesh_.pointInCell(sample, cellLabels_[index], decompMode_);
}
void mousse::treeDataCell::findNearestOp::operator()
(
  const labelUList& indices,
  const point& sample,
  scalar& nearestDistSqr,
  label& minIndex,
  point& nearestPoint
) const
{
  const treeDataCell& shape = tree_.shapes();
  FOR_ALL(indices, i)
  {
    label index = indices[i];
    label cellI = shape.cellLabels()[index];
    scalar distSqr = magSqr(sample - shape.mesh().cellCentres()[cellI]);
    if (distSqr < nearestDistSqr)
    {
      nearestDistSqr = distSqr;
      minIndex = index;
      nearestPoint = shape.mesh().cellCentres()[cellI];
    }
  }
}
void mousse::treeDataCell::findNearestOp::operator()
(
  const labelUList& /*indices*/,
  const linePointRef& /*ln*/,
  treeBoundBox& /*tightest*/,
  label& /*minIndex*/,
  point& /*linePoint*/,
  point& /*nearestPoint*/
) const
{
  NOT_IMPLEMENTED
  (
    "treeDataCell::findNearestOp::operator()"
    "("
    "    const labelUList&,"
    "    const linePointRef&,"
    "    treeBoundBox&,"
    "    label&,"
    "    point&,"
    "    point&"
    ") const"
  );
}
bool mousse::treeDataCell::findIntersectOp::operator()
(
  const label index,
  const point& start,
  const point& end,
  point& intersectionPoint
) const
{
  const treeDataCell& shape = tree_.shapes();
  // Do quick rejection test
  if (shape.cacheBb_)
  {
    const treeBoundBox& cellBb = shape.bbs_[index];
    if ((cellBb.posBits(start) & cellBb.posBits(end)) != 0)
    {
      // start and end in same block outside of cellBb.
      return false;
    }
  }
  else
  {
    const treeBoundBox cellBb = shape.calcCellBb(shape.cellLabels_[index]);
    if ((cellBb.posBits(start) & cellBb.posBits(end)) != 0)
    {
      // start and end in same block outside of cellBb.
      return false;
    }
  }
  // Do intersection with all faces of cell
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Disable picking up intersections behind us.
  scalar oldTol = intersection::setPlanarTol(0.0);
  const cell& cFaces = shape.mesh_.cells()[shape.cellLabels_[index]];
  const vector dir(end - start);
  scalar minDistSqr = magSqr(dir);
  bool hasMin = false;
  FOR_ALL(cFaces, i)
  {
    const face& f = shape.mesh_.faces()[cFaces[i]];
    pointHit inter = f.ray
    (
      start,
      dir,
      shape.mesh_.points(),
      intersection::HALF_RAY
    );
    if (inter.hit() && sqr(inter.distance()) <= minDistSqr)
    {
      // Note: no extra test on whether intersection is in front of us
      // since using half_ray AND zero tolerance. (note that tolerance
      // is used to look behind us)
      minDistSqr = sqr(inter.distance());
      intersectionPoint = inter.hitPoint();
      hasMin = true;
    }
  }
  // Restore picking tolerance
  intersection::setPlanarTol(oldTol);
  return hasMin;
}
