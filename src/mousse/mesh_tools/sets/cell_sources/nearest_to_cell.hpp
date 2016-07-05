#ifndef MESH_TOOLS_SETS_CELL_SOURCES_NEAREST_TO_CELL_NEAREST_TO_CELL_HPP_
#define MESH_TOOLS_SETS_CELL_SOURCES_NEAREST_TO_CELL_NEAREST_TO_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nearestToCell
// Description
//   A topoSetSource to select cells nearest to points.

#include "topo_set_source.hpp"


namespace mousse {

class nearestToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Points to select nearest to
    pointField points_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("nearestToCell");
  // Constructors
    //- Construct from components
    nearestToCell
    (
      const polyMesh& mesh,
      const pointField& points
    );
    //- Construct from dictionary
    nearestToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    nearestToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~nearestToCell();
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
