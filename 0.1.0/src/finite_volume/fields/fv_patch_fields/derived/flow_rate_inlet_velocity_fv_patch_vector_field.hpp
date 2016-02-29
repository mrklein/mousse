#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FLOW_RATE_INLET_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FLOW_RATE_INLET_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::flowRateInletVelocityFvPatchVectorField
// Group
//   grpInletBoundaryConditions
// Description
//   This boundary condition provides a velocity boundary condition, derived
//   from the flux (volumetric or mass-based), whose direction is assumed
//   to be normal to the patch.
//   For a mass-based flux:
//   - the flow rate should be provided in kg/s
//   - if \c rhoName is "none" the flow rate is in m3/s
//   - otherwise \c rhoName should correspond to the name of the density field
//   - if the density field cannot be found in the database, the user must
//    specify the inlet density using the \c rhoInlet entry
//   For a volumetric-based flux:
//   - the flow rate is in m3/s
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     massFlowRate | mass flow rate [kg/s]   | no          |
//     volumetricFlowRate | volumetric flow rate [m3/s]| no |
//     rhoInlet     | inlet density           | no          |
//   \endtable
//   Example of the boundary condition specification for a volumetric flow rate:
//   \verbatim
//   myPatch
//   {
//     type        flowRateInletVelocity;
//     volumetricFlowRate  0.2;
//     value       uniform (0 0 0); // placeholder
//   }
//   \endverbatim
//   Example of the boundary condition specification for a mass flow rate:
//   \verbatim
//   myPatch
//   {
//     type                flowRateInletVelocity;
//     massFlowRate        0.2;
//     rho                 rho;
//     rhoInlet            1.0;
//   }
//   \endverbatim
//   The \c flowRate entry is a \c DataEntry type, meaning that it can be
//   specified as constant, a polynomial fuction of time, and ...
// Note
//   - \c rhoInlet is required for the case of a mass flow rate, where the
//    density field is not available at start-up
//   - the value is positive into the domain (as an inlet)
//   - may not work correctly for transonic inlets
//   - strange behaviour with mousse-potential since the U equation is not
//     solved
// SeeAlso
//   mousse::DataEntry
//   mousse::fixedValueFvPatchField
// SourceFiles
//   flow_rate_inlet_velocity_fv_patch_vector_field.cpp
#include "fixed_value_fv_patch_fields.hpp"
#include "data_entry.hpp"
namespace mousse
{
class flowRateInletVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Inlet integral flow rate
    autoPtr<DataEntry<scalar> > flowRate_;
    //- Is volumetric?
    bool volumetric_;
    //- Name of the density field used to normalize the mass flux
    word rhoName_;
    //- Rho initialisation value (for start; if value not supplied)
    scalar rhoInlet_;
public:
 //- Runtime type information
 TYPE_NAME("flowRateInletVelocity");
 // Constructors
    //- Construct from patch and internal field
    flowRateInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    flowRateInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  flowRateInletVelocityFvPatchVectorField
    //  onto a new patch
    flowRateInletVelocityFvPatchVectorField
    (
      const flowRateInletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    flowRateInletVelocityFvPatchVectorField
    (
      const flowRateInletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new flowRateInletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    flowRateInletVelocityFvPatchVectorField
    (
      const flowRateInletVelocityFvPatchVectorField&,
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
        new flowRateInletVelocityFvPatchVectorField(*this, iF)
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
