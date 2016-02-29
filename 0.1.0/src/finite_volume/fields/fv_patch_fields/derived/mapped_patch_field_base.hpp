#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_MAPPED_PATCH_FIELD_BASE_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_MAPPED_PATCH_FIELD_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedPatchFieldBase
// Description
//   Functionality for sampling fields using mappedPatchBase. Every call to
//   mappedField() returns a sampled field, optionally scaled to maintain an
//   area-weighted average.
//   Example usage:
//   {
//     fieldName           T;          // default is same as fvPatchField
//     setAverage          false;
//     average             1.0;        // only if setAverage=true
//     interpolationScheme cellPoint;  // default is cell
//   }
// SourceFiles
//   mapped_patch_field_base.cpp
#include "fixed_value_fv_patch_fields.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
class mappedPatchBase;
template<class> class interpolation;
template<class Type>
class mappedPatchFieldBase
{
protected:
  // Protected data
    //- Mapping engine
    const mappedPatchBase& mapper_;
    //- Underlying patch field
    const fvPatchField<Type>& patchField_;
    //- Name of field to sample
    word fieldName_;
    //- If true adjust the mapped field to maintain average value average_
    const bool setAverage_;
    //- Average value the mapped field is adjusted to maintain if
    //  setAverage_ is set true
    const Type average_;
    //- Interpolation scheme to use for nearestcell mode
    word interpolationScheme_;
public:
  // Constructors
    //- Construct from components
    mappedPatchFieldBase
    (
      const mappedPatchBase& mapper,
      const fvPatchField<Type>& patchField,
      const word& fieldName,
      const bool setAverage,
      const Type average,
      const word& interpolationScheme
    );
    //- Construct from dictionary
    mappedPatchFieldBase
    (
      const mappedPatchBase& mapper,
      const fvPatchField<Type>& patchField,
      const dictionary& dict
    );
    //- Construct empty
    mappedPatchFieldBase
    (
      const mappedPatchBase& mapper,
      const fvPatchField<Type>& patchField
    );
    //- Construct copy
    mappedPatchFieldBase
    (
      const mappedPatchFieldBase<Type>& mapper
    );
    //- Construct copy, resetting patch and field
    mappedPatchFieldBase
    (
      const mappedPatchBase& mapper,
      const fvPatchField<Type>& patchField,
      const mappedPatchFieldBase<Type>& base
    );
  //- Destructor
  virtual ~mappedPatchFieldBase<Type>()
  {}
  // Member functions
    //- Field to sample. Either on my or nbr mesh
    const GeometricField<Type, fvPatchField, volMesh>& sampleField() const;
    //- Map sampleField onto *this patch
    virtual tmp<Field<Type> > mappedField() const;
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "mapped_patch_field_base.cpp"
#endif
#endif
