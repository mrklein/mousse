// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_curl.cpp
#ifndef fvc_curl_hpp_
#define fvc_curl_hpp_
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
