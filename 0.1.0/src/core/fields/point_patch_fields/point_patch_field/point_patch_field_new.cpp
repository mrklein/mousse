// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class Type>
mousse::autoPtr<mousse::pointPatchField<Type> > mousse::pointPatchField<Type>::New
(
  const word& patchFieldType,
  const word& actualPatchType,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
{
  if (debug)
  {
    Info<< "PointPatchField<Type>::"
       "New(const word&, const word&"
        ", const pointPatch&, const Field<Type>&) : "
       "constructing pointPatchField<Type>"
      << endl;
  }
  typename pointPatchConstructorTable::iterator cstrIter =
    pointPatchConstructorTablePtr_->find(patchFieldType);
  if (cstrIter == pointPatchConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "PointPatchField<Type>::New"
      "(const word&, const word&, const pointPatch&, const Field<Type>&)"
    )   << "Unknown patchFieldType type "
    << patchFieldType << nl << nl
    << "Valid patchField types are :" << endl
    << pointPatchConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  autoPtr<pointPatchField<Type>> pfPtr{cstrIter()(p, iF)};
  if (actualPatchType == word::null || actualPatchType != p.type())
  {
    if (pfPtr().constraintType() != p.constraintType())
    {
      // Use default constraint type
      typename pointPatchConstructorTable::iterator patchTypeCstrIter =
        pointPatchConstructorTablePtr_->find(p.type());
      if (patchTypeCstrIter == pointPatchConstructorTablePtr_->end())
      {
        FATAL_ERROR_IN
        (
          "PointPatchField<Type>::New"
          "(const word&, const word&"
          ", const pointPatch&, const Field<Type>&)"
        )
        << "inconsistent patch and patchField types for \n"
        << "    patch type " << p.type()
        << " and patchField type " << patchFieldType
        << exit(FatalError);
      }
      return patchTypeCstrIter()(p, iF);
    }
  }
  else
  {
    if (pointPatchConstructorTablePtr_->found(p.type()))
    {
      pfPtr().patchType() = actualPatchType;
    }
  }

  return pfPtr;
}


template<class Type>
mousse::autoPtr<mousse::pointPatchField<Type> > mousse::pointPatchField<Type>::New
(
  const word& patchFieldType,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
{
  return New(patchFieldType, word::null, p, iF);
}


template<class Type>
mousse::autoPtr<mousse::pointPatchField<Type> > mousse::pointPatchField<Type>::New
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
{
  if (debug)
  {
    Info<< "PointPatchField<Type>::"
       "New(const pointPatch&, const Field<Type>&, const dictionary&)"
       " : constructing pointPatchField<Type>"
      << endl;
  }
  word patchFieldType{dict.lookup("type")};
  typename dictionaryConstructorTable::iterator cstrIter
    = dictionaryConstructorTablePtr_->find(patchFieldType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    if (!disallowGenericPointPatchField)
    {
      cstrIter = dictionaryConstructorTablePtr_->find("generic");
    }
    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
      FATAL_IO_ERROR_IN
      (
        "PointPatchField<Type>::"
        "New(const pointPatch&, const Field<Type>&, const dictionary&)",
        dict
      )
      << "Unknown patchField type " << patchFieldType
      << " for patch type " << p.type() << nl << nl
      << "Valid patchField types are :" << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalIOError);
    }
  }
  // Construct (but not necesarily returned)
  autoPtr<pointPatchField<Type>> pfPtr{cstrIter()(p, iF, dict)};
  if (!dict.found("patchType") || word(dict.lookup("patchType")) != p.type())
  {
    if (pfPtr().constraintType() == p.constraintType())
    {
      // Compatible (constraint-wise) with the patch type
      return pfPtr;
    }
    else
    {
      // Use default constraint type
      typename dictionaryConstructorTable::iterator patchTypeCstrIter
        = dictionaryConstructorTablePtr_->find(p.type());
      if (patchTypeCstrIter == dictionaryConstructorTablePtr_->end())
      {
        FATAL_IO_ERROR_IN
        (
          "PointPatchField<Type>const pointPatch&, "
          "const Field<Type>&, const dictionary&)",
          dict
        )
        << "inconsistent patch and patchField types for \n"
        << "    patch type " << p.type()
        << " and patchField type " << patchFieldType
        << exit(FatalIOError);
      }
      return patchTypeCstrIter()(p, iF, dict);
    }
  }
  return cstrIter()(p, iF, dict);
}


// Return a pointer to a new patch created on freestore from
// a given pointPatchField<Type> mapped onto a new patch
template<class Type>
mousse::autoPtr<mousse::pointPatchField<Type> > mousse::pointPatchField<Type>::New
(
  const pointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& pfMapper
)
{
  if (debug)
  {
    Info<< "PointPatchField<Type>::"
       "New(const pointPatchField<Type>&,"
       " const pointPatch&, const Field<Type>&, "
       "const pointPatchFieldMapper&) : "
       "constructing pointPatchField<Type>"
      << endl;
  }
  typename patchMapperConstructorTable::iterator cstrIter =
    patchMapperConstructorTablePtr_->find(ptf.type());
  if (cstrIter == patchMapperConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "PointPatchField<Type>::New"
      "("
      "const pointPatchField<Type>&, "
      "const pointPatch&, "
      "const DimensionedField<Type, pointMesh>&, "
      "const pointPatchFieldMapper&"
      ")"
    )
    << "Unknown patchField type "
    << ptf.type() << nl << nl
    << "Valid patchField types are :" << endl
    << patchMapperConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }

  return cstrIter()(ptf, p, iF, pfMapper);
}
