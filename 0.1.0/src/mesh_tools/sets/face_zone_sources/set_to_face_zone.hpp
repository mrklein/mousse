#ifndef MESH_TOOLS_SETS_FACE_ZONE_SOURCES_SET_TO_FACE_ZONE_SET_TO_FACE_ZONE_HPP_
#define MESH_TOOLS_SETS_FACE_ZONE_SOURCES_SET_TO_FACE_ZONE_SET_TO_FACE_ZONE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::setToFaceZone
// Description
//   A topoSetSource to select faces based on usage in a faceSet. Sets flipMap
//   to true.

#include "topo_set_source.hpp"


namespace mousse {

class setToFaceZone
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
  TYPE_NAME("setToFaceZone");
  // Constructors
    //- Construct from components
    setToFaceZone
    (
      const polyMesh& mesh,
      const word& setName
    );
    //- Construct from dictionary
    setToFaceZone
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    setToFaceZone
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~setToFaceZone();
  // Member Functions
    virtual sourceType setType() const
    {
      return FACEZONESOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
