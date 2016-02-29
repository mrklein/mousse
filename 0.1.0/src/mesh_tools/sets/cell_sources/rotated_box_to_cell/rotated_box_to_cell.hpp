#ifndef MESH_TOOLS_SETS_CELL_SOURCES_ROTATED_BOX_TO_CELL_ROTATED_BOX_TO_CELL_HPP_
#define MESH_TOOLS_SETS_CELL_SOURCES_ROTATED_BOX_TO_CELL_ROTATED_BOX_TO_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rotatedBoxToCell
// Description
//   A topoSetSource to select cells based on cell centres inside
//   rotated/skewed box (parallelopiped?).
//   Box defined as origin and i,j,k vectors.
//   E.g. box rotated 45 degrees around z-axis with sizes sqrt(0.2^2+0.2^2)
//   (and extra large, 200 in z direction):
//   \verbatim
//    origin   ( 0.4 0.4 -100);
//    i        ( 0.2 0.2    0);
//    j        (-0.2 0.2    0);
//    k        ( 0.0 0.0  100);
//   \endverbatim
// SourceFiles
//   rotated_box_to_cell.cpp
#include "topo_set_source.hpp"
#include "tree_bound_box.hpp"
namespace mousse
{
class rotatedBoxToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Skewed box
    const vector origin_;
    const vector i_;
    const vector j_;
    const vector k_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("rotatedBoxToCell");
  // Constructors
    //- Construct from components
    rotatedBoxToCell
    (
      const polyMesh& mesh,
      const vector& origin,
      const vector& i,
      const vector& j,
      const vector& k
    );
    //- Construct from dictionary
    rotatedBoxToCell(const polyMesh& mesh, const dictionary& dict);
    //- Construct from Istream
    rotatedBoxToCell(const polyMesh& mesh, Istream&);
  //- Destructor
  virtual ~rotatedBoxToCell();
  // Member Functions
    virtual sourceType setType() const
    {
      return CELLSETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
