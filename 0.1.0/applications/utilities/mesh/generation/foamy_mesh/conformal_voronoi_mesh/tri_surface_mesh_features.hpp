#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_TRI_SURFACE_MESH_FEATURES_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_TRI_SURFACE_MESH_FEATURES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::triSurfaceMeshFeatures

#include "searchable_surface_features.hpp"


namespace mousse {

class triSurfaceMeshFeatures
:
  public searchableSurfaceFeatures
{
private:
  // Private Member Data
    const scalar includedAngle_;
    //- Which side of the surface to mesh
    const extendedFeatureEdgeMesh::sideVolumeType mode_;
public:
  //- Runtime type information
  TYPE_NAME("triSurfaceMeshFeatures");
  // Constructors
    //- Construct from dictionary (used by searchableSurface)
    triSurfaceMeshFeatures
    (
      const searchableSurface& surface,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    triSurfaceMeshFeatures(const triSurfaceMeshFeatures&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const triSurfaceMeshFeatures&) = delete;
  //- Destructor
  virtual ~triSurfaceMeshFeatures();
  // Member Functions
    //- Return true for a triSurfaceMesh having features
    virtual bool hasFeatures() const { return true; }
    //- Return an extendedFeatureEdgeMesh containing the features
    virtual autoPtr<extendedFeatureEdgeMesh> features() const;
};

}  // namespace mousse

#endif

