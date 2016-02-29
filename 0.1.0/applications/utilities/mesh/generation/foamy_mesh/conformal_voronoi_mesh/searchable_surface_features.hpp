#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_SEARCHABLE_SURFACE_FEATURES_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_SEARCHABLE_SURFACE_FEATURES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableSurfaceFeatures
// Description
//   Decorator that returns the features of a searchable surface.
// SourceFiles
//   searchable_surface_features.cpp
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "searchable_surface.hpp"
#include "extended_feature_edge_mesh.hpp"
namespace mousse
{
class searchableSurfaceFeatures
{
  // Private data
    const searchableSurface& surface_;
    const dictionary& dict_;
public:
  //- Runtime type information
  TYPE_NAME("searchableSurfaceFeatures");
  // Declare run-time constructor selection table
    // For the dictionary constructor
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      searchableSurfaceFeatures,
      dict,
      (
        const searchableSurface& surface,
        const dictionary& dict
      ),
      (surface, dict)
    );
  // Constructors
    //- Construct from components
    searchableSurfaceFeatures
    (
      const searchableSurface& surface,
      const dictionary& dict
    );
    //- Clone
    virtual autoPtr<searchableSurfaceFeatures> clone() const
    {
      NOT_IMPLEMENTED("autoPtr<searchableSurfaceFeatures> clone() const");
      return autoPtr<searchableSurfaceFeatures>(NULL);
    }
    //- Disallow default bitwise copy construct
    searchableSurfaceFeatures(const searchableSurfaceFeatures&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const searchableSurfaceFeatures&) = delete;
  // Selectors
    //- Return a reference to the selected searchableSurfaceFeatures
    static autoPtr<searchableSurfaceFeatures> New
    (
      const searchableSurface& surface,
      const dictionary& dict
    );
  //- Destructor
  virtual ~searchableSurfaceFeatures();
  // Member Functions
    //- Return a reference to the searchable surface
    const searchableSurface& surface() const
    {
      return surface_;
    }
    //- Return a reference to the dictionary
    const dictionary& dict() const
    {
      return dict_;
    }
    //- Return whether this searchable surface has features
    virtual bool hasFeatures() const = 0;
    //- Return an extendedFeatureEdgeMesh containing the features
    virtual autoPtr<extendedFeatureEdgeMesh> features() const
    {
      return autoPtr<extendedFeatureEdgeMesh>();
    }
};
}  // namespace mousse
#endif
