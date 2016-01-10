// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::timeVaryingUniformFixedValuePointPatchField
// Description
//   A time-varying form of a uniform fixed value boundary condition.
// See Also
//   mousse::timeVaryingUniformFixedValueFvField
// SourceFiles
//   time_varying_uniform_fixed_value_point_patch_field.cpp
#ifndef time_varying_uniform_fixed_value_point_patch_field_hpp_
#define time_varying_uniform_fixed_value_point_patch_field_hpp_
#include "fixed_value_point_patch_field.hpp"
#include "interpolation_table.hpp"
namespace mousse
{
template<class Type>
class timeVaryingUniformFixedValuePointPatchField
:
  public fixedValuePointPatchField<Type>
{
  // Private data
    //- The time series being used, including the bounding treatment
    interpolationTable<Type> timeSeries_;
public:
  //- Runtime type information
  TYPE_NAME("timeVaryingUniformFixedValue");
  // Constructors
    //- Construct from patch and internal field
    timeVaryingUniformFixedValuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&
    );
    //- Construct from patch, internal field and dictionary
    timeVaryingUniformFixedValuePointPatchField
    (
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patch field onto a new patch
    timeVaryingUniformFixedValuePointPatchField
    (
      const timeVaryingUniformFixedValuePointPatchField<Type>&,
      const pointPatch&,
      const DimensionedField<Type, pointMesh>&,
      const pointPatchFieldMapper&
    );
    //- Construct as copy
    timeVaryingUniformFixedValuePointPatchField
    (
      const timeVaryingUniformFixedValuePointPatchField<Type>&
    );
    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
    {
      return autoPtr<pointPatchField<Type> >
      (
        new timeVaryingUniformFixedValuePointPatchField<Type>(*this)
      );
    }
    //- Construct as copy setting internal field reference
    timeVaryingUniformFixedValuePointPatchField
    (
      const timeVaryingUniformFixedValuePointPatchField<Type>&,
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
        new timeVaryingUniformFixedValuePointPatchField<Type>(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return the time series used
      const interpolationTable<Type>& timeSeries() const
      {
        return timeSeries_;
      }
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "time_varying_uniform_fixed_value_point_patch_field.cpp"
#endif
#endif
