// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   surface_interpolate.cpp
#ifndef surface_interpolate_hpp_
#define surface_interpolate_hpp_

#include "tmp.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "surface_interpolation_scheme.hpp"
#include "one.hpp"
#include "field_field.hpp"

namespace mousse
{
namespace fvc
{
  //- Return weighting factors for scheme given from Istream
  template<class Type>
  static tmp<surfaceInterpolationScheme<Type> > scheme
  (
    const surfaceScalarField& faceFlux,
    Istream& schemeData
  );
  //- Return weighting factors for scheme given by name in dictionary
  template<class Type>
  static tmp<surfaceInterpolationScheme<Type> > scheme
  (
    const surfaceScalarField& faceFlux,
    const word& name
  );
  //- Return weighting factors for scheme given from Istream
  template<class Type>
  static tmp<surfaceInterpolationScheme<Type> > scheme
  (
    const fvMesh& mesh,
    Istream& schemeData
  );
  //- Return weighting factors for scheme given by name in dictionary
  template<class Type>
  static tmp<surfaceInterpolationScheme<Type> > scheme
  (
    const fvMesh& mesh,
    const word& name
  );
  //- Interpolate field onto faces using scheme given by Istream
  template<class Type>
  static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
  (
    const GeometricField<Type, fvPatchField, volMesh>& tvf,
    const surfaceScalarField& faceFlux,
    Istream& schemeData
  );
  //- Interpolate field onto faces using scheme given by name in fvSchemes
  template<class Type>
  static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
  (
    const GeometricField<Type, fvPatchField, volMesh>& tvf,
    const surfaceScalarField& faceFlux,
    const word& name
  );
  //- Interpolate field onto faces using scheme given by name in fvSchemes
  template<class Type>
  static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const surfaceScalarField& faceFlux,
    const word& name
  );
  //- Interpolate field onto faces using scheme given by name in fvSchemes
  template<class Type>
  static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
  (
    const GeometricField<Type, fvPatchField, volMesh>& tvf,
    const tmp<surfaceScalarField>& faceFlux,
    const word& name
  );
  //- Interpolate field onto faces using scheme given by name in fvSchemes
  template<class Type>
  static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const tmp<surfaceScalarField>& faceFlux,
    const word& name
  );
  //- Interpolate field onto faces using scheme given by Istream
  template<class Type>
  static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
  (
    const GeometricField<Type, fvPatchField, volMesh>& tvf,
    Istream& schemeData
  );
  //- Interpolate field onto faces using scheme given by name in fvSchemes
  template<class Type>
  static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
  (
    const GeometricField<Type, fvPatchField, volMesh>& tvf,
    const word& name
  );
  //- Interpolate field onto faces using scheme given by name in fvSchemes
  template<class Type>
  static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
  );
  //- Interpolate tmp field onto faces using 'interpolate(\<name\>)'
  template<class Type>
  static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
  );
  //- Interpolate field onto faces using 'interpolate(\<name\>)'
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-internal"
  template<class Type>
  static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
  (
    const GeometricField<Type, fvPatchField, volMesh>& tvf
  );
#pragma clang diagnostic pop
  //- Interpolate boundary field onto faces (simply a type conversion)
  template<class Type>
  static tmp<FieldField<fvsPatchField, Type> > interpolate
  (
    const FieldField<fvPatchField, Type>& fvpff
  );
  //- Interpolate boundary field onto faces (simply a type conversion)
  template<class Type>
  static tmp<FieldField<fvsPatchField, Type> > interpolate
  (
    const tmp<FieldField<fvPatchField, Type> >& tfvpff
  );
  //- Interpolate 'one' returning 'one'
  inline one interpolate(const one&)
  {
    return one();
  }
}
}  // namespace mousse
#ifdef NoRepository
#   include "surface_interpolate.cpp"
#endif
#endif
