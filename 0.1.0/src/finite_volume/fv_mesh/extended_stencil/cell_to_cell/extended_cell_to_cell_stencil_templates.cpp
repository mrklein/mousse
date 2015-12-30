// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_cell_to_cell_stencil.hpp"
#include "extended_cell_to_face_stencil.hpp"
// Member Functions 
template<class Type, class WeightType>
mousse::tmp
<
  mousse::GeometricField
  <
    typename mousse::outerProduct<WeightType, Type>::type,
    mousse::fvPatchField,
    mousse::volMesh
  >
> mousse::extendedCellToCellStencil::weightedSum
(
  const mapDistribute& map,
  const labelListList& stencil,
  const GeometricField<Type, fvPatchField, volMesh>& fld,
  const List<List<WeightType> >& stencilWeights
)
{
  typedef typename outerProduct<WeightType, Type>::type WeightedType;
  typedef GeometricField<WeightedType, fvPatchField, volMesh>
    WeightedFieldType;
  const fvMesh& mesh = fld.mesh();
  // Collect internal and boundary values
  List<List<Type> > stencilFld;
  extendedCellToFaceStencil::collectData(map, stencil, fld, stencilFld);
  tmp<WeightedFieldType> twf
  (
    new WeightedFieldType
    (
      IOobject
      (
        fld.name(),
        mesh.time().timeName(),
        mesh
      ),
      mesh,
      dimensioned<WeightedType>
      (
        fld.name(),
        fld.dimensions(),
        pTraits<WeightedType>::zero
      )
    )
  );
  WeightedFieldType& wf = twf();
  forAll(wf, celli)
  {
    const List<Type>& stField = stencilFld[celli];
    const List<WeightType>& stWeight = stencilWeights[celli];
    forAll(stField, i)
    {
      wf[celli] += stWeight[i]*stField[i];
    }
  }
  // Boundaries values?
  return twf;
}
