// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "block.hpp"
// Constructors 
mousse::block::block
(
  const pointField& blockPointField,
  const curvedEdgeList& edges,
  Istream& is
)
:
  blockDescriptor(blockPointField, edges, is),
  vertices_(0),
  cells_(0),
  boundaryPatches_(0)
{}
mousse::block::block(const blockDescriptor& blockDesc)
:
  blockDescriptor(blockDesc),
  vertices_(0),
  cells_(0),
  boundaryPatches_(0)
{}
// Destructor 
mousse::block::~block()
{}
// Member Functions 
const mousse::pointField& mousse::block::points() const
{
  if (vertices_.empty())
  {
    createPoints();
  }
  return vertices_;
}
const mousse::labelListList& mousse::block::cells() const
{
  if (cells_.empty())
  {
    createCells();
  }
  return cells_;
}
const mousse::labelListListList& mousse::block::boundaryPatches() const
{
  if (boundaryPatches_.empty())
  {
    createBoundary();
  }
  return boundaryPatches_;
}
// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const block& b)
{
  os << b.points() << nl
   << b.cells() << nl
   << b.boundaryPatches() << endl;
  return os;
}
