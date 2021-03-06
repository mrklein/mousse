// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_least_squares_grad.hpp"
#include "_least_squares_vectors.hpp"
#include "gauss_grad.hpp"
#include "fv_mesh.hpp"
#include "vol_mesh.hpp"
#include "zero_gradient_fv_patch_field.hpp"


template<class Type, class Stencil>
mousse::tmp
<
  mousse::GeometricField
  <
    typename mousse::outerProduct<mousse::vector, Type>::type,
    mousse::fvPatchField,
  mousse::volMesh
  >
>
mousse::fv::LeastSquaresGrad<Type, Stencil>::calcGrad
(
  const GeometricField<Type, fvPatchField, volMesh>& vtf,
  const word& name
) const
{
  typedef typename outerProduct<vector, Type>::type GradType;
  const fvMesh& mesh = vtf.mesh();
  // Get reference to least square vectors
  const LeastSquaresVectors<Stencil>& lsv = LeastSquaresVectors<Stencil>::New
  (
    mesh
  );
  tmp<GeometricField<GradType, fvPatchField, volMesh>> tlsGrad
  {
    new GeometricField<GradType, fvPatchField, volMesh>
    {
      {
        name,
        vtf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      // dimensioned<GradType>
      {"zero", vtf.dimensions()/dimLength, pTraits<GradType>::zero},
      zeroGradientFvPatchField<GradType>::typeName
    }
  };
  GeometricField<GradType, fvPatchField, volMesh>& lsGrad = tlsGrad();
  Field<GradType>& lsGradIf = lsGrad;
  const extendedCentredCellToCellStencil& stencil = lsv.stencil();
  const List<List<label>>& stencilAddr = stencil.stencil();
  const List<List<vector>>& lsvs = lsv.vectors();

  // Construct flat version of vtf
  // including all values referred to by the stencil
  List<Type> flatVtf{stencil.map().constructSize(), pTraits<Type>::zero};

  // Insert internal values
  FOR_ALL(vtf, celli) {
    flatVtf[celli] = vtf[celli];
  }

  // Insert boundary values
  FOR_ALL(vtf.boundaryField(), patchi) {
    const fvPatchField<Type>& ptf = vtf.boundaryField()[patchi];
    label nCompact =
      ptf.patch().start() - mesh.nInternalFaces() + mesh.nCells();
    FOR_ALL(ptf, i) {
      flatVtf[nCompact++] = ptf[i];
    }
  }

  // Do all swapping to complete flatVtf
  stencil.map().distribute(flatVtf);

  // Accumulate the cell-centred gradient from the
  // weighted least-squares vectors and the flattened field values
  FOR_ALL(stencilAddr, celli) {
    const labelList& compactCells = stencilAddr[celli];
    const List<vector>& lsvc = lsvs[celli];
    FOR_ALL(compactCells, i) {
      lsGradIf[celli] += lsvc[i]*flatVtf[compactCells[i]];
    }
  }

  // Correct the boundary conditions
  lsGrad.correctBoundaryConditions();
  gaussGrad<Type>::correctBoundaryConditions(vtf, lsGrad);
  return tlsGrad;
}

