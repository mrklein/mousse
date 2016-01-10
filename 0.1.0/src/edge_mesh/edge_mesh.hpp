// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::edgeMesh
// Description
//   Points connected by edges.
//   Can be read from fileName based on extension. Uses ::New factory method
//   to select the reader and transfer the result.
// SourceFiles
//   edge_mesh_i.hpp
//   edge_mesh.cpp
//   edge_mesh_io.cpp
//   edge_mesh_new.cpp
#ifndef edge_mesh_hpp_
#define edge_mesh_hpp_
#include "point_field.hpp"
#include "edge_list.hpp"
#include "edge_mesh_formats_core.hpp"
#include "run_time_selection_tables.hpp"
#include "member_function_selection_tables.hpp"
#include "hash_set.hpp"
namespace mousse
{
// Forward declaration of classes
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class edgeMesh;
Istream& operator>>(Istream&, edgeMesh&);
Ostream& operator<<(Ostream&, const edgeMesh&);
class edgeMesh
:
  public fileFormats::edgeMeshFormatsCore
{
  // Private data
    //- Vertices of the edges
    pointField points_;
    //- The edges defining the boundary
    edgeList edges_;
    //- From point to edges
    mutable autoPtr<labelListList> pointEdgesPtr_;
  // Private Member Functions
    //- Calculate point-edge addressing (inverse of edges)
    void calcPointEdges() const;
protected:
  // Protected Member Functions
    //- Non-const access to global points
    inline pointField& storedPoints();
    //- Non-const access to the edges
    inline edgeList& storedEdges();
public:
    //- Runtime type information
    TYPE_NAME("edgeMesh");
  // Static
    //- Can we read this file format?
    static bool canRead(const fileName&, const bool verbose=false);
    //- Can we read this file format?
    static bool canReadType(const word& ext, const bool verbose=false);
    //- Can we write this file format type?
    static bool canWriteType(const word& ext, const bool verbose=false);
    static wordHashSet readTypes();
    static wordHashSet writeTypes();
  // Constructors
    //- Construct null
    edgeMesh();
    //- Construct from components
    edgeMesh(const pointField&, const edgeList&);
    //- Construct by transferring components (points, edges).
    edgeMesh
    (
      const Xfer<pointField>&,
      const Xfer<edgeList>&
    );
    //- Construct as copy
    edgeMesh(const edgeMesh&);
    //- Construct from file name (uses extension to determine type)
    edgeMesh(const fileName&);
    //- Construct from file name (uses extension to determine type)
    edgeMesh(const fileName&, const word& ext);
    //- Construct from Istream
    edgeMesh(Istream&);
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      edgeMesh,
      fileExtension,
      (
        const fileName& name
      ),
      (name)
    );
  // Selectors
    //- Select constructed from filename (explicit extension)
    static autoPtr<edgeMesh> New
    (
      const fileName&,
      const word& ext
    );
    //- Select constructed from filename (implicit extension)
    static autoPtr<edgeMesh> New(const fileName&);
  //- Destructor
  virtual ~edgeMesh();
  // Member Function Selectors
    DECLARE_MEMBER_FUNCTION_SELECTION_TABLE
    (
      void,
      edgeMesh,
      write,
      fileExtension,
      (
        const fileName& name,
        const edgeMesh& mesh
      ),
      (name, mesh)
    );
    //- Write to file
    static void write(const fileName&, const edgeMesh&);
  // Member Functions
    //- Transfer the contents of the argument and annul the argument
    void transfer(edgeMesh&);
    //- Transfer contents to the Xfer container
    Xfer<edgeMesh > xfer();
  // Read
    //- Read from file. Chooses reader based on explicit extension
    bool read(const fileName&, const word& ext);
    //- Read from file. Chooses reader based on detected extension
    virtual bool read(const fileName&);
  // Access
    //- Return points
    inline const pointField& points() const;
    //- Return edges
    inline const edgeList& edges() const;
    //- Return edges
    inline const labelListList& pointEdges() const;
    //- Find connected regions. Set region number per edge.
    //  Returns number of regions.
    label regions(labelList& edgeRegion) const;
  // Edit
    //- Clear all storage
    virtual void clear();
    //- Reset primitive data (points, edges)
    //  Note, optimized to avoid overwriting data (with Xfer::null)
    virtual void reset
    (
      const Xfer<pointField>& points,
      const Xfer<edgeList>& edges
    );
    //- Scale points. A non-positive factor is ignored
    virtual void scalePoints(const scalar);
    //- Merge common points (points within mergeDist)
    void mergePoints(const scalar mergeDist);
    //- Merge similar edges
    void mergeEdges();
  // Write
    virtual void writeStats(Ostream&) const;
    //- Generic write routine. Chooses writer based on extension.
    virtual void write(const fileName& name) const
    {
      write(name, *this);
    }
  // Member Operators
    inline void operator=(const edgeMesh&);
    // Ostream Operator
      friend Ostream& operator<<(Ostream&, const edgeMesh&);
      friend Istream& operator>>(Istream&, edgeMesh&);
};
}  // namespace mousse
#include "edge_mesh_i.hpp"
#endif
