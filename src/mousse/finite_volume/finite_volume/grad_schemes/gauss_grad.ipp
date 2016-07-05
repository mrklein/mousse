// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gauss_grad.hpp"
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
mousse::fv::gaussGrad<Type>::gradf
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>& ssf,
  const word& name
)
{
  typedef typename outerProduct<vector, Type>::type GradType;
  const fvMesh& mesh = ssf.mesh();
  tmp<GeometricField<GradType, fvPatchField, volMesh>> tgGrad
  {
    new GeometricField<GradType, fvPatchField, volMesh>
    {
      {
        name,
        ssf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      // dimensioned<GradType>
      {"0", ssf.dimensions()/dimLength, pTraits<GradType>::zero},
      zeroGradientFvPatchField<GradType>::typeName
    }
  };
  GeometricField<GradType, fvPatchField, volMesh>& gGrad = tgGrad();
  const labelUList& owner = mesh.owner();
  const labelUList& neighbour = mesh.neighbour();
  const vectorField& Sf = mesh.Sf();
  Field<GradType>& igGrad = gGrad;
  const Field<Type>& issf = ssf;
  FOR_ALL(owner, facei) {
    GradType Sfssf = Sf[facei]*issf[facei];
    igGrad[owner[facei]] += Sfssf;
    igGrad[neighbour[facei]] -= Sfssf;
  }
  FOR_ALL(mesh.boundary(), patchi) {
    const labelUList& pFaceCells =
      mesh.boundary()[patchi].faceCells();
    const vectorField& pSf = mesh.Sf().boundaryField()[patchi];
    const fvsPatchField<Type>& pssf = ssf.boundaryField()[patchi];
    FOR_ALL(mesh.boundary()[patchi], facei) {
      igGrad[pFaceCells[facei]] += pSf[facei]*pssf[facei];
    }
  }
  igGrad /= mesh.V();
  gGrad.correctBoundaryConditions();
  return tgGrad;
}


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
mousse::fv::gaussGrad<Type>::calcGrad
(
  const GeometricField<Type, fvPatchField, volMesh>& vsf,
  const word& name
) const
{
  typedef typename outerProduct<vector, Type>::type GradType;
  tmp<GeometricField<GradType, fvPatchField, volMesh>> tgGrad
  {
    gradf(tinterpScheme_().interpolate(vsf), name)
  };
  GeometricField<GradType, fvPatchField, volMesh>& gGrad = tgGrad();
  correctBoundaryConditions(vsf, gGrad);
  return tgGrad;
}


template<class Type>
void mousse::fv::gaussGrad<Type>::correctBoundaryConditions
(
  const GeometricField<Type, fvPatchField, volMesh>& vsf,
  GeometricField
  <
    typename outerProduct<vector, Type>::type, fvPatchField, volMesh
  >& gGrad
)
{
  FOR_ALL(vsf.boundaryField(), patchi) {
    if (!vsf.boundaryField()[patchi].coupled()) {
      const vectorField n
      {
        vsf.mesh().Sf().boundaryField()[patchi]
        /vsf.mesh().magSf().boundaryField()[patchi]
      };
      gGrad.boundaryField()[patchi] += n *
      (
        vsf.boundaryField()[patchi].snGrad()
        - (n & gGrad.boundaryField()[patchi])
      );
    }
  }
}

