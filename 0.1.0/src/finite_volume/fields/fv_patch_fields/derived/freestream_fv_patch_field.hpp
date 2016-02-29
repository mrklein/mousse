#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FREESTREAM_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FREESTREAM_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::freestreamFvPatchField
// Group
//   grpInletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This boundary condition provides a free-stream condition.  It is a 'mixed'
//   condition derived from the \c inletOutlet condition, whereby the mode of
//   operation switches between fixed (free stream) value and zero gradient
//   based on the sign of the flux.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     freestreamValue   | freestream velocity          | yes         |
//     phi          | flux field name         | no          | phi
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            freestream;
//     phi             phi;
//   }
//   \endverbatim
// SeeAlso
//   mousse::mixedFvPatchField
//   mousse::inletOutletFvPatchField
// SourceFiles
//   freestream_fv_patch_field.cpp
#include "inlet_outlet_fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class freestreamFvPatchField
:
  public inletOutletFvPatchField<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("freestream");
  // Constructors
    //- Construct from patch and internal field
    freestreamFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    freestreamFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given freestreamFvPatchField onto a new patch
    freestreamFvPatchField
    (
      const freestreamFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    freestreamFvPatchField
    (
      const freestreamFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new freestreamFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    freestreamFvPatchField
    (
      const freestreamFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type> > clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type> >
      (
        new freestreamFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Return defining fields
      const Field<Type>& freestreamValue() const
      {
        return this->refValue();
      }
      Field<Type>& freestreamValue()
      {
        return this->refValue();
      }
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "freestream_fv_patch_field.cpp"
#endif
#endif
