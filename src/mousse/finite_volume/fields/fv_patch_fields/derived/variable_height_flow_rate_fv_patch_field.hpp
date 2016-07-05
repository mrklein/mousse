#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_VARIABLE_HEIGHT_FLOW_RATE_FV_PATCH_SCALAR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_VARIABLE_HEIGHT_FLOW_RATE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::variableHeightFlowRateFvPatchScalarField
// Group
//   grpInletBoundaryConditions
// Description
//   This boundary condition provides a phase fraction condition based on the
//   local flow conditions, whereby the values are constrained to lay between
//   user-specified upper and lower bounds.  The behaviour is described by:
//   if alpha > upperBound:
//   - apply a fixed value condition, with a uniform level of the upper bound
//   if lower bound <= alpha <= upper bound:
//   - apply a  zero-gradient condition
//   if alpha < lowerBound:
//   - apply a fixed value condition, with a uniform level of the lower bound
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//     lowerBound   | lower bound for clipping | yes        |
//     upperBound   | upper bound for clipping | yes        |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            variableHeightFlowRate;
//     lowerBound      0.0;
//     upperBound      0.9;
//     value           uniform 0;
//   }
//   \endverbatim

#include "mixed_fv_patch_fields.hpp"


namespace mousse {

class variableHeightFlowRateFvPatchScalarField
:
  public mixedFvPatchScalarField
{
protected:
  // Protected data
    //- Name of flux field
    word phiName_;
    //- Lower bound for alpha1
    scalar lowerBound_;
    //- Upper bound for alpha1
    scalar upperBound_;
public:
  //- Runtime scalar information
  TYPE_NAME("variableHeightFlowRate");
  // Constructors
    //- Construct from patch and internal field
    variableHeightFlowRateFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    variableHeightFlowRateFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  variableHeightFlowRateFvPatchScalarField onto a new patch
    variableHeightFlowRateFvPatchScalarField
    (
      const variableHeightFlowRateFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    variableHeightFlowRateFvPatchScalarField
    (
      const variableHeightFlowRateFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<scalar>> clone() const
    {
      return tmp<fvPatchField<scalar>>
      {
        new variableHeightFlowRateFvPatchScalarField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    variableHeightFlowRateFvPatchScalarField
    (
      const variableHeightFlowRateFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<scalar>> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<scalar>>
      {
        new variableHeightFlowRateFvPatchScalarField{*this, iF}
      };
    }
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
