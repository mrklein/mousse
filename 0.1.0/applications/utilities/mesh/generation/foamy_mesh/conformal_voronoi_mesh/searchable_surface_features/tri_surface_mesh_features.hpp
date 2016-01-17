// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::triSurfaceMeshFeatures
// Description
// SourceFiles
//   tri_surface_mesh_features.cpp
#ifndef tri_surface_mesh_features_hpp_
#define tri_surface_mesh_features_hpp_
#include "searchable_surface_features.hpp"
namespace mousse
{
class triSurfaceMeshFeatures
:
  public searchableSurfaceFeatures
{
private:
  // Private Member Data
    const scalar includedAngle_;
    //- Which side of the surface to mesh
    const extendedFeatureEdgeMesh::sideVolumeType mode_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    triSurfaceMeshFeatures(const triSurfaceMeshFeatures&);
    //- Disallow default bitwise assignment
    void operator=(const triSurfaceMeshFeatures&);
public:
  //- Runtime type information
  TypeName("triSurfaceMeshFeatures");
  // Constructors
    //- Construct from dictionary (used by searchableSurface)
    triSurfaceMeshFeatures
    (
      const searchableSurface& surface,
      const dictionary& dict
    );
  //- Destructor
  virtual ~triSurfaceMeshFeatures();
  // Member Functions
    //- Return true for a triSurfaceMesh having features
    virtual bool hasFeatures() const
    {
      return true;
    }
    //- Return an extendedFeatureEdgeMesh containing the features
    virtual autoPtr<extendedFeatureEdgeMesh> features() const;
};
}  // namespace mousse
#endif
