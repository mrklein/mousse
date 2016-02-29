#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_VOLUME_INTEGRATE_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_VOLUME_INTEGRATE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_volume_integrate.cpp
#include "vol_fields_fwd.hpp"
#include "primitive_fields_fwd.hpp"
#include "dimensioned_types.hpp"
namespace mousse
{
namespace fvc
{
  template<class Type>
  tmp<Field<Type> > volumeIntegrate
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<Field<Type> > volumeIntegrate
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
  template<class Type>
  tmp<Field<Type> > volumeIntegrate
  (
    const DimensionedField<Type, volMesh>&
  );
  template<class Type>
  tmp<Field<Type> > volumeIntegrate
  (
    const tmp<DimensionedField<Type, volMesh> >&
  );
  template<class Type>
  dimensioned<Type> domainIntegrate
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  dimensioned<Type> domainIntegrate
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
  template<class Type>
  dimensioned<Type> domainIntegrate
  (
    const DimensionedField<Type, volMesh>&
  );
  template<class Type>
  dimensioned<Type> domainIntegrate
  (
    const tmp<DimensionedField<Type, volMesh> >&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_volume_integrate.cpp"
#endif
#endif
