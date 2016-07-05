// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "laplacian_scheme.hpp"


namespace mousse {
namespace fv {

// Define the constructor function hash tables
#define MAKE_LAPLACIAN_G_TYPE_SCHEME(Type, GType)                             \
  typedef laplacianScheme<Type, GType> laplacianScheme##Type##GType;          \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                    \
  (                                                                           \
    laplacianScheme##Type##GType,                                             \
    Istream                                                                   \
  );

#define MAKE_LAPLACIAN_SCHEME(Type)                                           \
  MAKE_LAPLACIAN_G_TYPE_SCHEME(Type, scalar);                                 \
  MAKE_LAPLACIAN_G_TYPE_SCHEME(Type, symmTensor);                             \
  MAKE_LAPLACIAN_G_TYPE_SCHEME(Type, tensor);

MAKE_LAPLACIAN_SCHEME(scalar);
MAKE_LAPLACIAN_SCHEME(vector);
MAKE_LAPLACIAN_SCHEME(sphericalTensor);
MAKE_LAPLACIAN_SCHEME(symmTensor);
MAKE_LAPLACIAN_SCHEME(tensor);

}  // namespace fv
}  // namespace mousse

