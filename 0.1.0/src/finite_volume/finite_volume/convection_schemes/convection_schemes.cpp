// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "convection_scheme.hpp"
namespace mousse
{
namespace fv
{
// Define the constructor function hash tables
#define makeBaseConvectionScheme(Type)                                        \
                                       \
defineTemplateRunTimeSelectionTable                                           \
(                                                                             \
  convectionScheme<Type>,                                                   \
  Istream                                                                   \
);                                                                            \
                                       \
defineTemplateRunTimeSelectionTable                                           \
(                                                                             \
  convectionScheme<Type>,                                                   \
  Multivariate                                                              \
);
makeBaseConvectionScheme(scalar)
makeBaseConvectionScheme(vector)
makeBaseConvectionScheme(sphericalTensor)
makeBaseConvectionScheme(symmTensor)
makeBaseConvectionScheme(tensor)
}  // namespace fv
}  // namespace mousse
