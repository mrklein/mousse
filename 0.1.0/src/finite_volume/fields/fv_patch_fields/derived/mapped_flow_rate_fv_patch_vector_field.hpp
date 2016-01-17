// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedFlowRateFvPatchVectorField
// Group
//   grpInletBoundaryConditions grpCoupledBoundaryConditions
// Description
//   Describes a volumetric/mass flow normal vector boundary condition by its
//   magnitude as an integral over its area.
//   The inlet mass flux is taken from the neighbour region.
//   The basis of the patch (volumetric or mass) is determined by the
//   dimensions of the flux, phi.  The current density is used to correct the
//   velocity when applying the mass basis.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | rho
//     neigPhi      | name of flux field on neighbour mesh | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            mappedFlowRate;
//     phi             phi;
//     rho             rho;
//     neigPhi         phi;
//     value           uniform (0 0 0); // placeholder
//   }
//   \endverbatim
// SourceFiles
//   mapped_flow_rate_fv_patch_vector_field.cpp
#ifndef mapped_flow_rate_fv_patch_vector_field_hpp_
#define mapped_flow_rate_fv_patch_vector_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class mappedFlowRateFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Name of the neighbor flux setting the inlet mass flux
    word nbrPhiName_;
    //- Name of the local mass flux
    word phiName_;
    //- Name of the density field used to normalize the mass flux
    word rhoName_;
public:
 //- Runtime type information
 TYPE_NAME("mappedFlowRate");
 // Constructors
    //- Construct from patch and internal field
    mappedFlowRateFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    mappedFlowRateFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  mappedFlowRateFvPatchVectorField
    //  onto a new patch
    mappedFlowRateFvPatchVectorField
    (
      const mappedFlowRateFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    mappedFlowRateFvPatchVectorField
    (
      const mappedFlowRateFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new mappedFlowRateFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    mappedFlowRateFvPatchVectorField
    (
      const mappedFlowRateFvPatchVectorField&,
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
        new mappedFlowRateFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
