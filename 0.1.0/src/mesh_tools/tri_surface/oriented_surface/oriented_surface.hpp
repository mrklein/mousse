#ifndef MESH_TOOLS_TRI_SURFACE_ORIENTED_SURFACE_ORIENTED_SURFACE_HPP_
#define MESH_TOOLS_TRI_SURFACE_ORIENTED_SURFACE_ORIENTED_SURFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::orientedSurface
// Description
//   Given point flip all faces such that normals point in same direction.
// SourceFiles
//   oriented_surface.cpp


#include "tri_surface.hpp"
#include "type_info.hpp"

namespace mousse
{
// Forward declaration of classes
class triSurfaceSearch;
class orientedSurface
:
  public triSurface
{
public:
  // Data types
    //- Enumeration listing whether face needs to be flipped.
    enum sideStat
    {
      UNVISITED,
      FLIP,
      NOFLIP
    };
private:
  // Private Member Functions
    //- Return true if edge is used in opposite order in faces
    static bool consistentEdge
    (
      const edge& e,
      const triSurface::FaceType& f0,
      const triSurface::FaceType& f1
    );
    //- From changed faces get the changed edges
    static labelList faceToEdge
    (
      const triSurface&,
      const labelList& changedFaces
    );
    //- From changed edges check the orientation of the connected faces
    //  and flip them. Return changed faces.
    static labelList edgeToFace
    (
      const triSurface&,
      const labelList& changedEdges,
      labelList& flip
    );
    //- Walk from face across connected faces. Change orientation to be
    //  consistent with startFaceI.
    static void walkSurface
    (
      const triSurface& s,
      const label startFaceI,
      labelList& flipState
    );
    //- Given nearest point and face check orientation to nearest face
    //  and flip if necessary (only marked in flipState) and propagate.
    static void propagateOrientation
    (
      const triSurface&,
      const point& outsidePoint,
      const bool orientOutside,
      const label nearestFaceI,
      const point& nearestPt,
      labelList& flipState
    );
    //- Find a face on zoneI and count number of intersections to determine
    //  orientation
    static void findZoneSide
    (
      const triSurfaceSearch& surfSearches,
      const labelList& faceZone,
      const label zoneI,
      const point& visiblePoint,
      label& zoneFaceI,
      bool& isOutside
    );
    //- Given flipState reverse triangles of *this. Return true if
    //  anything flipped.
    static bool flipSurface(triSurface& s, const labelList& flipState);
    //- Make surface surface has consistent orientation across connected
    //  triangles.
    static bool orientConsistent(triSurface& s);
public:
  CLASS_NAME("orientedSurface");
  // Constructors
    //- Construct null
    orientedSurface();
    //- Construct from triSurface and sample point which is either
    //  outside (orientOutside = true) or inside (orientOutside = false).
    //  Uses orient.
    orientedSurface
    (
      const triSurface&,
      const point& samplePoint,
      const bool orientOutside = true
    );
    //- Construct from triSurface. Calculates outside point as being
    //  outside the bounding box of the surface.
    orientedSurface(const triSurface&, const bool orientOutside = true);
  // Member Functions
    //- Flip faces such that normals are consistent with point:
    //  orientOutside=true : point outside surface
    //  orientOutside=false : point inside surface
    //  Bases orientation on normal on nearest point (linear search) and
    //  walks to rest. Surface needs to be manifold.
    static bool orient(triSurface&, const point&, const bool orientOutside);
    //- Flip faces such that normals are consistent with point:
    //  orientOutside=true : point outside surface
    //  orientOutside=false : point inside surface
    //  Uses intersection count to orient. Handles open surfaces.
    static bool orient
    (
      triSurface& s,
      const triSurfaceSearch& querySurf,
      const point& samplePoint,
      const bool orientOutside
    );
};
}  // namespace mousse
#endif
