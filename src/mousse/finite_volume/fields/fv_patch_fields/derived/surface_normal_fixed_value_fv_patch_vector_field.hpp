#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_SURFACE_NORMAL_FIXED_VALUE_FV_PATCH_VECTOR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_SURFACE_NORMAL_FIXED_VALUE_FV_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceNormalFixedValueFvPatchVectorField
// Group
//   grpGenericBoundaryConditions grpInletBoundaryConditions
// Description
//   This boundary condition provides a surface-normal vector boundary condition
//   by its magnitude.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     refValue     | reference value         | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            surfaceNormalFixedValue;
//     refValue        uniform -10;           // 10 INTO the domain
//   }
//   \endverbatim
// Note
//   Sign conventions:
//   - the value is positive for outward-pointing vectors
// SeeAlso
//   mousse::fixedValueFvPatchField

#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"


namespace mousse {

class surfaceNormalFixedValueFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    scalarField refValue_;
public:
  //- Runtime type information
  TYPE_NAME("surfaceNormalFixedValue");
  // Constructors
    //- Construct from patch and internal field
    surfaceNormalFixedValueFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    surfaceNormalFixedValueFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  surfaceNormalFixedValueFvPatchVectorField
    //  onto a new patch
    surfaceNormalFixedValueFvPatchVectorField
    (
      const surfaceNormalFixedValueFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    surfaceNormalFixedValueFvPatchVectorField
    (
      const surfaceNormalFixedValueFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      {
        new surfaceNormalFixedValueFvPatchVectorField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    surfaceNormalFixedValueFvPatchVectorField
    (
      const surfaceNormalFixedValueFvPatchVectorField&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchVectorField> clone
    (
      const DimensionedField<vector, volMesh>& iF
    ) const
    {
      return tmp<fvPatchVectorField>
      {
        new surfaceNormalFixedValueFvPatchVectorField{*this, iF}
      };
    }
  // Member functions
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
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
