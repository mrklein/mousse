// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellShapeControlMesh
// Description
// SourceFiles
//   cell_shape_control_mesh.cpp
#ifndef cell_shape_control_mesh_hpp_
#define cell_shape_control_mesh_hpp_
#include "time.hpp"
#include "scalar.hpp"
#include "point.hpp"
#include "tensor.hpp"
#include "triad.hpp"
#include "file_name.hpp"
#include "searchable_surfaces.hpp"
#include "conformation_surfaces.hpp"
#include "distributed_delaunay_mesh.hpp"
#include "cgal_triangulation_3d_defs.hpp"
#include "background_mesh_decomposition.hpp"
#include "bound_box.hpp"
namespace mousse
{
class cellSizeAndAlignmentControls;
class cellShapeControlMesh
:
  public DistributedDelaunayMesh<CellSizeDelaunay>
{
public:
    typedef CellSizeDelaunay::Cell_handle      Cell_handle;
    typedef CellSizeDelaunay::Vertex_handle    Vertex_handle;
    typedef CellSizeDelaunay::Point            Point;
private:
    // Private data
    const Time& runTime_;
    mutable Cell_handle oldCellHandle_;
    // const scalar defaultCellSize_;
public:
  //- Runtime type information
  CLASS_NAME("cellShapeControlMesh");
  //- Return the mesh sub-directory name (usually "cellShapeControlMesh")
  static word meshSubDir;
  // Constructors
    explicit cellShapeControlMesh(const Time& runTime);
    //- Disallow default bitwise copy construct
    cellShapeControlMesh(const cellShapeControlMesh&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const cellShapeControlMesh&) = delete;
  //- Destructor
  ~cellShapeControlMesh();
  // Member Functions
    // Query
      const Time& time() const
      {
        return runTime_;
      }
      //- Calculate and return the barycentric coordinates for
      //  interpolating quantities on the background mesh
      void barycentricCoords
      (
        const mousse::point& pt,
        scalarList& bary,
        Cell_handle& ch
      ) const;
      boundBox bounds() const;
    // Edit
      label removePoints();
      //- Get the centres of all the tets
      tmp<pointField> cellCentres() const;
      inline Vertex_handle insert
      (
        const mousse::point& pt,
        const scalar& size,
        const triad& alignment,
        const mousse::indexedVertexEnum::vertexType type = Vb::vtInternal
      );
      inline Vertex_handle insertFar
      (
        const mousse::point& pt
      );
      void distribute(const backgroundMeshDecomposition& decomposition);
      tensorField dumpAlignments() const;
      void writeTriangulation();
      void write() const;
      label estimateCellCount
      (
        const autoPtr<backgroundMeshDecomposition>& decomposition
      ) const;
};
}  // namespace mousse

// Member Functions 
mousse::cellShapeControlMesh::Vertex_handle mousse::cellShapeControlMesh::insert
(
  const mousse::point& pt,
  const scalar& size,
  const triad& alignment,
  const mousse::indexedVertexEnum::vertexType type
)
{
  Vertex_handle v = CellSizeDelaunay::insert
  (
    Point{pt.x(), pt.y(), pt.z()}
  );
  v->type() = type;
  v->index() = getNewVertexIndex();
  v->procIndex() = Pstream::myProcNo();
  v->targetCellSize() = size;
  v->alignment() = tensor(alignment.x(), alignment.y(), alignment.z());
  return v;
}

mousse::cellShapeControlMesh::Vertex_handle mousse::cellShapeControlMesh::insertFar
(
  const mousse::point& pt
)
{
  Vertex_handle v = CellSizeDelaunay::insert
  (
    Point(pt.x(), pt.y(), pt.z())
  );
  v->type() = Vb::vtFar;
  v->index() = getNewVertexIndex();
  v->procIndex() = Pstream::myProcNo();
  return v;
}

#endif
