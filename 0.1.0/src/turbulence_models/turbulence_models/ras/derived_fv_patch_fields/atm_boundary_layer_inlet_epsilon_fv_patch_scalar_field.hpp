// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::atmBoundaryLayerInletEpsilonFvPatchScalarField
// Group
//   grpRASBoundaryConditions grpInletBoundaryConditions
// Description
//   This boundary condition specifies an inlet value for the turbulence
//   dissipation, \f$\epsilon\f$, appropriate for atmospheric boundary layers.
//   See mousse::atmBoundaryLayer for details.
//   Example of the boundary condition specification:
//   \verbatim
//   ground
//   {
//     type            atmBoundaryLayerInletEpsilon;
//     z               (0 0 1);
//     Uref            10.0;
//     Zref            20.0;
//     z0              uniform 0.1;
//     zGround         uniform 0.0;
//   }
//   \endverbatim
// SeeAlso
//   mousse::atmBoundaryLayer,
//   mousse::atmBoundaryLayerInletVelocityFvPatchVectorField,
//   mousse::atmBoundaryLayerInletKFvPatchScalarField
// SourceFiles
//   atm_boundary_layer_inlet_epsilon_fv_patch_scalar_field.cpp
#ifndef atm_boundary_layer_inlet_epsilon_fv_patch_scalar_field_hpp_
#define atm_boundary_layer_inlet_epsilon_fv_patch_scalar_field_hpp_
#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "atm_boundary_layer.hpp"
namespace mousse
{
class atmBoundaryLayerInletEpsilonFvPatchScalarField
:
  public fixedValueFvPatchScalarField,
  public atmBoundaryLayer
{
public:
  //- Runtime type information
  TypeName("atmBoundaryLayerInletEpsilon");
  // Constructors
    //- Construct from patch and internal field
    atmBoundaryLayerInletEpsilonFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    atmBoundaryLayerInletEpsilonFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  atmBoundaryLayerInletEpsilonFvPatchScalarField onto a new patch
    atmBoundaryLayerInletEpsilonFvPatchScalarField
    (
      const atmBoundaryLayerInletEpsilonFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new atmBoundaryLayerInletEpsilonFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    atmBoundaryLayerInletEpsilonFvPatchScalarField
    (
      const atmBoundaryLayerInletEpsilonFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchScalarField>
      (
        new atmBoundaryLayerInletEpsilonFvPatchScalarField(*this, iF)
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
        const fvPatchScalarField&,
        const labelList&
      );
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
