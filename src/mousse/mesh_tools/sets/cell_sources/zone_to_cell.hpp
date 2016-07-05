#ifndef MESH_TOOLS_SETS_CELL_SOURCES_ZONE_TO_CELL_ZONE_TO_CELL_HPP_
#define MESH_TOOLS_SETS_CELL_SOURCES_ZONE_TO_CELL_ZONE_TO_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::zoneToCell
// Description
//   A topoSetSource to select faces based on cellZone.

#include "topo_set_source.hpp"
#include "word_re.hpp"


namespace mousse {

class zoneToCell
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name/regular expression of cellZone
    wordRe zoneName_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("zoneToCell");
  // Constructors
    //- Construct from components
    zoneToCell
    (
      const polyMesh& mesh,
      const word& zoneName
    );
    //- Construct from dictionary
    zoneToCell
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    zoneToCell
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~zoneToCell();
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
