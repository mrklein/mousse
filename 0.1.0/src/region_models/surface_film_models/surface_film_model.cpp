// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_film_model.hpp"
#include "fv_mesh.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(surfaceFilmModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(surfaceFilmModel, mesh);


// Protected Member Functions 
bool surfaceFilmModel::read()
{
  if (singleLayerRegion::read()) {
    return true;
  }
  return false;
}


// Constructors 
surfaceFilmModel::surfaceFilmModel
(
  const word& modelType,
  const fvMesh& mesh,
  const dimensionedVector& g,
  const word& regionType
)
:
  singleLayerRegion{mesh, regionType, modelType},
  g_{g}
{
  if (active_) {
    read();
  }
}


// Destructor 
surfaceFilmModel::~surfaceFilmModel()
{}


// Member Functions 
mousse::scalar surfaceFilmModel::CourantNumber() const
{
  return ROOTVSMALL;
}


tmp<DimensionedField<scalar, volMesh>> surfaceFilmModel::Srho() const
{
  NOT_IMPLEMENTED
  (
    "tmp<DimensionedField<scalar, volMesh>> surfaceFilmModel::Srho() const"
  )
  return tmp<DimensionedField<scalar, volMesh>>{NULL};
}


tmp<DimensionedField<scalar, volMesh>>
surfaceFilmModel::Srho(const label) const
{
  NOT_IMPLEMENTED
  (
    "tmp<DimensionedField<scalar, volMesh>> surfaceFilmModel::Srho"
    "(const label) const"
  )
  return tmp<DimensionedField<scalar, volMesh>>{NULL};
}


tmp<DimensionedField<scalar, volMesh>> surfaceFilmModel::Sh() const
{
  NOT_IMPLEMENTED
  (
    "tmp<DimensionedField<scalar, volMesh>> surfaceFilmModel::Sh() const"
  )
  return tmp<DimensionedField<scalar, volMesh>>{NULL};
}


}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
