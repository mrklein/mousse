// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time_varying_uniform_fixed_value_point_patch_field.hpp"
#include "time.hpp"
#include "ifstream.hpp"


// Constructors 
template<class Type>
mousse::
timeVaryingUniformFixedValuePointPatchField<Type>::
timeVaryingUniformFixedValuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  fixedValuePointPatchField<Type>{p, iF}
{}


template<class Type>
mousse::
timeVaryingUniformFixedValuePointPatchField<Type>::
timeVaryingUniformFixedValuePointPatchField
(
  const timeVaryingUniformFixedValuePointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<Type>{ptf, p, iF, mapper},
  timeSeries_{ptf.timeSeries_}
{
  this->operator==(timeSeries_(this->db().time().timeOutputValue()));
}


template<class Type>
mousse::
timeVaryingUniformFixedValuePointPatchField<Type>::
timeVaryingUniformFixedValuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchField<Type>{p, iF},
  timeSeries_{dict}
{
  this->operator==(timeSeries_(this->db().time().timeOutputValue()));
}


template<class Type>
mousse::
timeVaryingUniformFixedValuePointPatchField<Type>::
timeVaryingUniformFixedValuePointPatchField
(
  const timeVaryingUniformFixedValuePointPatchField<Type>& ptf
)
:
  fixedValuePointPatchField<Type>{ptf},
  timeSeries_{ptf.timeSeries_}
{}


template<class Type>
mousse::
timeVaryingUniformFixedValuePointPatchField<Type>::
timeVaryingUniformFixedValuePointPatchField
(
  const timeVaryingUniformFixedValuePointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  fixedValuePointPatchField<Type>{ptf, iF},
  timeSeries_{ptf.timeSeries_}
{}


// Member Functions 
template<class Type>
void mousse::timeVaryingUniformFixedValuePointPatchField<Type>::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  this->operator==(timeSeries_(this->db().time().timeOutputValue()));
  fixedValuePointPatchField<Type>::updateCoeffs();
}


template<class Type>
void mousse::timeVaryingUniformFixedValuePointPatchField<Type>::write
(
  Ostream& os
) const
{
  fixedValuePointPatchField<Type>::write(os);
  timeSeries_.write(os);
}

