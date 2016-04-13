#ifndef MESH_TOOLS_SETS_CELL_ZONE_SOURCES_SET_TO_CELL_ZONE_SET_TO_CELL_ZONE_HPP_
#define MESH_TOOLS_SETS_CELL_ZONE_SOURCES_SET_TO_CELL_ZONE_SET_TO_CELL_ZONE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::setToCellZone
// Description
//   A topoSetSource to select cells based on usage in a cellSet.

#include "topo_set_source.hpp"


namespace mousse {

class setToCellZone
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name of set to use
    word setName_;
public:
  //- Runtime type information
  TYPE_NAME("setToCellZone");
  // Constructors
    //- Construct from components
    setToCellZone
    (
      const polyMesh& mesh,
      const word& setName
    );
    //- Construct from dictionary
    setToCellZone
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    setToCellZone
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~setToCellZone();
  // Member Functions
    virtual sourceType setType() const
    {
      return CELLZONESOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
