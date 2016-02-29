#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_DERIVED_FV_PATCH_FIELDS_ATM_BOUNDARY_LAYER_INLET_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_RAS_DERIVED_FV_PATCH_FIELDS_ATM_BOUNDARY_LAYER_INLET_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::atmBoundaryLayerInletVelocityFvPatchVectorField
// Group
//   grpRASBoundaryConditions grpInletBoundaryConditions
// Description
//   This boundary condition specifies a velocity inlet profile appropriate
//   for atmospheric boundary layers (ABL).
//   See mousse::atmBoundaryLayer for details.
//   Example of the boundary condition specification:
//   \verbatim
//   ground
//   {
//     type            atmBoundaryLayerInletVelocity;
//     n               (1 0 0);
//     z               (0 0 1);
//     Uref            10.0;
//     Zref            20.0;
//     z0              uniform 0.1;
//     zGround         uniform 0.0;
//   }
//   \endverbatim
// SeeAlso
//   mousse::atmBoundaryLayer,
//   mousse::atmBoundaryLayerInletKFvPatchScalarField,
//   mousse::atmBoundaryLayerInletEpsilonFvPatchScalarField
// SourceFiles
//   atm_boundary_layer_inlet_velocity_fv_patch_vector_field.cpp
#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "atm_boundary_layer.hpp"
namespace mousse
{
class atmBoundaryLayerInletVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField,
  public atmBoundaryLayer
{
public:
  //- Runtime type information
  TYPE_NAME("atmBoundaryLayerInletVelocity");
  // Constructors
    //- Construct from patch and internal field
    atmBoundaryLayerInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    atmBoundaryLayerInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    // atmBoundaryLayerInletVelocityFvPatchVectorField onto a new patch
    atmBoundaryLayerInletVelocityFvPatchVectorField
    (
      const atmBoundaryLayerInletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new atmBoundaryLayerInletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    atmBoundaryLayerInletVelocityFvPatchVectorField
    (
      const atmBoundaryLayerInletVelocityFvPatchVectorField&,
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
        new atmBoundaryLayerInletVelocityFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      );
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchVectorField&,
        const labelList&
      );
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
