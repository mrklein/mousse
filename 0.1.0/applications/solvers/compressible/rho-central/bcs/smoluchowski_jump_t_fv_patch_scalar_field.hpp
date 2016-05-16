#ifndef SMOLUCHOWSKI_JUMP_T_FV_PATCH_SCALAR_FIELDS_HPP_
#define SMOLUCHOWSKI_JUMP_T_FV_PATCH_SCALAR_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::smoluchowskiJumpTFvPatchScalarField
// Description
//   Smoluchowski temperature jump boundary condition

#include "mixed_fv_patch_fields.hpp"


namespace mousse {

class smoluchowskiJumpTFvPatchScalarField
:
  public mixedFvPatchScalarField
{
  // Private data
    //- Velocity field name, default = "U"
    word UName_;
    //- Density field name, default = "rho"
    word rhoName_;
    //- Compressibility field name, default = "thermo:psi"
    word psiName_;
    //- Dynamic viscosity field name, default = "thermo:mu"
    word muName_;
    //- Accommodation coefficient
    scalar accommodationCoeff_;
    //- Wall surface temperature
    scalarField Twall_;
    //- Heat capacity ratio (default 1.4)
    scalar gamma_;
public:
  //- Runtime type information
  TYPE_NAME("smoluchowskiJumpT");
  // Constructors
    //- Construct from patch and internal field
    smoluchowskiJumpTFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    smoluchowskiJumpTFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given smoluchowskiJumpTFvPatchScalarField
    //  onto a new patch
    smoluchowskiJumpTFvPatchScalarField
    (
      const smoluchowskiJumpTFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new smoluchowskiJumpTFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    smoluchowskiJumpTFvPatchScalarField
    (
      const smoluchowskiJumpTFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return
        tmp<fvPatchScalarField>
        {
          new smoluchowskiJumpTFvPatchScalarField{*this, iF}
        };
    }
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      );
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchField<scalar>&,
        const labelList&
      );
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

