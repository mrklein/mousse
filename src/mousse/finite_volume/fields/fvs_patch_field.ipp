// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ioobject.hpp"
#include "dictionary.hpp"
#include "fv_mesh.hpp"
#include "fv_patch_field_mapper.hpp"


namespace mousse {

// Constructors 
template<class Type>
fvsPatchField<Type>::fvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  Field<Type>{p.size()},
  patch_{p},
  internalField_{iF}
{}


template<class Type>
fvsPatchField<Type>::fvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const Field<Type>& f
)
:
  Field<Type>{f},
  patch_{p},
  internalField_{iF}
{}


template<class Type>
fvsPatchField<Type>::fvsPatchField
(
  const fvsPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  Field<Type>{ptf, mapper},
  patch_{p},
  internalField_{iF}
{}


template<class Type>
fvsPatchField<Type>::fvsPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, surfaceMesh>& iF,
  const dictionary& dict
)
:
  Field<Type>{p.size()},
  patch_{p},
  internalField_{iF}
{
  if (dict.found("value")) {
    fvsPatchField<Type>::operator=
    (
      Field<Type>("value", dict, p.size())
    );
  } else {
    FATAL_IO_ERROR_IN
    (
      "fvsPatchField<Type>::fvsPatchField\n"
      "(\n"
      "    const fvPatch& p,\n"
      "    const DimensionedField<Type, surfaceMesh>& iF,\n"
      "    const dictionary& dict\n"
      ")\n",
      dict
    )
    << "essential value entry not provided"
    << exit(FatalIOError);
  }
}


template<class Type>
fvsPatchField<Type>::fvsPatchField
(
  const fvsPatchField<Type>& ptf
)
:
  Field<Type>{ptf},
  patch_{ptf.patch_},
  internalField_{ptf.internalField_}
{}


template<class Type>
fvsPatchField<Type>::fvsPatchField
(
  const fvsPatchField<Type>& ptf,
  const DimensionedField<Type, surfaceMesh>& iF
)
:
  Field<Type>{ptf},
  patch_{ptf.patch_},
  internalField_{iF}
{}


// Member Functions 
template<class Type>
const objectRegistry& fvsPatchField<Type>::db() const
{
  return patch_.boundaryMesh().mesh();
}


template<class Type>
void fvsPatchField<Type>::check(const fvsPatchField<Type>& ptf) const
{
  if (&patch_ != &(ptf.patch_)) {
    FATAL_ERROR_IN("PatchField<Type>::check(const fvsPatchField<Type>&)")
      << "different patches for fvsPatchField<Type>s"
      << abort(FatalError);
  }
}


// Map from self
template<class Type>
void fvsPatchField<Type>::autoMap
(
  const fvPatchFieldMapper& m
)
{
  Field<Type>::autoMap(m);
}


// Reverse-map the given fvsPatchField onto this fvsPatchField
template<class Type>
void fvsPatchField<Type>::rmap
(
  const fvsPatchField<Type>& ptf,
  const labelList& addr
)
{
  Field<Type>::rmap(ptf, addr);
}


// Write
template<class Type>
void fvsPatchField<Type>::write(Ostream& os) const
{
  os.writeKeyword("type") << type() << token::END_STATEMENT << nl;
  this->writeEntry("value", os);
}


// Member Operators 
template<class Type>
void fvsPatchField<Type>::operator=
(
  const UList<Type>& ul
)
{
  Field<Type>::operator=(ul);
}


template<class Type>
void fvsPatchField<Type>::operator=
(
  const fvsPatchField<Type>& ptf
)
{
  check(ptf);
  Field<Type>::operator=(ptf);
}


template<class Type>
void fvsPatchField<Type>::operator+=
(
  const fvsPatchField<Type>& ptf
)
{
  check(ptf);
  Field<Type>::operator+=(ptf);
}


template<class Type>
void fvsPatchField<Type>::operator-=
(
  const fvsPatchField<Type>& ptf
)
{
  check(ptf);
  Field<Type>::operator-=(ptf);
}


template<class Type>
void fvsPatchField<Type>::operator*=
(
  const fvsPatchField<scalar>& ptf
)
{
  if (&patch_ != &ptf.patch()) {
    FATAL_ERROR_IN
    (
      "PatchField<Type>::operator*=(const fvsPatchField<scalar>& ptf)"
    )
    << "incompatible patches for patch fields"
    << abort(FatalError);
  }
  Field<Type>::operator*=(ptf);
}


template<class Type>
void fvsPatchField<Type>::operator/=
(
  const fvsPatchField<scalar>& ptf
)
{
  if (&patch_ != &ptf.patch()) {
    FATAL_ERROR_IN
    (
      "PatchField<Type>::operator/=(const fvsPatchField<scalar>& ptf)"
    )
    << "    incompatible patches for patch fields"
    << abort(FatalError);
  }
  Field<Type>::operator/=(ptf);
}


template<class Type>
void fvsPatchField<Type>::operator+=
(
  const Field<Type>& tf
)
{
  Field<Type>::operator+=(tf);
}


template<class Type>
void fvsPatchField<Type>::operator-=
(
  const Field<Type>& tf
)
{
  Field<Type>::operator-=(tf);
}


template<class Type>
void fvsPatchField<Type>::operator*=
(
  const scalarField& tf
)
{
  Field<Type>::operator*=(tf);
}


template<class Type>
void fvsPatchField<Type>::operator/=
(
  const scalarField& tf
)
{
  Field<Type>::operator/=(tf);
}


template<class Type>
void fvsPatchField<Type>::operator=
(
  const Type& t
)
{
  Field<Type>::operator=(t);
}


template<class Type>
void fvsPatchField<Type>::operator+=
(
  const Type& t
)
{
  Field<Type>::operator+=(t);
}


template<class Type>
void fvsPatchField<Type>::operator-=
(
  const Type& t
)
{
  Field<Type>::operator-=(t);
}


template<class Type>
void fvsPatchField<Type>::operator*=
(
  const scalar s
)
{
  Field<Type>::operator*=(s);
}


template<class Type>
void fvsPatchField<Type>::operator/=
(
  const scalar s
)
{
  Field<Type>::operator/=(s);
}


// Force an assignment, overriding fixedValue status
template<class Type>
void fvsPatchField<Type>::operator==
(
  const fvsPatchField<Type>& ptf
)
{
  Field<Type>::operator=(ptf);
}


template<class Type>
void fvsPatchField<Type>::operator==
(
  const Field<Type>& tf
)
{
  Field<Type>::operator=(tf);
}


template<class Type>
void fvsPatchField<Type>::operator==
(
  const Type& t
)
{
  Field<Type>::operator=(t);
}


// IOstream Operators 
template<class Type>
Ostream& operator<<(Ostream& os, const fvsPatchField<Type>& ptf)
{
  ptf.write(os);
  os.check("Ostream& operator<<(Ostream&, const fvsPatchField<Type>&");
  return os;
}

}  // namespace mousse

#   include "fvs_patch_field_new.ipp"
