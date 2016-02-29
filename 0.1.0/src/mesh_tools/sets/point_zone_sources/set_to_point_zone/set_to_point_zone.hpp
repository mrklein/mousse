#ifndef MESH_TOOLS_SETS_POINT_ZONE_SOURCES_SET_TO_POINT_ZONE_SET_TO_POINT_ZONE_HPP_
#define MESH_TOOLS_SETS_POINT_ZONE_SOURCES_SET_TO_POINT_ZONE_SET_TO_POINT_ZONE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::setToPointZone
// Description
//   A topoSetSource to select points based on usage in a pointSet.
// SourceFiles
//   set_to_point_zone.cpp
#include "topo_set_source.hpp"
namespace mousse
{
class setToPointZone
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
  TYPE_NAME("setToPointZone");
  // Constructors
    //- Construct from components
    setToPointZone
    (
      const polyMesh& mesh,
      const word& setName
    );
    //- Construct from dictionary
    setToPointZone
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    setToPointZone
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~setToPointZone();
  // Member Functions
    virtual sourceType setType() const
    {
      return POINTZONESOURCE;
    }
    virtual void applyToSet
    (
      const topoSetSource::setAction action,
      topoSet&
    ) const;
};
}  // namespace mousse
#endif
