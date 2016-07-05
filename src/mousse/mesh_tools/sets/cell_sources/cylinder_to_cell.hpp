#ifndef MESH_TOOLS_SETS_CELL_SOURCES_CYLINDER_TO_CELL_CYLINDER_TO_CELL_HPP_
#define MESH_TOOLS_SETS_CELL_SOURCES_CYLINDER_TO_CELL_CYLINDER_TO_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cylinderToCell
// Description
//   A topoSetSource to select cells based on cell centres inside a cylinder.

#include "topo_set_source.hpp"


namespace mousse {

class cylinderToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- First point on cylinder axis
    vector p1_;
    //- Second point on cylinder axis
    vector p2_;
    //- Radius
    scalar radius_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("cylinderToCell");
  // Constructors
    //- Construct from components
    cylinderToCell
    (
      const polyMesh& mesh,
      const vector& p1,
      const vector& p2,
      const scalar radius
    );
    //- Construct from dictionary
    cylinderToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    cylinderToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~cylinderToCell();
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
