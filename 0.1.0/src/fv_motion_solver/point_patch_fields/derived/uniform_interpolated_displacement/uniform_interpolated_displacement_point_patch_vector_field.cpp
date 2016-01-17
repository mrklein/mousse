// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_interpolated_displacement_point_patch_vector_field.hpp"
#include "point_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "interpolation_weights.hpp"
#include "uniform_interpolate.hpp"
#include "_read_fields.hpp"
namespace mousse
{
// Constructors 
uniformInterpolatedDisplacementPointPatchVectorField::
uniformInterpolatedDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>{p, iF}
{}
uniformInterpolatedDisplacementPointPatchVectorField::
uniformInterpolatedDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchField<vector>{p, iF, dict},
  fieldName_{dict.lookup("fieldName")},
  interpolationScheme_{dict.lookup("interpolationScheme")}
{
  const pointMesh& pMesh = this->dimensionedInternalField().mesh();
  // Read time values
  instantList allTimes = Time::findTimes(pMesh().time().path());
  // Only keep those that contain the field
  DynamicList<word> names(allTimes.size());
  DynamicList<scalar> values(allTimes.size());
  FOR_ALL(allTimes, i)
  {
    IOobject io
    (
      fieldName_,
      allTimes[i].name(),
      pMesh(),
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    );
    if (io.headerOk())
    {
      names.append(allTimes[i].name());
      values.append(allTimes[i].value());
    }
  }
  timeNames_.transfer(names);
  timeVals_.transfer(values);
  Info<< type() << " : found " << fieldName_ << " for times "
    << timeNames_ << endl;
  if (timeNames_.size() < 1)
  {
    FATAL_ERROR_IN
    (
      "uniformInterpolatedDisplacementPointPatchVectorField::\n"
      "uniformInterpolatedDisplacementPointPatchVectorField\n"
      "(\n"
      "    const pointPatch&,\n"
      "    const DimensionedField<vector, pointMesh>&,\n"
      "    const dictionary&\n"
      ")\n"
    )
    << "Did not find any times with " << fieldName_
    << exit(FatalError);
  }
  if (!dict.found("value"))
  {
    updateCoeffs();
  }
}
uniformInterpolatedDisplacementPointPatchVectorField::
uniformInterpolatedDisplacementPointPatchVectorField
(
  const uniformInterpolatedDisplacementPointPatchVectorField& ptf,
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<vector>{ptf, p, iF, mapper},
  fieldName_{ptf.fieldName_},
  interpolationScheme_{ptf.interpolationScheme_},
  timeNames_{ptf.timeNames_},
  timeVals_{ptf.timeVals_},
  interpolatorPtr_{ptf.interpolatorPtr_}
{}
uniformInterpolatedDisplacementPointPatchVectorField::
uniformInterpolatedDisplacementPointPatchVectorField
(
  const uniformInterpolatedDisplacementPointPatchVectorField& ptf,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>{ptf, iF},
  fieldName_{ptf.fieldName_},
  interpolationScheme_{ptf.interpolationScheme_},
  timeNames_{ptf.timeNames_},
  timeVals_{ptf.timeVals_},
  interpolatorPtr_{ptf.interpolatorPtr_}
{}
// Member Functions 
void uniformInterpolatedDisplacementPointPatchVectorField::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  if (!interpolatorPtr_.valid())
  {
    interpolatorPtr_ = interpolationWeights::New
    (
      interpolationScheme_,
      timeVals_
    );
  }
  const pointMesh& pMesh = this->dimensionedInternalField().mesh();
  const Time& t = pMesh().time();
  // Update indices of times and weights
  bool timesChanged = interpolatorPtr_->valueWeights
  (
    t.timeOutputValue(),
    currentIndices_,
    currentWeights_
  );
  const wordList currentTimeNames
  {
    UIndirectList<word>{timeNames_, currentIndices_}
  };
  // Load if necessary fields for this interpolation
  if (timesChanged)
  {
    objectRegistry& fieldsCache = const_cast<objectRegistry&>
    (
      pMesh.thisDb().subRegistry("fieldsCache", true)
    );
    // Save old times so we know which ones have been loaded and need
    // 'correctBoundaryConditions'. Bit messy.
    HashSet<word> oldTimes{fieldsCache.toc()};
    ReadFields<pointVectorField>
    (
      fieldName_,
      pMesh,
      currentTimeNames
    );
    FOR_ALL_CONST_ITER(objectRegistry, fieldsCache, fieldsCacheIter)
    {
      if (!oldTimes.found(fieldsCacheIter.key()))
      {
        // Newly loaded fields. Make sure the internal
        // values are consistent with the boundary conditions.
        // This is quite often not the case since these
        // fields typically are constructed 'by hand'
        const objectRegistry& timeCache = dynamic_cast
        <
          const objectRegistry&
        >(*fieldsCacheIter());
        pointVectorField& d = const_cast<pointVectorField&>
        (
          timeCache.lookupObject<pointVectorField>
          (
            fieldName_
          )
        );
        d.correctBoundaryConditions();
      }
    }
  }
  // Interpolate the whole field
  pointVectorField result
  {
    uniformInterpolate<pointVectorField>
    (
      // IOobject
      {
        word("uniformInterpolate(")
        + this->dimensionedInternalField().name()
        + ')',
        pMesh.time().timeName(),
        pMesh.thisDb(),
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
      },
      fieldName_,
      currentTimeNames,
      currentWeights_
    )
  };
  // Extract back from the internal field
  this->operator==
  (
    this->patchInternalField(result.dimensionedInternalField())
  );
  fixedValuePointPatchField<vector>::updateCoeffs();
}
void uniformInterpolatedDisplacementPointPatchVectorField::write(Ostream& os)
const
{
  pointPatchField<vector>::write(os);
  os.writeKeyword("fieldName") << fieldName_ << token::END_STATEMENT << nl;
  os.writeKeyword("interpolationScheme")
    << interpolationScheme_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
MAKE_POINT_PATCH_TYPE_FIELD
(
  pointPatchVectorField,
  uniformInterpolatedDisplacementPointPatchVectorField
);
}  // namespace mousse
