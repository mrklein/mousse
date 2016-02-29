#ifndef CORE_FIELDS_POINT_PATCH_FIELDS_BASIC_VALUE_POINT_PATCH_FIELD_HPP_
#define CORE_FIELDS_POINT_PATCH_FIELDS_BASIC_VALUE_POINT_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::valuePointPatchField
// Description
//   mousse::valuePointPatchField
// SourceFiles
//   value_point_patch_field.cpp
#include "point_patch_field.hpp"
namespace mousse
{
template<class Type>
class valuePointPatchField
:
  public pointPatchField<Type>,
  public Field<Type>
{
  // Private Member Functions
    void checkFieldSize() const;
public:
  //- Runtime type information
  TYPE_NAME("value");
  // Constructors
    //- Construct from patch and internal field
    valuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    valuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&,
      const bool valueRequired=true
    );
    //- Construct by mapping given patchField<Type> onto a new patch
    valuePointPatchField
    (
      const valuePointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new valuePointPatchField<Type>
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    valuePointPatchField
    (
      const valuePointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type> > clone
    (
      const DimensionedField<Type, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new valuePointPatchField<Type>
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    // Access
      //- Return size
      label size() const
      {
        return Field<Type>::size();
      }
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const pointPatchFieldMapper&
      );
      //- Reverse map the given PointPatchField onto
      //  this PointPatchField
      virtual void rmap
      (
        const pointPatchField<Type>&,
        const labelList&
      );
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
    //- Write
    virtual void write(Ostream&) const;
  // Member operators
    // Assignment operators
      virtual void operator=
      (
        const valuePointPatchField<Type>&
      );
      virtual void operator=
      (
        const pointPatchField<Type>&
      );
      virtual void operator=(const Field<Type>&);
      virtual void operator=(const Type&);
    // Force an assignment irrespective of form of patch
      virtual void operator==
      (
        const valuePointPatchField<Type>&
      );
      virtual void operator==
      (
        const pointPatchField<Type>&
      );
      virtual void operator==(const Field<Type>&);
      virtual void operator==(const Type&);
};
// This function is added to overide the hack in pointPatchField.H
// which enables simple backward compatability with verions using
// referenceLevel in GeometicField
template<class Type>
tmp<Field<Type> > operator+
(
  const valuePointPatchField<Type>& vsppf,
  const Type& t
)
{
  return static_cast<const Field<Type>&>(vsppf) + t;
}
}  // namespace mousse
#ifdef NoRepository
#    include "value_point_patch_field.cpp"
#endif
#endif
