// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_reconstruct.cpp

#ifndef fvc_reconstruct_hpp_
#define fvc_reconstruct_hpp_

#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "tmp.hpp"

namespace mousse
{

namespace fvc
{

  template<class Type>
  tmp
  <
    GeometricField
    <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
  > reconstruct
  (
    const GeometricField<Type, fvsPatchField, surfaceMesh>&
  );
  template<class Type>
  tmp
  <
    GeometricField
    <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
  > reconstruct
  (
    const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >&
  );
  tmp<volScalarField> reconstructMag(const surfaceScalarField&);
  tmp<volScalarField> reconstructMag(const tmp<surfaceScalarField>&);
}

}  // namespace mousse

#ifdef NoRepository
#   include "fvc_reconstruct.cpp"
#endif

#endif
