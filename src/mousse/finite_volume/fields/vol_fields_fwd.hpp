#ifndef FINITE_VOLUME_FIELDS_VOL_FIELDS_FWD_HPP_
#define FINITE_VOLUME_FIELDS_VOL_FIELDS_FWD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_types.hpp"


namespace mousse {

class volMesh;

template<class Type>
class fvPatchField;

template<class Type, template<class> class PatchField, class GeoMesh>
class GeometricField;

typedef GeometricField<scalar, fvPatchField, volMesh> volScalarField;

typedef GeometricField<vector, fvPatchField, volMesh> volVectorField;

typedef GeometricField<sphericalTensor, fvPatchField, volMesh>
  volSphericalTensorField;

typedef GeometricField<symmTensor, fvPatchField, volMesh> volSymmTensorField;

typedef GeometricField<tensor, fvPatchField, volMesh> volTensorField;

}  // namespace mousse
#endif
