// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rotatingPressureInletOutletVelocityFvPatchVectorField
// Group
//   grpInletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This velocity inlet/outlet boundary condition is applied to patches in a
//   rotating frame where the pressure is specified.  A zero-gradient is applied
//   for outflow (as defined by the flux); for inflow, the velocity is obtained
//   from the flux with a direction normal to the patch faces.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//     tangentialVelocity | tangential velocity field | no  |
//     omega        | angular velocty of the frame [rad/s] | yes    |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            rotatingPressureInletOutletVelocity;
//     phi             phi;
//     tangentialVelocity uniform (0 0 0);
//     omega           100;
//   }
//   \endverbatim
//   The \c omega entry is a DataEntry type, able to describe time varying
//   functions.
// Note
//   Sign conventions:
//   - positive flux (out of domain): apply zero-gradient condition
//   - negative flux (into of domain): derive from the flux in the patch-normal
//    direction
// SeeAlso
//   mousse::pressureInletOutletVelocityFvPatchVectorField
// SourceFiles
//   rotating_pressure_inlet_outlet_velocity_fv_patch_vector_field.cpp
#ifndef rotating_pressure_inlet_outlet_velocity_fv_patch_vector_field_hpp_
#define rotating_pressure_inlet_outlet_velocity_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "pressure_inlet_outlet_velocity_fv_patch_vector_field.hpp"
#include "data_entry.hpp"
namespace mousse
{
class rotatingPressureInletOutletVelocityFvPatchVectorField
:
  public pressureInletOutletVelocityFvPatchVectorField
{
  // Private data
    //- Angular velocity of the frame
    autoPtr<DataEntry<vector> > omega_;
  // Private Member Functions
    //- Calculate the tangentialVelocity from omega
    void calcTangentialVelocity();
public:
  //- Runtime type information
  TYPE_NAME("rotatingPressureInletOutletVelocity");
  // Constructors
    //- Construct from patch and internal field
    rotatingPressureInletOutletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    rotatingPressureInletOutletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  rotatingPressureInletOutletVelocityFvPatchVectorField
    //  onto a new patch
    rotatingPressureInletOutletVelocityFvPatchVectorField
    (
      const rotatingPressureInletOutletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    rotatingPressureInletOutletVelocityFvPatchVectorField
    (
      const rotatingPressureInletOutletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new rotatingPressureInletOutletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    rotatingPressureInletOutletVelocityFvPatchVectorField
    (
      const rotatingPressureInletOutletVelocityFvPatchVectorField&,
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
        new rotatingPressureInletOutletVelocityFvPatchVectorField
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
