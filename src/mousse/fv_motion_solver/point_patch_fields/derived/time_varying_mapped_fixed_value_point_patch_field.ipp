// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time_varying_mapped_fixed_value_point_patch_field.hpp"
#include "time.hpp"
#include "average_io_field.hpp"


// Constructors 
template<class Type>
mousse::
timeVaryingMappedFixedValuePointPatchField<Type>::
timeVaryingMappedFixedValuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  fixedValuePointPatchField<Type>{p, iF},
  fieldTableName_{iF.name()},
  setAverage_{false},
  perturb_{0},
  mapperPtr_{nullptr},
  sampleTimes_{0},
  startSampleTime_{-1},
  startSampledValues_{0},
  startAverage_{pTraits<Type>::zero},
  endSampleTime_{-1},
  endSampledValues_{0},
  endAverage_{pTraits<Type>::zero},
  offset_{}
{}


template<class Type>
mousse::
timeVaryingMappedFixedValuePointPatchField<Type>::
timeVaryingMappedFixedValuePointPatchField
(
  const timeVaryingMappedFixedValuePointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<Type>{ptf, p, iF, mapper},
  fieldTableName_{ptf.fieldTableName_},
  setAverage_{ptf.setAverage_},
  perturb_{ptf.perturb_},
  mapMethod_{ptf.mapMethod_},
  mapperPtr_{nullptr},
  sampleTimes_{0},
  startSampleTime_{-1},
  startSampledValues_{0},
  startAverage_{pTraits<Type>::zero},
  endSampleTime_{-1},
  endSampledValues_{0},
  endAverage_{pTraits<Type>::zero},
  offset_
  {
    ptf.offset_.valid()
    ? ptf.offset_().clone().ptr()
    : nullptr
  }
{}


template<class Type>
mousse::
timeVaryingMappedFixedValuePointPatchField<Type>::
timeVaryingMappedFixedValuePointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchField<Type>{p, iF},
  fieldTableName_{iF.name()},
  setAverage_{readBool(dict.lookup("setAverage"))},
  perturb_{dict.lookupOrDefault("perturb", 1e-5)},
  mapMethod_
  {
    dict.lookupOrDefault<word>("mapMethod", "planarInterpolation")
  },
  mapperPtr_(nullptr),
  sampleTimes_{0},
  startSampleTime_{-1},
  startSampledValues_{0},
  startAverage_{pTraits<Type>::zero},
  endSampleTime_{-1},
  endSampledValues_{0},
  endAverage_{pTraits<Type>::zero},
  offset_{}
{
  if (dict.found("offset")) {
    offset_ = DataEntry<Type>::New("offset", dict);
  }
  dict.readIfPresent("fieldTableName", fieldTableName_);
  if (dict.found("value")) {
    fixedValuePointPatchField<Type>::operator==
    (
      Field<Type>("value", dict, p.size())
    );
  } else {
    // Note: use evaluate to do updateCoeffs followed by a reset
    //       of the pointPatchField::updated_ flag. This is
    //       so if first use is in the next time step it retriggers
    //       a new update.
    pointPatchField<Type>::evaluate(Pstream::blocking);
  }
}


template<class Type>
mousse::
timeVaryingMappedFixedValuePointPatchField<Type>::
timeVaryingMappedFixedValuePointPatchField
(
  const timeVaryingMappedFixedValuePointPatchField<Type>& ptf
)
:
  fixedValuePointPatchField<Type>{ptf},
  fieldTableName_{ptf.fieldTableName_},
  setAverage_{ptf.setAverage_},
  perturb_{ptf.perturb_},
  mapMethod_{ptf.mapMethod_},
  mapperPtr_{ptf.mapperPtr_},
  sampleTimes_{ptf.sampleTimes_},
  startSampleTime_{ptf.startSampleTime_},
  startSampledValues_{ptf.startSampledValues_},
  startAverage_{ptf.startAverage_},
  endSampleTime_{ptf.endSampleTime_},
  endSampledValues_{ptf.endSampledValues_},
  endAverage_{ptf.endAverage_},
  offset_
  {
    ptf.offset_.valid()
    ? ptf.offset_().clone().ptr()
    : NULL
  }
{}


template<class Type>
mousse::
timeVaryingMappedFixedValuePointPatchField<Type>::
timeVaryingMappedFixedValuePointPatchField
(
  const timeVaryingMappedFixedValuePointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  fixedValuePointPatchField<Type>{ptf, iF},
  fieldTableName_{ptf.fieldTableName_},
  setAverage_{ptf.setAverage_},
  perturb_{ptf.perturb_},
  mapMethod_{ptf.mapMethod_},
  mapperPtr_{ptf.mapperPtr_},
  sampleTimes_{ptf.sampleTimes_},
  startSampleTime_{ptf.startSampleTime_},
  startSampledValues_{ptf.startSampledValues_},
  startAverage_{ptf.startAverage_},
  endSampleTime_{ptf.endSampleTime_},
  endSampledValues_{ptf.endSampledValues_},
  endAverage_{ptf.endAverage_},
  offset_
  {
    ptf.offset_.valid()
    ? ptf.offset_().clone().ptr()
    : nullptr
  }
{}


// Member Functions 
template<class Type>
void mousse::timeVaryingMappedFixedValuePointPatchField<Type>::autoMap
(
  const pointPatchFieldMapper& m
)
{
  fixedValuePointPatchField<Type>::autoMap(m);
  if (startSampledValues_.size()) {
    startSampledValues_.autoMap(m);
    endSampledValues_.autoMap(m);
  }
  // Clear interpolator
  mapperPtr_.clear();
  startSampleTime_ = -1;
  endSampleTime_ = -1;
}


template<class Type>
void mousse::timeVaryingMappedFixedValuePointPatchField<Type>::rmap
(
  const pointPatchField<Type>& ptf,
  const labelList& addr
)
{
  fixedValuePointPatchField<Type>::rmap(ptf, addr);
  const timeVaryingMappedFixedValuePointPatchField<Type>& tiptf =
    refCast<const timeVaryingMappedFixedValuePointPatchField<Type> >(ptf);
  startSampledValues_.rmap(tiptf.startSampledValues_, addr);
  endSampledValues_.rmap(tiptf.endSampledValues_, addr);
  // Clear interpolator
  mapperPtr_.clear();
  startSampleTime_ = -1;
  endSampleTime_ = -1;
}


template<class Type>
void mousse::timeVaryingMappedFixedValuePointPatchField<Type>::checkTable()
{
  // Initialise
  if (startSampleTime_ == -1 && endSampleTime_ == -1) {
    const polyMesh& pMesh = this->patch().boundaryMesh().mesh()();
    // Read the initial point position
    pointField meshPts;
    if (pMesh.pointsInstance() == pMesh.facesInstance()) {
      meshPts = pointField(pMesh.points(), this->patch().meshPoints());
    } else {
      // Load points from facesInstance
      if (debug) {
        Info << "Reloading points0 from " << pMesh.facesInstance()
          << endl;
      }
      pointIOField points0
      {
        IOobject
        {
          "points",
          pMesh.facesInstance(),
          polyMesh::meshSubDir,
          pMesh,
          IOobject::MUST_READ,
          IOobject::NO_WRITE,
          false
        }
      };
      meshPts = pointField{points0, this->patch().meshPoints()};
    }
    pointIOField samplePoints
    {
      IOobject
      {
        "points",
        this->db().time().constant(),
        "boundaryData"/this->patch().name(),
        this->db(),
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE,
        false
      }
    };
    // tbd: run-time selection
    bool nearestOnly =
      !mapMethod_.empty() && mapMethod_ != "planarInterpolation";
    // Allocate the interpolator
    mapperPtr_.reset
    (
      new pointToPointPlanarInterpolation
      {
        samplePoints,
        meshPts,
        perturb_,
        nearestOnly
      }
    );
    // Read the times for which data is available
    const fileName samplePointsFile = samplePoints.filePath();
    const fileName samplePointsDir = samplePointsFile.path();
    sampleTimes_ = Time::findTimes(samplePointsDir);
    if (debug) {
      Info << "timeVaryingMappedFixedValuePointPatchField : In directory "
        << samplePointsDir << " found times "
        << pointToPointPlanarInterpolation::timeNames(sampleTimes_)
        << endl;
    }
  }
  // Find current time in sampleTimes
  label lo = -1;
  label hi = -1;
  bool foundTime =
    mapperPtr_().findTime
    (
      sampleTimes_,
      startSampleTime_,
      this->db().time().value(),
      lo,
      hi
    );
  if (!foundTime) {
    FATAL_ERROR_IN
    (
      "timeVaryingMappedFixedValuePointPatchField<Type>::checkTable"
    )
    << "Cannot find starting sampling values for current time "
    << this->db().time().value() << nl
    << "Have sampling values for times "
    << pointToPointPlanarInterpolation::timeNames(sampleTimes_) << nl
    << "In directory "
    <<  this->db().time().constant()/"boundaryData"/this->patch().name()
    << "\n    on patch " << this->patch().name()
    << " of field " << fieldTableName_
    << exit(FatalError);
  }
  // Update sampled data fields.
  if (lo != startSampleTime_) {
    startSampleTime_ = lo;
    if (startSampleTime_ == endSampleTime_) {
      // No need to reread since are end values
      if (debug) {
        const label& sST = startSampleTime_;
        Pout << "checkTable : Setting startValues to (already read) "
          << "boundaryData"/this->patch().name()/sampleTimes_[sST].name()
          << endl;
      }
      startSampledValues_ = endSampledValues_;
      startAverage_ = endAverage_;
    } else {
      if (debug) {
        Pout << "checkTable : Reading startValues from "
          << "boundaryData"/this->patch().name()/sampleTimes_[lo].name()
          << endl;
      }
      // Reread values and interpolate
      AverageIOField<Type> vals
      {
        IOobject
        {
          fieldTableName_,
          this->db().time().constant(),
          "boundaryData"/this->patch().name()/sampleTimes_[startSampleTime_].name(),
          this->db(),
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE,
          false
        }
      };
      if (vals.size() != mapperPtr_().sourceSize()) {
        FATAL_ERROR_IN
        (
          "timeVaryingMappedFixedValuePointPatchField<Type>::"
          "checkTable()"
        )
        << "Number of values (" << vals.size()
        << ") differs from the number of points ("
        <<  mapperPtr_().sourceSize()
        << ") in file " << vals.objectPath() << exit(FatalError);
      }
      startAverage_ = vals.average();
      startSampledValues_ = mapperPtr_().interpolate(vals);
    }
  }
  if (hi != endSampleTime_) {
    endSampleTime_ = hi;
    if (endSampleTime_ == -1) {
      // endTime no longer valid. Might as well clear endValues.
      if (debug) {
        Pout << "checkTable : Clearing endValues" << endl;
      }
      endSampledValues_.clear();
    } else {
      if (debug) {
        Pout << "checkTable : Reading endValues from "
          << "boundaryData"/this->patch().name()/sampleTimes_[endSampleTime_].name()
          << endl;
      }
      // Reread values and interpolate
      AverageIOField<Type> vals
      {
        IOobject
        {
          fieldTableName_,
          this->db().time().constant(),
          "boundaryData"/this->patch().name()/sampleTimes_[endSampleTime_].name(),
          this->db(),
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE,
          false
        }
      };
      if (vals.size() != mapperPtr_().sourceSize()) {
        FATAL_ERROR_IN
        (
          "timeVaryingMappedFixedValuePointPatchField<Type>::"
          "checkTable()"
        )
        << "Number of values (" << vals.size()
        << ") differs from the number of points ("
        <<  mapperPtr_().sourceSize()
        << ") in file " << vals.objectPath() << exit(FatalError);
      }
      endAverage_ = vals.average();
      endSampledValues_ = mapperPtr_().interpolate(vals);
    }
  }
}


template<class Type>
void mousse::timeVaryingMappedFixedValuePointPatchField<Type>::updateCoeffs()
{
  if (this->updated()) {
    return;
  }
  checkTable();
  // Interpolate between the sampled data
  Type wantedAverage;
  if (endSampleTime_ == -1) {
    // only start value
    if (debug) {
      Pout << "updateCoeffs : Sampled, non-interpolated values"
        << " from start time:"
        << sampleTimes_[startSampleTime_].name() << nl;
    }
    this->operator==(startSampledValues_);
    wantedAverage = startAverage_;
  } else {
    scalar start = sampleTimes_[startSampleTime_].value();
    scalar end = sampleTimes_[endSampleTime_].value();
    scalar s = (this->db().time().value()-start)/(end-start);
    if (debug) {
      Pout << "updateCoeffs : Sampled, interpolated values"
        << " between start time:"
        << sampleTimes_[startSampleTime_].name()
        << " and end time:" << sampleTimes_[endSampleTime_].name()
        << " with weight:" << s << endl;
    }
    this->operator==((1-s)*startSampledValues_ + s*endSampledValues_);
    wantedAverage = (1-s)*startAverage_ + s*endAverage_;
  }
  // Enforce average. Either by scaling (if scaling factor > 0.5) or by
  // offsetting.
  if (setAverage_) {
    const Field<Type>& fld = *this;
    Type averagePsi = gAverage(fld);
    if (debug) {
      Pout << "updateCoeffs :"
        << " actual average:" << averagePsi
        << " wanted average:" << wantedAverage
        << endl;
    }
    if (mag(averagePsi) < VSMALL) {
      // Field too small to scale. Offset instead.
      const Type offset = wantedAverage - averagePsi;
      if (debug) {
        Pout << "updateCoeffs :" << " offsetting with:" << offset << endl;
      }
      this->operator==(fld+offset);
    } else {
      const scalar scale = mag(wantedAverage)/mag(averagePsi);
      if (debug) {
        Pout << "updateCoeffs :" << " scaling with:" << scale << endl;
      }
      this->operator==(scale*fld);
    }
  }
  // apply offset to mapped values
  if (offset_.valid()) {
    const scalar t = this->db().time().timeOutputValue();
    this->operator==(*this + offset_->value(t));
  }
  if (debug) {
    Pout << "updateCoeffs : set fixedValue to min:" << gMin(*this)
      << " max:" << gMax(*this)
      << " avg:" << gAverage(*this) << endl;
  }
  fixedValuePointPatchField<Type>::updateCoeffs();
}


template<class Type>
void mousse::timeVaryingMappedFixedValuePointPatchField<Type>::write
(
  Ostream& os
) const
{
  fixedValuePointPatchField<Type>::write(os);
  os.writeKeyword("setAverage") << setAverage_ << token::END_STATEMENT << nl;
  if (perturb_ != 1e-5) {
    os.writeKeyword("perturb") << perturb_ << token::END_STATEMENT << nl;
  }
  if (fieldTableName_ != this->dimensionedInternalField().name()) {
    os.writeKeyword("fieldTableName") << fieldTableName_
      << token::END_STATEMENT << nl;
  }
  if ((!mapMethod_.empty() && mapMethod_ != "planarInterpolation")) {
    os.writeKeyword("mapMethod") << mapMethod_ << token::END_STATEMENT << nl;
  }
  if (offset_.valid()) {
    offset_->writeData(os);
  }
}

