#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_WAVE_TRANSMISSIVE_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_WAVE_TRANSMISSIVE_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::waveTransmissiveFvPatchField
// Group
//   grpOutletBoundaryConditions
// Description
//   This boundary condition provides a wave transmissive outflow condition,
//   based onsolving DDt(psi, U) = 0 at the boundary.
//   The wave speed is calculated using:
//     \f[
//       x_p = \frac{\phi_p}{|Sf|} + \sqrt{\frac{\gamma}{\psi_p}}
//     \f]
//   where
//   \vartable
//     x_p     | patch values
//     \phi_p  | patch face flux
//     \psi_p  | patch compressibility
//     Sf      | patch face area vector
//     \gamma  | ratio of specific heats
//   \endvartable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | rho
//     psi          | compressibility field name | no       | psi
//     gamma        | ratio of specific heats (Cp/Cv) | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            waveTransmissive;
//     phi             phi;
//     psi             psi;
//     gamma           1.4;
//   }
//   \endverbatim
// SeeAlso
//   mousse::advectiveFvPatchField

#include "advective_fv_patch_fields.hpp"


namespace mousse {

template<class Type>
class waveTransmissiveFvPatchField
:
  public advectiveFvPatchField<Type>
{
  // Private data
    //- Name of the compressibility field used to calculate the wave speed
    word psiName_;
    //- Heat capacity ratio
    scalar gamma_;
public:
  //- Runtime type information
  TYPE_NAME("waveTransmissive");
  // Constructors
    //- Construct from patch and internal field
    waveTransmissiveFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    waveTransmissiveFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given waveTransmissiveFvPatchField
    //  onto a new patch
    waveTransmissiveFvPatchField
    (
      const waveTransmissiveFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    waveTransmissiveFvPatchField
    (
      const waveTransmissiveFvPatchField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type>> clone() const
    {
      return tmp<fvPatchField<Type>>
      {
        new waveTransmissiveFvPatchField<Type>{*this}
      };
    }
    //- Construct as copy setting internal field reference
    waveTransmissiveFvPatchField
    (
      const waveTransmissiveFvPatchField&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type>> clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type>>
      {
        new waveTransmissiveFvPatchField<Type>{*this, iF}
      };
    }
  // Member functions
    // Access
      //- Return the heat capacity ratio
      scalar gamma() const
      {
        return gamma_;
      }
      //- Return reference to the heat capacity ratio to allow adjustment
      scalar& gamma()
      {
        return gamma_;
      }
    // Evaluation functions
      //- Calculate and return the advection speed at the boundary
      virtual tmp<scalarField> advectionSpeed() const;
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#include "wave_transmissive_fv_patch_field.ipp"

#endif
