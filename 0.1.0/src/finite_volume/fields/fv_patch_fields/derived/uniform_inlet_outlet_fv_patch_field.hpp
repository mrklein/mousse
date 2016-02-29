#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_INLET_OUTLET_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_INLET_OUTLET_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformInletOutletFvPatchField
// Group
//   grpOutletBoundaryConditions
// Description
//   Variant of inletOutlet boundary condition with uniform inletValue.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//     uniformInletValue   | inlet value for reverse flow | yes    |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type                uniformInletOutlet;
//     phi                 phi;
//     uniformInletValue   0;
//     value               uniform 0;
//   }
//   \endverbatim
//   The mode of operation is determined by the sign of the flux across the
//   patch faces.
// Note
//   Sign conventions:
//   - positive flux (out of domain): apply zero-gradient condition
//   - negative flux (into of domain): apply the user-specified fixed value
// SeeAlso
//   mousse::inletOutletFvPatchField
// SourceFiles
//   uniform_inlet_outlet_fv_patch_field.cpp
#include "mixed_fv_patch_field.hpp"
#include "data_entry.hpp"
namespace mousse
{
template<class Type>
class uniformInletOutletFvPatchField
:
  public mixedFvPatchField<Type>
{
protected:
  // Protected data
    //- Name of flux field
    word phiName_;
    //- Value
    autoPtr<DataEntry<Type> > uniformInletValue_;
public:
  //- Runtime type information
  TYPE_NAME("uniformInletOutlet");
  // Constructors
    //- Construct from patch and internal field
    uniformInletOutletFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    uniformInletOutletFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given uniformInletOutletFvPatchField
    //  onto a new patch
    uniformInletOutletFvPatchField
    (
      const uniformInletOutletFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    uniformInletOutletFvPatchField
    (
      const uniformInletOutletFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new uniformInletOutletFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    uniformInletOutletFvPatchField
    (
      const uniformInletOutletFvPatchField<Type>&,
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
        new uniformInletOutletFvPatchField<Type>(*this, iF)
      );
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
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
  // Member operators
    virtual void operator=(const fvPatchField<Type>& pvf);
};
}  // namespace mousse
#ifdef NoRepository
#   include "uniform_inlet_outlet_fv_patch_field.cpp"
#endif
#endif
