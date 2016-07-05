// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_normal_fixed_value_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "fv_patch_field_mapper.hpp"


// Constructors 

mousse::surfaceNormalFixedValueFvPatchVectorField::
surfaceNormalFixedValueFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{p, iF},
  refValue_{p.size()}
{}


mousse::surfaceNormalFixedValueFvPatchVectorField::
surfaceNormalFixedValueFvPatchVectorField
(
  const surfaceNormalFixedValueFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField{p, iF},
  refValue_{ptf.refValue_, mapper}
{
  // Note: calculate product only on ptf to avoid multiplication on
  // unset values in reconstructPar.
  fixedValueFvPatchVectorField::operator=
  (
    vectorField
    {
      ptf.refValue_*ptf.patch().nf(),
      mapper
    }
  );
}


mousse::surfaceNormalFixedValueFvPatchVectorField::
surfaceNormalFixedValueFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField{p, iF},
  refValue_{"refValue", dict, p.size()}
{
  fvPatchVectorField::operator=(refValue_*patch().nf());
}


mousse::surfaceNormalFixedValueFvPatchVectorField::
surfaceNormalFixedValueFvPatchVectorField
(
  const surfaceNormalFixedValueFvPatchVectorField& pivpvf
)
:
  fixedValueFvPatchVectorField{pivpvf},
  refValue_{pivpvf.refValue_}
{}


mousse::surfaceNormalFixedValueFvPatchVectorField::
surfaceNormalFixedValueFvPatchVectorField
(
  const surfaceNormalFixedValueFvPatchVectorField& pivpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{pivpvf, iF},
  refValue_{pivpvf.refValue_}
{}


// Member Functions 
void mousse::surfaceNormalFixedValueFvPatchVectorField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedValueFvPatchVectorField::autoMap(m);
  refValue_.autoMap(m);
}


void mousse::surfaceNormalFixedValueFvPatchVectorField::rmap
(
  const fvPatchVectorField& ptf,
  const labelList& addr
)
{
  fixedValueFvPatchVectorField::rmap(ptf, addr);
  const surfaceNormalFixedValueFvPatchVectorField& tiptf =
    refCast<const surfaceNormalFixedValueFvPatchVectorField>(ptf);
  refValue_.rmap(tiptf.refValue_, addr);
}


void mousse::surfaceNormalFixedValueFvPatchVectorField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  fvPatchVectorField::operator=(refValue_*patch().nf());
  fvPatchVectorField::updateCoeffs();
}


void mousse::surfaceNormalFixedValueFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  refValue_.writeEntry("refValue", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  surfaceNormalFixedValueFvPatchVectorField
);

}

