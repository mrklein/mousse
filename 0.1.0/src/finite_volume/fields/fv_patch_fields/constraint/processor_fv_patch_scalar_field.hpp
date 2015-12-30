// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef processor_fv_patch_scalar_field_hpp_
#define processor_fv_patch_scalar_field_hpp_
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
