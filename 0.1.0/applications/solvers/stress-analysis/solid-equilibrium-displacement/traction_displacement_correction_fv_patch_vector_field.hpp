// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef traction_displacement_correction_fv_patch_vector_field_hpp_
#define traction_displacement_correction_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "fixed_gradient_fv_patch_fields.hpp"
namespace mousse
{
class tractionDisplacementCorrectionFvPatchVectorField
:
  public fixedGradientFvPatchVectorField
{
  // Private Data
    vectorField traction_;
    scalarField pressure_;
public:
  //- Runtime type information
  TYPE_NAME("tractionDisplacement");
  // Constructors
    //- Construct from patch and internal field
    tractionDisplacementCorrectionFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and Istream
    tractionDisplacementCorrectionFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      Istream&
    );
    //- Construct from patch, internal field and dictionary
    tractionDisplacementCorrectionFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  tractionDisplacementCorrectionFvPatchVectorField onto a new patch
    tractionDisplacementCorrectionFvPatchVectorField
    (
      const tractionDisplacementCorrectionFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy setting internal field reference
    tractionDisplacementCorrectionFvPatchVectorField
    (
      const tractionDisplacementCorrectionFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new tractionDisplacementCorrectionFvPatchVectorField{*this}
      );
    }
    //- Construct as copy setting internal field reference
    tractionDisplacementCorrectionFvPatchVectorField
    (
      const tractionDisplacementCorrectionFvPatchVectorField&,
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
        new tractionDisplacementCorrectionFvPatchVectorField{*this, iF}
      );
    }
  // Member functions
    // Access
      virtual const vectorField& traction() const
      {
        return traction_;
      }
      virtual vectorField& traction()
      {
        return traction_;
      }
      virtual const scalarField& pressure() const
      {
        return pressure_;
      }
      virtual  scalarField& pressure()
      {
        return pressure_;
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
        const fvPatchVectorField&,
        const labelList&
      );
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
