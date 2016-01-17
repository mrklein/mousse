// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvPatchField
// Description
//   Abstract base class with a fat-interface to all derived classes
//   covering all possible ways in which they might be used.
//   The first level of derivation is to basic patchFields which cover
//   zero-gradient, fixed-gradient, fixed-value and mixed conditions.
//   The next level of derivation covers all the specialised types with
//   specific evaluation proceedures, particularly with respect to specific
//   fields.
// SourceFiles
//   fv_patch_field.cpp
//   fv_patch_field_new.cpp

#ifndef fv_patch_field_hpp_
#define fv_patch_field_hpp_

#include "fv_patch.hpp"
#include "dimensioned_field.hpp"
#include "pstream.hpp"

namespace mousse
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class fvPatchFieldMapper;
class volMesh;

// Forward declaration of friend functions and operators
template<class Type>
class fvPatchField;
template<class Type>
class fvMatrix;
template<class Type>
Ostream& operator<<(Ostream&, const fvPatchField<Type>&);

template<class Type>
class fvPatchField
:
  public Field<Type>
{
  // Private data

    //- Reference to patch
    const fvPatch& patch_;

    //- Reference to internal field
    const DimensionedField<Type, volMesh>& internalField_;

    //- Update index used so that updateCoeffs is called only once during
    //  the construction of the matrix
    bool updated_;

    //- Update index used so that manipulateMatrix is called only once
    //  during the construction of the matrix
    bool manipulatedMatrix_;

    //- Optional patch type, used to allow specified boundary conditions
    //  to be applied to constraint patches by providing the constraint
    //  patch type as 'patchType'
    word patchType_;

public:

  typedef fvPatch Patch;

  //- Runtime type information
  TYPE_NAME("fvPatchField");

  //- Debug switch to disallow the use of genericFvPatchField
  static int disallowGenericFvPatchField;
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      fvPatchField,
      patch,
      (
        const fvPatch& p,
        const DimensionedField<Type, volMesh>& iF
      ),
      (p, iF)
    );

    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      fvPatchField,
      patchMapper,
      (
        const fvPatchField<Type>& ptf,
        const fvPatch& p,
        const DimensionedField<Type, volMesh>& iF,
        const fvPatchFieldMapper& m
      ),
      (dynamic_cast<const fvPatchFieldType&>(ptf), p, iF, m)
    );

    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      fvPatchField,
      dictionary,
      (
        const fvPatch& p,
        const DimensionedField<Type, volMesh>& iF,
        const dictionary& dict
      ),
      (p, iF, dict)
    );

  // Constructors

    //- Construct from patch and internal field
    fvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );

    //- Construct from patch and internal field and patch type
    fvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const word& patchType
    );

    //- Construct from patch and internal field and patch field
    fvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const Field<Type>&
    );

    //- Construct from patch, internal field and dictionary
    fvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&,
      const bool valueRequired=false
    );

    //- Construct by mapping the given fvPatchField onto a new patch
    fvPatchField
    (
      const fvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );

    //- Construct as copy
    fvPatchField(const fvPatchField<Type>&);

    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>(new fvPatchField<Type>(*this));
    }

    //- Construct as copy setting internal field reference
    fvPatchField
    (
      const fvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );

    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type>> clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type>>{new fvPatchField<Type>{*this, iF}};
    }

  // Selectors

    //- Return a pointer to a new patchField created on freestore given
    //  patch and internal field
    //  (does not set the patch field values)
    static tmp<fvPatchField<Type>> New
    (
      const word&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );

    //- Return a pointer to a new patchField created on freestore given
    //  patch and internal field
    //  (does not set the patch field values).
    //  Allows override of constraint type
    static tmp<fvPatchField<Type>> New
    (
      const word&,
      const word& actualPatchType,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );

    //- Return a pointer to a new patchField created on freestore from
    //  a given fvPatchField mapped onto a new patch
    static tmp<fvPatchField<Type>> New
    (
      const fvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );

    //- Return a pointer to a new patchField created on freestore
    //  from dictionary
    static tmp<fvPatchField<Type>> New
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );

    //- Return a pointer to a new calculatedFvPatchField created on
    //  freestore without setting patchField values
    static tmp<fvPatchField<Type>> NewCalculatedType
    (
      const fvPatch&
    );

    //- Return a pointer to a new calculatedFvPatchField created on
    //  freestore without setting patchField values
    template<class Type2>
    static tmp<fvPatchField<Type>> NewCalculatedType
    (
      const fvPatchField<Type2>&
    );

  //- Destructor
  virtual ~fvPatchField<Type>()
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
      const DimensionedField<Type, volMesh>&
      dimensionedInternalField() const
      {
        return internalField_;
      }

      //- Return internal field reference
      const Field<Type>& internalField() const
      {
        return internalField_;
      }

      //- Optional patch type
      const word& patchType() const
      {
        return patchType_;
      }

      //- Optional patch type
      word& patchType()
      {
        return patchType_;
      }

      //- Return the type of the calculated for of fvPatchField
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

      //- Return true if the boundary condition has already been updated
      bool updated() const
      {
        return updated_;
      }

      //- Return true if the matrix has already been manipulated
      bool manipulatedMatrix() const
      {
        return manipulatedMatrix_;
      }

    // Mapping functions

      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      );

      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchField<Type>&,
        const labelList&
      );

    // Evaluation functions

      //- Return patch-normal gradient
      virtual tmp<Field<Type> > snGrad() const;

      //- Return patch-normal gradient for coupled-patches
      //  using the deltaCoeffs provided
      virtual tmp<Field<Type> > snGrad
      (
        const scalarField& /*deltaCoeffs*/
      ) const
      {
        NOT_IMPLEMENTED
        (
          type() + "::snGrad(const scalarField& deltaCoeffs)"
        );
        return *this;
      }

      //- Update the coefficients associated with the patch field
      //  Sets Updated to true
      virtual void updateCoeffs();

      //- Update the coefficients associated with the patch field
      //  and apply weight field
      //  Sets Updated to true
      virtual void updateCoeffs(const scalarField& weights);

      //- Return internal field next to patch as patch field
      virtual tmp<Field<Type>> patchInternalField() const;

      //- Return internal field next to patch as patch field
      virtual void patchInternalField(Field<Type>&) const;

      //- Return patchField on the opposite patch of a coupled patch
      virtual tmp<Field<Type>> patchNeighbourField() const
      {
        NOT_IMPLEMENTED(type() + "patchNeighbourField()");
        return *this;
      }

      //- Initialise the evaluation of the patch field
      virtual void initEvaluate
      (
        const Pstream::commsTypes=Pstream::blocking
      )
      {}

      //- Evaluate the patch field, sets Updated to false
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );

      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type>> valueInternalCoeffs
      (
        const tmp<Field<scalar>>&
      ) const
      {
        NOT_IMPLEMENTED
        (
          type()
         + "::valueInternalCoeffs(const tmp<Field<scalar> >&)"
        );
        return *this;
      }

      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the value of this patchField with given weights
      virtual tmp<Field<Type>> valueBoundaryCoeffs
      (
        const tmp<Field<scalar>>&
      ) const
      {
        NOT_IMPLEMENTED
        (
          type()
         + "::valueBoundaryCoeffs(const tmp<Field<scalar> >&)"
        );
        return *this;
      }

      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type>> gradientInternalCoeffs() const
      {
        NOT_IMPLEMENTED(type() + "::gradientInternalCoeffs()");
        return *this;
      }

      //- Return the matrix diagonal coefficients corresponding to the
      //  evaluation of the gradient of this coupled patchField
      //  using the deltaCoeffs provided
      virtual tmp<Field<Type>> gradientInternalCoeffs
      (
        const scalarField& /*deltaCoeffs*/
      ) const
      {
        NOT_IMPLEMENTED
        (
          type()
         + "::gradientInternalCoeffs(const scalarField& deltaCoeffs)"
        );
        return *this;
      }

      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the gradient of this patchField
      virtual tmp<Field<Type>> gradientBoundaryCoeffs() const
      {
        NOT_IMPLEMENTED(type() + "::gradientBoundaryCoeffs()");
        return *this;
      }

      //- Return the matrix source coefficients corresponding to the
      //  evaluation of the gradient of this coupled patchField
      //  using the deltaCoeffs provided
      virtual tmp<Field<Type>> gradientBoundaryCoeffs
      (
        const scalarField& /*deltaCoeffs*/
      ) const
      {
        NOT_IMPLEMENTED
        (
          type()
         + "::gradientBoundaryCoeffs(const scalarField& deltaCoeffs)"
        );
        return *this;
      }

      //- Manipulate matrix
      virtual void manipulateMatrix(fvMatrix<Type>& matrix);

      //- Manipulate matrix with given weights
      virtual void manipulateMatrix
      (
        fvMatrix<Type>& matrix,
        const scalarField& weights
      );

    // I-O

      //- Write
      virtual void write(Ostream&) const;

      //- Helper function to write the keyword and entry only if the
      //  values are not equal. The value is then output as value2
      template<class EntryType>
      void writeEntryIfDifferent
      (
        Ostream& os,
        const word& entryName,
        const EntryType& value1,
        const EntryType& value2
      ) const;

    // Check

      //- Check fvPatchField<Type> against given fvPatchField<Type>
      void check(const fvPatchField<Type>&) const;

  // Member operators
    virtual void operator=(const UList<Type>&);
    virtual void operator=(const fvPatchField<Type>&);
    virtual void operator+=(const fvPatchField<Type>&);
    virtual void operator-=(const fvPatchField<Type>&);
    virtual void operator*=(const fvPatchField<scalar>&);
    virtual void operator/=(const fvPatchField<scalar>&);
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
    virtual void operator==(const fvPatchField<Type>&);
    virtual void operator==(const Field<Type>&);
    virtual void operator==(const Type&);

  // Ostream operator
    friend Ostream& operator<< <Type>(Ostream&, const fvPatchField<Type>&);
};

}  // namespace mousse
#ifdef NoRepository
#   include "fv_patch_field.cpp"
#   include "calculated_fv_patch_field.hpp"
#endif

#define MAKE_FV_PATCH_FIELD(fvPatchTypeField)                                 \
                                                                              \
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(fvPatchTypeField, 0);               \
template<>                                                                    \
int fvPatchTypeField::disallowGenericFvPatchField                             \
(                                                                             \
  debug::debugSwitch("disallowGenericFvPatchField", 0)                        \
);                                                                            \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(fvPatchTypeField, patch);            \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(fvPatchTypeField, patchMapper);      \
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(fvPatchTypeField, dictionary);

#define ADD_TO_PATCH_FIELD_RUN_TIME_SELECTION\
(PatchTypeField, typePatchTypeField)                                          \
  ADD_TO_RUN_TIME_SELECTION_TABLE                                             \
  (                                                                           \
    PatchTypeField,                                                           \
    typePatchTypeField,                                                       \
    patch                                                                     \
  );                                                                          \
  ADD_TO_RUN_TIME_SELECTION_TABLE                                             \
  (                                                                           \
    PatchTypeField,                                                           \
    typePatchTypeField,                                                       \
    patchMapper                                                               \
  );                                                                          \
  ADD_TO_RUN_TIME_SELECTION_TABLE                                             \
  (                                                                           \
    PatchTypeField,                                                           \
    typePatchTypeField,                                                       \
    dictionary                                                                \
  );

// use with caution
#define ADD_REMOVABLE_TO_PATCH_FIELD_RUN_TIME_SELECTION                       \
(PatchTypeField, typePatchTypeField)                                          \
                                                                              \
  ADD_REMOVABLE_TO_RUN_TIME_SELECTION_TABLE                                   \
  (                                                                           \
    PatchTypeField,                                                           \
    typePatchTypeField,                                                       \
    patch                                                                     \
  );                                                                          \
  ADD_REMOVABLE_TO_RUN_TIME_SELECTION_TABLE                                   \
  (                                                                           \
    PatchTypeField,                                                           \
    typePatchTypeField,                                                       \
    patchMapper                                                               \
  );                                                                          \
  ADD_REMOVABLE_TO_RUN_TIME_SELECTION_TABLE                                   \
  (                                                                           \
    PatchTypeField,                                                           \
    typePatchTypeField,                                                       \
    dictionary                                                                \
  );

// for non-templated patch fields
#define MAKE_PATCH_TYPE_FIELD(PatchTypeField, typePatchTypeField)             \
  DEFINE_TYPE_NAME_AND_DEBUG(typePatchTypeField, 0);                          \
  ADD_TO_PATCH_FIELD_RUN_TIME_SELECTION(PatchTypeField, typePatchTypeField)

// for non-templated patch fields - use with caution
#define MAKE_REMOVABLE_PATCH_TYPE_FIELD(PatchTypeField, typePatchTypeField)   \
  DEFINE_TYPE_NAME_AND_DEBUG(typePatchTypeField, 0);                          \
  ADD_REMOVABLE_TO_PATCH_FIELD_RUN_TIME_SELECTION\
  (\
    PatchTypeField,\
    typePatchTypeField\
  )

// for templated patch fields
#define MAKE_TEMPLATE_PATCH_TYPE_FIELD(PatchTypeField, typePatchTypeField)    \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(typePatchTypeField, 0);           \
  ADD_TO_PATCH_FIELD_RUN_TIME_SELECTION(PatchTypeField, typePatchTypeField)

#define MAKE_PATCH_FIELDS(type)                                               \
  MAKE_TEMPLATE_PATCH_TYPE_FIELD                                              \
  (                                                                           \
    fvPatchScalarField,                                                       \
    type##FvPatchScalarField                                                  \
  );                                                                          \
  MAKE_TEMPLATE_PATCH_TYPE_FIELD                                              \
  (                                                                           \
    fvPatchVectorField,                                                       \
    type##FvPatchVectorField                                                  \
  );                                                                          \
  MAKE_TEMPLATE_PATCH_TYPE_FIELD                                              \
  (                                                                           \
    fvPatchSphericalTensorField,                                              \
    type##FvPatchSphericalTensorField                                         \
  );                                                                          \
  MAKE_TEMPLATE_PATCH_TYPE_FIELD                                              \
  (                                                                           \
    fvPatchSymmTensorField,                                                   \
    type##FvPatchSymmTensorField                                              \
  );                                                                          \
  MAKE_TEMPLATE_PATCH_TYPE_FIELD                                              \
  (                                                                           \
    fvPatchTensorField,                                                       \
    type##FvPatchTensorField                                                  \
  );

#define MAKE_PATCH_FIELDS_TYPE_NAME(type)                                     \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(type##FvPatchScalarField, 0);     \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(type##FvPatchVectorField, 0);     \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG\
  (\
    type##FvPatchSphericalTensorField,\
   0\
  );                                                                          \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(type##FvPatchSymmTensorField, 0); \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(type##FvPatchTensorField, 0)

#define MAKE_PATCH_TYPE_FIELD_TYPEDEFS(type)                                      \
  typedef type##FvPatchField<scalar> type##FvPatchScalarField;                \
  typedef type##FvPatchField<vector> type##FvPatchVectorField;                \
  typedef type##FvPatchField<sphericalTensor>                                 \
    type##FvPatchSphericalTensorField;                                        \
  typedef type##FvPatchField<symmTensor> type##FvPatchSymmTensorField;        \
  typedef type##FvPatchField<tensor> type##FvPatchTensorField;

#endif
