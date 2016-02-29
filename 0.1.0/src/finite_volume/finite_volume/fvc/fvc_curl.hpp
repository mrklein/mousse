#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_CURL_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_CURL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_curl.cpp
#include "vol_fields_fwd.hpp"
namespace mousse
{
namespace fvc
{
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > curl
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > curl
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_curl.cpp"
#endif
#endif
