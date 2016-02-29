#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_CONSTRAINT_PROCESSOR_FV_PATCH_SCALAR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_CONSTRAINT_PROCESSOR_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_fv_patch_field.hpp"
namespace mousse
{
// Member Functions 
template<>
void processorFvPatchField<scalar>::initInterfaceMatrixUpdate
(
  scalarField&,
  const scalarField&,
  const scalarField&,
  const direction,
  const Pstream::commsTypes commsType
) const;
template<>
void processorFvPatchField<scalar>::updateInterfaceMatrix
(
  scalarField& result,
  const scalarField&,
  const scalarField& coeffs,
  const direction,
  const Pstream::commsTypes commsType
) const;
}  // namespace mousse
#endif
