// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh.hpp"
#include "harmonic.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(harmonic, 0);

surfaceInterpolationScheme<scalar>::addMeshFluxConstructorToTable<harmonic>
  addharmonicScalarMeshFluxConstructorToTable_;
surfaceInterpolationScheme<scalar>::addMeshConstructorToTable<harmonic>
  addharmonicScalarMeshConstructorToTable_;

}
