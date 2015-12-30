// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "construct.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
// Private Member Functions 
template<class Type>
mousse::scalar mousse::CLASS::t() const
{
  return this->db().time().timeOutputValue();
}
// Constructors 
template<class Type>
mousse::CLASS::
CONSTRUCT
(
  const fvPatch& p,
  const DimensionedField<TYPE, volMesh>& iF
)
:
  PARENT(p, iF),
  scalarData_(0.0),
  data_(pTraits<TYPE>::zero),
  fieldData_(p.size(), pTraits<TYPE>::zero),
  timeVsData_(),
  wordData_("wordDefault"),
  labelData_(-1),
  boolData_(false)
{
  this->refValue() = pTraits<TYPE>::zero;
  this->refGrad() = pTraits<TYPE>::zero;
  this->valueFraction() = 0.0;
}
template<class Type>
mousse::CLASS::
CONSTRUCT
(
  const fvPatch& p,
  const DimensionedField<TYPE, volMesh>& iF,
  const dictionary& dict
)
:
  PARENT(p, iF),
  scalarData_(readScalar(dict.lookup("scalarData"))),
  data_(pTraits<TYPE>(dict.lookup("data"))),
  fieldData_("fieldData", dict, p.size()),
  timeVsData_(DataEntry<TYPE>::New("timeVsData", dict)),
  wordData_(dict.lookupOrDefault<word>("wordName", "wordDefault")),
  labelData_(-1),
  boolData_(false)
{
  this->refGrad() = pTraits<TYPE>::zero;
  this->valueFraction() = 0.0;
  this->refValue() = FIELD("fieldData", dict, p.size());
  FVPATCHF::operator=(this->refValue());
  PARENT::evaluate();
  /*
  //Initialise with the value entry if evaluation is not possible
  FVPATCHF::operator=
  (
    FIELD("value", dict, p.size())
  );
  this->refValue() = *this;
  */
}
template<class Type>
mousse::CLASS::
CONSTRUCT
(
  const CLASS& ptf,
  const fvPatch& p,
  const DimensionedField<TYPE, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  PARENT(ptf, p, iF, mapper),
  scalarData_(ptf.scalarData_),
  data_(ptf.data_),
  fieldData_(ptf.fieldData_, mapper),
  timeVsData_(ptf.timeVsData_, false),
  wordData_(ptf.wordData_),
  labelData_(-1),
  boolData_(ptf.boolData_)
{}
template<class Type>
mousse::CLASS::
CONSTRUCT
(
  const CLASS& ptf
)
:
  PARENT(ptf),
  scalarData_(ptf.scalarData_),
  data_(ptf.data_),
  fieldData_(ptf.fieldData_),
  timeVsData_(ptf.timeVsData_, false),
  wordData_(ptf.wordData_),
  labelData_(-1),
  boolData_(ptf.boolData_)
{}
template<class Type>
mousse::CLASS::
CONSTRUCT
(
  const CLASS& ptf,
  const DimensionedField<TYPE, volMesh>& iF
)
:
  PARENT(ptf, iF),
  scalarData_(ptf.scalarData_),
  data_(ptf.data_),
  fieldData_(ptf.fieldData_),
  timeVsData_(ptf.timeVsData_, false),
  wordData_(ptf.wordData_),
  labelData_(-1),
  boolData_(ptf.boolData_)
{}
// Member Functions 
template<class Type>
void mousse::CLASS::autoMap
(
  const fvPatchFieldMapper& m
)
{
  PARENT::autoMap(m);
  fieldData_.autoMap(m);
}
template<class Type>
void mousse::CLASS::rmap
(
  const FVPATCHF& ptf,
  const labelList& addr
)
{
  PARENT::rmap(ptf, addr);
  const CLASS& tiptf =
    refCast<const CLASS>(ptf);
  fieldData_.rmap(tiptf.fieldData_, addr);
}
template<class Type>
void mousse::CLASS::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  PARENT::operator==
  (
    data_
   + fieldData_
   + scalarData_*timeVsData_->value(t())
  );
  const scalarField& phip =
    this->patch().template lookupPatchField<surfaceScalarField, scalar>("phi");
  this->valueFraction() = 1.0 - pos(phip);
  PARENT::updateCoeffs();
}
template<class Type>
void mousse::CLASS::write
(
  Ostream& os
) const
{
  FVPATCHF::write(os);
  os.writeKeyword("scalarData") << scalarData_ << token::END_STATEMENT << nl;
  os.writeKeyword("data") << data_ << token::END_STATEMENT << nl;
  fieldData_.writeEntry("fieldData", os);
  timeVsData_->writeData(os);
  os.writeKeyword("wordData") << wordData_ << token::END_STATEMENT << nl;
  this->writeEntry("value", os);
}
// Build Macro Function 
namespace mousse
{
  makePatchTypeField
  (
    FVPATCHF,
    CLASS
  );
}
