// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::timeVaryingMappedFixedValuePointPatchField
// Description
//   A time-varying form of a mapped fixed value boundary condition.
// See Also
//   mousse::timeVaryingMappedFixedValueFvPatchField
// SourceFiles
//   time_varying_mapped_fixed_value_point_patch_field.cpp
#ifndef time_varying_mapped_fixed_value_point_patch_field_hpp_
#define time_varying_mapped_fixed_value_point_patch_field_hpp_
#include "fixed_value_point_patch_field.hpp"
#include "instant_list.hpp"
#include "point_to_point_planar_interpolation.hpp"
#include "data_entry.hpp"
namespace mousse
{
template<class Type>
class timeVaryingMappedFixedValuePointPatchField
:
  public fixedValuePointPatchField<Type>
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
    timeVaryingMappedFixedValuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    timeVaryingMappedFixedValuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patch field onto a new patch
    timeVaryingMappedFixedValuePointPatchField
    (
      const timeVaryingMappedFixedValuePointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct as copy
    timeVaryingMappedFixedValuePointPatchField
    (
      const timeVaryingMappedFixedValuePointPatchField<Type>&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new timeVaryingMappedFixedValuePointPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    timeVaryingMappedFixedValuePointPatchField
    (
      const timeVaryingMappedFixedValuePointPatchField<Type>&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual autoPtr<pointPatchField<Type> > clone
    (
      const DimensionedField<Type, pointMesh>& iF
    ) const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new timeVaryingMappedFixedValuePointPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Utility functions
      //- Find boundary data inbetween current time and interpolate
      void checkTable();
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const pointPatchFieldMapper&
      );
      //- Reverse map the given PointPatchField onto
      //  this PointPatchField
      virtual void rmap
      (
        const pointPatchField<Type>&,
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
#   include "time_varying_mapped_fixed_value_point_patch_field.cpp"
#endif
#endif
