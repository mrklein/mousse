#ifndef MESH_TOOLS_SETS_FACE_SOURCES_ZONE_TO_FACE_ZONE_TO_FACE_HPP_
#define MESH_TOOLS_SETS_FACE_SOURCES_ZONE_TO_FACE_ZONE_TO_FACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::zoneToFace
// Description
//   A topoSetSource to select faces based on faceZone.

#include "topo_set_source.hpp"
#include "word_re.hpp"


namespace mousse {

class zoneToFace
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
  TYPE_NAME("zoneToFace");
  // Constructors
    //- Construct from components
    zoneToFace
    (
      const polyMesh& mesh,
      const word& zoneName
    );
    //- Construct from dictionary
    zoneToFace
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    zoneToFace
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~zoneToFace();
  // Member Functions
    virtual sourceType setType() const
    {
      return FACESETSOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
