// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "generic_fv_patch_field.hpp"
#include "fv_patch_field_mapper.hpp"
// Constructors 
template<class Type>
mousse::genericFvPatchField<Type>::genericFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  calculatedFvPatchField<Type>(p, iF)
{
  FATAL_ERROR_IN
  (
    "genericFvPatchField<Type>::genericFvPatchField"
    "(const fvPatch& p, const DimensionedField<Type, volMesh>& iF)"
  )   << "Not Implemented\n    "
  << "Trying to construct an genericFvPatchField on patch "
  << this->patch().name()
  << " of field " << this->dimensionedInternalField().name()
  << abort(FatalError);
}
template<class Type>
mousse::genericFvPatchField<Type>::genericFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  calculatedFvPatchField<Type>{p, iF, dict, false},
  actualTypeName_{dict.lookup("type")},
  dict_{dict}
{
  if (!dict.found("value"))
  {
    FATAL_IO_ERROR_IN
    (
      "genericFvPatchField<Type>::genericFvPatchField"
      "(const fvPatch&, const Field<Type>&, const dictionary&)",
      dict
    )
    << "\n    Cannot find 'value' entry"
    << " on patch " << this->patch().name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << nl
    << "    which is required to set the"
    " values of the generic patch field." << nl
    << "    (Actual type " << actualTypeName_ << ")" << nl
    << "\n    Please add the 'value' entry to the write function "
    "of the user-defined boundary-condition\n"
    << exit(FatalIOError);
  }
  FOR_ALL_CONST_ITER(dictionary, dict_, iter)
  {
    if (iter().keyword() != "type" && iter().keyword() != "value")
    {
      if (iter().isStream() && iter().stream().size())
      {
        ITstream& is = iter().stream();
        // Read first token
        token firstToken(is);
        if (firstToken.isWord() && firstToken.wordToken() == "nonuniform")
        {
          token fieldToken(is);
          if (!fieldToken.isCompound())
          {
            if (fieldToken.isLabel() && fieldToken.labelToken() == 0)
            {
              scalarFields_.insert
              (
                iter().keyword(),
                new scalarField(0)
              );
            }
            else
            {
              FATAL_IO_ERROR_IN
              (
                "genericFvPatchField<Type>::genericFvPatchField"
                "(const fvPatch&, const Field<Type>&, "
                "const dictionary&)",
                dict
              )
              << "\n    token following 'nonuniform' "
              "is not a compound"
              << "\n    on patch " << this->patch().name()
              << " of field "
              << this->dimensionedInternalField().name()
              << " in file "
              << this->dimensionedInternalField().objectPath()
              << exit(FatalIOError);
            }
          }
          else if (fieldToken.compoundToken().type()
                   == token::Compound<List<scalar>>::typeName)
          {
            scalarField* fPtr = new scalarField;
            fPtr->transfer
            (
              dynamicCast<token::Compound<List<scalar>>>
              (
                fieldToken.transferCompoundToken(is)
              )
            );
            if (fPtr->size() != this->size())
            {
              FATAL_IO_ERROR_IN
              (
                "genericFvPatchField<Type>::genericFvPatchField"
                "(const fvPatch&, const Field<Type>&, "
                "const dictionary&)",
                dict
              )
              << "\n    size of field " << iter().keyword()
              << " (" << fPtr->size() << ')'
              << " is not the same size as the patch ("
              << this->size() << ')'
              << "\n    on patch " << this->patch().name()
              << " of field "
              << this->dimensionedInternalField().name()
              << " in file "
              << this->dimensionedInternalField().objectPath()
              << exit(FatalIOError);
            }
            scalarFields_.insert(iter().keyword(), fPtr);
          }
          else if (fieldToken.compoundToken().type()
                   == token::Compound<List<vector>>::typeName)
          {
            vectorField* fPtr = new vectorField;
            fPtr->transfer
            (
              dynamicCast<token::Compound<List<vector>>>
              (
                fieldToken.transferCompoundToken(is)
              )
            );
            if (fPtr->size() != this->size())
            {
              FATAL_IO_ERROR_IN
              (
                "genericFvPatchField<Type>::genericFvPatchField"
                "(const fvPatch&, const Field<Type>&, "
                "const dictionary&)",
                dict
              )   << "\n    size of field " << iter().keyword()
                << " (" << fPtr->size() << ')'
                << " is not the same size as the patch ("
                << this->size() << ')'
                << "\n    on patch " << this->patch().name()
                << " of field "
                << this->dimensionedInternalField().name()
                << " in file "
                << this->dimensionedInternalField().objectPath()
                << exit(FatalIOError);
            }
            vectorFields_.insert(iter().keyword(), fPtr);
          }
          else if
          (
            fieldToken.compoundToken().type()
          == token::Compound<List<sphericalTensor>>::typeName
          )
          {
            sphericalTensorField* fPtr = new sphericalTensorField;
            fPtr->transfer
            (
              dynamicCast
              <
                token::Compound<List<sphericalTensor>>
              >
              (
                fieldToken.transferCompoundToken(is)
              )
            );
            if (fPtr->size() != this->size())
            {
              FATAL_IO_ERROR_IN
              (
                "genericFvPatchField<Type>::genericFvPatchField"
                "(const fvPatch&, const Field<Type>&, "
                "const dictionary&)",
                dict
              )   << "\n    size of field " << iter().keyword()
                << " (" << fPtr->size() << ')'
                << " is not the same size as the patch ("
                << this->size() << ')'
                << "\n    on patch " << this->patch().name()
                << " of field "
                << this->dimensionedInternalField().name()
                << " in file "
                << this->dimensionedInternalField().objectPath()
                << exit(FatalIOError);
            }
            sphericalTensorFields_.insert(iter().keyword(), fPtr);
          }
          else if (fieldToken.compoundToken().type()
                   == token::Compound<List<symmTensor>>::typeName)
          {
            symmTensorField* fPtr = new symmTensorField;
            fPtr->transfer
            (
              dynamicCast
              <
                token::Compound<List<symmTensor>>
              >
              (
                fieldToken.transferCompoundToken(is)
              )
            );
            if (fPtr->size() != this->size())
            {
              FATAL_IO_ERROR_IN
              (
                "genericFvPatchField<Type>::genericFvPatchField"
                "(const fvPatch&, const Field<Type>&, "
                "const dictionary&)",
                dict
              )
              << "\n    size of field " << iter().keyword()
              << " (" << fPtr->size() << ')'
              << " is not the same size as the patch ("
              << this->size() << ')'
              << "\n    on patch " << this->patch().name()
              << " of field "
              << this->dimensionedInternalField().name()
              << " in file "
              << this->dimensionedInternalField().objectPath()
              << exit(FatalIOError);
            }
            symmTensorFields_.insert(iter().keyword(), fPtr);
          }
          else if (fieldToken.compoundToken().type()
                   == token::Compound<List<tensor>>::typeName)
          {
            tensorField* fPtr = new tensorField;
            fPtr->transfer
            (
              dynamicCast<token::Compound<List<tensor>>>
              (
                fieldToken.transferCompoundToken(is)
              )
            );
            if (fPtr->size() != this->size())
            {
              FATAL_IO_ERROR_IN
              (
                "genericFvPatchField<Type>::genericFvPatchField"
                "(const fvPatch&, const Field<Type>&, "
                "const dictionary&)",
                dict
              )
              << "\n    size of field " << iter().keyword()
              << " (" << fPtr->size() << ')'
              << " is not the same size as the patch ("
              << this->size() << ')'
              << "\n    on patch " << this->patch().name()
              << " of field "
              << this->dimensionedInternalField().name()
              << " in file "
              << this->dimensionedInternalField().objectPath()
              << exit(FatalIOError);
            }
            tensorFields_.insert(iter().keyword(), fPtr);
          }
          else
          {
            FATAL_IO_ERROR_IN
            (
              "genericFvPatchField<Type>::genericFvPatchField"
              "(const fvPatch&, const Field<Type>&, "
              "const dictionary&)",
              dict
            )
            << "\n    compound " << fieldToken.compoundToken()
            << " not supported"
            << "\n    on patch " << this->patch().name()
            << " of field "
            << this->dimensionedInternalField().name()
            << " in file "
            << this->dimensionedInternalField().objectPath()
            << exit(FatalIOError);
          }
        }
        else if (firstToken.isWord()
                 && firstToken.wordToken() == "uniform")
        {
          token fieldToken{is};
          if (!fieldToken.isPunctuation())
          {
            scalarFields_.insert
            (
              iter().keyword(),
              new scalarField
              (
                this->size(),
                fieldToken.number()
              )
            );
          }
          else
          {
            // Read as scalarList.
            is.putBack(fieldToken);
            scalarList l(is);
            if (l.size() == vector::nComponents)
            {
              vector vs(l[0], l[1], l[2]);
              vectorFields_.insert
              (
                iter().keyword(),
                new vectorField(this->size(), vs)
              );
            }
            else if (l.size() == sphericalTensor::nComponents)
            {
              sphericalTensor vs(l[0]);
              sphericalTensorFields_.insert
              (
                iter().keyword(),
                new sphericalTensorField(this->size(), vs)
              );
            }
            else if (l.size() == symmTensor::nComponents)
            {
              symmTensor vs(l[0], l[1], l[2], l[3], l[4], l[5]);
              symmTensorFields_.insert
              (
                iter().keyword(),
                new symmTensorField(this->size(), vs)
              );
            }
            else if (l.size() == tensor::nComponents)
            {
              tensor vs
              (
                l[0], l[1], l[2],
                l[3], l[4], l[5],
                l[6], l[7], l[8]
              );
              tensorFields_.insert
              (
                iter().keyword(),
                new tensorField(this->size(), vs)
              );
            }
            else
            {
              FATAL_IO_ERROR_IN
              (
                "genericFvPatchField<Type>::genericFvPatchField"
                "(const fvPatch&, const Field<Type>&, "
                "const dictionary&)",
                dict
              )
              << "\n    unrecognised native type " << l
              << "\n    on patch " << this->patch().name()
              << " of field "
              << this->dimensionedInternalField().name()
              << " in file "
              << this->dimensionedInternalField().objectPath()
              << exit(FatalIOError);
            }
          }
        }
      }
    }
  }
}
template<class Type>
mousse::genericFvPatchField<Type>::genericFvPatchField
(
  const genericFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  calculatedFvPatchField<Type>(ptf, p, iF, mapper),
  actualTypeName_(ptf.actualTypeName_),
  dict_(ptf.dict_)
{
  FOR_ALL_CONST_ITER
  (
    HashPtrTable<scalarField>,
    ptf.scalarFields_,
    iter
  )
  {
    scalarFields_.insert
    (
      iter.key(),
      new scalarField(*iter(), mapper)
    );
  }
  FOR_ALL_CONST_ITER
  (
    HashPtrTable<vectorField>,
    ptf.vectorFields_,
    iter
  )
  {
    vectorFields_.insert
    (
      iter.key(),
      new vectorField(*iter(), mapper)
    );
  }
  FOR_ALL_CONST_ITER
  (
    HashPtrTable<sphericalTensorField>,
    ptf.sphericalTensorFields_,
    iter
  )
  {
    sphericalTensorFields_.insert
    (
      iter.key(),
      new sphericalTensorField(*iter(), mapper)
    );
  }
  FOR_ALL_CONST_ITER
  (
    HashPtrTable<symmTensorField>,
    ptf.symmTensorFields_,
    iter
  )
  {
    symmTensorFields_.insert
    (
      iter.key(),
      new symmTensorField(*iter(), mapper)
    );
  }
  FOR_ALL_CONST_ITER
  (
    HashPtrTable<tensorField>,
    ptf.tensorFields_,
    iter
  )
  {
    tensorFields_.insert
    (
      iter.key(),
      new tensorField(*iter(), mapper)
    );
  }
}
template<class Type>
mousse::genericFvPatchField<Type>::genericFvPatchField
(
  const genericFvPatchField<Type>& ptf
)
:
  calculatedFvPatchField<Type>{ptf},
  actualTypeName_{ptf.actualTypeName_},
  dict_{ptf.dict_},
  scalarFields_{ptf.scalarFields_},
  vectorFields_{ptf.vectorFields_},
  sphericalTensorFields_{ptf.sphericalTensorFields_},
  symmTensorFields_{ptf.symmTensorFields_},
  tensorFields_{ptf.tensorFields_}
{}
template<class Type>
mousse::genericFvPatchField<Type>::genericFvPatchField
(
  const genericFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  calculatedFvPatchField<Type>{ptf, iF},
  actualTypeName_{ptf.actualTypeName_},
  dict_{ptf.dict_},
  scalarFields_{ptf.scalarFields_},
  vectorFields_{ptf.vectorFields_},
  sphericalTensorFields_{ptf.sphericalTensorFields_},
  symmTensorFields_{ptf.symmTensorFields_},
  tensorFields_{ptf.tensorFields_}
{}
// Member Functions 
template<class Type>
void mousse::genericFvPatchField<Type>::autoMap
(
  const fvPatchFieldMapper& m
)
{
  calculatedFvPatchField<Type>::autoMap(m);
  FOR_ALL_ITER
  (
    HashPtrTable<scalarField>,
    scalarFields_,
    iter
  )
  {
    iter()->autoMap(m);
  }
  FOR_ALL_ITER
  (
    HashPtrTable<vectorField>,
    vectorFields_,
    iter
  )
  {
    iter()->autoMap(m);
  }
  FOR_ALL_ITER
  (
    HashPtrTable<sphericalTensorField>,
    sphericalTensorFields_,
    iter
  )
  {
    iter()->autoMap(m);
  }
  FOR_ALL_ITER
  (
    HashPtrTable<symmTensorField>,
    symmTensorFields_,
    iter
  )
  {
    iter()->autoMap(m);
  }
  FOR_ALL_ITER
  (
    HashPtrTable<tensorField>,
    tensorFields_,
    iter
  )
  {
    iter()->autoMap(m);
  }
}
template<class Type>
void mousse::genericFvPatchField<Type>::rmap
(
  const fvPatchField<Type>& ptf,
  const labelList& addr
)
{
  calculatedFvPatchField<Type>::rmap(ptf, addr);
  const genericFvPatchField<Type>& dptf =
    refCast<const genericFvPatchField<Type>>(ptf);
  FOR_ALL_ITER
  (
    HashPtrTable<scalarField>,
    scalarFields_,
    iter
  )
  {
    HashPtrTable<scalarField>::const_iterator dptfIter =
      dptf.scalarFields_.find(iter.key());
    if (dptfIter != dptf.scalarFields_.end())
    {
      iter()->rmap(*dptfIter(), addr);
    }
  }
  FOR_ALL_ITER
  (
    HashPtrTable<vectorField>,
    vectorFields_,
    iter
  )
  {
    HashPtrTable<vectorField>::const_iterator dptfIter =
      dptf.vectorFields_.find(iter.key());
    if (dptfIter != dptf.vectorFields_.end())
    {
      iter()->rmap(*dptfIter(), addr);
    }
  }
  FOR_ALL_ITER
  (
    HashPtrTable<sphericalTensorField>,
    sphericalTensorFields_,
    iter
  )
  {
    HashPtrTable<sphericalTensorField>::const_iterator dptfIter =
      dptf.sphericalTensorFields_.find(iter.key());
    if (dptfIter != dptf.sphericalTensorFields_.end())
    {
      iter()->rmap(*dptfIter(), addr);
    }
  }
  FOR_ALL_ITER
  (
    HashPtrTable<symmTensorField>,
    symmTensorFields_,
    iter
  )
  {
    HashPtrTable<symmTensorField>::const_iterator dptfIter =
      dptf.symmTensorFields_.find(iter.key());
    if (dptfIter != dptf.symmTensorFields_.end())
    {
      iter()->rmap(*dptfIter(), addr);
    }
  }
  FOR_ALL_ITER
  (
    HashPtrTable<tensorField>,
    tensorFields_,
    iter
  )
  {
    HashPtrTable<tensorField>::const_iterator dptfIter =
      dptf.tensorFields_.find(iter.key());
    if (dptfIter != dptf.tensorFields_.end())
    {
      iter()->rmap(*dptfIter(), addr);
    }
  }
}
template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::genericFvPatchField<Type>::valueInternalCoeffs
(
  const tmp<scalarField>&
) const
{
  FATAL_ERROR_IN
  (
    "genericFvPatchField<Type>::"
    "valueInternalCoeffs(const tmp<scalarField>&) const"
  )
  << "\n    "
  "valueInternalCoeffs cannot be called for a genericFvPatchField"
  " (actual type " << actualTypeName_ << ")"
  << "\n    on patch " << this->patch().name()
  << " of field " << this->dimensionedInternalField().name()
  << " in file " << this->dimensionedInternalField().objectPath()
  << "\n    You are probably trying to solve for a field with a "
  "generic boundary condition."
  << exit(FatalError);
  return *this;
}
template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::genericFvPatchField<Type>::valueBoundaryCoeffs
(
  const tmp<scalarField>&
) const
{
  FATAL_ERROR_IN
  (
    "genericFvPatchField<Type>::"
    "valueBoundaryCoeffs(const tmp<scalarField>&) const"
  )
  << "\n    "
  "valueBoundaryCoeffs cannot be called for a genericFvPatchField"
  " (actual type " << actualTypeName_ << ")"
  << "\n    on patch " << this->patch().name()
  << " of field " << this->dimensionedInternalField().name()
  << " in file " << this->dimensionedInternalField().objectPath()
  << "\n    You are probably trying to solve for a field with a "
  "generic boundary condition."
  << exit(FatalError);
  return *this;
}
template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::genericFvPatchField<Type>::gradientInternalCoeffs() const
{
  FATAL_ERROR_IN
  (
    "genericFvPatchField<Type>::"
    "gradientInternalCoeffs() const"
  )
  << "\n    "
  "gradientInternalCoeffs cannot be called for a genericFvPatchField"
  " (actual type " << actualTypeName_ << ")"
  << "\n    on patch " << this->patch().name()
  << " of field " << this->dimensionedInternalField().name()
  << " in file " << this->dimensionedInternalField().objectPath()
  << "\n    You are probably trying to solve for a field with a "
  "generic boundary condition."
  << exit(FatalError);
  return *this;
}
template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::genericFvPatchField<Type>::gradientBoundaryCoeffs() const
{
  FATAL_ERROR_IN
  (
    "genericFvPatchField<Type>::"
    "gradientBoundaryCoeffs() const"
  )
  << "\n    "
  "gradientBoundaryCoeffs cannot be called for a genericFvPatchField"
  " (actual type " << actualTypeName_ << ")"
  << "\n    on patch " << this->patch().name()
  << " of field " << this->dimensionedInternalField().name()
  << " in file " << this->dimensionedInternalField().objectPath()
  << "\n    You are probably trying to solve for a field with a "
  "generic boundary condition."
  << exit(FatalError);
  return *this;
}
template<class Type>
void mousse::genericFvPatchField<Type>::write(Ostream& os) const
{
  os.writeKeyword("type") << actualTypeName_ << token::END_STATEMENT << nl;
  FOR_ALL_CONST_ITER(dictionary, dict_, iter)
  {
    if (iter().keyword() != "type" && iter().keyword() != "value")
    {
      if (iter().isStream()
          && iter().stream().size()
          && iter().stream()[0].isWord()
          && iter().stream()[0].wordToken() == "nonuniform")
      {
        if (scalarFields_.found(iter().keyword()))
        {
          scalarFields_.find(iter().keyword())()
            ->writeEntry(iter().keyword(), os);
        }
        else if (vectorFields_.found(iter().keyword()))
        {
          vectorFields_.find(iter().keyword())()
            ->writeEntry(iter().keyword(), os);
        }
        else if (sphericalTensorFields_.found(iter().keyword()))
        {
          sphericalTensorFields_.find(iter().keyword())()
            ->writeEntry(iter().keyword(), os);
        }
        else if (symmTensorFields_.found(iter().keyword()))
        {
          symmTensorFields_.find(iter().keyword())()
            ->writeEntry(iter().keyword(), os);
        }
        else if (tensorFields_.found(iter().keyword()))
        {
          tensorFields_.find(iter().keyword())()
            ->writeEntry(iter().keyword(), os);
        }
      }
      else
      {
       iter().write(os);
      }
    }
  }
  this->writeEntry("value", os);
}
