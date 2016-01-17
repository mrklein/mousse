// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time_state.hpp"
#include "time.hpp"
// Constructors 
mousse::TimeState::TimeState()
:
  dimensionedScalar(Time::timeName(0), dimTime, 0),
  timeIndex_(0),
  deltaT_(0),
  deltaTSave_(0),
  deltaT0_(0),
  deltaTchanged_(false),
  outputTimeIndex_(0),
  primaryOutputTime_(false),
  secondaryOutputTimeIndex_(0),
  secondaryOutputTime_(false),
  outputTime_(false)
{}
// Destructor
mousse::TimeState::~TimeState()
{}
// Member Functions 
mousse::scalar mousse::TimeState::userTimeToTime(const scalar theta) const
{
  return theta;
}
mousse::scalar mousse::TimeState::timeToUserTime(const scalar t) const
{
  return t;
}
mousse::scalar mousse::TimeState::timeOutputValue() const
{
  return timeToUserTime(value());
}
mousse::label mousse::TimeState::timeIndex() const
{
  return timeIndex_;
}
mousse::dimensionedScalar mousse::TimeState::deltaT() const
{
  return dimensionedScalar("deltaT", dimTime, deltaT_);
}
mousse::dimensionedScalar mousse::TimeState::deltaT0() const
{
  return dimensionedScalar("deltaT0", dimTime, deltaT0_);
}
bool mousse::TimeState::outputTime() const
{
  return outputTime_;
}
