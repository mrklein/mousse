#ifndef MESH_TOOLS_SURFACE_SETS_SURFACE_SETS_HPP_
#define MESH_TOOLS_SURFACE_SETS_SURFACE_SETS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceSets
// Description
//   Various utilities to handle sets relating mesh to surface.
//   Note: work in progress. Used in meshing tools.
// SourceFiles
//   surface_sets.cpp
#include "label.hpp"
#include "scalar.hpp"
#include "file_name.hpp"
#include "point_field.hpp"
#include "poly_mesh.hpp"
#include "tri_surface.hpp"
#include "tri_surface_search.hpp"
#include "point_set.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class triSurface;
class triSurfaceSearch;
class pointSet;
class surfaceSets
{
  //- Visibility of points: from inside cells, from outside cells or
  //  from both
  enum pointStatus
  {
    NOTSET,
    INSIDE,
    MIXED,
    OUTSIDE
  };
  // Static
    ////- Min length of connected mesh edges
    //static scalar minEdgeLen
    //(
    //    const primitiveMesh& mesh,
    //    const label pointI
    //);
    //
    ////- Returns true if cell uses at least one selected point
    //static bool usesPoint
    //(
    //    const primitiveMesh& mesh,
    //    const boolList& selectedPoint,
    //    const label cellI
    //);
    ////- Find points used only by internalCells and remove from
    ////  internalCells all
    ////  cells which don't use any internal point. Return number of
    ////  removed cells.
    //static label removeHangingCells
    //(
    //    const primitiveMesh&,
    //    const triSurfaceSearch& querySurf,
    //    labelHashSet& internalCells
    //);
    ////- Select all points out of pointSet where the distance to
    ////  the surface is less than a factor times a local length
    ////  scale (minimum length of connected edges)
    //static void getNearPoints
    //(
    //    const primitiveMesh& mesh,
    //    const triSurface& surf,
    //    const triSurfaceSearch& querySurf,
    //    const scalar edgeFactor,
    //    const pointSet& candidateSet,
    //    pointSet& nearPointSet
    //);
public:
  // Static Functions
    //- Divide cells into cut,inside and outside
    //  nCutLayers>0 : remove cutCells (set to type inside) if further
    //  than nCutLayers away from outside type cell.
    static void getSurfaceSets
    (
      const polyMesh& mesh,
      const fileName& surfName,
      const triSurface& surf,
      const triSurfaceSearch& querySurf,
      const pointField& outsidePts,
      const label nCutLayers,
      labelHashSet& inside,
      labelHashSet& outside,
      labelHashSet& cut
    );
    //- Get cells using points on 'outside' only
    static labelHashSet getHangingCells
    (
      const primitiveMesh& mesh,
      const labelHashSet& internalCells
    );
//        //- Write cell sets with cells 'inside' and 'outside' surface.
//        static void writeSurfaceSets
//        (
//            const polyMesh& mesh,
//            const fileName& surfName,
//            const triSurface& surf,
//            const triSurfaceSearch& querySurf,
//            const pointField& outsidePts,
//            const scalar edgeFactor
//        );
};
}  // namespace mousse
#endif
