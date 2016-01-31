// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DistributedDelaunayMesh
// Description
// SourceFiles
//   distributed_delaunay_mesh_i.hpp
//   distributed_delaunay_mesh.cpp
//   distributed_delaunay_mesh_io.cpp
#ifndef distributed_delaunay_mesh_hpp_
#define distributed_delaunay_mesh_hpp_
#include "delaunay_mesh.hpp"
#include "background_mesh_decomposition.hpp"
#include "auto_ptr.hpp"
#include "bound_box.hpp"
#include "indexed_vertex.hpp"
namespace mousse
{
class mapDistribute;
template<class Triangulation>
class DistributedDelaunayMesh
:
  public DelaunayMesh<Triangulation>
{
public:
    typedef typename Triangulation::Vertex_handle Vertex_handle;
    typedef typename Triangulation::Cell_handle   Cell_handle;
    typedef typename Triangulation::Point         Point;
    typedef typename Triangulation::Finite_vertices_iterator
      Finite_vertices_iterator;
    typedef typename Triangulation::Finite_cells_iterator
      Finite_cells_iterator;
    typedef typename Triangulation::All_cells_iterator
      All_cells_iterator;
    typedef typename DelaunayMesh<Triangulation>::labelPairHashSet
      labelPairHashSet;
private:
    autoPtr<List<boundBox> > allBackgroundMeshBounds_;
  // Private Member Functions
    //-
    bool distributeBoundBoxes(const boundBox& bb);
    //-
    bool isLocal(const Vertex_handle& v) const;
    bool isLocal(const label localProcIndex) const;
    labelList overlapProcessors
    (
      const point& centre,
      const scalar radiusSqr
    ) const;
    bool checkProcBoundaryCell
    (
      const Cell_handle& cit,
      Map<labelList>& circumsphereOverlaps
    ) const;
    void findProcessorBoundaryCells
    (
      Map<labelList>& circumsphereOverlaps
    ) const;
    void markVerticesToRefer
    (
      const Map<labelList>& circumsphereOverlaps,
      PtrList<labelPairHashSet>& referralVertices,
      DynamicList<label>& targetProcessor,
      DynamicList<Vb>& parallelInfluenceVertices
    );
    label referVertices
    (
      const DynamicList<label>& targetProcessor,
      DynamicList<Vb>& parallelVertices,
      PtrList<labelPairHashSet>& referralVertices,
      labelPairHashSet& receivedVertices
    );
public:
  // Constructors
    //- Construct from components
    explicit DistributedDelaunayMesh(const Time& runTime);
    DistributedDelaunayMesh
    (
      const Time& runTime,
      const word& meshName
    );
    //- Disallow default bitwise copy construct
    DistributedDelaunayMesh(const DistributedDelaunayMesh<Triangulation>&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const DistributedDelaunayMesh<Triangulation>&) = delete;
  //- Destructor
  ~DistributedDelaunayMesh();
  // Queries
    //- Use DelaunayMesh timeCheck function
    using DelaunayMesh<Triangulation>::timeCheck;
    scalar calculateLoadUnbalance() const;
  // Member Functions
    //- Build a mapDistribute for the supplied destination processor data
    static autoPtr<mapDistribute> buildMap(const List<label>& toProc);
    //-
    bool distribute(const boundBox& bb);
    autoPtr<mapDistribute> distribute
    (
      const backgroundMeshDecomposition& decomposition,
      List<mousse::point>& points
    );
    //- Refer vertices so that the processor interfaces are consistent
    void sync(const boundBox& bb);
    //- Refer vertices so that the processor interfaces are consistent
    void sync
    (
      const boundBox& bb,
      PtrList<labelPairHashSet>& referralVertices,
      labelPairHashSet& receivedVertices,
      bool iterateReferral = true
    );
    //- Inserts points into the triangulation if the point is within
    //  the circumsphere of another cell. Returns HashSet of failed
    //  point insertions
    template<class PointIterator>
    labelPairHashSet rangeInsertReferredWithInfo
    (
      PointIterator begin,
      PointIterator end,
      bool printErrors = true
    );
//        distributeField();
  // Queries
};
}  // namespace mousse
//#include "distributed_delaunay_mesh_i.hpp"
#ifdef NoRepository
  #include "distributed_delaunay_mesh.cpp"
#endif
#endif
