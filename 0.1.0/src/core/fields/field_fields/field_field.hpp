#ifndef CORE_FIELDS_FIELD_FIELDS_FIELD_FIELD_HPP_
#define CORE_FIELDS_FIELD_FIELDS_FIELD_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FieldField
// Description
//   Generic field type.
// SourceFiles
//   field_field.cpp
#include "tmp.hpp"
#include "ptr_list.hpp"
#include "scalar.hpp"
#include "direction.hpp"
#include "vector_space.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<template<class> class Field, class Type>
class FieldField;
template<template<class> class Field, class Type>
Ostream& operator<<
(
  Ostream&,
  const FieldField<Field, Type>&
);
template<template<class> class Field, class Type>
Ostream& operator<<
(
  Ostream&,
  const tmp<FieldField<Field, Type> >&
);
template<template<class> class Field, class Type>
class FieldField
:
  public refCount,
  public PtrList<Field<Type> >
{
public:
  //- Component type
  typedef typename pTraits<Type>::cmptType cmptType;
  // Constructors
    //- Construct null
    //  Used for temporary fields which are initialised after construction
    FieldField();
    //- Construct given size
    //  Used for temporary fields which are initialised after construction
    explicit FieldField(const label);
    //- Construct using the Field sizes from the given FieldField
    //  and the given Field type.
    //  Used for temporary fields which are initialised after construction
    FieldField(const word&, const FieldField<Field, Type>&);
    //- Construct as copy
    FieldField(const FieldField<Field, Type>&);
    //- Construct as copy or re-use as specified.
    FieldField(FieldField<Field, Type>&, bool reUse);
    //- Construct as copy of a PtrList<Field, Type>
    FieldField(const PtrList<Field<Type> >&);
    //- Construct as copy of tmp<FieldField>
    #ifndef NoConstructFromTmp
    FieldField(const tmp<FieldField<Field, Type> >&);
    #endif
    //- Construct from Istream
    FieldField(Istream&);
    //- Clone
    tmp<FieldField<Field, Type> > clone() const;
    //- Return a pointer to a new calculatedFvPatchFieldField created on
    //  freestore without setting patchField values
    template<class Type2>
    static tmp<FieldField<Field, Type> > NewCalculatedType
    (
      const FieldField<Field, Type2>& ff
    );
  // Member functions
    //- Negate this field
    void negate();
    //- Return a component field of the field
    tmp<FieldField<Field, cmptType> > component(const direction) const;
    //- Replace a component field of the field
    void replace(const direction, const FieldField<Field, cmptType>&);
    //- Replace a component field of the field
    void replace(const direction, const cmptType&);
    //- Return the field transpose (only defined for second rank tensors)
    tmp<FieldField<Field, Type> > T() const;
  // Member operators
    void operator=(const FieldField<Field, Type>&);
    void operator=(const tmp<FieldField<Field, Type> >&);
    void operator=(const Type&);
    void operator+=(const FieldField<Field, Type>&);
    void operator+=(const tmp<FieldField<Field, Type> >&);
    void operator-=(const FieldField<Field, Type>&);
    void operator-=(const tmp<FieldField<Field, Type> >&);
    void operator*=(const FieldField<Field, scalar>&);
    void operator*=(const tmp<FieldField<Field, scalar> >&);
    void operator/=(const FieldField<Field, scalar>&);
    void operator/=(const tmp<FieldField<Field, scalar> >&);
    void operator+=(const Type&);
    void operator-=(const Type&);
    void operator*=(const scalar&);
    void operator/=(const scalar&);
  // IOstream operators
    friend Ostream& operator<< <Field, Type>
    (
      Ostream&,
      const FieldField<Field, Type>&
    );
    friend Ostream& operator<< <Field, Type>
    (
      Ostream&,
      const tmp<FieldField<Field, Type> >&
    );
};
}  // namespace mousse
#include "field_field_functions.hpp"
#ifdef NoRepository
#   include "field_field.cpp"
#endif
#endif
