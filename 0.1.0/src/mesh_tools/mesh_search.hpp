#ifndef MESH_TOOLS_MESH_SEARCH_MESH_SEARCH_HPP_
#define MESH_TOOLS_MESH_SEARCH_MESH_SEARCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshSearch
// Description
//   Various (local, not parallel) searches on polyMesh;
//   uses (demand driven) octree to search.

#include "point_index_hit.hpp"
#include "point_field.hpp"
#include "poly_mesh.hpp"


namespace mousse {

// Forward declaration of classes
class treeDataCell;
class treeDataFace;
template<class Type> class indexedOctree;
class treeBoundBox;


class meshSearch
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
    //- Whether to use cell decomposition for all geometric tests
    const polyMesh::cellDecomposition cellDecompMode_;
    //- Data bounding box
    mutable autoPtr<treeBoundBox> overallBbPtr_;
    //- Demand driven octrees
    mutable autoPtr<indexedOctree<treeDataFace> > boundaryTreePtr_;
    mutable autoPtr<indexedOctree<treeDataCell> > cellTreePtr_;
  // Private Member Functions
    //- Updates nearestI, nearestDistSqr from any closer ones.
    static bool findNearer
    (
      const point& sample,
      const pointField& points,
      label& nearestI,
      scalar& nearestDistSqr
    );
    //- Updates nearestI, nearestDistSqr from any selected closer ones.
    static bool findNearer
    (
      const point& sample,
      const pointField& points,
      const labelList& indices,
      label& nearestI,
      scalar& nearestDistSqr
    );
    // Cells
      //- Nearest cell centre using octree
      label findNearestCellTree(const point&) const;
      //- Nearest cell centre going through all cells
      label findNearestCellLinear(const point&) const;
      //- Walk from seed. Does not 'go around' boundary, just returns
      //  last cell before boundary.
      label findNearestCellWalk(const point&, const label) const;
      //- Cell containing location. Linear search.
      label findCellLinear(const point&) const;
      //- Walk from seed. Does not 'go around' boundary, just returns
      //  last cell before boundary.
      label findCellWalk(const point&, const label) const;
    // Faces
      label findNearestFaceTree(const point&) const;
      label findNearestFaceLinear(const point&) const;
      label findNearestFaceWalk(const point&, const label) const;
    // Boundary faces
      //- Walk from seed to find nearest boundary face. Gets stuck in
      //  local minimum.
      label findNearestBoundaryFaceWalk
      (
        const point& location,
        const label seedFaceI
      ) const;
      //- Calculate offset vector in direction dir with as length a
      //  fraction of the cell size (of the cell straddling boundary face)
      vector offset
      (
        const point& bPoint,
        const label bFaceI,
        const vector& dir
      ) const;
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("meshSearch");
  // Static data members
    //- Tolerance on linear dimensions
    static scalar tol_;
  // Constructors
    //- Construct from components. Constructs bb slightly bigger than
    //  mesh points bb.
    meshSearch
    (
      const polyMesh& mesh,
      const polyMesh::cellDecomposition = polyMesh::CELL_TETS
    );
    //- Construct with a custom bounding box. Any mesh element outside
    //  bb will not be found. Up to user to make sure bb
    //  extends slightly beyond wanted elements.
    meshSearch
    (
      const polyMesh& mesh,
      const treeBoundBox& bb,
      const polyMesh::cellDecomposition = polyMesh::CELL_TETS
    );
    //- Disallow default bitwise copy construct
    meshSearch(const meshSearch&) = delete;
    //- Disallow default bitwise assignment
    meshSearch& operator=(const meshSearch&) = delete;
  //- Destructor
  ~meshSearch();
  // Member Functions
    // Access
      const polyMesh& mesh() const
      {
        return mesh_;
      }
      polyMesh::cellDecomposition decompMode() const
      {
        return cellDecompMode_;
      }
      //- Get (demand driven) reference to octree holding all
      //  boundary faces
      const indexedOctree<treeDataFace>& boundaryTree() const;
      //- Get (demand driven) reference to octree holding all cells
      const indexedOctree<treeDataCell>& cellTree() const;
    // Queries
      //- Find nearest cell in terms of cell centre.
      //  Options:
      //  - use octree
      //  - use linear search
      //  - if seed is provided walk. (uses findNearestCellWalk;
      //    does not handle holes in domain)
      label findNearestCell
      (
        const point& location,
        const label seedCellI = -1,
        const bool useTreeSearch = true
      ) const;
      label findNearestFace
      (
        const point& location,
        const label seedFaceI = -1,
        const bool useTreeSearch = true
      ) const;
      //- Find cell containing location.
      //  If seed provided walks and falls back to linear/tree search.
      //  (so handles holes correctly)s
      //  Returns -1 if not in domain.
      label findCell
      (
        const point& location,
        const label seedCellI = -1,
        const bool useTreeSearch = true
      ) const;
      //- Find nearest boundary face
      //  If seed provided walks but then does not pass local minima
      //  in distance. Also does not jump from one connected region to
      //  the next.
      label findNearestBoundaryFace
      (
        const point& location,
        const label seedFaceI = -1,
        const bool useTreeSearch = true
      ) const;
      //- Find first intersection of boundary in segment [pStart, pEnd]
      //  (so inclusive of endpoints). Always octree for now
      pointIndexHit intersection(const point& pStart, const point& pEnd)
      const;
      //- Find all intersections of boundary within segment pStart .. pEnd
      //  Always octree for now
      List<pointIndexHit> intersections
      (
        const point& pStart,
        const point& pEnd
      ) const;
      //- Determine inside/outside status
      bool isInside(const point&) const;
    //- Delete all storage
    void clearOut();
    //- Correct for mesh geom/topo changes
    void correct();
};

}  // namespace mousse
#endif
