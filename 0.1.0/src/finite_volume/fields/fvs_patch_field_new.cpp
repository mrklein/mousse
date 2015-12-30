// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse
{
// Selectors
template<class Type>
tmp<fvsPatchField<Type> > fvsPatchField<Type>::New
(
  const word& patchFieldType,
  const word& actualPatchType,
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF
)
{
  if (debug)
  {
    Info<< "fvsPatchField<Type>::New(const word&, const word&"
       ", const fvPatch&, const Field<Type>&) : "
       "constructing fvsPatchField<Type>"
      << endl;
  }
  typename patchConstructorTable::iterator cstrIter =
    patchConstructorTablePtr_->find(patchFieldType);
  if (cstrIter == patchConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "fvsPatchField<Type>::New(const word&, const word&, const fvPatch&"
      ", const Field<Type>&)"
    )   << "Unknown patchField type "
      << patchFieldType << nl << nl
      << "Valid patchField types are :" << endl
      << patchConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  if
  (
    actualPatchType == word::null
  || actualPatchType != p.type()
  )
  {
    typename patchConstructorTable::iterator patchTypeCstrIter =
      patchConstructorTablePtr_->find(p.type());
    if (patchTypeCstrIter != patchConstructorTablePtr_->end())
    {
      return patchTypeCstrIter()(p, iF);
    }
    else
    {
      return cstrIter()(p, iF);
    }
  }
  else
  {
    return cstrIter()(p, iF);
  }
}
template<class Type>
tmp<fvsPatchField<Type> > fvsPatchField<Type>::New
(
  const word& patchFieldType,
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF
)
{
  return New(patchFieldType, word::null, p, iF);
}
template<class Type>
tmp<fvsPatchField<Type> > fvsPatchField<Type>::New
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const dictionary& dict
)
{
  if (debug)
  {
    Info<< "fvsPatchField<Type>::New(const fvPatch&, const Field<Type>&, "
       "const dictionary&) : "
       "constructing fvsPatchField<Type>"
      << endl;
  }
  const word patchFieldType(dict.lookup("type"));
  typename dictionaryConstructorTable::iterator cstrIter
    = dictionaryConstructorTablePtr_->find(patchFieldType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    if (!disallowGenericFvsPatchField)
    {
      cstrIter = dictionaryConstructorTablePtr_->find("generic");
    }
    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
      FatalIOErrorIn
      (
        "fvsPatchField<Type>::New(const fvPatch&, const Field<Type>&, "
        "const dictionary&)",
        dict
      )   << "Unknown patchField type " << patchFieldType
        << " for patch type " << p.type() << nl << nl
        << "Valid patchField types are :" << endl
        << dictionaryConstructorTablePtr_->sortedToc()
        << exit(FatalIOError);
    }
  }
  if
  (
    !dict.found("patchType")
  || word(dict.lookup("patchType")) != p.type()
  )
  {
    typename dictionaryConstructorTable::iterator patchTypeCstrIter
      = dictionaryConstructorTablePtr_->find(p.type());
    if
    (
      patchTypeCstrIter != dictionaryConstructorTablePtr_->end()
    && patchTypeCstrIter() != cstrIter()
    )
    {
      FatalIOErrorIn
      (
        "fvsPatchField<Type>const fvPatch&, const Field<Type>&, "
        "const dictionary&)",
        dict
      )   << "inconsistent patch and patchField types for \n"
         "    patch type " << p.type()
        << " and patchField type " << patchFieldType
        << exit(FatalIOError);
    }
  }
  return cstrIter()(p, iF, dict);
}
// Return a pointer to a new patch created on freestore from
// a given fvsPatchField<Type> mapped onto a new patch
template<class Type>
tmp<fvsPatchField<Type> > fvsPatchField<Type>::New
(
  const fvsPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const fvPatchFieldMapper& pfMapper
)
{
  if (debug)
  {
    Info<< "fvsPatchField<Type>::New(const fvsPatchField<Type>&,"
       " const fvPatch&, const Field<Type>&, "
       "const fvPatchFieldMapper&) : "
       "constructing fvsPatchField<Type>"
      << endl;
  }
  typename patchMapperConstructorTable::iterator cstrIter =
    patchMapperConstructorTablePtr_->find(ptf.type());
  if (cstrIter == patchMapperConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "fvsPatchField<Type>::New(const fvsPatchField<Type>&, "
      "const fvPatch&, const Field<Type>&, "
      "const fvPatchFieldMapper&)"
    )   << "Unknown patchField type " << ptf.type() << nl << nl
      << "Valid patchField types are :" << endl
      << patchMapperConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  typename patchMapperConstructorTable::iterator
    patchTypeCstrIter = patchMapperConstructorTablePtr_->find(p.type());
  if (patchTypeCstrIter != patchMapperConstructorTablePtr_->end())
  {
    return patchTypeCstrIter()(ptf, p, iF, pfMapper);
  }
  else
  {
    return cstrIter()(ptf, p, iF, pfMapper);
  }
}
}  // namespace mousse
