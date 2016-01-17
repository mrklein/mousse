// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolation.hpp"
#include "vol_fields.hpp"

template<class Type>
mousse::autoPtr<mousse::interpolation<Type> > mousse::interpolation<Type>::New
(
  const word& interpolationType,
  const GeometricField<Type, fvPatchField, volMesh>& psi
)
{
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(interpolationType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "interpolation::New(const word&, "
      "const GeometricField<Type, fvPatchField, volMesh>&)"
    )
    << "Unknown interpolation type " << interpolationType
    << " for field " << psi.name() << nl << nl
    << "Valid interpolation types : " << endl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<interpolation<Type>>{cstrIter()(psi)};
}


template<class Type>
mousse::autoPtr<mousse::interpolation<Type> > mousse::interpolation<Type>::New
(
  const dictionary& interpolationSchemes,
  const GeometricField<Type, fvPatchField, volMesh>& psi
)
{
  return New(word(interpolationSchemes.lookup(psi.name())), psi);
}
