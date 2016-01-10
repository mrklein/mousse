// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ioobject.hpp"
#include "dictionary.hpp"
#include "fv_mesh.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_mesh.hpp"

// Constructors 
template<class Type>
mousse::fvPatchField<Type>::fvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  Field<Type>{p.size()},
  patch_{p},
  internalField_{iF},
  updated_{false},
  manipulatedMatrix_{false},
  patchType_{word::null}
{}


template<class Type>
mousse::fvPatchField<Type>::fvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const word& patchType
)
:
  Field<Type>{p.size()},
  patch_{p},
  internalField_{iF},
  updated_{false},
  manipulatedMatrix_{false},
  patchType_{patchType}
{}


template<class Type>
mousse::fvPatchField<Type>::fvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const Field<Type>& f
)
:
  Field<Type>{f},
  patch_{p},
  internalField_{iF},
  updated_{false},
  manipulatedMatrix_{false},
  patchType_{word::null}
{}


template<class Type>
mousse::fvPatchField<Type>::fvPatchField
(
  const fvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  Field<Type>{p.size()},
  patch_{p},
  internalField_{iF},
  updated_{false},
  manipulatedMatrix_{false},
  patchType_{ptf.patchType_}
{
  // For unmapped faces set to internal field value (zero-gradient)
  if (notNull(iF) && iF.size())
  {
    fvPatchField<Type>::operator=(this->patchInternalField());
  }
  this->map(ptf, mapper);
}


template<class Type>
mousse::fvPatchField<Type>::fvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict,
  const bool valueRequired
)
:
  Field<Type>{p.size()},
  patch_{p},
  internalField_{iF},
  updated_{false},
  manipulatedMatrix_{false},
  patchType_{dict.lookupOrDefault<word>("patchType", word::null)}
{
  if (dict.found("value"))
  {
    fvPatchField<Type>::operator=
    (
      Field<Type>("value", dict, p.size())
    );
  }
  else if (!valueRequired)
  {
    fvPatchField<Type>::operator=(pTraits<Type>::zero);
  }
  else
  {
    FATAL_IO_ERROR_IN
    (
      "fvPatchField<Type>::fvPatchField"
      "("
      "const fvPatch& p,"
      "const DimensionedField<Type, volMesh>& iF,"
      "const dictionary& dict,"
      "const bool valueRequired"
      ")",
      dict
    )
    << "Essential entry 'value' missing"
    << exit(FatalIOError);
  }
}


template<class Type>
mousse::fvPatchField<Type>::fvPatchField
(
  const fvPatchField<Type>& ptf
)
:
  Field<Type>{ptf},
  patch_{ptf.patch_},
  internalField_{ptf.internalField_},
  updated_{false},
  manipulatedMatrix_{false},
  patchType_{ptf.patchType_}
{}


template<class Type>
mousse::fvPatchField<Type>::fvPatchField
(
  const fvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  Field<Type>{ptf},
  patch_{ptf.patch_},
  internalField_{iF},
  updated_{false},
  manipulatedMatrix_{false},
  patchType_{ptf.patchType_}
{}


// Member Functions 
template<class Type>
const mousse::objectRegistry& mousse::fvPatchField<Type>::db() const
{
  return patch_.boundaryMesh().mesh();
}


template<class Type>
void mousse::fvPatchField<Type>::check(const fvPatchField<Type>& ptf) const
{
  if (&patch_ != &(ptf.patch_))
  {
    FATAL_ERROR_IN("PatchField<Type>::check(const fvPatchField<Type>&)")
      << "different patches for fvPatchField<Type>s"
      << abort(FatalError);
  }
}


template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::fvPatchField<Type>::snGrad() const
{
  return patch_.deltaCoeffs()*(*this - patchInternalField());
}


template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::fvPatchField<Type>::patchInternalField() const
{
  return patch_.patchInternalField(internalField_);
}


template<class Type>
void mousse::fvPatchField<Type>::patchInternalField(Field<Type>& pif) const
{
  patch_.patchInternalField(internalField_, pif);
}


template<class Type>
void mousse::fvPatchField<Type>::autoMap
(
  const fvPatchFieldMapper& mapper
)
{
  Field<Type>& f = *this;
  if (!this->size())
  {
    f.setSize(mapper.size());
    if (f.size())
    {
      f = this->patchInternalField();
    }
  }
  else
  {
    // Map all faces provided with mapping data
    Field<Type>::autoMap(mapper);
    // For unmapped faces set to internal field value (zero-gradient)
    if (mapper.direct()
        && notNull(mapper.directAddressing())
        && mapper.directAddressing().size())
    {
      Field<Type> pif{this->patchInternalField()};
      const labelList& mapAddressing = mapper.directAddressing();
      FOR_ALL(mapAddressing, i)
      {
        if (mapAddressing[i] < 0)
        {
          f[i] = pif[i];
        }
      }
    }
    else if (!mapper.direct() && mapper.addressing().size())
    {
      Field<Type> pif{this->patchInternalField()};
      const labelListList& mapAddressing = mapper.addressing();
      FOR_ALL(mapAddressing, i)
      {
        const labelList& localAddrs = mapAddressing[i];
        if (!localAddrs.size())
        {
          f[i] = pif[i];
        }
      }
    }
  }
}


template<class Type>
void mousse::fvPatchField<Type>::rmap
(
  const fvPatchField<Type>& ptf,
  const labelList& addr
)
{
  Field<Type>::rmap(ptf, addr);
}


template<class Type>
void mousse::fvPatchField<Type>::updateCoeffs()
{
  updated_ = true;
}


template<class Type>
void mousse::fvPatchField<Type>::updateCoeffs(const scalarField& weights)
{
  if (!updated_)
  {
    updateCoeffs();
    Field<Type>& fld = *this;
    fld *= weights;
    updated_ = true;
  }
}


template<class Type>
void mousse::fvPatchField<Type>::evaluate(const Pstream::commsTypes)
{
  if (!updated_)
  {
    updateCoeffs();
  }
  updated_ = false;
  manipulatedMatrix_ = false;
}


template<class Type>
void mousse::fvPatchField<Type>::manipulateMatrix(fvMatrix<Type>& /*matrix*/)
{
  manipulatedMatrix_ = true;
}


template<class Type>
void mousse::fvPatchField<Type>::manipulateMatrix
(
  fvMatrix<Type>& /*matrix*/,
  const scalarField& /*weights*/
)
{
  manipulatedMatrix_ = true;
}


template<class Type>
void mousse::fvPatchField<Type>::write(Ostream& os) const
{
  os.writeKeyword("type") << type() << token::END_STATEMENT << nl;
  if (patchType_.size())
  {
    os.writeKeyword("patchType") << patchType_
      << token::END_STATEMENT << nl;
  }
}


template<class Type>
template<class EntryType>
void mousse::fvPatchField<Type>::writeEntryIfDifferent
(
  Ostream& os,
  const word& entryName,
  const EntryType& value1,
  const EntryType& value2
) const
{
  if (value1 != value2)
  {
    os.writeKeyword(entryName) << value2 << token::END_STATEMENT << nl;
  }
}


// Member Operators 
template<class Type>
void mousse::fvPatchField<Type>::operator=
(
  const UList<Type>& ul
)
{
  Field<Type>::operator=(ul);
}


template<class Type>
void mousse::fvPatchField<Type>::operator=
(
  const fvPatchField<Type>& ptf
)
{
  check(ptf);
  Field<Type>::operator=(ptf);
}


template<class Type>
void mousse::fvPatchField<Type>::operator+=
(
  const fvPatchField<Type>& ptf
)
{
  check(ptf);
  Field<Type>::operator+=(ptf);
}


template<class Type>
void mousse::fvPatchField<Type>::operator-=
(
  const fvPatchField<Type>& ptf
)
{
  check(ptf);
  Field<Type>::operator-=(ptf);
}


template<class Type>
void mousse::fvPatchField<Type>::operator*=
(
  const fvPatchField<scalar>& ptf
)
{
  if (&patch_ != &ptf.patch())
  {
    FATAL_ERROR_IN
    (
      "PatchField<Type>::operator*=(const fvPatchField<scalar>& ptf)"
    )
    << "incompatible patches for patch fields"
    << abort(FatalError);
  }
  Field<Type>::operator*=(ptf);
}


template<class Type>
void mousse::fvPatchField<Type>::operator/=
(
  const fvPatchField<scalar>& ptf
)
{
  if (&patch_ != &ptf.patch())
  {
    FATAL_ERROR_IN
    (
      "PatchField<Type>::operator/=(const fvPatchField<scalar>& ptf)"
    )
    << "    incompatible patches for patch fields"
    << abort(FatalError);
  }
  Field<Type>::operator/=(ptf);
}


template<class Type>
void mousse::fvPatchField<Type>::operator+=
(
  const Field<Type>& tf
)
{
  Field<Type>::operator+=(tf);
}


template<class Type>
void mousse::fvPatchField<Type>::operator-=
(
  const Field<Type>& tf
)
{
  Field<Type>::operator-=(tf);
}


template<class Type>
void mousse::fvPatchField<Type>::operator*=
(
  const scalarField& tf
)
{
  Field<Type>::operator*=(tf);
}


template<class Type>
void mousse::fvPatchField<Type>::operator/=
(
  const scalarField& tf
)
{
  Field<Type>::operator/=(tf);
}


template<class Type>
void mousse::fvPatchField<Type>::operator=
(
  const Type& t
)
{
  Field<Type>::operator=(t);
}


template<class Type>
void mousse::fvPatchField<Type>::operator+=
(
  const Type& t
)
{
  Field<Type>::operator+=(t);
}


template<class Type>
void mousse::fvPatchField<Type>::operator-=
(
  const Type& t
)
{
  Field<Type>::operator-=(t);
}


template<class Type>
void mousse::fvPatchField<Type>::operator*=
(
  const scalar s
)
{
  Field<Type>::operator*=(s);
}


template<class Type>
void mousse::fvPatchField<Type>::operator/=
(
  const scalar s
)
{
  Field<Type>::operator/=(s);
}


// Force an assignment, overriding fixedValue status
template<class Type>
void mousse::fvPatchField<Type>::operator==
(
  const fvPatchField<Type>& ptf
)
{
  Field<Type>::operator=(ptf);
}


template<class Type>
void mousse::fvPatchField<Type>::operator==
(
  const Field<Type>& tf
)
{
  Field<Type>::operator=(tf);
}


template<class Type>
void mousse::fvPatchField<Type>::operator==
(
  const Type& t
)
{
  Field<Type>::operator=(t);
}


// IOstream Operators 
template<class Type>
mousse::Ostream& mousse::operator<<(Ostream& os, const fvPatchField<Type>& ptf)
{
  ptf.write(os);
  os.check("Ostream& operator<<(Ostream&, const fvPatchField<Type>&");
  return os;
}


#include "fv_patch_field_new.cpp"
