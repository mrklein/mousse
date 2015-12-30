// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::inclinedFilmNusseltHeightFvPatchScalarField
// Description
//   Film height boundary condition for inclined films that imposes a
//   sinusoidal perturbation on top of a mean flow rate, where the height is
//   calculated using the Nusselt solution.
// SourceFiles
//   inclined_film_nusselt_height_fv_patch_scalar_field.cpp
#ifndef inclined_film_nusselt_height_fv_patch_scalar_field_hpp_
#define inclined_film_nusselt_height_fv_patch_scalar_field_hpp_
#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "data_entry.hpp"
namespace mousse
{
class inclinedFilmNusseltHeightFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
  // Private data
    //- Mean mass flow rate per unit length [kg/s/m]
    autoPtr<DataEntry<scalar> > GammaMean_;
    //- Perturbation amplitude [m]
    autoPtr<DataEntry<scalar> > a_;
    //- Perturbation frequency [rad/s/m]
    autoPtr<DataEntry<scalar> > omega_;
public:
  //- Runtime type information
  TypeName("inclinedFilmNusseltHeight");
  // Constructors
    //- Construct from patch and internal field
    inclinedFilmNusseltHeightFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    inclinedFilmNusseltHeightFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    // inclinedFilmNusseltHeightFvPatchScalarField onto a new patch
    inclinedFilmNusseltHeightFvPatchScalarField
    (
      const inclinedFilmNusseltHeightFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    inclinedFilmNusseltHeightFvPatchScalarField
    (
      const inclinedFilmNusseltHeightFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new inclinedFilmNusseltHeightFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    inclinedFilmNusseltHeightFvPatchScalarField
    (
      const inclinedFilmNusseltHeightFvPatchScalarField&,
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
        new inclinedFilmNusseltHeightFvPatchScalarField(*this, iF)
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
