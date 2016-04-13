// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_average.hpp"
#include "fvc_surface_integrate.hpp"
#include "fv_mesh.hpp"
#include "linear.hpp"


namespace mousse {
namespace fvc {

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
average
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>& ssf
)
{
  const fvMesh& mesh = ssf.mesh();
  tmp<GeometricField<Type, fvPatchField, volMesh>> taverage
  {
    new GeometricField<Type, fvPatchField, volMesh>
    {
      {
        "average("+ssf.name()+')',
        ssf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      ssf.dimensions()
    }
  };
  GeometricField<Type, fvPatchField, volMesh>& av = taverage();
  av.internalField() =
  (
    surfaceSum(mesh.magSf()*ssf)/surfaceSum(mesh.magSf())
  )().internalField();
  typename GeometricField<Type, fvPatchField, volMesh>::
  GeometricBoundaryField& bav = av.boundaryField();
  FOR_ALL(bav, patchi) {
    bav[patchi] = ssf.boundaryField()[patchi];
  }
  av.correctBoundaryConditions();
  return taverage;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
average
(
  const tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>& tssf
)
{
  tmp<GeometricField<Type, fvPatchField, volMesh>> taverage
  {
    fvc::average(tssf())
  };
  tssf.clear();
  return taverage;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
average
(
  const GeometricField<Type, fvPatchField, volMesh>& vtf
)
{
  return fvc::average(linearInterpolate(vtf));
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
average
(
  const tmp<GeometricField<Type, fvPatchField, volMesh>>& tvtf
)
{
  tmp<GeometricField<Type, fvPatchField, volMesh>> taverage
  {
    fvc::average(tvtf())
  };
  tvtf.clear();
  return taverage;
}

}  // namespace fvc
}  // namespace mousse
