// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "output_filter_function_object.hpp"
#include "io_output_filter.hpp"
#include "poly_mesh.hpp"
#include "map_poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


// Private Members
template<class OutputFilter>
void mousse::OutputFilterFunctionObject<OutputFilter>::readDict()
{
  dict_.readIfPresent("region", regionName_);
  dict_.readIfPresent("dictionary", dictName_);
  dict_.readIfPresent("enabled", enabled_);
  dict_.readIfPresent("storeFilter", storeFilter_);
  dict_.readIfPresent("timeStart", timeStart_);
  dict_.readIfPresent("timeEnd", timeEnd_);
  dict_.readIfPresent("nStepsToStartTimeChange", nStepsToStartTimeChange_);
}


template<class OutputFilter>
bool mousse::OutputFilterFunctionObject<OutputFilter>::active() const
{
  return
    enabled_ && time_.value() >= timeStart_ && time_.value() <= timeEnd_;
}


template<class OutputFilter>
void mousse::OutputFilterFunctionObject<OutputFilter>::allocateFilter()
{
  if (dictName_.size()) {
    ptr_.reset
    (
      new IOOutputFilter<OutputFilter>
      {
        name(),
        time_.lookupObject<objectRegistry>(regionName_),
        dictName_
      }
    );
  } else {
    ptr_.reset
    (
      new OutputFilter
      {
        name(),
        time_.lookupObject<objectRegistry>(regionName_),
        dict_
      }
    );
  }
}


template<class OutputFilter>
void mousse::OutputFilterFunctionObject<OutputFilter>::destroyFilter()
{
  ptr_.reset();
}


// Constructors 
template<class OutputFilter>
mousse::OutputFilterFunctionObject<OutputFilter>::OutputFilterFunctionObject
(
  const word& name,
  const Time& t,
  const dictionary& dict
)
:
  functionObject{name},
  time_{t},
  dict_{dict},
  regionName_{polyMesh::defaultRegion},
  dictName_{},
  enabled_{true},
  storeFilter_{true},
  timeStart_{-VGREAT},
  timeEnd_{VGREAT},
  nStepsToStartTimeChange_
  {
    dict.lookupOrDefault("nStepsToStartTimeChange", 3)
  },
  outputControl_{t, dict, "output"},
  evaluateControl_{t, dict, "evaluate"}
{
  readDict();
}


// Member Functions 
template<class OutputFilter>
void mousse::OutputFilterFunctionObject<OutputFilter>::on()
{
  enabled_ = true;
}


template<class OutputFilter>
void mousse::OutputFilterFunctionObject<OutputFilter>::off()
{
  enabled_ = false;
}


template<class OutputFilter>
bool mousse::OutputFilterFunctionObject<OutputFilter>::start()
{
  readDict();
  if (enabled_ && storeFilter_) {
    allocateFilter();
  }
  return true;
}


template<class OutputFilter>
bool mousse::OutputFilterFunctionObject<OutputFilter>::execute
(
  const bool forceWrite
)
{
  if (active()) {
    if (!storeFilter_) {
      allocateFilter();
    }
    if (evaluateControl_.output()) {
      ptr_->execute();
    }
    if (forceWrite || outputControl_.output()) {
      ptr_->write();
    }
    if (!storeFilter_) {
      destroyFilter();
    }
  }
  return true;
}


template<class OutputFilter>
bool mousse::OutputFilterFunctionObject<OutputFilter>::end()
{
  if (enabled_) {
    if (!storeFilter_) {
      allocateFilter();
    }
    ptr_->end();
    if (outputControl_.output()) {
      ptr_->write();
    }
    if (!storeFilter_) {
      destroyFilter();
    }
  }
  return true;
}


template<class OutputFilter>
bool mousse::OutputFilterFunctionObject<OutputFilter>::timeSet()
{
  if (active()) {
    ptr_->timeSet();
  }
  return true;
}


template<class OutputFilter>
bool mousse::OutputFilterFunctionObject<OutputFilter>::adjustTimeStep()
{
  bool aT =
    outputControl_.outputControl() == outputFilterOutputControl::ocAdjustableTime;
  if (active() && aT) {
    const label  outputTimeIndex = outputControl_.outputTimeLastDump();
    const scalar writeInterval = outputControl_.writeInterval();
    scalar timeToNextWrite = max
    (
      0.0,
      (outputTimeIndex + 1)*writeInterval
      - (time_.value() - time_.startTime().value())
    );
    scalar deltaT = time_.deltaTValue();
    scalar nSteps = timeToNextWrite/deltaT - SMALL;
    // function objects modify deltaT inside nStepsToStartTimeChange range
    // NOTE: Potential problem if two function objects dump inside the same
    // interval
    if (nSteps < nStepsToStartTimeChange_) {
      label nStepsToNextWrite = label(nSteps) + 1;
      scalar newDeltaT = timeToNextWrite/nStepsToNextWrite;
      // Adjust time step
      if (newDeltaT < deltaT) {
        deltaT = max(newDeltaT, 0.2*deltaT);
        const_cast<Time&>(time_).setDeltaT(deltaT, false);
      }
    }
  }
  return true;
}


template<class OutputFilter>
bool mousse::OutputFilterFunctionObject<OutputFilter>::read
(
  const dictionary& dict
)
{
  if (dict != dict_) {
    dict_ = dict;
    outputControl_.read(dict);
    return start();
  } else {
    return false;
  }
}


template<class OutputFilter>
void mousse::OutputFilterFunctionObject<OutputFilter>::updateMesh
(
  const mapPolyMesh& mpm
)
{
  if (active() && mpm.mesh().name() == regionName_) {
    ptr_->updateMesh(mpm);
  }
}


template<class OutputFilter>
void mousse::OutputFilterFunctionObject<OutputFilter>::movePoints
(
  const polyMesh& mesh
)
{
  if (active() && mesh.name() == regionName_) {
    ptr_->movePoints(mesh);
  }
}
