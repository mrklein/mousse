// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface_fields.hpp"
namespace mousse
{
// Static Data Members
template<>
const word triSurfaceLabelField::typeName("triSurfaceLabelField");
template<>
const word triSurfaceScalarField::typeName("triSurfaceScalarField");
template<>
const word triSurfaceVectorField::typeName("triSurfaceVectorField");
template<>
const word triSurfaceSphericalTensorField::typeName
("triSurfaceSphericalTensorField");
template<>
const word triSurfaceSymmTensorField::typeName
("triSurfaceSymmTensorField");
template<>
const word triSurfaceTensorField::typeName("triSurfaceTensorField");
template<>
const word triSurfacePointLabelField::typeName("triSurfacePointLabelField");
template<>
const word triSurfacePointScalarField::typeName("triSurfacePointScalarField");
template<>
const word triSurfacePointVectorField::typeName("triSurfacePointVectorField");
template<>
const word triSurfacePointSphericalTensorField::typeName
("triSurfacePointSphericalTensorField");
template<>
const word triSurfacePointSymmTensorField::typeName
("triSurfacePointSymmTensorField");
template<>
const word triSurfacePointTensorField::typeName("triSurfacePointTensorField");
}  // namespace mousse
