// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_interpolate.hpp"
namespace mousse
{
namespace fvc
{
// Return weighting factors for scheme given by name in dictionary
template<class Type>
tmp<surfaceInterpolationScheme<Type> > scheme
(
  const surfaceScalarField& faceFlux,
  Istream& streamData
)
{
  return surfaceInterpolationScheme<Type>::New
  (
    faceFlux.mesh(),
    faceFlux,
    streamData
  );
}
// Return weighting factors for scheme given by name in dictionary
template<class Type>
tmp<surfaceInterpolationScheme<Type> > scheme
(
  const surfaceScalarField& faceFlux,
  const word& name
)
{
  return surfaceInterpolationScheme<Type>::New
  (
    faceFlux.mesh(),
    faceFlux,
    faceFlux.mesh().interpolationScheme(name)
  );
}
// Return weighting factors for scheme given by name in dictionary
template<class Type>
tmp<surfaceInterpolationScheme<Type> > scheme
(
  const fvMesh& mesh,
  Istream& streamData
)
{
  return surfaceInterpolationScheme<Type>::New
  (
    mesh,
    streamData
  );
}
// Return weighting factors for scheme given by name in dictionary
template<class Type>
tmp<surfaceInterpolationScheme<Type> > scheme
(
  const fvMesh& mesh,
  const word& name
)
{
  return surfaceInterpolationScheme<Type>::New
  (
    mesh,
    mesh.interpolationScheme(name)
  );
}
// Interpolate field onto faces using scheme given by name in dictionary
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
interpolate
(
  const GeometricField<Type, fvPatchField, volMesh>& vf,
  const surfaceScalarField& faceFlux,
  Istream& schemeData
)
{
  if (surfaceInterpolation::debug)
  {
    Info<< "interpolate"
      << "(const GeometricField<Type, fvPatchField, volMesh>&, "
      << "const surfaceScalarField&, Istream&) : "
      << "interpolating GeometricField<Type, fvPatchField, volMesh> "
      << endl;
  }
  return scheme<Type>(faceFlux, schemeData)().interpolate(vf);
}
// Interpolate field onto faces using scheme given by name in dictionary
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
interpolate
(
  const GeometricField<Type, fvPatchField, volMesh>& vf,
  const surfaceScalarField& faceFlux,
  const word& name
)
{
  if (surfaceInterpolation::debug)
  {
    Info<< "interpolate"
      << "(const GeometricField<Type, fvPatchField, volMesh>&, "
      << "const surfaceScalarField&, const word&) : "
      << "interpolating GeometricField<Type, fvPatchField, volMesh> "
      << "using " << name
      << endl;
  }
  return scheme<Type>(faceFlux, name)().interpolate(vf);
}
// Interpolate field onto faces using scheme given by name in dictionary
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
interpolate
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
  const surfaceScalarField& faceFlux,
  const word& name
)
{
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf =
    interpolate(tvf(), faceFlux, name);
  tvf.clear();
  return tsf;
}
// Interpolate field onto faces using scheme given by name in dictionary
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
interpolate
(
  const GeometricField<Type, fvPatchField, volMesh>& vf,
  const tmp<surfaceScalarField>& tFaceFlux,
  const word& name
)
{
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf =
    interpolate(vf, tFaceFlux(), name);
  tFaceFlux.clear();
  return tsf;
}
// Interpolate field onto faces using scheme given by name in dictionary
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
interpolate
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
  const tmp<surfaceScalarField>& tFaceFlux,
  const word& name
)
{
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf =
    interpolate(tvf(), tFaceFlux(), name);
  tvf.clear();
  tFaceFlux.clear();
  return tsf;
}
// Interpolate field onto faces using scheme given by name in dictionary
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
interpolate
(
  const GeometricField<Type, fvPatchField, volMesh>& vf,
  Istream& schemeData
)
{
  if (surfaceInterpolation::debug)
  {
    Info<< "interpolate"
      << "(const GeometricField<Type, fvPatchField, volMesh>&, "
      << "Istream&) : "
      << "interpolating GeometricField<Type, fvPatchField, volMesh> "
      << endl;
  }
  return scheme<Type>(vf.mesh(), schemeData)().interpolate(vf);
}
// Interpolate field onto faces using scheme given by name in dictionary
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
interpolate
(
  const GeometricField<Type, fvPatchField, volMesh>& vf,
  const word& name
)
{
  if (surfaceInterpolation::debug)
  {
    Info<< "interpolate"
      << "(const GeometricField<Type, fvPatchField, volMesh>&, "
      << "const word&) : "
      << "interpolating GeometricField<Type, fvPatchField, volMesh> "
      << "using " << name
      << endl;
  }
  return scheme<Type>(vf.mesh(), name)().interpolate(vf);
}
// Interpolate field onto faces using scheme given by name in dictionary
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
interpolate
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
  const word& name
)
{
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf =
    interpolate(tvf(), name);
  tvf.clear();
  return tsf;
}
// Interpolate field onto faces using central differencing
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
interpolate
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  if (surfaceInterpolation::debug)
  {
    Info<< "interpolate"
      << "(const GeometricField<Type, fvPatchField, volMesh>&) : "
      << "interpolating GeometricField<Type, fvPatchField, volMesh> "
      << "using run-time selected scheme"
      << endl;
  }
  return interpolate(vf, "interpolate(" + vf.name() + ')');
}
// Interpolate field onto faces using central differencing
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
interpolate
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf =
    interpolate(tvf());
  tvf.clear();
  return tsf;
}
template<class Type>
tmp<FieldField<fvsPatchField, Type> > interpolate
(
  const FieldField<fvPatchField, Type>& fvpff
)
{
  FieldField<fvsPatchField, Type>* fvspffPtr
  (
    new FieldField<fvsPatchField, Type>(fvpff.size())
  );
  forAll(*fvspffPtr, patchi)
  {
    fvspffPtr->set
    (
      patchi,
      fvsPatchField<Type>::NewCalculatedType(fvpff[patchi].patch()).ptr()
    );
    (*fvspffPtr)[patchi] = fvpff[patchi];
  }
  return tmp<FieldField<fvsPatchField, Type> >(fvspffPtr);
}
template<class Type>
tmp<FieldField<fvsPatchField, Type> > interpolate
(
  const tmp<FieldField<fvPatchField, Type> >& tfvpff
)
{
  tmp<FieldField<fvsPatchField, Type> > tfvspff = interpolate(tfvpff());
  tfvpff.clear();
  return tfvspff;
}
}  // namespace fvc
}  // namespace mousse
