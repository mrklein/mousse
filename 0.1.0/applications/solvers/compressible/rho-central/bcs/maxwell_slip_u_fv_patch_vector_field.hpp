#ifndef SOLVERS_COMPRESSIBLE_RHO_CENTRAL_BCS_MAXWELL_SLIP_U_FV_PATCH_VECTOR_FIELD_HPP_
#define SOLVERS_COMPRESSIBLE_RHO_CENTRAL_BCS_MAXWELL_SLIP_U_FV_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::maxwellSlipUFvPatchVectorField
// Description
//   Maxwell slip boundary condition including thermal creep and surface
//   curvature terms that can be optionally switched off.

#include "mixed_fixed_value_slip_fv_patch_fields.hpp"
#include "switch.hpp"


namespace mousse {

class maxwellSlipUFvPatchVectorField
:
  public mixedFixedValueSlipFvPatchVectorField
{
  // Private data
    //- Temperature field name, default = "T"
    word TName_;
    //- Density field name, default = "rho"
    word rhoName_;
    //- Compressibility field name, default = "thermo:psi"
    word psiName_;
    //- Dynamic viscosity field name, default = "thermo:mu"
    word muName_;
    //- tauMC field name, default = "tauMC"
    word tauMCName_;
    // Accommodation coefficient
    scalar accommodationCoeff_;
    // Wall velocity
    vectorField Uwall_;
    // Include thermal creep term (default on)
    Switch thermalCreep_;
    // Include boundary curvature term (default on)
    Switch curvature_;
public:
  //- Runtime type information
  TYPE_NAME("maxwellSlipU");
  // Constructors
    //- Construct from patch and internal field
    maxwellSlipUFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    maxwellSlipUFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  maxwellSlipUFvPatchVectorField onto a new patch
    maxwellSlipUFvPatchVectorField
    (
      const maxwellSlipUFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return
        tmp<fvPatchVectorField>
        {
          new maxwellSlipUFvPatchVectorField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    maxwellSlipUFvPatchVectorField
    (
      const maxwellSlipUFvPatchVectorField&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchVectorField> clone
    (
      const DimensionedField<vector, volMesh>& iF
    ) const
    {
      return
        tmp<fvPatchVectorField>
        {
          new maxwellSlipUFvPatchVectorField{*this, iF}
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

