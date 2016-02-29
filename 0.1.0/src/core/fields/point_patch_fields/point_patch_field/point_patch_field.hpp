#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_POINT_PATCH_FIELD_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointPatchField
// Description
//   Abstract base class for point-mesh patch fields.
//   The base-field does not store values as they are part of the
//   "internal field".  There are derived classes to store constraint values
//   e.g. fixedValuePointPatchField derived from the generic
//   valuePointPatchField which ensures the values in the "internal field"
//   are reset to the fixed-values by applying the stored values.
// SourceFiles
//   point_patch_field.cpp
//   new_point_patch_field.cpp


#include "point_patch.hpp"
#include "dimensioned_field.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "pstream.hpp"

namespace mousse
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class pointPatchFieldMapper;
class pointMesh;

// Forward declaration of friend functions and operators
template<class Type>
class pointPatchField;
template<class Type>
Ostream& operator<<
(
  Ostream&,
  const pointPatchField<Type>&
);

template<class Type>
class pointPatchField
{
  // Private data
    //- Reference to patch
    const pointPatch& patch_;
    //- Reference to internal field
    const DimensionedField<Type, pointMesh>& internalField_;
    //- Update index used so that updateCoeffs is called only once during
    //  the construction of the matrix
    bool updated_;
    //- Optional patch type, used to allow specified boundary conditions
    //  to be applied to constraint patches by providing the constraint
    //  patch type as 'patchType'
    word patchType_;

public:
  typedef pointPatch Patch;

  //- Runtime type information
  TYPE_NAME("pointPatchField");

  //- Debug switch to disallow the use of genericPointPatchField
  static int disallowGenericPointPatchField;

  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      pointPatchField,
      pointPatch,
      (
        const pointPatch& p,
        const DimensionedField<Type, pointMesh>& iF
      ),
      (p, iF)
    );

    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      pointPatchField,
      patchMapper,
      (
        const pointPatchField<Type>& ptf,
        const pointPatch& p,
        const DimensionedField<Type, pointMesh>& iF,
        const pointPatchFieldMapper& m
      ),
      (dynamic_cast<const pointPatchFieldType&>(ptf), p, iF, m)
    );

    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      pointPatchField,
      dictionary,
      (
        const pointPatch& p,
        const DimensionedField<Type, pointMesh>& iF,
        const dictionary& dict
      ),
      (p, iF, dict)
    );

  // Constructors
    //- Construct from patch and internal field
    pointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );

    //- Construct from patch, internal field and dictionary
    pointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );

    //- Construct by mapping given patchField<Type> onto a new patch
    pointPatchField
    (
      const pointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );

    //- Construct as copy
    pointPatchField(const pointPatchField<Type>&);

    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type>> clone() const = 0;

    //- Construct as copy setting internal field reference
    pointPatchField
    (
      const pointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );

    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type>> clone
    (
      const DimensionedField<Type, pointMesh>& iF
    ) const = 0;

  // Selectors
    //- Return a pointer to a new patchField created on freestore given
    //  patch and internal field
    //  (does not set the patch field values)
    static autoPtr<pointPatchField<Type>> New
    (
      const word&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );

    //- Return a pointer to a new patchField created on freestore given
    //  patch and internal field
    //  (does not set the patch field values).
    //  Allows override of constraint type
    static autoPtr<pointPatchField<Type>> New
    (
      const word&,
      const word& actualPatchType,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );

    //- Return a pointer to a new patchField created on freestore from
    //  a given pointPatchField mapped onto a new patch
    static autoPtr<pointPatchField<Type>> New
    (
      const pointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );

    //- Return a pointer to a new patchField created on freestore
    //  from dictionary
    static autoPtr<pointPatchField<Type>> New
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );

    //- Return a pointer to a new CalculatedpointPatchField created on
    //  freestore without setting patchField values
    template<class Type2>
    static autoPtr<pointPatchField<Type>>
    NewCalculatedType
    (
      const pointPatchField<Type2>&
    );

  //- Destructor
  virtual ~pointPatchField<Type>()
  {}

  // Member functions
    // Access
      //- Return local objectRegistry
      const objectRegistry& db() const;

      //- Return size
      label size() const
      {
        return patch().size();
      }

      //- Return patch
      const pointPatch& patch() const
      {
        return patch_;
      }

      //- Return dimensioned internal field reference
      const DimensionedField<Type, pointMesh>&
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

      //- Return true if this patch field fixes a value
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

      //- Return field created from appropriate internal field values
      tmp<Field<Type>> patchInternalField() const;

      //- Return field created from appropriate internal field values
      //  given internal field reference
      template<class Type1>
      tmp<Field<Type1>> patchInternalField
      (
        const Field<Type1>& iF
      ) const;

      //- Return field created from selected internal field values
      //  given internal field reference
      template<class Type1>
      tmp<Field<Type1>> patchInternalField
      (
        const Field<Type1>& iF,
        const labelList& meshPoints
      ) const;

      //- Given the internal field and a patch field,
      //  add the patch field to the internal field
      template<class Type1>
      void addToInternalField
      (
        Field<Type1>& iF,
        const Field<Type1>& pF
      ) const;

      //- Given the internal field and a patch field,
      //  add selected elements of the patch field to the internal field
      template<class Type1>
      void addToInternalField
      (
        Field<Type1>& iF,
        const Field<Type1>& pF,
        const labelList& points
      ) const;

      //- Given the internal field and a patch field,
      //  set the patch field in the internal field
      template<class Type1>
      void setInInternalField
      (
        Field<Type1>& iF,
        const Field<Type1>& pF,
        const labelList& meshPoints
      ) const;

      //- Given the internal field and a patch field,
      //  set the patch field in the internal field
      template<class Type1>
      void setInInternalField
      (
        Field<Type1>& iF,
        const Field<Type1>& pF
      ) const;

      //- Return the type of the calculated form of pointPatchField
      static const word& calculatedType();

      //- Return the constraint type this pointPatchField implements.
      virtual const word& constraintType() const
      {
        return word::null;
      }

    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const pointPatchFieldMapper&
      )
      {}

      //- Reverse map the given pointPatchField onto this pointPatchField
      virtual void rmap
      (
        const pointPatchField<Type>&,
        const labelList&
      )
      {}

    // Evaluation functions
      //- Update the coefficients associated with the patch field
      //  Sets Updated to true
      virtual void updateCoeffs()
      {
        updated_ = true;
      }

      //- Initialise evaluation of the patch field (do nothing)
      virtual void initEvaluate
      (
        const Pstream::commsTypes /*commsType*/=Pstream::blocking
      )
      {}

      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );

    //- Write
    virtual void write(Ostream&) const;

  // Member operators
    virtual void operator=
    (
      const pointPatchField<Type>&
    )
    {}

    virtual void operator+=
    (
      const pointPatchField<Type>&
    )
    {}

    virtual void operator-=
    (
      const pointPatchField<Type>&
    )
    {}

    virtual void operator*=
    (
      const pointPatchField<scalar>&
    )
    {}

    virtual void operator/=
    (
      const pointPatchField<scalar>&
    )
    {}

    virtual void operator=(const Field<Type>&){}

    virtual void operator+=(const Field<Type>&){}

    virtual void operator-=(const Field<Type>&){}

    virtual void operator*=(const Field<scalar>&){}

    virtual void operator/=(const Field<scalar>&){}

    virtual void operator=(const Type&){}

    virtual void operator+=(const Type&){}

    virtual void operator-=(const Type&){}

    virtual void operator*=(const scalar){}

    virtual void operator/=(const scalar){}

    // Force an assignment irrespective of form of patch
    // By generic these do nothing unless the patch actually has boundary
    // values
      virtual void operator==
      (
        const pointPatchField<Type>&
      )
      {}

      virtual void operator==(const Field<Type>&){}

      virtual void operator==(const Type&){}

  // Ostream operator
    friend Ostream& operator<< <Type>
    (
      Ostream&,
      const pointPatchField<Type>&
    );

};
// This function is added as a hack to enable simple backward compatability
// with verions using referenceLevel in GeometicField
template<class Type>
const pointPatchField<Type>& operator+
(
  const pointPatchField<Type>& ppf,
  const Type&
)
{
  return ppf;
}
}  // namespace mousse
#include "point_patch_field_functions.hpp"

#ifdef NoRepository
#   include "point_patch_field.cpp"
#   include "calculated_point_patch_field.hpp"
#endif

#define ADD_TO_POINT_PATCH_FIELD_RUN_TIME_SELECTION\
(PatchTypeField, typePatchTypeField)                                          \
  ADD_TO_RUN_TIME_SELECTION_TABLE                                             \
  (                                                                           \
    PatchTypeField,                                                           \
    typePatchTypeField,                                                       \
    pointPatch                                                                \
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

// for non-templated patch fields
#define MAKE_POINT_PATCH_TYPE_FIELD(PatchTypeField,typePatchTypeField)        \
  DEFINE_TYPE_NAME_AND_DEBUG(typePatchTypeField, 0);                          \
  ADD_TO_POINT_PATCH_FIELD_RUN_TIME_SELECTION                                 \
  (                                                                           \
    PatchTypeField,                                                           \
    typePatchTypeField                                                        \
  )

// for templated patch fields
#define MAKE_TEMPLATE_POINT_PATCH_TYPE_FIELD\
(PatchTypeField, typePatchTypeField)                                          \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(typePatchTypeField, 0);           \
  ADD_TO_POINT_PATCH_FIELD_RUN_TIME_SELECTION                                 \
  (                                                                           \
    PatchTypeField,                                                           \
    typePatchTypeField                                                        \
  )

#define MAKE_POINT_PATCH_FIELDS(type)                                         \
  MAKE_TEMPLATE_POINT_PATCH_TYPE_FIELD                                        \
  (                                                                           \
    pointPatchScalarField,                                                    \
    type##PointPatchScalarField                                               \
  );                                                                          \
  MAKE_TEMPLATE_POINT_PATCH_TYPE_FIELD                                        \
  (                                                                           \
    pointPatchVectorField,                                                    \
    type##PointPatchVectorField                                               \
  );                                                                          \
  MAKE_TEMPLATE_POINT_PATCH_TYPE_FIELD                                        \
  (                                                                           \
    pointPatchSphericalTensorField,                                           \
    type##PointPatchSphericalTensorField                                      \
  );                                                                          \
  MAKE_TEMPLATE_POINT_PATCH_TYPE_FIELD                                        \
  (                                                                           \
    pointPatchSymmTensorField,                                                \
    type##PointPatchSymmTensorField                                           \
  );                                                                          \
  MAKE_TEMPLATE_POINT_PATCH_TYPE_FIELD                                        \
  (                                                                           \
    pointPatchTensorField,                                                    \
    type##PointPatchTensorField                                               \
);

#define MAKE_POINT_PATCH_FIELDS_TYPE_NAME(type)                               \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(type##PointPatchScalarField, 0);  \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(type##PointPatchVectorField, 0);  \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    type##PointPatchSphericalTensorField, 0                                   \
  );                                                                          \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    type##PointPatchSymmTensorField,                                          \
    0                                                                         \
  );                                                                          \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(type##PointPatchTensorField, 0)

#define MAKE_POINT_PATCH_FIELD_TYPEDEFS(type)                                 \
  typedef type##PointPatchField<scalar> type##PointPatchScalarField;          \
  typedef type##PointPatchField<vector> type##PointPatchVectorField;          \
  typedef type##PointPatchField<sphericalTensor>                              \
    type##PointPatchSphericalTensorField;                                     \
  typedef type##PointPatchField<symmTensor> type##PointPatchSymmTensorField;  \
  typedef type##PointPatchField<tensor> type##PointPatchTensorField;

#endif
