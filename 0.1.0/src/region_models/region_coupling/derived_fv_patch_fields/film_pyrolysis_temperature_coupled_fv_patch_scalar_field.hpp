// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::filmPyrolysisTemperatureCoupledFvPatchScalarField
// Description
//   This boundary condition is designed to be used in conjunction with surface
//   film and pyrolysis modelling.  It provides a temperature boundary condition
//   for patches on the primary region based on whether the patch is seen to
//   be 'wet', retrieved from the film alpha field.
//   \li if the patch is wet, the temperature is set using the film temperature
//   \li otherwise, it is set using pyrolysis temperature
//   Example of the boundary condition specification:
//   \verbatim
//     myPatch
//     {
//       type            filmPyrolysisTemperatureCoupled;
//       phi             phi;      // name of flux field (default = phi)
//       rho             rho;      // name of density field (default = rho)
//       deltaWet        1e-4;     // threshold height for 'wet' film
//       value           uniform   300; // initial temperature / [K]
//     }
//   \endverbatim
// SourceFiles
//   film_pyrolysis_temperature_coupled_fv_patch_scalar_field.cpp
#ifndef film_pyrolysis_temperature_coupled_fv_patch_scalar_field_hpp_
#define film_pyrolysis_temperature_coupled_fv_patch_scalar_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class filmPyrolysisTemperatureCoupledFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
  // Private data
    //- Name of film region
    const word filmRegionName_;
    //- Name of pyrolysis region
    const word pyrolysisRegionName_;
    //- Name of flux field
    word phiName_;
    //- Name of density field
    word rhoName_;
public:
  //- Runtime type information
  TYPE_NAME("filmPyrolysisTemperatureCoupled");
  // Constructors
    //- Construct from patch and internal field
    filmPyrolysisTemperatureCoupledFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    filmPyrolysisTemperatureCoupledFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  filmPyrolysisTemperatureCoupledFvPatchScalarField onto a new patch
    filmPyrolysisTemperatureCoupledFvPatchScalarField
    (
      const filmPyrolysisTemperatureCoupledFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    filmPyrolysisTemperatureCoupledFvPatchScalarField
    (
      const filmPyrolysisTemperatureCoupledFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new filmPyrolysisTemperatureCoupledFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    filmPyrolysisTemperatureCoupledFvPatchScalarField
    (
      const filmPyrolysisTemperatureCoupledFvPatchScalarField&,
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
        new filmPyrolysisTemperatureCoupledFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return the name of phi
      const word& phiName() const
      {
        return phiName_;
      }
      //- Return reference to the name of phi to allow adjustment
      word& phiName()
      {
        return phiName_;
      }
      //- Return the name of rho
      const word& rhoName() const
      {
        return rhoName_;
      }
      //- Return reference to the name of rho to allow adjustment
      word& rhoName()
      {
        return rhoName_;
      }
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
