// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedVelocityFluxFixedValueFvPatchField
// Group
//   grpInletBoundaryConditions grpCoupledBoundaryConditions
// Description
//   This boundary condition maps the velocity and flux from a neighbour patch
//   to this patch
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            mappedVelocityFlux;
//     phi             phi;
//     value           uniform 0;  // place holder
//   }
//   \endverbatim
//   The underlying sample mode should be set to \c nearestPatchFace or
//   \c nearestFace
// Note
//   This boundary condition can only be applied to patches that are of
//   the \c mappedPolyPatch type.
// SeeAlso
//   mousse::mappedPatchBase
//   mousse::mappedPolyPatch
//   mousse::mappedFvPatch
//   mousse::fixedValueFvPatchVectorField
// SourceFiles
//   mapped_velocity_flux_fixed_value_fv_patch_field.cpp
#ifndef mapped_velocity_flux_fixed_value_fv_patch_field_hpp_
#define mapped_velocity_flux_fixed_value_fv_patch_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
#include "mapped_fv_patch.hpp"
namespace mousse
{
class mappedVelocityFluxFixedValueFvPatchField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Name of flux field
    word phiName_;
public:
  //- Runtime type information
  TypeName("mappedVelocityFlux");
  // Constructors
    //- Construct from patch and internal field
    mappedVelocityFluxFixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    mappedVelocityFluxFixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  mappedVelocityFluxFixedValueFvPatchField
    //  onto a new patch
    mappedVelocityFluxFixedValueFvPatchField
    (
      const mappedVelocityFluxFixedValueFvPatchField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    mappedVelocityFluxFixedValueFvPatchField
    (
      const mappedVelocityFluxFixedValueFvPatchField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new mappedVelocityFluxFixedValueFvPatchField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    mappedVelocityFluxFixedValueFvPatchField
    (
      const mappedVelocityFluxFixedValueFvPatchField&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchVectorField> clone
    (
      const DimensionedField<vector, volMesh>& iF
    ) const
    {
      return tmp<fvPatchVectorField>
      (
        new mappedVelocityFluxFixedValueFvPatchField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
