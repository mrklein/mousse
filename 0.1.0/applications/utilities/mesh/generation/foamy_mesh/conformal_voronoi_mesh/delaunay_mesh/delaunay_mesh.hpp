// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DelaunayMesh
// Description
//   The vertex and cell classes must have an index defined
// SourceFiles
//   delaunay_mesh_i.hpp
//   delaunay_mesh.cpp
//   delaunay_mesh_io.cpp
#ifndef delaunay_mesh_hpp_
#define delaunay_mesh_hpp_
#include "pair.hpp"
#include "hash_set.hpp"
#include "fixed_list.hpp"
#include "bound_box.hpp"
#include "indexed_vertex.hpp"
#include "cgal_triangulation3_ddefs.hpp"
#include "time.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class fvMesh;
template<class Triangulation>
class DelaunayMesh
:
  public Triangulation
{
public:
    typedef typename Triangulation::Cell_handle Cell_handle;
    typedef typename Triangulation::Vertex_handle Vertex_handle;
    typedef typename Triangulation::Edge Edge;
    typedef typename Triangulation::Point Point;
    typedef typename Triangulation::Facet Facet;
    typedef typename Triangulation::Finite_vertices_iterator
      Finite_vertices_iterator;
    typedef typename Triangulation::Finite_cells_iterator
      Finite_cells_iterator;
    typedef typename Triangulation::Finite_facets_iterator
      Finite_facets_iterator;
    typedef HashSet
    <
     Pair<label>,
     FixedList<label, 2>::Hash<>
    > labelPairHashSet;
    typedef HashTable
    <
      label,
      labelPair,
      FixedList<label, 2>::Hash<>
    > labelTolabelPairHashTable;
private:
  // Private data
    //- Keep track of the number of vertices that have been added.
    //  This allows a unique index to be assigned to each vertex.
    mutable label vertexCount_;
    //- Keep track of the number of cells that have been added.
    //  This allows a unique index to be assigned to each cell.
    mutable label cellCount_;
    //- Reference to Time
    const Time& runTime_;
    //- Spatial sort traits to use with a pair of point pointers and an int.
    //  Taken from a post on the CGAL lists: 2010-01/msg00004.html by
    //  Sebastien Loriot (Geometry Factory).
    struct Traits_for_spatial_sort
    :
      public Triangulation::Geom_traits
    {
      typedef typename Triangulation::Geom_traits Gt;
      typedef std::pair<const typename Triangulation::Point*, label>
        Point_3;
      struct Less_x_3
      {
        bool operator()(const Point_3& p, const Point_3& q) const;
      };
      struct Less_y_3
      {
        bool operator()(const Point_3& p, const Point_3& q) const;
      };
      struct Less_z_3
      {
        bool operator()(const Point_3& p, const Point_3& q) const;
      };
      Less_x_3 less_x_3_object() const;
      Less_y_3 less_y_3_object() const;
      Less_z_3 less_z_3_object() const;
    };
  // Private Member Functions
    void sortFaces
    (
      faceList& faces,
      labelList& owner,
      labelList& neighbour
    ) const;
    void addPatches
    (
      const label nInternalFaces,
      faceList& faces,
      labelList& owner,
      PtrList<dictionary>& patchDicts,
      const List<DynamicList<face> >& patchFaces,
      const List<DynamicList<label> >& patchOwners
    ) const;
    //- Disallow default bitwise copy construct
    DelaunayMesh(const DelaunayMesh<Triangulation>&);
    //- Disallow default bitwise assignment
    void operator=(const DelaunayMesh<Triangulation>&);
public:
  // Constructors
    //- Construct from components
    explicit DelaunayMesh(const Time& runTime);
    DelaunayMesh
    (
      const Time& runTime,
      const word& meshName
    );
  //- Destructor
  ~DelaunayMesh();
  // Member Functions
    // Access
      //- Return a reference to the Time object
      inline const Time& time() const;
    // Check
      //- Write the cpuTime to screen
      inline void timeCheck
      (
        const string& description,
        const bool check = true
      ) const;
    // Indexing functions
      //- Create a new unique cell index and return
      inline label getNewCellIndex() const;
      //- Create a new unique vertex index and return
      inline label getNewVertexIndex() const;
      //- Return the cell count (the next unique cell index)
      inline label cellCount() const;
      //- Return the vertex count (the next unique vertex index)
      inline label vertexCount() const;
      //- Set the cell count to zero
      inline void resetCellCount();
      //- Set the vertex count to zero
      inline void resetVertexCount();
    // Triangulation manipulation functions
      //- Clear the entire triangulation
      void reset();
      //- Insert the list of vertices (calls rangeInsertWithInfo)
      Map<label> insertPoints
      (
        const List<Vb>& vertices,
        const bool reIndex
      );
      //- Function inserting points into a triangulation and setting the
      //  index and type data of the point in the correct order. This is
      //  faster than inserting points individually.
      //
      //  Adapted from a post on the CGAL lists: 2010-01/msg00004.html by
      //  Sebastien Loriot (Geometry Factory).
      template<class PointIterator>
      Map<label> rangeInsertWithInfo
      (
        PointIterator begin,
        PointIterator end,
        bool printErrors = false,
        bool reIndex = true
      );
    // Write
      //- Write mesh statistics to stream
      void printInfo(Ostream& os) const;
      //- Write vertex statistics in the form of a table to stream
      void printVertexInfo(Ostream& os) const;
      //- Create an fvMesh from the triangulation.
      //  The mesh is not parallel consistent - only used for viewing
      autoPtr<polyMesh> createMesh
      (
        const fileName& name,
        labelTolabelPairHashTable& vertexMap,
        labelList& cellMap,
        const bool writeDelaunayData = true
      ) const;
};
}  // namespace mousse
#include "delaunay_mesh_i.hpp"
#ifdef NoRepository
  #include "delaunay_mesh.cpp"
#endif
#endif
