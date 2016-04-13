#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_OUTLET_INLET_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_OUTLET_INLET_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::outletInletFvPatchField
// Group
//   grpInletBoundaryConditions
// Description
//   This boundary condition provides a generic inflow condition, with
//   specified outflow for the case of reverse flow.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | Flux field name         | no          | phi
//     outletValue  | Outlet value for reverse flow | yes   |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            outletInlet;
//     phi             phi;
//     outletValue     uniform 0;
//     value           uniform 0;
//   }
//   \endverbatim
//   The mode of operation is determined by the sign of the flux across the
//   patch faces.
// Note
//   Sign conventions:
//   - Positive flux (out of domain): apply the "outletValue" fixed-value
//   - Negative flux (into of domain): apply zero-gradient condition
// SeeAlso
//   mousse::mixedFvPatchField
//   mousse::zeroGradientFvPatchField
//   mousse::inletOutletFvPatchField

#include "mixed_fv_patch_field.hpp"


namespace mousse {

template<class Type>
class outletInletFvPatchField
:
  public mixedFvPatchField<Type>
{
protected:
  // Protected data
    //- Name of flux field
    word phiName_;
public:
  //- Runtime type information
  TYPE_NAME("outletInlet");
  // Constructors
    //- Construct from patch and internal field
    outletInletFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    outletInletFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given outletInletFvPatchField onto a new patch
    outletInletFvPatchField
    (
      const outletInletFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    outletInletFvPatchField
    (
      const outletInletFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new outletInletFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    outletInletFvPatchField
    (
      const outletInletFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type>> clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type>>
      {
        new outletInletFvPatchField<Type>{*this, iF}
      };
    }
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse

#include "outlet_inlet_fv_patch_field.ipp"

#endif
