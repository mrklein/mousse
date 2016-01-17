// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::activeBaffleVelocityFvPatchVectorField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This velocity boundary condition simulates the opening of a baffle due
//   to local flow conditions, by merging the behaviours of wall and cyclic
//   conditions.  The baffle joins two mesh regions, where the open fraction
//   determines the interpolation weights applied to each cyclic- and
//   neighbour-patch contribution.
//   We determine whether the baffle is opening or closing from the sign of
//   the net force across the baffle, from which the baffle open fraction is
//   updated using:
//     \f[
//       x = x_{old} + sign(F_{net})\frac{dt}{DT}
//     \f]
//   where
//   \vartable
//     x       | baffle open fraction [0-1]
//     x_{old} | baffle open fraction on previous evaluation
//     dt      | simulation time step
//     DT      | time taken to open the baffle
//     F_{net} | net force across the baffle
//   \endvartable
//   The open fraction is then applied to scale the patch areas.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     p            | pressure field name     | no          | p
//     cyclicPatch  | cylclic patch name      | yes         |
//     orientation  | 1 or -1 used to switch flow direction | yes|
//     openFraction | current opatch open fraction [0-1]| yes |
//     openingTime  | time taken to open the baffle | yes |
//     maxOpenFractionDelta | max open fraction change per timestep | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            activeBaffleVelocity;
//     p               p;
//     cyclicPatch     cyclic1;
//     orientation     1;
//     openFraction    0.2;
//     openingTime     5.0;
//     maxOpenFractionDelta 0.1;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
//   mousse::cyclicFvPatchField
// SourceFiles
//   active_baffle_velocity_fv_patch_vector_field.cpp
#ifndef active_baffle_velocity_fv_patch_vector_field_hpp_
#define active_baffle_velocity_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class activeBaffleVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Name of the pressure field used to calculate the force
    //  on the active baffle
    word pName_;
    //- Name of the cyclic patch used when the active baffle is open
    word cyclicPatchName_;
    //- Index of the cyclic patch used when the active baffle is open
    label cyclicPatchLabel_;
    //- Orientation (1 or -1) of the active baffle patch.
    //  Used to change the direction of opening without the need for
    //  reordering the patch faces
    label orientation_;
    //- Initial wall patch areas
    vectorField initWallSf_;
    //- Initial this-side cyclic patch areas
    vectorField initCyclicSf_;
    //- Initial neighbour-side cyclic patch areas
    vectorField nbrCyclicSf_;
    //- Current fraction of the active baffle which is open
    scalar openFraction_;
    //- Time taken for the active baffle to open
    scalar openingTime_;
    //- Maximum fractional change to the active baffle openness
    //  per time-step
    scalar maxOpenFractionDelta_;
    label curTimeIndex_;
public:
  //- Runtime type information
  TYPE_NAME("activeBaffleVelocity");
  // Constructors
    //- Construct from patch and internal field
    activeBaffleVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    activeBaffleVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given activeBaffleVelocityFvPatchVectorField
    //  onto a new patch
    activeBaffleVelocityFvPatchVectorField
    (
      const activeBaffleVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    activeBaffleVelocityFvPatchVectorField
    (
      const activeBaffleVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new activeBaffleVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    activeBaffleVelocityFvPatchVectorField
    (
      const activeBaffleVelocityFvPatchVectorField&,
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
        new activeBaffleVelocityFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap(const fvPatchFieldMapper&);
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap(const fvPatchVectorField&, const labelList&);
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
