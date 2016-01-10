// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "output_filter_output_control.hpp"
#include "pstream_reduce_ops.hpp"
// Static Member Data 
namespace mousse
{
  template<>
  const char* NamedEnum<outputFilterOutputControl::outputControls, 7>::
  names[] =
  {
    "timeStep",
    "outputTime",
    "adjustableTime",
    "runTime",
    "clockTime",
    "cpuTime",
    "none"
  };
}
const mousse::NamedEnum<mousse::outputFilterOutputControl::outputControls, 7>
  mousse::outputFilterOutputControl::outputControlNames_;
// Constructors 
mousse::outputFilterOutputControl::outputFilterOutputControl
(
  const Time& t,
  const dictionary& dict,
  const word& prefix
)
:
  time_(t),
  prefix_(prefix),
  outputControl_(ocTimeStep),
  outputInterval_(0),
  outputTimeLastDump_(0),
  writeInterval_(-1)
{
  read(dict);
}
// Destructor 
mousse::outputFilterOutputControl::~outputFilterOutputControl()
{}
// Member Functions 
void mousse::outputFilterOutputControl::read(const dictionary& dict)
{
  const word controlName(prefix_ + "Control");
  const word intervalName(prefix_ + "Interval");
  if (dict.found(controlName))
  {
    outputControl_ = outputControlNames_.read(dict.lookup(controlName));
  }
  else
  {
    outputControl_ = ocTimeStep;
  }
  switch (outputControl_)
  {
    case ocTimeStep:
    {
      outputInterval_ = dict.lookupOrDefault<label>(intervalName, 0);
      break;
    }
    case ocOutputTime:
    {
      outputInterval_ = dict.lookupOrDefault<label>(intervalName, 1);
      break;
    }
    case ocClockTime:
    case ocRunTime:
    case ocCpuTime:
    case ocAdjustableTime:
    {
      writeInterval_ = readScalar(dict.lookup("writeInterval"));
      break;
    }
    default:
    {
      // do nothing
      break;
    }
  }
}
bool mousse::outputFilterOutputControl::output()
{
  switch (outputControl_)
  {
    case ocTimeStep:
    {
      return
      (
        (outputInterval_ <= 1)
      || !(time_.timeIndex() % outputInterval_)
      );
      break;
    }
    case ocOutputTime:
    {
      if (time_.outputTime())
      {
        outputTimeLastDump_ ++;
        return !(outputTimeLastDump_ % outputInterval_);
      }
      break;
    }
    case ocRunTime:
    case ocAdjustableTime:
    {
      label outputIndex = label
      (
        (
          (time_.value() - time_.startTime().value())
         + 0.5*time_.deltaTValue()
        )
        / writeInterval_
      );
      if (outputIndex > outputTimeLastDump_)
      {
        outputTimeLastDump_ = outputIndex;
        return true;
      }
      break;
    }
    case ocCpuTime:
    {
      label outputIndex = label
      (
        returnReduce(time_.elapsedCpuTime(), maxOp<double>())
        / writeInterval_
      );
      if (outputIndex > outputTimeLastDump_)
      {
        outputTimeLastDump_ = outputIndex;
        return true;
      }
      break;
    }
    case ocClockTime:
    {
      label outputIndex = label
      (
        returnReduce(label(time_.elapsedClockTime()), maxOp<label>())
        / writeInterval_
      );
      if (outputIndex > outputTimeLastDump_)
      {
        outputTimeLastDump_ = outputIndex;
        return true;
      }
      break;
    }
    case ocNone:
    {
      return false;
    }
    default:
    {
      // this error should not actually be possible
      FATAL_ERROR_IN("bool mousse::outputFilterOutputControl::output()")
        << "Undefined output control: "
        << outputControlNames_[outputControl_] << nl
        << abort(FatalError);
      break;
    }
  }
  return false;
}
