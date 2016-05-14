// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_shape_control.hpp"
#include "point_field.hpp"
#include "scalar_field.hpp"
#include "triad_field.hpp"
#include "cell_size_and_alignment_control.hpp"
#include "searchable_surface_control.hpp"
#include "cell_size_function.hpp"
#include "indexed_vertex_ops.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cellShapeControl, 0);

}


// Private Member Functions 
// Constructors 
mousse::cellShapeControl::cellShapeControl
(
  const Time& runTime,
  const cvControls& foamyHexMeshControls,
  const searchableSurfaces& /*allGeometry*/,
  const conformationSurfaces& geometryToConformTo
)
:
  dictionary
  {
    foamyHexMeshControls.foamyHexMeshDict().subDict("motionControl")
  },
  // runTime_(runTime),
  // allGeometry_(allGeometry),
  geometryToConformTo_{geometryToConformTo},
  defaultCellSize_{foamyHexMeshControls.defaultCellSize()},
  minimumCellSize_{foamyHexMeshControls.minimumCellSize()},
  shapeControlMesh_{runTime},
  aspectRatio_{*this},
  sizeAndAlignment_
  {
    runTime,
    subDict("shapeControlFunctions"),
    geometryToConformTo_,
    defaultCellSize_
  }
{}


// Destructor 
mousse::cellShapeControl::~cellShapeControl()
{}


// Member Functions 
mousse::scalarField mousse::cellShapeControl::cellSize
(
  const pointField& pts
) const
{
  scalarField cellSizes{pts.size()};
  FOR_ALL(pts, i) {
    cellSizes[i] = cellSize(pts[i]);
  }
  return cellSizes;
}


mousse::scalar mousse::cellShapeControl::cellSize(const point& pt) const
{
  scalarList bary;
  cellShapeControlMesh::Cell_handle ch;
  shapeControlMesh_.barycentricCoords(pt, bary, ch);
  scalar size = 0;
  if (shapeControlMesh_.dimension() < 3) {
    size = sizeAndAlignment_.cellSize(pt);
  } else if (shapeControlMesh_.is_infinite(ch)) {
    // Find nearest surface. This can be quite slow if there are a lot of
    // surfaces
    size = sizeAndAlignment_.cellSize(pt);
  } else {
    label nFarPoints = 0;
    for (label pI = 0; pI < 4; ++pI) {
      if (ch->vertex(pI)->farPoint()) {
        nFarPoints++;
      }
    }
    if (nFarPoints != 0) {
      for (label pI = 0; pI < 4; ++pI) {
        if (!CGAL::indexedVertexOps::uninitialised(ch->vertex(pI))) {
          size = ch->vertex(pI)->targetCellSize();
          return size;
        }
      }
    } else {
      FOR_ALL(bary, pI) {
        size += bary[pI]*ch->vertex(pI)->targetCellSize();
      }
    }
  }
  return size;
}


//- Return the cell alignment at the given location
mousse::tensor mousse::cellShapeControl::cellAlignment(const point& pt) const
{
  scalarList bary;
  cellShapeControlMesh::Cell_handle ch;
  shapeControlMesh_.barycentricCoords(pt, bary, ch);
  tensor alignment = tensor::zero;
  if (shapeControlMesh_.dimension() < 3 || shapeControlMesh_.is_infinite(ch)) {
    alignment = tensor::I;
  } else {
    label nFarPoints = 0;
    for (label pI = 0; pI < 4; ++pI) {
      if (ch->vertex(pI)->farPoint()) {
        nFarPoints++;
      }
    }

    {
      triad tri;
      for (label pI = 0; pI < 4; ++pI) {
        if (bary[pI] > SMALL) {
          tri += triad(bary[pI]*ch->vertex(pI)->alignment());
        }
      }
      tri.normalize();
      tri.orthogonalize();
      tri = tri.sortxyz();
      alignment = tri;
    }
  }
  return alignment;
}


void mousse::cellShapeControl::cellSizeAndAlignment
(
  const point& pt,
  scalar& size,
  tensor& alignment
) const
{
  scalarList bary;
  cellShapeControlMesh::Cell_handle ch;
  shapeControlMesh_.barycentricCoords(pt, bary, ch);
  alignment = tensor::zero;
  size = 0;
  if (shapeControlMesh_.dimension() < 3 || shapeControlMesh_.is_infinite(ch)) {
    // Find nearest surface
    size = sizeAndAlignment_.cellSize(pt);
    alignment = tensor::I;
  } else {
    label nFarPoints = 0;
    for (label pI = 0; pI < 4; ++pI) {
      if (ch->vertex(pI)->farPoint()) {
        nFarPoints++;
      }
    }
    if (nFarPoints != 0) {
      for (label pI = 0; pI < 4; ++pI) {
        if (!CGAL::indexedVertexOps::uninitialised(ch->vertex(pI))) {
          size = ch->vertex(pI)->targetCellSize();
          alignment = ch->vertex(pI)->alignment();
        }
      }
    } else {
      triad tri;
      for (label pI = 0; pI < 4; ++pI) {
        size += bary[pI]*ch->vertex(pI)->targetCellSize();
        if (bary[pI] > SMALL) {
          tri += triad(bary[pI]*ch->vertex(pI)->alignment());
        }
      }
      tri.normalize();
      tri.orthogonalize();
      tri = tri.sortxyz();
      alignment = tri;
    }
  }
  for (label dir = 0; dir < 3; dir++) {
    triad v = alignment;
    if (!v.set(dir) || size == 0) {
      // Force orthogonalization of triad.
      scalar dotProd = GREAT;
      if (dir == 0) {
        dotProd = v[1] & v[2];
        v[dir] = v[1] ^ v[2];
      }
      if (dir == 1) {
        dotProd = v[0] & v[2];
        v[dir] = v[0] ^ v[2];
      }
      if (dir == 2) {
        dotProd = v[0] & v[1];
        v[dir] = v[0] ^ v[1];
      }
      v.normalize();
      v.orthogonalize();
      Pout << "Dot prod = " << dotProd << endl;
      Pout << "Alignment = " << v << endl;
      alignment = v;
    }
  }
}

