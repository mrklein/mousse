// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cylindricalInletVelocityFvPatchVectorField
// Group
//   grpInletBoundaryConditions
// Description
//   This boundary condition describes an inlet vector boundary condition in
//   cylindrical co-ordinates given a central axis, central point, rpm, axial
//   and radial velocity.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     axis         | axis of rotation        | yes         |
//     centre       | centre of rotation      | yes         |
//     axialVelocity | axial velocity profile [m/s] | yes    |
//     radialVelocity | radial velocity profile [m/s] | yes  |
//     rpm          | rotational speed (revolutions per minute) | yes|
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            cylindricalInletVelocity;
//     axis            (0 0 1);
//     centre          (0 0 0);
//     axialVelocity   constant 30;
//     radialVelocity  constant -10;
//     rpm             constant 100;
//   }
//   \endverbatim
// Note
//   The \c axialVelocity, \c radialVelocity and \c rpm entries are DataEntry
//   types, able to describe time varying functions.  The example above gives
//   the usage for supplying constant values.
// SeeAlso
//   mousse::fixedValueFvPatchField
//   mousse::DataEntry
// SourceFiles
//   cylindrical_inlet_velocity_fv_patch_vector_field.cpp
#ifndef cylindrical_inlet_velocity_fv_patch_vector_field_hpp_
#define cylindrical_inlet_velocity_fv_patch_vector_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
#include "data_entry.hpp"
namespace mousse
{
class cylindricalInletVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Central point
    const vector centre_;
    //- Axis
    const vector axis_;
    //- Axial velocity
    autoPtr<DataEntry<scalar> > axialVelocity_;
    //- Radial velocity
    autoPtr<DataEntry<scalar> > radialVelocity_;
    //- RPM
    autoPtr<DataEntry<scalar> > rpm_;
public:
 //- Runtime type information
 TYPE_NAME("cylindricalInletVelocity");
 // Constructors
    //- Construct from patch and internal field
    cylindricalInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    cylindricalInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  flowRateInletVelocityFvPatchVectorField
    //  onto a new patch
    cylindricalInletVelocityFvPatchVectorField
    (
      const cylindricalInletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    cylindricalInletVelocityFvPatchVectorField
    (
      const cylindricalInletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new cylindricalInletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    cylindricalInletVelocityFvPatchVectorField
    (
      const cylindricalInletVelocityFvPatchVectorField&,
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
        new cylindricalInletVelocityFvPatchVectorField(*this, iF)
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
