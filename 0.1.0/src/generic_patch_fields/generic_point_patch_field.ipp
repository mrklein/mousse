// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "generic_point_patch_field.hpp"
#include "point_patch_field_mapper.hpp"


namespace mousse {

// Member Functions 
template<class Type>
genericPointPatchField<Type>::genericPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  calculatedPointPatchField<Type>{p, iF}
{
  NOT_IMPLEMENTED
  (
    "genericPointPatchField<Type>::genericPointPatchField"
    "(const pointPatch& p, const DimensionedField<Type, volMesh>& iF)"
  );
}


template<class Type>
genericPointPatchField<Type>::genericPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  calculatedPointPatchField<Type>{p, iF, dict},
  actualTypeName_{dict.lookup("type")},
  dict_{dict}
{
  FOR_ALL_CONST_ITER(dictionary, dict_, iter) {
    if (iter().keyword() != "type") {
      if (iter().isStream() && iter().stream().size()) {
        ITstream& is = iter().stream();
        // Read first token
        token firstToken{is};
        if (firstToken.isWord() && firstToken.wordToken() == "nonuniform") {
          token fieldToken{is};
          if (!fieldToken.isCompound()) {
            if (fieldToken.isLabel() && fieldToken.labelToken() == 0) {
              scalarFields_.insert
              (
                iter().keyword(),
                new scalarField{0}
              );
            } else {
              FATAL_IO_ERROR_IN
              (
                "genericPointPatchField<Type>::"
                "genericPointPatchField"
                "(const pointPatch&, const Field<Type>&, "
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
          } else if (fieldToken.compoundToken().type()
                     == token::Compound<List<scalar>>::typeName) {
            scalarField* fPtr = new scalarField;
            fPtr->transfer
            (
              dynamicCast<token::Compound<List<scalar>>>
              (
                fieldToken.transferCompoundToken(is)
              )
            );
            if (fPtr->size() != this->size()) {
              FATAL_IO_ERROR_IN
              (
                "genericPointPatchField<Type>::"
                "genericPointPatchField"
                "(const pointPatch&, const Field<Type>&, "
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
          } else if (fieldToken.compoundToken().type()
                     == token::Compound<List<vector>>::typeName) {
            vectorField* fPtr = new vectorField;
            fPtr->transfer
            (
              dynamicCast<token::Compound<List<vector>>>
              (
                fieldToken.transferCompoundToken(is)
              )
            );
            if (fPtr->size() != this->size()) {
              FATAL_IO_ERROR_IN
              (
                "genericPointPatchField<Type>::"
                "genericPointPatchField"
                "(const pointPatch&, const Field<Type>&, "
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
            vectorFields_.insert(iter().keyword(), fPtr);
          } else if (fieldToken.compoundToken().type()
                     == token::Compound<List<sphericalTensor>>::typeName) {
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
            if (fPtr->size() != this->size()) {
              FATAL_IO_ERROR_IN
              (
                "genericPointPatchField<Type>::"
                "genericPointPatchField"
                "(const pointPatch&, const Field<Type>&, "
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
            sphericalTensorFields_.insert(iter().keyword(), fPtr);
          } else if (fieldToken.compoundToken().type()
                     == token::Compound<List<symmTensor>>::typeName) {
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
            if (fPtr->size() != this->size()) {
              FATAL_IO_ERROR_IN
              (
                "genericPointPatchField<Type>::"
                "genericPointPatchField"
                "(const pointPatch&, const Field<Type>&, "
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
          } else if (fieldToken.compoundToken().type()
                     == token::Compound<List<tensor>>::typeName) {
            tensorField* fPtr = new tensorField;
            fPtr->transfer
            (
              dynamicCast<token::Compound<List<tensor>>>
              (
                fieldToken.transferCompoundToken(is)
              )
            );
            if (fPtr->size() != this->size()) {
              FATAL_IO_ERROR_IN
              (
                "genericPointPatchField<Type>::"
                "genericPointPatchField"
                "(const pointPatch&, const Field<Type>&, "
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
          } else {
            FATAL_IO_ERROR_IN
            (
              "genericPointPatchField<Type>::"
              "genericPointPatchField"
              "(const pointPatch&, const Field<Type>&, "
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
      }
    }
  }
}


template<class Type>
genericPointPatchField<Type>::genericPointPatchField
(
  const genericPointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  calculatedPointPatchField<Type>{ptf, p, iF, mapper},
  actualTypeName_{ptf.actualTypeName_},
  dict_{ptf.dict_}
{
  FOR_ALL_CONST_ITER(HashPtrTable<scalarField>,
                     ptf.scalarFields_,
                     iter) {
    scalarFields_.insert
    (
      iter.key(),
      new scalarField{*iter(), mapper}
    );
  }
  FOR_ALL_CONST_ITER(HashPtrTable<vectorField>,
                     ptf.vectorFields_,
                     iter) {
    vectorFields_.insert
    (
      iter.key(),
      new vectorField{*iter(), mapper}
    );
  }
  FOR_ALL_CONST_ITER(HashPtrTable<sphericalTensorField>,
                     ptf.sphericalTensorFields_,
                     iter) {
    sphericalTensorFields_.insert
    (
      iter.key(),
      new sphericalTensorField{*iter(), mapper}
    );
  }
  FOR_ALL_CONST_ITER(HashPtrTable<symmTensorField>,
                     ptf.symmTensorFields_,
                     iter) {
    symmTensorFields_.insert
    (
      iter.key(),
      new symmTensorField{*iter(), mapper}
    );
  }
  FOR_ALL_CONST_ITER(HashPtrTable<tensorField>,
                     ptf.tensorFields_,
                     iter) {
    tensorFields_.insert
    (
      iter.key(),
      new tensorField{*iter(), mapper}
    );
  }
}


template<class Type>
genericPointPatchField<Type>::genericPointPatchField
(
  const genericPointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  calculatedPointPatchField<Type>{ptf, iF},
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
void genericPointPatchField<Type>::autoMap
(
  const pointPatchFieldMapper& m
)
{
  FOR_ALL_ITER(HashPtrTable<scalarField>, scalarFields_, iter) {
    iter()->autoMap(m);
  }
  FOR_ALL_ITER(HashPtrTable<vectorField>, vectorFields_, iter) {
    iter()->autoMap(m);
  }
  FOR_ALL_ITER(HashPtrTable<sphericalTensorField>,
               sphericalTensorFields_,
               iter) {
    iter()->autoMap(m);
  }
  FOR_ALL_ITER(HashPtrTable<symmTensorField>, symmTensorFields_, iter) {
    iter()->autoMap(m);
  }
  FOR_ALL_ITER(HashPtrTable<tensorField>, tensorFields_, iter) {
    iter()->autoMap(m);
  }
}


template<class Type>
void genericPointPatchField<Type>::rmap
(
  const pointPatchField<Type>& ptf,
  const labelList& addr
)
{
  const genericPointPatchField<Type>& dptf =
    refCast<const genericPointPatchField<Type>>(ptf);
  FOR_ALL_ITER(HashPtrTable<scalarField>, scalarFields_, iter) {
    HashPtrTable<scalarField>::const_iterator dptfIter =
      dptf.scalarFields_.find(iter.key());
    if (dptfIter != scalarFields_.end()) {
      iter()->rmap(*dptfIter(), addr);
    }
  }
  FOR_ALL_ITER(HashPtrTable<vectorField>, vectorFields_, iter) {
    HashPtrTable<vectorField>::const_iterator dptfIter =
      dptf.vectorFields_.find(iter.key());
    if (dptfIter != vectorFields_.end()) {
      iter()->rmap(*dptfIter(), addr);
    }
  }
  FOR_ALL_ITER(HashPtrTable<sphericalTensorField>,
               sphericalTensorFields_,
               iter) {
    HashPtrTable<sphericalTensorField>::const_iterator dptfIter =
      dptf.sphericalTensorFields_.find(iter.key());
    if (dptfIter != sphericalTensorFields_.end()) {
      iter()->rmap(*dptfIter(), addr);
    }
  }
  FOR_ALL_ITER(HashPtrTable<symmTensorField>, symmTensorFields_, iter) {
    HashPtrTable<symmTensorField>::const_iterator dptfIter =
      dptf.symmTensorFields_.find(iter.key());
    if (dptfIter != symmTensorFields_.end()) {
      iter()->rmap(*dptfIter(), addr);
    }
  }
  FOR_ALL_ITER(HashPtrTable<tensorField>, tensorFields_, iter) {
    HashPtrTable<tensorField>::const_iterator dptfIter =
      dptf.tensorFields_.find(iter.key());
    if (dptfIter != tensorFields_.end()) {
      iter()->rmap(*dptfIter(), addr);
    }
  }
}


template<class Type>
void genericPointPatchField<Type>::write(Ostream& os) const
{
  os.writeKeyword("type") << actualTypeName_ << token::END_STATEMENT << nl;
  FOR_ALL_CONST_ITER(dictionary, dict_, iter) {
    if (iter().keyword() != "type") {
      if (iter().isStream()
          && iter().stream().size()
          && iter().stream()[0].isWord()
          && iter().stream()[0].wordToken() == "nonuniform") {
        if (scalarFields_.found(iter().keyword())) {
          scalarFields_.find(iter().keyword())()
            ->writeEntry(iter().keyword(), os);
        } else if (vectorFields_.found(iter().keyword())) {
          vectorFields_.find(iter().keyword())()
            ->writeEntry(iter().keyword(), os);
        } else if (sphericalTensorFields_.found(iter().keyword())) {
          sphericalTensorFields_.find(iter().keyword())()
            ->writeEntry(iter().keyword(), os);
        } else if (symmTensorFields_.found(iter().keyword())) {
          symmTensorFields_.find(iter().keyword())()
            ->writeEntry(iter().keyword(), os);
        } else if (tensorFields_.found(iter().keyword())) {
          tensorFields_.find(iter().keyword())()
            ->writeEntry(iter().keyword(), os);
        }
      } else {
       iter().write(os);
      }
    }
  }
}

}  // namespace mousse
