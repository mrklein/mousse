#ifndef MESH_TOOLS_SETS_FACE_ZONE_SOURCES_SEARCHABLE_SURFACE_TO_FACE_ZONE_SEARCHABLE_SURFACE_TO_FACE_ZONE_HPP_
#define MESH_TOOLS_SETS_FACE_ZONE_SOURCES_SEARCHABLE_SURFACE_TO_FACE_ZONE_SEARCHABLE_SURFACE_TO_FACE_ZONE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableSurfaceToFaceZone
// Description
//   A topoSetSource to select faces based on intersection (of cell-cell
//   vector) with a surface.

#include "topo_set_source.hpp"


namespace mousse {

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
  TYPE_NAME("searchableSurfaceToFaceZone");
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
