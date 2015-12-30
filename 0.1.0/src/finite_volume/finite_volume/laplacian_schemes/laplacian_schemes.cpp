// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "laplacian_scheme.hpp"
namespace mousse
{
namespace fv
{
// Define the constructor function hash tables
#define makeLaplacianGTypeScheme(Type, GType)                                 \
  typedef laplacianScheme<Type, GType> laplacianScheme##Type##GType;        \
  defineTemplateRunTimeSelectionTable(laplacianScheme##Type##GType, Istream);
#define makeLaplacianScheme(Type)                                             \
  makeLaplacianGTypeScheme(Type, scalar);                                   \
  makeLaplacianGTypeScheme(Type, symmTensor);                               \
  makeLaplacianGTypeScheme(Type, tensor);
makeLaplacianScheme(scalar);
makeLaplacianScheme(vector);
makeLaplacianScheme(sphericalTensor);
makeLaplacianScheme(symmTensor);
makeLaplacianScheme(tensor);
}  // namespace fv
}  // namespace mousse
