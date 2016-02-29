#ifndef MESH_BLOCK_MESH_BLOCK_DESCRIPTOR_BLOCK_DESCRIPTOR_HPP_
#define MESH_BLOCK_MESH_BLOCK_DESCRIPTOR_BLOCK_DESCRIPTOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::blockDescriptor
// Description
//   Takes the description of the block and the list of curved edges and
//   creates a list of points on edges together with the weighting factors
// SourceFiles
//   block_descriptor.cpp
//   block_descriptor_edges.cpp
#include "cell_shape.hpp"
#include "point_field.hpp"
#include "scalar_list.hpp"
#include "curved_edge_list.hpp"
#include "grading_descriptors.hpp"
namespace mousse
{
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class blockDescriptor;
Ostream& operator<<(Ostream&, const blockDescriptor&);
class blockDescriptor
{
  // Private data
    //- Reference to point field defining the block mesh
    const pointField& blockPointField_;
    //- Reference to a list of curved edges
    const curvedEdgeList& curvedEdges_;
    //- Block shape
    cellShape blockShape_;
    //- The number of cells in the i,j,k directions
    Vector<label> meshDensity_;
    //- Block edge points
    List< List<point> > edgePoints_;
    //- Block edge weighting factors
    scalarListList edgeWeights_;
    //- Expansion ratios in all directions
    List<gradingDescriptors> expand_;
    //- Name of the zone (empty string if none)
    word zoneName_;
  // Private Member Functions
    //- Set the points/weights for all edges
    void makeBlockEdges();
    //- Set the edge points/weights
    void setEdge(label edgeI, label start, label end, label dim);
public:
  // Constructors
    //- Construct from components. Optional cellSet/zone name.
    blockDescriptor
    (
      const cellShape&,
      const pointField& blockPointField,
      const curvedEdgeList&,
      const Vector<label>& meshDensity,
      const UList<gradingDescriptors>& expand,
      const word& zoneName = ""
    );
    //- Construct from Istream
    blockDescriptor
    (
      const pointField& blockPointField,
      const curvedEdgeList&,
      Istream&
    );
    //- Clone
    autoPtr<blockDescriptor> clone() const
    {
      NOT_IMPLEMENTED("blockDescriptor::clone()");
      return autoPtr<blockDescriptor>{NULL};
    }
    //- Disallow default bitwise assignment
    blockDescriptor& operator=(const blockDescriptor&) = delete;
  //- Destructor
  ~blockDescriptor();
  // Member Functions
    // Access
    //- Return the number of cells in the i,j,k directions
    const Vector<label>& density() const
    {
      return meshDensity_;
    }
    //- Reference to point field defining the block mesh
    const pointField& blockPointField() const;
    //- Return the block shape
    const cellShape& blockShape() const;
    //- Return the block points along each edge
    const List< List<point> >& blockEdgePoints() const;
    //- Return the weightings along each edge
    const scalarListList& blockEdgeWeights() const;
    //- Return the mesh density (number of cells) in the i,j,k directions
    const Vector<label>& meshDensity() const;
    //- Return the (optional) zone name
    const word& zoneName() const;
    //- Return the number of points
    label nPoints() const;
    //- Return the number of cells
    label nCells() const;
    //- Return block point at local label i
    const point& blockPoint(const label i) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const blockDescriptor&);
};
}  // namespace mousse
#endif
