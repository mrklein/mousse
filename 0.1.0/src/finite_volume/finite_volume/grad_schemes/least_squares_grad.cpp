// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "least_squares_grad.hpp"
#include "least_squares_vectors.hpp"
#include "gauss_grad.hpp"
#include "fv_mesh.hpp"
#include "vol_mesh.hpp"
#include "surface_mesh.hpp"
#include "geometric_field.hpp"
#include "zero_gradient_fv_patch_field.hpp"
template<class Type>
mousse::tmp
<
  mousse::GeometricField
  <
    typename mousse::outerProduct<mousse::vector, Type>::type,
    mousse::fvPatchField,
    mousse::volMesh
  >
>
mousse::fv::leastSquaresGrad<Type>::calcGrad
(
  const GeometricField<Type, fvPatchField, volMesh>& vsf,
  const word& name
) const
{
  typedef typename outerProduct<vector, Type>::type GradType;
  const fvMesh& mesh = vsf.mesh();
  tmp<GeometricField<GradType, fvPatchField, volMesh> > tlsGrad
  (
    new GeometricField<GradType, fvPatchField, volMesh>
    (
      IOobject
      (
        name,
        vsf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh,
      dimensioned<GradType>
      (
        "zero",
        vsf.dimensions()/dimLength,
        pTraits<GradType>::zero
      ),
      zeroGradientFvPatchField<GradType>::typeName
    )
  );
  GeometricField<GradType, fvPatchField, volMesh>& lsGrad = tlsGrad();
  // Get reference to least square vectors
  const leastSquaresVectors& lsv = leastSquaresVectors::New(mesh);
  const surfaceVectorField& ownLs = lsv.pVectors();
  const surfaceVectorField& neiLs = lsv.nVectors();
  const labelUList& own = mesh.owner();
  const labelUList& nei = mesh.neighbour();
  FOR_ALL(own, facei)
  {
    label ownFaceI = own[facei];
    label neiFaceI = nei[facei];
    Type deltaVsf = vsf[neiFaceI] - vsf[ownFaceI];
    lsGrad[ownFaceI] += ownLs[facei]*deltaVsf;
    lsGrad[neiFaceI] -= neiLs[facei]*deltaVsf;
  }
  // Boundary faces
  FOR_ALL(vsf.boundaryField(), patchi)
  {
    const fvsPatchVectorField& patchOwnLs = ownLs.boundaryField()[patchi];
    const labelUList& faceCells =
      lsGrad.boundaryField()[patchi].patch().faceCells();
    if (vsf.boundaryField()[patchi].coupled())
    {
      const Field<Type> neiVsf
      (
        vsf.boundaryField()[patchi].patchNeighbourField()
      );
      FOR_ALL(neiVsf, patchFaceI)
      {
        lsGrad[faceCells[patchFaceI]] +=
          patchOwnLs[patchFaceI]
         *(neiVsf[patchFaceI] - vsf[faceCells[patchFaceI]]);
      }
    }
    else
    {
      const fvPatchField<Type>& patchVsf = vsf.boundaryField()[patchi];
      FOR_ALL(patchVsf, patchFaceI)
      {
        lsGrad[faceCells[patchFaceI]] +=
          patchOwnLs[patchFaceI]
          *(patchVsf[patchFaceI] - vsf[faceCells[patchFaceI]]);
      }
    }
  }
  lsGrad.correctBoundaryConditions();
  gaussGrad<Type>::correctBoundaryConditions(vsf, lsGrad);
  return tlsGrad;
}
