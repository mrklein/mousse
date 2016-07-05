#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FAN_PRESSURE_FV_PATCH_SCALAR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FAN_PRESSURE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fanPressureFvPatchScalarField
// Group
//   grpInletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This boundary condition can be applied to assign either a pressure inlet
//   or outlet total pressure condition for a fan.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     fileName     | fan curve file name     | yes         |
//     outOfBounds  | out of bounds handling  | yes         |
//     direction    | direction of flow through fan [in/out] | yes |
//     p0           | environmental total pressure | yes    |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   inlet
//   {
//     type            fanPressure;
//     fileName        "fanCurve";
//     outOfBounds     clamp;
//     direction       in;
//     p0              uniform 0;
//     value           uniform 0;
//   }
//   outlet
//   {
//     type            fanPressure;
//     fileName        "fanCurve";
//     outOfBounds     clamp;
//     direction       out;
//     p0              uniform 0;
//     value           uniform 0;
//   }
//   \endverbatim
// See Also
//   mousse::fanFvPatchField
//   mousse::totalPressureFvPatchScalarField
//   mousse::interpolationTable

#include "total_pressure_fv_patch_scalar_field.hpp"
#include "interpolation_table.hpp"


namespace mousse {

class fanPressureFvPatchScalarField
:
  public totalPressureFvPatchScalarField
{
public:
    //- Fan flow direction
    enum fanFlowDirection
    {
      ffdIn,
      ffdOut
    };
    //- Fan flow directions names
    static const NamedEnum<fanFlowDirection, 2> fanFlowDirectionNames_;
private:
  // Private data
    //- Tabulated fan curve
    interpolationTable<scalar> fanCurve_;
    //- Direction of flow through the fan relative to patch
    fanFlowDirection direction_;
public:
  //- Runtime type information
  TYPE_NAME("fanPressure");
  // Constructors
    //- Construct from patch and internal field
    fanPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fanPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  fanPressureFvPatchScalarField
    //  onto a new patch
    fanPressureFvPatchScalarField
    (
      const fanPressureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fanPressureFvPatchScalarField
    (
      const fanPressureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      {
        new fanPressureFvPatchScalarField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    fanPressureFvPatchScalarField
    (
      const fanPressureFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchScalarField>
      {
        new fanPressureFvPatchScalarField{*this, iF}
      };
    }
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
