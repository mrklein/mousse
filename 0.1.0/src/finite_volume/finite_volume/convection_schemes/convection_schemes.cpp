// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "convection_scheme.hpp"

namespace mousse {
namespace fv {

// Define the constructor function hash tables
#define MAKE_BASE_CONVECTION_SCHEME(Type)                                     \
                                                                              \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                      \
(                                                                             \
  convectionScheme<Type>,                                                     \
  Istream                                                                     \
);                                                                            \
                                                                              \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                      \
(                                                                             \
  convectionScheme<Type>,                                                     \
  Multivariate                                                                \
);


MAKE_BASE_CONVECTION_SCHEME(scalar)
MAKE_BASE_CONVECTION_SCHEME(vector)
MAKE_BASE_CONVECTION_SCHEME(sphericalTensor)
MAKE_BASE_CONVECTION_SCHEME(symmTensor)
MAKE_BASE_CONVECTION_SCHEME(tensor)

}  // namespace fv
}  // namespace mousse
