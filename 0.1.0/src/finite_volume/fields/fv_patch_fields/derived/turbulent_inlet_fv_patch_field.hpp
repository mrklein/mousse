#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_TURBULENT_INLET_FV_PATCH_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_TURBULENT_INLET_FV_PATCH_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulentInletFvPatchField
// Group
//   grpInletBoundaryConditions
// Description
//   This boundary condition generates a fluctuating inlet condition by adding
//   a random component to a reference (mean) field.
//   \f[
//     x_p = (1 - \alpha) x_p^{n-1} + \alpha (x_{ref} + s C_{RMS} x_{ref})
//   \f]
//   where
//   \vartable
//     x_p     | patch values
//     x_{ref} | reference patch values
//     n       | time level
//     \alpha  | fraction of new random component added to previous time value
//     C_{RMS} | RMS coefficient
//     s       | fluctuation scale
//   \endvartable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     fluctuationScale | RMS fluctuation scale (fraction of mean) | yes |
//     referenceField | reference (mean) field | yes        |
//     alpha | fraction of new random component added to previous| no| 0.1
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            turbulentInlet;
//     fluctuationScale 0.1;
//     referenceField  uniform 10;
//     alpha           0.1;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   turbulent_inlet_fv_patch_field.cpp
#include "random.hpp"
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
template<class Type>
class turbulentInletFvPatchField
:
  public fixedValueFvPatchField<Type>
{
  // Private data
    //- Random number generator
    Random ranGen_;
    //- Fluctuation scake
    Type fluctuationScale_;
    //- Reference field
    Field<Type> referenceField_;
    //- Fraction of RMS component to apply to last time step values
    scalar alpha_;
    //- Current time index (used for updating)
    label curTimeIndex_;
public:
  //- Runtime type information
  TYPE_NAME("turbulentInlet");
  // Constructors
    //- Construct from patch and internal field
    turbulentInletFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    turbulentInletFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given turbulentInletFvPatchField
    //  onto a new patch
    turbulentInletFvPatchField
    (
      const turbulentInletFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    turbulentInletFvPatchField
    (
      const turbulentInletFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new turbulentInletFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    turbulentInletFvPatchField
    (
      const turbulentInletFvPatchField<Type>&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<Type> > clone
    (
      const DimensionedField<Type, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<Type> >
      (
        new turbulentInletFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return the fluctuation scale
      const Type& fluctuationScale() const
      {
        return fluctuationScale_;
      }
      //- Return reference to the fluctuation scale to allow adjustment
      Type& fluctuationScale()
      {
        return fluctuationScale_;
      }
      //- Return the reference field
      const Field<Type>& referenceField() const
      {
        return referenceField_;
      }
      //- Return reference to the reference field to allow adjustment
      Field<Type>& referenceField()
      {
        return referenceField_;
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
        const fvPatchField<Type>&,
        const labelList&
      );
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "turbulent_inlet_fv_patch_field.cpp"
#endif
#endif
