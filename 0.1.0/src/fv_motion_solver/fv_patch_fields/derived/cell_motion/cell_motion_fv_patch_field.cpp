// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_motion_fv_patch_field.hpp"
#include "fv_mesh.hpp"
#include "vol_mesh.hpp"
#include "point_fields.hpp"
// Constructors 
template<class Type>
mousse::cellMotionFvPatchField<Type>::cellMotionFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>(p, iF)
{}
template<class Type>
mousse::cellMotionFvPatchField<Type>::cellMotionFvPatchField
(
  const cellMotionFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<Type>(ptf, p, iF, mapper)
{}
template<class Type>
mousse::cellMotionFvPatchField<Type>::cellMotionFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<Type>(p, iF)
{
  fvPatchField<Type>::operator=(Field<Type>("value", dict, p.size()));
}
template<class Type>
mousse::cellMotionFvPatchField<Type>::cellMotionFvPatchField
(
  const cellMotionFvPatchField<Type>& ptf
)
:
  fixedValueFvPatchField<Type>(ptf)
{}
template<class Type>
mousse::cellMotionFvPatchField<Type>::cellMotionFvPatchField
(
  const cellMotionFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  fixedValueFvPatchField<Type>(ptf, iF)
{}
// Member Functions 
template<class Type>
void mousse::cellMotionFvPatchField<Type>::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  const fvPatch& p = this->patch();
  const polyPatch& pp = p.patch();
  const fvMesh& mesh = this->dimensionedInternalField().mesh();
  const pointField& points = mesh.points();
  word pfName = this->dimensionedInternalField().name();
  pfName.replace("cell", "point");
  const GeometricField<Type, pointPatchField, pointMesh>& pointMotion =
    this->db().objectRegistry::template
      lookupObject<GeometricField<Type, pointPatchField, pointMesh> >
      (pfName);
  forAll(p, i)
  {
    this->operator[](i) = pp[i].average(points, pointMotion);
  }
  fixedValueFvPatchField<Type>::updateCoeffs();
}
template<class Type>
void mousse::cellMotionFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  this->writeEntry("value", os);
}
