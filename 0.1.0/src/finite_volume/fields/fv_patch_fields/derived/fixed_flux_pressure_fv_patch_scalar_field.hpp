// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedFluxPressureFvPatchScalarField
// Group
//   grpInletBoundaryConditions grpWallBoundaryConditions
// Description
//   This boundary condition sets the pressure gradient to the provided value
//   such that the flux on the boundary is that specified by the velocity
//   boundary condition.
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            fixedFluxPressure;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedGradientFvPatchField
// SourceFiles
//   fixed_flux_pressure_fv_patch_scalar_field.cpp
#ifndef fixedFluxPressureFvPatchScalarFields_H
#define fixedFluxPressureFvPatchScalarFields_H
#include "fv_patch_fields.hpp"
#include "fixed_gradient_fv_patch_fields.hpp"
namespace mousse
{
class fixedFluxPressureFvPatchScalarField
:
  public fixedGradientFvPatchScalarField
{
  // Private data
    //- Current time index (used for updating)
    label curTimeIndex_;
public:
  //- Runtime type information
  TypeName("fixedFluxPressure");
  // Constructors
    //- Construct from patch and internal field
    fixedFluxPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedFluxPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given fixedFluxPressureFvPatchScalarField onto
    //  a new patch
    fixedFluxPressureFvPatchScalarField
    (
      const fixedFluxPressureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedFluxPressureFvPatchScalarField
    (
      const fixedFluxPressureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new fixedFluxPressureFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fixedFluxPressureFvPatchScalarField
    (
      const fixedFluxPressureFvPatchScalarField&,
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
        new fixedFluxPressureFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    //- Update the patch pressure gradient field from the given snGradp
    virtual void updateCoeffs(const scalarField& snGradp);
    //- Update the patch pressure gradient field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#include "vol_fields.hpp"
namespace mousse
{
  template<class GradBC>
  inline void setSnGrad
  (
    volScalarField::GeometricBoundaryField& bf,
    const FieldField<fvsPatchField, scalar>& snGrad
  )
  {
    forAll(bf, patchi)
    {
      if (isA<GradBC>(bf[patchi]))
      {
        refCast<GradBC>(bf[patchi]).updateCoeffs(snGrad[patchi]);
      }
    }
  }
  template<class GradBC>
  inline void setSnGrad
  (
    volScalarField::GeometricBoundaryField& bf,
    const tmp<FieldField<fvsPatchField, scalar> >& tsnGrad
  )
  {
    setSnGrad<GradBC>(bf, tsnGrad());
  }
}
#endif
