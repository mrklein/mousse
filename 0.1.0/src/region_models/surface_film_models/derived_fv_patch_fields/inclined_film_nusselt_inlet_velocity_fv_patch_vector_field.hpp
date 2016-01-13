// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::inclinedFilmNusseltInletVelocityFvPatchVectorField
// Description
//   Film velocity boundary condition for inclined films that imposes a
//   sinusoidal perturbation on top of a mean flow rate, where the velocity is
//   calculated using the Nusselt solution.
// SourceFiles
//   inclined_film_nusselt_inlet_velocity_fv_patch_vector_field.cpp
#ifndef inclined_film_nusselt_inlet_velocity_fv_patch_vector_field_hpp_
#define inclined_film_nusselt_inlet_velocity_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "data_entry.hpp"
namespace mousse
{
class inclinedFilmNusseltInletVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
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
  TYPE_NAME("inclinedFilmNusseltInletVelocity");
  // Constructors
    //- Construct from patch and internal field
    inclinedFilmNusseltInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    inclinedFilmNusseltInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    // inclinedFilmNusseltInletVelocityFvPatchVectorField onto a new patch
    inclinedFilmNusseltInletVelocityFvPatchVectorField
    (
      const inclinedFilmNusseltInletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    inclinedFilmNusseltInletVelocityFvPatchVectorField
    (
      const inclinedFilmNusseltInletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new inclinedFilmNusseltInletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    inclinedFilmNusseltInletVelocityFvPatchVectorField
    (
      const inclinedFilmNusseltInletVelocityFvPatchVectorField&,
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
        new inclinedFilmNusseltInletVelocityFvPatchVectorField
        (
          *this, iF
        )
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
