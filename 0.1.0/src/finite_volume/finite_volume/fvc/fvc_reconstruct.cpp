// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_reconstruct.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fvc_surface_integrate.hpp"
#include "zero_gradient_fv_patch_fields.hpp"

namespace mousse
{

namespace fvc
{

template<class Type>
tmp
<
  GeometricField
  <
    typename outerProduct<vector,Type>::type, fvPatchField, volMesh
  >
>
reconstruct
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>& ssf
)
{
  typedef typename outerProduct<vector, Type>::type GradType;
  const fvMesh& mesh = ssf.mesh();
  surfaceVectorField SfHat{mesh.Sf()/mesh.magSf()};
  tmp<GeometricField<GradType, fvPatchField, volMesh>> treconField
  {
    new GeometricField<GradType, fvPatchField, volMesh>
    {
      IOobject
      {
        "volIntegrate(" + ssf.name() + ')',
        ssf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      inv(surfaceSum(SfHat*mesh.Sf())) & surfaceSum(SfHat*ssf),
      zeroGradientFvPatchField<GradType>::typeName
    }
  };
  treconField().correctBoundaryConditions();

  return treconField;
}


template<class Type>
tmp
<
  GeometricField
  <
    typename outerProduct<vector, Type>::type, fvPatchField, volMesh
  >
>
reconstruct
(
  const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >& tssf
)
{
  typedef typename outerProduct<vector, Type>::type GradType;
  tmp<GeometricField<GradType, fvPatchField, volMesh> > tvf
  {
    fvc::reconstruct(tssf())
  };
  tssf.clear();

  return tvf;
}

}  // namespace fvc

}  // namespace mousse
