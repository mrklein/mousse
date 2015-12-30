// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::timeVaryingMappedFixedValueFvPatchField
// Group
//   grpInletBoundaryConditions grpCoupledBoundaryConditions
// Description
//   This boundary conditions interpolates the values from a set of supplied
//   points in space and time.  Supplied data should be specified in
//   constant/boundaryData/\<patchname\> where:
//   - points : pointField with locations
//   - ddd    : supplied values at time ddd
//   The default mode of operation (mapMethod planarInterpolation) is
//   to project the points onto a plane (constructed from the first threee
//   points) and construct a 2D triangulation and finds for the face centres
//   the triangle it is in and the weights to the 3 vertices.
//   The optional mapMethod nearest will avoid all projection and
//   triangulation and just use the value at the nearest vertex.
//   Values are interpolated linearly between times.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     setAverage   | flag to activate setting of average value | yes |
//     perturb      | perturb points for regular geometries | no | 1e-5
//     fieldTableName | alternative field name to sample | no| this field name
//     mapMethod    | type of mapping | no | planarInterpolation
//     offset   | for applying offset to mapped values  | no | constant 0.0
//   \endtable
//   \verbatim
//   myPatch
//   {
//     type            timeVaryingMappedFixedValue;
//     setAverage      false;
//     //perturb       0.0;
//     //fieldTableName samples;
//     //offset    constant 0.2;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   time_varying_mapped_fixed_value_fv_patch_field.cpp
#ifndef time_varying_mapped_fixed_value_fv_patch_field_hpp_
#define time_varying_mapped_fixed_value_fv_patch_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
#include "fixed_list.hpp"
#include "instant_list.hpp"
#include "point_to_point_planar_interpolation.hpp"
#include "data_entry.hpp"
namespace mousse
{
template<class Type>
class timeVaryingMappedFixedValueFvPatchField
:
  public fixedValueFvPatchField<Type>
{
  // Private data
    //- Name of the field data table, defaults to the name of the field
    word fieldTableName_;
    //- If true adjust the mapped field to maintain average value
    bool setAverage_;
    //- Fraction of perturbation (fraction of bounding box) to add
    scalar perturb_;
    //- Interpolation scheme to use
    word mapMethod_;
    //- 2D interpolation (for 'planarInterpolation' mapMethod)
    autoPtr<pointToPointPlanarInterpolation> mapperPtr_;
    //- List of boundaryData time directories
    instantList sampleTimes_;
    //- Current starting index in sampleTimes
    label startSampleTime_;
    //- Interpolated values from startSampleTime
    Field<Type> startSampledValues_;
    //- If setAverage: starting average value
    Type startAverage_;
    //- Current end index in sampleTimes
    label endSampleTime_;
    //- Interpolated values from endSampleTime
    Field<Type> endSampledValues_;
    //- If setAverage: end average value
    Type endAverage_;
    //- Time varying offset values to interpolated data
    autoPtr<DataEntry<Type> > offset_;
public:
  //- Runtime type information
  TypeName("timeVaryingMappedFixedValue");
  // Constructors
    //- Construct from patch and internal field
    timeVaryingMappedFixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    timeVaryingMappedFixedValueFvPatchField
    (
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given timeVaryingMappedFixedValueFvPatchField
    //  onto a new patch
    timeVaryingMappedFixedValueFvPatchField
    (
      const timeVaryingMappedFixedValueFvPatchField<Type>&,
      const fvPatch&,
      const DimensionedField<Type, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    timeVaryingMappedFixedValueFvPatchField
    (
      const timeVaryingMappedFixedValueFvPatchField<Type>&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
    {
      return tmp<fvPatchField<Type> >
      (
        new timeVaryingMappedFixedValueFvPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    timeVaryingMappedFixedValueFvPatchField
    (
      const timeVaryingMappedFixedValueFvPatchField<Type>&,
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
        new timeVaryingMappedFixedValueFvPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return startSampledValues
      const Field<Type> startSampledValues()
      {
        return startSampledValues_;
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
    // Utility functions
      //- Find boundary data inbetween current time and interpolate
      void checkTable();
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "time_varying_mapped_fixed_value_fv_patch_field.cpp"
#endif
#endif
