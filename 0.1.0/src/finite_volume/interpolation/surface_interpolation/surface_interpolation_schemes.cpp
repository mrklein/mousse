// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_interpolation_scheme.hpp"

namespace mousse
{

#define MAKE_BASE_SURFACE_INTERPOLATION_SCHEME(Type)                          \
                                                                              \
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(surfaceInterpolationScheme<Type>, 0);\
                                                                              \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                      \
(                                                                             \
  surfaceInterpolationScheme<Type>,                                           \
  Mesh                                                                        \
);                                                                            \
                                                                              \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                      \
(                                                                             \
  surfaceInterpolationScheme<Type>,                                           \
  MeshFlux                                                                    \
);

MAKE_BASE_SURFACE_INTERPOLATION_SCHEME(scalar)
MAKE_BASE_SURFACE_INTERPOLATION_SCHEME(vector)
MAKE_BASE_SURFACE_INTERPOLATION_SCHEME(sphericalTensor)
MAKE_BASE_SURFACE_INTERPOLATION_SCHEME(symmTensor)
MAKE_BASE_SURFACE_INTERPOLATION_SCHEME(tensor)

}  // namespace mousse
