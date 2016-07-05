// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surf_fields.hpp"


namespace mousse {

// Static Data Members
template<> const word surfLabelField::typeName("surfLabelField");
template<> const word surfScalarField::typeName("surfScalarField");
template<> const word surfVectorField::typeName("surfVectorField");
template<> const word surfSphericalTensorField::typeName
(
  "surfSphericalTensorField"
);
template<> const word surfSymmTensorField::typeName("surfSymmTensorField");
template<> const word surfTensorField::typeName("surfTensorField");

}  // namespace mousse
