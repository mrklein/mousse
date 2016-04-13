#ifndef FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_BASIC_SLICED_FVS_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FVS_PATCH_FIELDS_BASIC_SLICED_FVS_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::slicedFvsPatchField
// Description
//   Specialization of fvsPatchField which creates the underlying
//   fvsPatchField as a slice of the given complete field.
//   The destructor is wrapped to avoid deallocation of the storage of the
//   complete fields when this is destroyed.
//   Should only used as a template argument for SlicedGeometricField.

#include "fvs_patch_field.hpp"


namespace mousse {

template<class Type>
class slicedFvsPatchField
:
  public fvsPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("sliced");
  // Constructors
    //- Construct from patch, internal field and field to slice
    slicedFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const Field<Type>&
    );
    //- Construct from patch and internal field
    slicedFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct from patch, internal field and dictionary
    slicedFvsPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const dictionary&
    );
    //- Construct by mapping the given slicedFvsPatchField<Type>
    //  onto a new patch
    slicedFvsPatchField
    (
      const slicedFvsPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, surfaceMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    slicedFvsPatchField(const slicedFvsPatchField<Type>&);
    //- Construct and return a clone
    virtual tmp<fvsPatchField<Type>> clone() const;
    //- Construct as copy setting internal field reference
    slicedFvsPatchField
    (
      const slicedFvsPatchField<Type>&,
      const DimensionedField<Type, surfaceMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvsPatchField<Type>> clone
    (
      const DimensionedField<Type, surfaceMesh>& iF
    ) const;
  //- Destructor
  virtual ~slicedFvsPatchField<Type>();
  // Member functions
    // Access
      //- Return true if this patch field fixes a value.
      //  Needed to check if a level has to be specified while solving
      //  Poissons equations.
      virtual bool fixesValue() const
      {
        return true;
      }
  // Member operators
    virtual void operator=(const UList<Type>&) {}
    virtual void operator=(const fvsPatchField<Type>&) {}
    virtual void operator+=(const fvsPatchField<Type>&) {}
    virtual void operator-=(const fvsPatchField<Type>&) {}
    virtual void operator*=(const fvsPatchField<scalar>&) {}
    virtual void operator/=(const fvsPatchField<scalar>&) {}
    virtual void operator+=(const Field<Type>&) {}
    virtual void operator-=(const Field<Type>&) {}
    virtual void operator*=(const Field<scalar>&) {}
    virtual void operator/=(const Field<scalar>&) {}
    virtual void operator=(const Type&) {}
    virtual void operator+=(const Type&) {}
    virtual void operator-=(const Type&) {}
    virtual void operator*=(const scalar) {}
    virtual void operator/=(const scalar) {}
};
}  // namespace mousse

#include "sliced_fvs_patch_field.ipp"

#endif
