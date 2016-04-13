// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surf_point_fields.hpp"


namespace mousse {

// Static Data Members
template<> const word surfPointLabelField::typeName("surfPointLabelField");
template<> const word surfPointScalarField::typeName("surfPointScalarField");
template<> const word surfPointVectorField::typeName("surfPointVectorField");
template<>
const word surfPointSphericalTensorField::typeName
(
  "surfPointSphericalTensorField"
);
template<> const word surfPointSymmTensorField::typeName
(
  "surfPointSymmTensorField"
);
template<> const word surfPointTensorField::typeName("surfPointTensorField");

}  // namespace mousse
