#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_INLET_OUTLET_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_INLET_OUTLET_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::inletOutletFvPatchField
// Group
//   grpOutletBoundaryConditions
// Description
//   This boundary condition provides a generic outflow condition, with
//   specified inflow for the case of return flow.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | Flux field name         | no          | phi
//     inletValue   | Inlet value for reverse flow | yes    |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            inletOutlet;
//     phi             phi;
//     inletValue      uniform 0;
//     value           uniform 0;
//   }
//   \endverbatim
//   The mode of operation is determined by the sign of the flux across the
//   patch faces.
// Note
//   Sign conventions:
//   - Positive flux (out of domain): apply zero-gradient condition
//   - Negative flux (into of domain): apply the "inletValue" fixed-value
// SeeAlso
//   mousse::mixedFvPatchField
//   mousse::zeroGradientFvPatchField
//   mousse::outletInletFvPatchField

#include "mixed_fv_patch_field.hpp"


namespace mousse {

template<class Type>
class inletOutletFvPatchField
:
  public mixedFvPatchField<Type>
{
protected:
  // Protected data
    //- Name of flux field
    word phiName_;
public:
  //- Runtime type information
  TYPE_NAME("inletOutlet");
  // Constructors
    //- Construct from patch and internal field
    inletOutletFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    inletOutletFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given inletOutletFvPatchField onto a new patch
    inletOutletFvPatchField
    (
      const inletOutletFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    inletOutletFvPatchField
    (
      const inletOutletFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new inletOutletFvPatchField<Type>(*this)
      };
    }
    //- Construct as copy setting internal field reference
    inletOutletFvPatchField
    (
      const inletOutletFvPatchField<Type>&,
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
        new inletOutletFvPatchField<Type>{*this, iF}
      };
    }
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
  // Member operators
    virtual void operator=(const fvPatchField<Type>& pvf);
};

}  // namespace mousse

#include "inlet_outlet_fv_patch_field.ipp"

#endif
