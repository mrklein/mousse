#ifndef MESH_TOOLS_SETS_POINT_SOURCES_ZONE_TO_POINT_ZONE_TO_POINT_HPP_
#define MESH_TOOLS_SETS_POINT_SOURCES_ZONE_TO_POINT_ZONE_TO_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::zoneToPoint
// Description
//   A topoSetSource to select points based on pointZone.

#include "topo_set_source.hpp"
#include "word_re.hpp"


namespace mousse {

class zoneToPoint
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name/regular expression of zone
    wordRe zoneName_;
  // Private Member Functions
    void combine(topoSet& set, const bool add) const;
public:
  //- Runtime type information
  TYPE_NAME("zoneToPoint");
  // Constructors
    //- Construct from components
    zoneToPoint
    (
      const polyMesh& mesh,
      const word& zoneName
    );
    //- Construct from dictionary
    zoneToPoint
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    zoneToPoint
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~zoneToPoint();
  // Member Functions
    virtual sourceType setType() const
    {
      return POINTSETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
