// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_surface_interpolation_scheme.hpp"
namespace mousse
{
#define makeBaseLimitedSurfaceInterpolationScheme(Type)                       \
                                       \
defineNamedTemplateTypeNameAndDebug                                           \
(                                                                             \
  limitedSurfaceInterpolationScheme<Type>,                                  \
  0                                                                         \
);                                                                            \
                                       \
defineTemplateRunTimeSelectionTable                                           \
(                                                                             \
  limitedSurfaceInterpolationScheme<Type>,                                  \
  Mesh                                                                      \
);                                                                            \
                                       \
defineTemplateRunTimeSelectionTable                                           \
(                                                                             \
  limitedSurfaceInterpolationScheme<Type>,                                  \
  MeshFlux                                                                  \
);
makeBaseLimitedSurfaceInterpolationScheme(scalar)
makeBaseLimitedSurfaceInterpolationScheme(vector)
makeBaseLimitedSurfaceInterpolationScheme(sphericalTensor)
makeBaseLimitedSurfaceInterpolationScheme(symmTensor)
makeBaseLimitedSurfaceInterpolationScheme(tensor)
}  // namespace mousse
