// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableSurfaceToFaceZone
// Description
//   A topoSetSource to select faces based on intersection (of cell-cell
//   vector) with a surface.
// SourceFiles
//   searchable_surface_to_face_zone.cpp
#ifndef searchable_surface_to_face_zone_hpp_
#define searchable_surface_to_face_zone_hpp_
#include "topo_set_source.hpp"
namespace mousse
{
class searchableSurface;
class searchableSurfaceToFaceZone
:
  public topoSetSource
{
  // Private data
    //- Add usage string
    static addToUsageTable usage_;
    //- Surface
    autoPtr<searchableSurface> surfacePtr_;
public:
  //- Runtime type information
  TypeName("searchableSurfaceToFaceZone");
  // Constructors
    //- Construct from dictionary
    searchableSurfaceToFaceZone
    (
      const polyMesh& mesh,
      const dictionary& dict
    );
  //- Destructor
  virtual ~searchableSurfaceToFaceZone();
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
