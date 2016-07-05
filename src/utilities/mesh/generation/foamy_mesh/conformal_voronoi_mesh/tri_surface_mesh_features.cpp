// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface_mesh_features.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tri_surface_mesh.hpp"
#include "surface_features.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(triSurfaceMeshFeatures, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  searchableSurfaceFeatures,
  triSurfaceMeshFeatures,
  dict
);

}


// Private Member Functions 
// Constructors 
mousse::triSurfaceMeshFeatures::triSurfaceMeshFeatures
(
  const searchableSurface& surface,
  const dictionary& dict
)
:
  searchableSurfaceFeatures{surface, dict},
  includedAngle_{readScalar(dict.lookup("includedAngle"))},
  mode_
  {
    extendedFeatureEdgeMesh::sideVolumeTypeNames_
    [
      dict.lookupOrDefault<word>("meshableSide", "inside")
    ]
  }
{
  Info << indent
    << "    Included angle = " << includedAngle_ << nl
    << "    Meshable region = "
    << extendedFeatureEdgeMesh::sideVolumeTypeNames_[mode_]
    << endl;
}


// Destructor 
mousse::triSurfaceMeshFeatures::~triSurfaceMeshFeatures()
{}


// Member Functions 
mousse::autoPtr<mousse::extendedFeatureEdgeMesh>
mousse::triSurfaceMeshFeatures::features() const
{
  autoPtr<extendedFeatureEdgeMesh> features;
  const triSurfaceMesh& surfMesh = refCast<const triSurfaceMesh>(surface());
  surfaceFeatures sFeat{surfMesh, includedAngle_};
  // @todo Need to read on a per region basis
  boolList surfBaffleRegions
  {
    surfMesh.patches().size(),
    (mode_ == extendedFeatureEdgeMesh::BOTH ? true : false)
  };
  features.set
  (
    new extendedFeatureEdgeMesh
    {
      sFeat,
      surface().db(),
      surface().name() + ".extendedFeatureEdgeMesh",
      surfBaffleRegions
    }
  );
  return features;
}

