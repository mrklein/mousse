// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceToPoint
// Description
//   A topoSetSource to selects points based on relation to surface.
//   Select based on:
//   - distance to surface
//   - inside/outside status to surface
//    (Uses normal of nearest surface triangle so requires valid surface
//    topology!)
// SourceFiles
//   surface_to_point.cpp
#ifndef surface_to_point_hpp_
#define surface_to_point_hpp_
#include "topo_set_source.hpp"
#include "map.hpp"
namespace mousse
{
class triSurfaceSearch;
class surfaceToPoint
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Name of surface file
    fileName surfName_;
    //- If > 0 : include points with distance to surface less than nearDist.
    scalar nearDist_;
    //- Include all points on opposite sign of normal on nearest surface.
    bool includeInside_;
    //- Include all points on this sign of normal on nearest surface.
    bool includeOutside_;
  // Private Member Functions
    //- Depending on surface add to or delete from pointSet.
    void combine(topoSet& set, const bool add) const;
    //- Check settings at construction time.
    void checkSettings() const;
public:
  //- Runtime type information
  TypeName("surfaceToPoint");
  // Constructors
    //- Construct from components
    surfaceToPoint
    (
      const polyMesh& mesh,
      const fileName& surfName,
      const scalar nearDist,
      const bool includeInside,
      const bool includeOutside
    );
    //- Construct from dictionary
    surfaceToPoint
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
    //- Construct from Istream
    surfaceToPoint
    (
      const polyMesh& mesh,
      Istream&
    );
  //- Destructor
  virtual ~surfaceToPoint();
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
