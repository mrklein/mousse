#ifndef FINITE_VOLUME_FIELDS_FVS_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FVS_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvsPatchField
// Description
//   An abstract base class with a fat-interface to all derived classes
//   covering all possible ways in which they might be used.
//   The first level of derivation is to basic patchFields which cover
//   zero-gradient, fixed-gradient, fixed-value and mixed conditions.
//   The next level of derivation covers all the specialised typed with
//   specific evaluation proceedures, particularly with respect to specific
//   fields.

#include "fv_patch.hpp"
#include "dimensioned_field.hpp"


namespace mousse {

// Forward declaration of classes
class objectRegistry;
class dictionary;
class fvPatchFieldMapper;
class surfaceMesh;

// Forward declaration of friend functions and operators
template<class Type>
class fvsPatchField;
template<class Type>
Ostream& operator<<(Ostream&, const fvsPatchField<Type>&);


template<class Type>
class fvsPatchField
:
  public Field<Type>
{
  // Private data
    //- Reference to patch
    const fvPatch& patch_;
    //- Reference to internal field
    const DimensionedField<Type, surfaceMesh>& internalField_;
public:
  typedef fvPatch Patch;
  //- Runtime type information
  TYPE_NAME("fvsPatchField");
  //- Debug switch to disallow the use of genericFvsPatchField
  static int disallowGenericFvsPatchField;
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      fvsPatchField,
      patch,
      (
        const fvPatch& p,
        const DimensionedField<Type, surfaceMesh>& iF
      ),
      (p, iF)
    );
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      fvsPatchField,
      patchMapper,
      (
        const fvsPatchField<Type>& ptf,
        const fvPatch& p,
        const DimensionedField<Type, surfaceMesh>& iF,
        const fvPatchFieldMapper& m
      ),
      (dynamic_cast<const fvsPatchFieldType&>(ptf), p, iF, m)
    );
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      fvsPatchField,
      dictionary,
      (
        const fvPatch& p,
        const DimensionedField<Type, surfaceMesh>& iF,
        const dictionary& dict
      ),
      (p, iF, dict)
    );
  // Constructors
    //- Construct from patch and internal field
    fvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct from patch and internal field and patch field
    fvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const Field<Type>&
    );
    //- Construct from patch, internal field and dictionary
    fvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );
    //- Construct by mapping the given fvsPatchField onto a new patch
    fvsPatchField
    (
      const fvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fvsPatchField(const fvsPatchField<Type>&);
    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type>> clone() const
    {
      return tmp<fvsPatchField<Type>>{new fvsPatchField<Type>{*this}};
    }
    //- Construct as copy setting internal field reference
    fvsPatchField
    (
      const fvsPatchField<Type>&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvsPatchField<Type>> clone
    (
      const DimensionedField<Type, surfaceMesh>& iF
    ) const
    {
      return tmp<fvsPatchField<Type>>{new fvsPatchField<Type>{*this, iF}};
    }
  // Selectors
    //- Return a pointer to a new patchField created on freestore given
    //  patch and internal field
    //  (does not set the patch field values)
    static tmp<fvsPatchField<Type>> New
    (
      const word&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Return a pointer to a new patchField created on freestore given
    //  patch and internal field
    //  (does not set the patch field values)
    //  Allows override of constraint type
    static tmp<fvsPatchField<Type>> New
    (
      const word&,
      const word& actualPatchType,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Return a pointer to a new patchField created on freestore from
    //  a given fvsPatchField mapped onto a new patch
    static tmp<fvsPatchField<Type>> New
    (
      const fvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );
    //- Return a pointer to a new patchField created on freestore
    //  from dictionary
    static tmp<fvsPatchField<Type>> New
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );
    //- Return a pointer to a new calculatedFvsPatchField created on
    //  freestore without setting patchField values
    static tmp<fvsPatchField<Type>> NewCalculatedType
    (
      const fvPatch&
    );
    //- Return a pointer to a new calculatedFvsPatchField created on
    //  freestore without setting patchField values
    template<class Type2>
    static tmp<fvsPatchField<Type>> NewCalculatedType
    (
      const fvsPatchField<Type2>&
    );
  //- Destructor
  virtual ~fvsPatchField<Type>()
  {}
  // Member functions
    // Access
      //- Return local objectRegistry
      const objectRegistry& db() const;
      //- Return patch
      const fvPatch& patch() const
      {
        return patch_;
      }
      //- Return dimensioned internal field reference
      const DimensionedField<Type, surfaceMesh>&
      dimensionedInternalField() const
      {
        return internalField_;
      }
      //- Return internal field reference
      const Field<Type>& internalField() const
      {
        return internalField_;
      }
      //- Return the type of the calculated for of fvsPatchField
      static const word& calculatedType();
      //- Return true if this patch field fixes a value.
      //  Needed to check if a level has to be specified while solving
      //  Poissons equations.
      virtual bool fixesValue() const
      {
        return false;
      }
      //- Return true if this patch field is coupled
      virtual bool coupled() const
      {
        return false;
      }
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      );
      //- Reverse map the given fvsPatchField onto this fvsPatchField
      virtual void rmap
      (
        const fvsPatchField<Type>&,
        const labelList&
      );
    //- Write
    virtual void write(Ostream&) const;
    // Check
      //- Check fvsPatchField<Type> against given fvsPatchField<Type>
      void check(const fvsPatchField<Type>&) const;
  // Member operators
    virtual void operator=(const UList<Type>&);
    virtual void operator=(const fvsPatchField<Type>&);
    virtual void operator+=(const fvsPatchField<Type>&);
    virtual void operator-=(const fvsPatchField<Type>&);
    virtual void operator*=(const fvsPatchField<scalar>&);
    virtual void operator/=(const fvsPatchField<scalar>&);
    virtual void operator+=(const Field<Type>&);
    virtual void operator-=(const Field<Type>&);
    virtual void operator*=(const Field<scalar>&);
    virtual void operator/=(const Field<scalar>&);
    virtual void operator=(const Type&);
    virtual void operator+=(const Type&);
    virtual void operator-=(const Type&);
    virtual void operator*=(const scalar);
    virtual void operator/=(const scalar);
    // Force an assignment irrespective of form of patch
    virtual void operator==(const fvsPatchField<Type>&);
    virtual void operator==(const Field<Type>&);
    virtual void operator==(const Type&);
  // Ostream operator
    friend Ostream& operator<< <Type>(Ostream&, const fvsPatchField<Type>&);
};
}  // namespace mousse

#include "fvs_patch_field.ipp"


#include "calculated_fvs_patch_field.hpp"

#define MAKE_FVS_PATCH_TYPE_FIELD_TYPE_NAME(type)                             \
                                                                              \
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(type, 0);


#define MAKE_FVS_PATCH_FIELDS_TYPE_NAME(type)                                 \
                                                                              \
MAKE_FVS_PATCH_TYPE_FIELD_TYPE_NAME(type##FvsPatchScalarField);               \
MAKE_FVS_PATCH_TYPE_FIELD_TYPE_NAME(type##FvsPatchVectorField);               \
MAKE_FVS_PATCH_TYPE_FIELD_TYPE_NAME(type##FvsPatchSphericalTensorField);      \
MAKE_FVS_PATCH_TYPE_FIELD_TYPE_NAME(type##FvsPatchSymmTensorField);           \
MAKE_FVS_PATCH_TYPE_FIELD_TYPE_NAME(type##FvsPatchTensorField);


#define MAKE_FVS_PATCH_TYPE_FIELD(PatchTypeField, typePatchTypeField)         \
                                                                              \
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(typePatchTypeField, 0);             \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  PatchTypeField, typePatchTypeField, patch                                   \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  PatchTypeField,                                                             \
  typePatchTypeField,                                                         \
  patchMapper                                                                 \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  PatchTypeField, typePatchTypeField, dictionary                              \
);


#define MAKE_FVS_PATCH_FIELDS(type)                                           \
                                                                              \
MAKE_FVS_PATCH_TYPE_FIELD(fvsPatchScalarField, type##FvsPatchScalarField);    \
MAKE_FVS_PATCH_TYPE_FIELD(fvsPatchVectorField, type##FvsPatchVectorField);    \
MAKE_FVS_PATCH_TYPE_FIELD\
(                                                                             \
  fvsPatchSphericalTensorField,                                               \
  type##FvsPatchSphericalTensorField                                          \
);                                                                            \
MAKE_FVS_PATCH_TYPE_FIELD                                                     \
(                                                                             \
  fvsPatchSymmTensorField,                                                    \
  type##FvsPatchSymmTensorField                                               \
);                                                                            \
MAKE_FVS_PATCH_TYPE_FIELD(fvsPatchTensorField, type##FvsPatchTensorField);


#define MAKE_FVS_PATCH_TYPE_FIELD_TYPEDEFS(type)                              \
                                                                              \
typedef type##FvsPatchField<scalar> type##FvsPatchScalarField;                \
typedef type##FvsPatchField<vector> type##FvsPatchVectorField;                \
typedef type##FvsPatchField<sphericalTensor>                                  \
  type##FvsPatchSphericalTensorField;                                         \
typedef type##FvsPatchField<symmTensor> type##FvsPatchSymmTensorField;        \
typedef type##FvsPatchField<tensor> type##FvsPatchTensorField;

#endif
