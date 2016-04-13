#ifndef MESH_TOOLS_SETS_POINT_SOURCES_POINT_TO_POINT_POINT_TO_POINT_HPP_
#define MESH_TOOLS_SETS_POINT_SOURCES_POINT_TO_POINT_POINT_TO_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointToPoint
// Description
//   A topoSetSource to select points based on usage in pointSet.

#include "topo_set_source.hpp"


namespace mousse {

class pointToPoint
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
  TYPE_NAME("pointToPoint");
  // Constructors
    //- Construct from components
    pointToPoint
    (
      const polyMesh& mesh,
      const word& setName
    );
    //- Construct from dictionary
    pointToPoint
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    pointToPoint
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~pointToPoint();
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
