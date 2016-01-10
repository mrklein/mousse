// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::alphaContactAngleFvPatchScalarField
// Group
//   grpWallBoundaryConditions grpGenericBoundaryConditions
// Description
//   Abstract base class for alphaContactAngle boundary conditions.
//   Derived classes must implement the theta() fuction which returns the
//   wall contact angle field.
//   The essential entry "limit" controls the gradient of alpha1 on the wall:
//   limit none;         // Calculate the gradient from the contact-angle without
//             // limiter
//   limit gradient;     // Limit the wall-gradient such that alpha1 remains
//             // bounded on the wall
//   limit alpha;        // Bound the calculated alpha1 on the wall
//   limit zeroGradient; // Set the gradient of alpha1 to 0 on the wall
//             // i.e. reproduce previous behaviour
//   Note that if any of the first three options are used the boundary condition
//   on p_rgh must set to guarantee that the flux is corrected to be zero at the
//   wall e.g.
//   \verbatim
//   myPatch
//   {
//     type            alphaContactAngle;
//   }
//   \endverbatim
//   If "limit zeroGradient;" is used the pressure BCs can be left as before.
// SourceFiles
//   alpha_contact_angle_fv_patch_scalar_field.cpp
#ifndef alpha_contact_angle_fv_patch_scalar_field_hpp_
#define alpha_contact_angle_fv_patch_scalar_field_hpp_
#include "fixed_gradient_fv_patch_fields.hpp"
#include "fvs_patch_fields.hpp"
namespace mousse
{
class alphaContactAngleFvPatchScalarField
:
  public fixedGradientFvPatchScalarField
{
public:
  //- Runtime type information
  TYPE_NAME("alphaContactAngle");
  //- Alpha limit options
  enum limitControls
  {
    lcNone,
    lcGradient,
    lcZeroGradient,
    lcAlpha
  };
  static const NamedEnum<limitControls, 4> limitControlNames_;
  limitControls limit_;
  // Constructors
    //- Construct from patch and internal field
    alphaContactAngleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    alphaContactAngleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given alphaContactAngleFvPatchScalarField
    //  onto a new patch
    alphaContactAngleFvPatchScalarField
    (
      const alphaContactAngleFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    alphaContactAngleFvPatchScalarField
    (
      const alphaContactAngleFvPatchScalarField&
    );
    //- Construct as copy setting internal field reference
    alphaContactAngleFvPatchScalarField
    (
      const alphaContactAngleFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
  // Member functions
    //- Return the contact angle
    virtual tmp<scalarField> theta
    (
      const fvPatchVectorField& Up,
      const fvsPatchVectorField& nHat
    ) const = 0;
    //- Evaluate the patch field
    virtual void evaluate
    (
      const Pstream::commsTypes commsType=Pstream::blocking
    );
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
