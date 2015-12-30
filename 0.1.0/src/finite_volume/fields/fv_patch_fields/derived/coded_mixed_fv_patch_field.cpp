// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coded_mixed_fv_patch_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "dynamic_code.hpp"
#include "dynamic_code_context.hpp"
#include "string_ops.hpp"
// Static Data Members
template<class Type>
const mousse::word mousse::codedMixedFvPatchField<Type>::codeTemplateC
  = "mixed_fv_patch_field_template.cpp";
template<class Type>
const mousse::word mousse::codedMixedFvPatchField<Type>::codeTemplateH
  = "mixed_fv_patch_field_template.hpp";
// Static Member Functions
template<class Type>
void mousse::codedMixedFvPatchField<Type>::setFieldTemplates
(
  dynamicCode& dynCode
)
{
  word fieldType(pTraits<Type>::typeName);
  // template type for fvPatchField
  dynCode.setFilterVariable("TemplateType", fieldType);
  // Name for fvPatchField - eg, ScalarField, VectorField, ...
  fieldType[0] = toupper(fieldType[0]);
  dynCode.setFilterVariable("FieldType", fieldType + "Field");
}
// Private Member Functions
template<class Type>
const mousse::IOdictionary& mousse::codedMixedFvPatchField<Type>::dict() const
{
  const objectRegistry& obr = this->db();
  if (obr.foundObject<IOdictionary>("codeDict"))
  {
    return obr.lookupObject<IOdictionary>("codeDict");
  }
  else
  {
    return obr.store
    (
      new IOdictionary
      (
        IOobject
        (
          "codeDict",
          this->db().time().system(),
          this->db(),
          IOobject::MUST_READ_IF_MODIFIED,
          IOobject::NO_WRITE
        )
      )
    );
  }
}
template<class Type>
mousse::dlLibraryTable& mousse::codedMixedFvPatchField<Type>::libs() const
{
  return const_cast<dlLibraryTable&>(this->db().time().libs());
}
template<class Type>
void mousse::codedMixedFvPatchField<Type>::prepare
(
  dynamicCode& dynCode,
  const dynamicCodeContext& context
) const
{
  // take no chances - typeName must be identical to redirectType_
  dynCode.setFilterVariable("typeName", redirectType_);
  // set TemplateType and FieldType filter variables
  // (for fvPatchField)
  setFieldTemplates(dynCode);
  // compile filtered C template
  dynCode.addCompileFile(codeTemplateC);
  // copy filtered H template
  dynCode.addCopyFile(codeTemplateH);
  // debugging: make BC verbose
  //  dynCode.setFilterVariable("verbose", "true");
  //  Info<<"compile " << redirectType_ << " sha1: "
  //      << context.sha1() << endl;
  // define _make/options
  dynCode.setMakeOptions
    (
      "EXE_INC = -g \\\n"
      "-I$(LIB_SRC)/finite_volume/_lninclude \\\n"
      + context.options()
      + "\n\nLIB_LIBS = \\\n"
      + "    -lmousse_core\\\n"
      + "    -lmousse_finite_volume \\\n"
      + context.libs()
    );
}
template<class Type>
const mousse::dictionary& mousse::codedMixedFvPatchField<Type>::codeDict()
const
{
  // use system/codeDict or in-line
  return
  (
    dict_.found("code")
   ? dict_
   : this->dict().subDict(redirectType_)
  );
}
template<class Type>
mousse::string mousse::codedMixedFvPatchField<Type>::description() const
{
  return
    "patch "
   + this->patch().name()
   + " on field "
   + this->dimensionedInternalField().name();
}
template<class Type>
void mousse::codedMixedFvPatchField<Type>::clearRedirect() const
{
  // remove instantiation of fvPatchField provided by library
  redirectPatchFieldPtr_.clear();
}
// Constructors
template<class Type>
mousse::codedMixedFvPatchField<Type>::codedMixedFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  mixedFvPatchField<Type>(p, iF),
  codedBase(),
  redirectPatchFieldPtr_()
{}
template<class Type>
mousse::codedMixedFvPatchField<Type>::codedMixedFvPatchField
(
  const codedMixedFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchField<Type>(ptf, p, iF, mapper),
  codedBase(),
  dict_(ptf.dict_),
  redirectType_(ptf.redirectType_),
  redirectPatchFieldPtr_()
{}
template<class Type>
mousse::codedMixedFvPatchField<Type>::codedMixedFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchField<Type>(p, iF, dict),
  codedBase(),
  dict_(dict),
  redirectType_(dict.lookup("redirectType")),
  redirectPatchFieldPtr_()
{
  updateLibrary(redirectType_);
}
template<class Type>
mousse::codedMixedFvPatchField<Type>::codedMixedFvPatchField
(
  const codedMixedFvPatchField<Type>& ptf
)
:
  mixedFvPatchField<Type>(ptf),
  codedBase(),
  dict_(ptf.dict_),
  redirectType_(ptf.redirectType_),
  redirectPatchFieldPtr_()
{}
template<class Type>
mousse::codedMixedFvPatchField<Type>::codedMixedFvPatchField
(
  const codedMixedFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  mixedFvPatchField<Type>(ptf, iF),
  codedBase(),
  dict_(ptf.dict_),
  redirectType_(ptf.redirectType_),
  redirectPatchFieldPtr_()
{}
// Member Functions
template<class Type>
const mousse::mixedFvPatchField<Type>&
mousse::codedMixedFvPatchField<Type>::redirectPatchField() const
{
  if (!redirectPatchFieldPtr_.valid())
  {
    // Construct a patch
    // Make sure to construct the patchfield with up-to-date value
    // Write the data from the mixed b.c.
    OStringStream os;
    mixedFvPatchField<Type>::write(os);
    IStringStream is(os.str());
    // Construct dictionary from it.
    dictionary dict(is);
    // Override the type to enforce the fvPatchField::New constructor
    // to choose our type
    dict.set("type", redirectType_);
    redirectPatchFieldPtr_.set
    (
      dynamic_cast<mixedFvPatchField<Type>*>
      (
        fvPatchField<Type>::New
        (
          this->patch(),
          this->dimensionedInternalField(),
          dict
        ).ptr()
      )
    );
  }
  return redirectPatchFieldPtr_();
}
template<class Type>
void mousse::codedMixedFvPatchField<Type>::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  // Make sure library containing user-defined fvPatchField is up-to-date
  updateLibrary(redirectType_);
  const mixedFvPatchField<Type>& fvp = redirectPatchField();
  const_cast<mixedFvPatchField<Type>&>(fvp).updateCoeffs();
  // Copy through coefficients
  this->refValue() = fvp.refValue();
  this->refGrad() = fvp.refGrad();
  this->valueFraction() = fvp.valueFraction();
  mixedFvPatchField<Type>::updateCoeffs();
}
template<class Type>
void mousse::codedMixedFvPatchField<Type>::evaluate
(
  const Pstream::commsTypes commsType
)
{
  // Make sure library containing user-defined fvPatchField is up-to-date
  updateLibrary(redirectType_);
  const mixedFvPatchField<Type>& fvp = redirectPatchField();
  // - updates the value of fvp (though not used)
  // - resets the updated() flag
  const_cast<mixedFvPatchField<Type>&>(fvp).evaluate(commsType);
  // Update the value (using the coefficients) locally
  mixedFvPatchField<Type>::evaluate(commsType);
}
template<class Type>
void mousse::codedMixedFvPatchField<Type>::write(Ostream& os) const
{
  mixedFvPatchField<Type>::write(os);
  os.writeKeyword("redirectType") << redirectType_
    << token::END_STATEMENT << nl;
  if (dict_.found("codeInclude"))
  {
    os.writeKeyword("codeInclude")
      << token::HASH << token::BEGIN_BLOCK;
    os.writeQuoted(string(dict_["codeInclude"]), false)
      << token::HASH << token::END_BLOCK
      << token::END_STATEMENT << nl;
  }
  if (dict_.found("localCode"))
  {
    os.writeKeyword("localCode")
      << token::HASH << token::BEGIN_BLOCK;
    os.writeQuoted(string(dict_["localCode"]), false)
      << token::HASH << token::END_BLOCK
      << token::END_STATEMENT << nl;
  }
  if (dict_.found("code"))
  {
    os.writeKeyword("code")
      << token::HASH << token::BEGIN_BLOCK;
    os.writeQuoted(string(dict_["code"]), false)
      << token::HASH << token::END_BLOCK
      << token::END_STATEMENT << nl;
  }
  if (dict_.found("codeOptions"))
  {
    os.writeKeyword("codeOptions")
      << token::HASH << token::BEGIN_BLOCK;
    os.writeQuoted(string(dict_["codeOptions"]), false)
      << token::HASH << token::END_BLOCK
      << token::END_STATEMENT << nl;
  }
  if (dict_.found("codeLibs"))
  {
    os.writeKeyword("codeLibs")
      << token::HASH << token::BEGIN_BLOCK;
    os.writeQuoted(string(dict_["codeLibs"]), false)
      << token::HASH << token::END_BLOCK
      << token::END_STATEMENT << nl;
  }
}
