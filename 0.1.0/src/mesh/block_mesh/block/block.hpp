// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::block
// Description
//   Creates a single block of cells from point coordinates, numbers of
//   cells in each direction and an expansion ratio.
// Note
//   The vertices and cells for filling the block are demand-driven.
// SourceFiles
//   block.cpp
//   block_create.cpp
#ifndef block_hpp_
#define block_hpp_
#include "point_field.hpp"
#include "label_list.hpp"
#include "block_descriptor.hpp"
namespace mousse
{
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class block;
Ostream& operator<<(Ostream&, const block&);
class block
:
  public blockDescriptor
{
  // Private data
    //- List of vertices
    mutable pointField vertices_;
    //- List of cells
    mutable labelListList cells_;
    //- Boundary patches
    mutable labelListListList boundaryPatches_;
  // Private Member Functions
    //- Creates vertices for cells filling the block
    void createPoints() const;
    //- Creates cells for filling the block
    void createCells() const;
    //- Creates boundary patch faces for the block
    void createBoundary() const;
    //- Disallow default bitwise copy construct
    block(const block&);
    //- Disallow default bitwise assignment
    void operator=(const block&);
public:
  // Constructors
    //- Construct from components with Istream
    block
    (
      const pointField& blockPointField,
      const curvedEdgeList&,
      Istream&
    );
    //- Construct from a block definition
    block(const blockDescriptor&);
    //- Clone
    autoPtr<block> clone() const
    {
      notImplemented("block::clone()");
      return autoPtr<block>(NULL);
    }
  //- Destructor
  ~block();
  // Member Functions
    // Access
      //- Return the block definition
      inline const blockDescriptor& blockDef() const;
      //- Vertex label offset for a particular i,j,k position
      inline label vtxLabel(label i, label j, label k) const;
      //- Return the points for filling the block
      const pointField& points() const;
      //- Return the cells for filling the block
      const labelListList& cells() const;
      //- Return the boundary patch faces for the block
      const labelListListList& boundaryPatches() const;
    // Edit
      //- Clear geometry (internal points, cells, boundaryPatches)
      void clearGeom();
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const block&);
};
}  // namespace mousse
#include "block_i.hpp"
#endif
