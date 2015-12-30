// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_film_model.hpp"
#include "fv_mesh.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
defineTypeNameAndDebug(surfaceFilmModel, 0);
defineRunTimeSelectionTable(surfaceFilmModel, mesh);
// Protected Member Functions 
bool surfaceFilmModel::read()
{
  if (singleLayerRegion::read())
  {
    return true;
  }
  else
  {
    return false;
  }
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
  singleLayerRegion(mesh, regionType, modelType),
  g_(g)
{
  if (active_)
  {
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
tmp<DimensionedField<scalar, volMesh> > surfaceFilmModel::Srho() const
{
  notImplemented
  (
    "tmp<DimensionedField<scalar, volMesh> > surfaceFilmModel::Srho() const"
  )
  return tmp<DimensionedField<scalar, volMesh> >(NULL);
}
tmp<DimensionedField<scalar, volMesh> >
surfaceFilmModel::Srho(const label) const
{
  notImplemented
  (
    "tmp<DimensionedField<scalar, volMesh> > surfaceFilmModel::Srho"
    "(const label) const"
  )
  return tmp<DimensionedField<scalar, volMesh> >(NULL);
}
tmp<DimensionedField<scalar, volMesh> > surfaceFilmModel::Sh() const
{
  notImplemented
  (
    "tmp<DimensionedField<scalar, volMesh> > surfaceFilmModel::Sh() const"
  )
  return tmp<DimensionedField<scalar, volMesh> >(NULL);
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
