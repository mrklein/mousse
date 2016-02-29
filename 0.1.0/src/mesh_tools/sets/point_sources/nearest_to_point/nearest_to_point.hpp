#ifndef MESH_TOOLS_SETS_POINT_SOURCES_NEAREST_TO_POINT_NEAREST_TO_POINT_HPP_
#define MESH_TOOLS_SETS_POINT_SOURCES_NEAREST_TO_POINT_NEAREST_TO_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nearestToPoint
// Description
//   A topoSetSource to select points nearest to points.
// SourceFiles
//   nearest_to_point.cpp
#include "topo_set_source.hpp"
namespace mousse
{
class nearestToPoint
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
  TYPE_NAME("nearestToPoint");
  // Constructors
    //- Construct from components
    nearestToPoint
    (
      const polyMesh& mesh,
      const pointField& points
    );
    //- Construct from dictionary
    nearestToPoint
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    nearestToPoint
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~nearestToPoint();
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
