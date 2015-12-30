// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_interpolation_scheme.hpp"
namespace mousse
{
#define makeBaseSurfaceInterpolationScheme(Type)                              \
                                       \
defineNamedTemplateTypeNameAndDebug(surfaceInterpolationScheme<Type>, 0);     \
                                       \
defineTemplateRunTimeSelectionTable                                           \
(                                                                             \
  surfaceInterpolationScheme<Type>,                                         \
  Mesh                                                                      \
);                                                                            \
                                       \
defineTemplateRunTimeSelectionTable                                           \
(                                                                             \
  surfaceInterpolationScheme<Type>,                                         \
  MeshFlux                                                                  \
);
makeBaseSurfaceInterpolationScheme(scalar)
makeBaseSurfaceInterpolationScheme(vector)
makeBaseSurfaceInterpolationScheme(sphericalTensor)
makeBaseSurfaceInterpolationScheme(symmTensor)
makeBaseSurfaceInterpolationScheme(tensor)
}  // namespace mousse
