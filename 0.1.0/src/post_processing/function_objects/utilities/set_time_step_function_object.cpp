// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "set_time_step_function_object.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(setTimeStepFunctionObject, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  functionObject,
  setTimeStepFunctionObject,
  dictionary
);

}


// Constructors 
mousse::setTimeStepFunctionObject::setTimeStepFunctionObject
(
  const word& name,
  const Time& runTime,
  const dictionary& dict
)
:
  functionObject{name},
  time_{runTime},
  enabled_{true}
{
  read(dict);
}


// Member Functions 
void mousse::setTimeStepFunctionObject::on()
{
  enabled_ = true;
}


void mousse::setTimeStepFunctionObject::off()
{
  enabled_ = false;
}


bool mousse::setTimeStepFunctionObject::start()
{
  return true;
}


bool mousse::setTimeStepFunctionObject::execute(const bool /*forceWrite*/)
{
  return true;
}


bool mousse::setTimeStepFunctionObject::end()
{
  return true;
}


bool mousse::setTimeStepFunctionObject::timeSet()
{
  return true;
}


bool mousse::setTimeStepFunctionObject::adjustTimeStep()
{
  if (enabled()) {
    // Wanted timestep
    scalar newDeltaT = timeStepPtr_().value(time_.timeOutputValue());
    const_cast<Time&>(time()).setDeltaT(newDeltaT, false);
    return true;
  }
  return false;
}


bool mousse::setTimeStepFunctionObject::read(const dictionary& dict)
{
  enabled_ = dict.lookupOrDefault("enabled", true);
  if (enabled_) {
    timeStepPtr_ = DataEntry<scalar>::New("deltaT", dict);
    // Check that time has adjustTimeStep
    const dictionary& controlDict = time_.controlDict();
    Switch adjust;
    if (!controlDict.readIfPresent<Switch>("adjustTimeStep", adjust)
        || !adjust) {
      FATAL_IO_ERROR_IN("setTimeStep::read(const dictionary&)", dict)
        << "Need to have 'adjustTimeStep' true to enable external"
        << " timestep control" << exit(FatalIOError);
    }
  }
  return true;
}


void mousse::setTimeStepFunctionObject::updateMesh(const mapPolyMesh&)
{}


void mousse::setTimeStepFunctionObject::movePoints(const polyMesh&)
{}

