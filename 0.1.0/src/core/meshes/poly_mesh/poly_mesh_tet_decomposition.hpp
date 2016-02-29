#ifndef CORE_MESHES_POLY_MESH_POLY_MESH_TET_DECOMPOSITION_HPP_
#define CORE_MESHES_POLY_MESH_POLY_MESH_TET_DECOMPOSITION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyMeshTetDecomposition
// Description
//   Tools for performing the minimum decomposition of faces of the
//   mesh into triangles so that the cells may be tet decomposed.
//   Includes functions for finding variable face starting (base)
//   points on each face to avoid the decomposition of cells into tets
//   that have negative or zero volume.
// SourceFiles
//   poly_mesh_tet_decomposition.cpp
#include "poly_mesh.hpp"
#include "coupled_poly_patch.hpp"
#include "sync_tools.hpp"
#include "tetrahedron.hpp"
#include "tet_indices.hpp"
namespace mousse
{
class polyMeshTetDecomposition
{
public:
  // Static data members
    //- Minimum tetrahedron quality
    static const scalar minTetQuality;
  // Member Functions
    //- Find the first suitable base point to use for a minimum
    //  triangle decomposition of the face, suiting owner and
    //  neighbour cells.  Finds the first base point on the face
    //  whose worst quality tet from either cell is better than
    //  tolerance.  Neighbour cell centre supplied.  For coupled
    //  patches.
    static label findSharedBasePoint
    (
      const polyMesh& mesh,
      label fI,
      const point& nCc,
      scalar tol,
      bool report = false
    );
    //- As for findSharedBasePoint, but using neighbour cell
    //  centre from the mesh.  For internal faces.
    static label findSharedBasePoint
    (
      const polyMesh& mesh,
      label fI,
      scalar tol,
      bool report = false
    );
    //- Find the base point to use for a minimum triangle
    //  decomposition of the face, using only the owner
    //  information.  For non-coupled boundary faces.
    static label findBasePoint
    (
      const polyMesh& mesh,
      label fI,
      scalar tol,
      bool report = false
    );
    //- Find a suitable base point for each face for decomposition
    //  into tets
    static labelList findFaceBasePts
    (
      const polyMesh& mesh,
      scalar tol = minTetQuality,
      bool report = false
    );
    //- Check face-decomposition tet volume
    static bool checkFaceTets
    (
      const polyMesh& mesh,
      scalar tol = minTetQuality,
      const bool report = false,
      labelHashSet* setPtr = NULL
    );
    //- Return the tet decomposition of the given face, with
    //  respect to the given cell
    static List<tetIndices> faceTetIndices
    (
      const polyMesh& mesh,
      label fI,
      label cI
    );
    //- Return the tet decomposition of the given triangle of the given face
    static tetIndices triangleTetIndices
    (
      const polyMesh& mesh,
      label fI,
      label cI,
      const label tetPtI      // offset in face
    );
    //- Return the tet decomposition of the given cell, see
    //  findFacePt for the meaning of the indices
    static List<tetIndices> cellTetIndices
    (
      const polyMesh& mesh,
      label cI
    );
    //- Find the tet decomposition of the cell containing the given point
    static tetIndices findTet
    (
      const polyMesh& mesh,
      label cI,
      const point& pt
    );
};
}  // namespace mousse
#endif
