// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fourth_grad.hpp"
#include "least_squares_grad.hpp"
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
mousse::fv::fourthGrad<Type>::calcGrad
(
  const GeometricField<Type, fvPatchField, volMesh>& vsf,
  const word& name
) const
{
  // The fourth-order gradient is calculated in two passes.  First,
  // the standard least-square gradient is assembled.  Then, the
  // fourth-order correction is added to the second-order accurate
  // gradient to complete the accuracy.
  typedef typename outerProduct<vector, Type>::type GradType;
  const fvMesh& mesh = vsf.mesh();
  // Assemble the second-order least-square gradient
  // Calculate the second-order least-square gradient
  tmp<GeometricField<GradType, fvPatchField, volMesh> > tsecondfGrad
   = leastSquaresGrad<Type>(mesh).grad
    (
      vsf,
      "leastSquaresGrad(" + vsf.name() + ")"
    );
  const GeometricField<GradType, fvPatchField, volMesh>& secondfGrad =
    tsecondfGrad();
  tmp<GeometricField<GradType, fvPatchField, volMesh> > tfGrad
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
      secondfGrad
    )
  );
  GeometricField<GradType, fvPatchField, volMesh>& fGrad = tfGrad();
  const vectorField& C = mesh.C();
  const surfaceScalarField& lambda = mesh.weights();
  // Get reference to least square vectors
  const leastSquaresVectors& lsv = leastSquaresVectors::New(mesh);
  const surfaceVectorField& ownLs = lsv.pVectors();
  const surfaceVectorField& neiLs = lsv.nVectors();
  // owner/neighbour addressing
  const labelUList& own = mesh.owner();
  const labelUList& nei = mesh.neighbour();
  // Assemble the fourth-order gradient
  // Internal faces
  forAll(own, facei)
  {
    Type dDotGradDelta = 0.5*
    (
     (C[nei[facei]] - C[own[facei]])
    & (secondfGrad[nei[facei]] - secondfGrad[own[facei]])
    );
    fGrad[own[facei]] -= lambda[facei]*ownLs[facei]*dDotGradDelta;
    fGrad[nei[facei]] -= (1.0 - lambda[facei])*neiLs[facei]*dDotGradDelta;
  }
  // Boundary faces
  forAll(vsf.boundaryField(), patchi)
  {
    if (secondfGrad.boundaryField()[patchi].coupled())
    {
      const fvsPatchVectorField& patchOwnLs =
        ownLs.boundaryField()[patchi];
      const scalarField& lambdap = lambda.boundaryField()[patchi];
      const fvPatch& p = fGrad.boundaryField()[patchi].patch();
      const labelUList& faceCells = p.faceCells();
      // Build the d-vectors
      vectorField pd(p.delta());
      const Field<GradType> neighbourSecondfGrad
      (
        secondfGrad.boundaryField()[patchi].patchNeighbourField()
      );
      forAll(faceCells, patchFaceI)
      {
        fGrad[faceCells[patchFaceI]] -=
          0.5*lambdap[patchFaceI]*patchOwnLs[patchFaceI]
         *(
            pd[patchFaceI]
           & (
              neighbourSecondfGrad[patchFaceI]
             - secondfGrad[faceCells[patchFaceI]]
            )
          );
      }
    }
  }
  fGrad.correctBoundaryConditions();
  gaussGrad<Type>::correctBoundaryConditions(vsf, fGrad);
  return tfGrad;
}
