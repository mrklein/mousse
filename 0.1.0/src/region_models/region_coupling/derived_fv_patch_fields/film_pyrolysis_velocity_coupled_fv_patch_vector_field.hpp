#ifndef REGION_MODELS_REGION_COUPLING_DERIVED_FV_PATCH_FIELDS_FILM_PYROLYSIS_VELOCITY_COUPLED_FV_PATCH_VECTOR_FIELD_HPP_
#define REGION_MODELS_REGION_COUPLING_DERIVED_FV_PATCH_FIELDS_FILM_PYROLYSIS_VELOCITY_COUPLED_FV_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::filmPyrolysisVelocityCoupledFvPatchVectorField
// Description
//   This boundary condition is designed to be used in conjunction with surface
//   film and pyrolysis modelling.  It provides a velocity boundary condition
//   for patches on the primary region based on whether the patch is seen to
//   be 'wet', retrieved from the film alpha field.
//   \li if the patch is wet, the velocity is set using the film velocity
//   \li otherwise, it is set using pyrolysis out-gassing velocity
//   Example of the boundary condition specification:
//   \verbatim
//     myPatch
//     {
//       type            filmPyrolysisVelocityCoupled;
//       phi             phi;      // name of flux field (default = phi)
//       rho             rho;      // name of density field (default = rho)
//       deltaWet        1e-4;     // threshold height for 'wet' film
//       value           uniform   (0 0 0); // initial velocity / [m/s]
//     }
//   \endverbatim
// SourceFiles
//   film_pyrolysis_velocity_coupled_fv_patch_vector_field.cpp
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class filmPyrolysisVelocityCoupledFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Name of film region
    word filmRegionName_;
    //- Name of pyrolysis region
    word pyrolysisRegionName_;
    //- Name of flux field
    word phiName_;
    //- Name of density field
    word rhoName_;
public:
  //- Runtime type information
  TYPE_NAME("filmPyrolysisVelocityCoupled");
  // Constructors
    //- Construct from patch and internal field
    filmPyrolysisVelocityCoupledFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    filmPyrolysisVelocityCoupledFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  filmPyrolysisVelocityCoupledFvPatchVectorField onto a new patch
    filmPyrolysisVelocityCoupledFvPatchVectorField
    (
      const filmPyrolysisVelocityCoupledFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    filmPyrolysisVelocityCoupledFvPatchVectorField
    (
      const filmPyrolysisVelocityCoupledFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new filmPyrolysisVelocityCoupledFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    filmPyrolysisVelocityCoupledFvPatchVectorField
    (
      const filmPyrolysisVelocityCoupledFvPatchVectorField&,
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
        new filmPyrolysisVelocityCoupledFvPatchVectorField(*this, iF)
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
