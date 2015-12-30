// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "engine_time.hpp"
#include "unit_conversion.hpp"
// Private Member Functions 
void mousse::engineTime::timeAdjustment()
{
  deltaT_  = degToTime(deltaT_);
  endTime_ = degToTime(endTime_);
  if
  (
    writeControl_ == wcRunTime
  || writeControl_ == wcAdjustableRunTime
  )
  {
    writeInterval_ = degToTime(writeInterval_);
  }
}
// Constructors 
//- Construct from objectRegistry arguments
mousse::engineTime::engineTime
(
  const word& name,
  const fileName& rootPath,
  const fileName& caseName,
  const fileName& systemName,
  const fileName& constantName,
  const fileName& dictName
)
:
  Time
  (
    name,
    rootPath,
    caseName,
    systemName,
    constantName
  ),
  dict_
  (
    IOobject
    (
      "engineGeometry",
      constant(),
      *this,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE,
      false
    )
  ),
  rpm_(dict_.lookup("rpm")),
  conRodLength_(dimensionedScalar("conRodLength", dimLength, 0)),
  bore_(dimensionedScalar("bore", dimLength, 0)),
  stroke_(dimensionedScalar("stroke", dimLength, 0)),
  clearance_(dimensionedScalar("clearance", dimLength, 0))
{
  // geometric parameters are not strictly required for Time
  dict_.readIfPresent("conRodLength", conRodLength_);
  dict_.readIfPresent("bore", bore_);
  dict_.readIfPresent("stroke", stroke_);
  dict_.readIfPresent("clearance", clearance_);
  timeAdjustment();
  startTime_  = degToTime(startTime_);
  value()     = degToTime(value());
  deltaTSave_ = deltaT_;
  deltaT0_    = deltaT_;
}
// Member Functions 
// Read the controlDict and set all the parameters
void mousse::engineTime::readDict()
{
  Time::readDict();
  timeAdjustment();
}
// Read the controlDict and set all the parameters
bool mousse::engineTime::read()
{
  if (Time::read())
  {
    timeAdjustment();
    return true;
  }
  else
  {
    return false;
  }
}
mousse::scalar mousse::engineTime::degToTime(const scalar theta) const
{
  // 6 * rpm => deg/s
  return theta/(6.0*rpm_.value());
}
mousse::scalar mousse::engineTime::timeToDeg(const scalar t) const
{
  // 6 * rpm => deg/s
  return t*(6.0*rpm_.value());
}
mousse::scalar mousse::engineTime::theta() const
{
  return timeToDeg(value());
}
// Return current crank-angle translated to a single revolution
// (value between -180 and 180 with 0 = top dead centre)
mousse::scalar mousse::engineTime::thetaRevolution() const
{
  scalar t = theta();
  while (t > 180.0)
  {
    t -= 360.0;
  }
  while (t < -180.0)
  {
    t += 360.0;
  }
  return t;
}
mousse::scalar mousse::engineTime::deltaTheta() const
{
  return timeToDeg(deltaTValue());
}
mousse::scalar mousse::engineTime::pistonPosition(const scalar theta) const
{
  return
  (
    conRodLength_.value()
   + stroke_.value()/2.0
   + clearance_.value()
  )
 - (
    stroke_.value()*::cos(degToRad(theta))/2.0
   + ::sqrt
    (
      sqr(conRodLength_.value())
      - sqr(stroke_.value()*::sin(degToRad(theta))/2.0)
    )
  );
}
mousse::dimensionedScalar mousse::engineTime::pistonPosition() const
{
  return dimensionedScalar
  (
    "pistonPosition",
    dimLength,
    pistonPosition(theta())
  );
}
mousse::dimensionedScalar mousse::engineTime::pistonDisplacement() const
{
  return dimensionedScalar
  (
    "pistonDisplacement",
    dimLength,
    pistonPosition(theta() - deltaTheta()) - pistonPosition().value()
  );
}
mousse::dimensionedScalar mousse::engineTime::pistonSpeed() const
{
  return dimensionedScalar
  (
    "pistonSpeed",
    dimVelocity,
    pistonDisplacement().value()/(deltaTValue() + VSMALL)
  );
}
mousse::scalar mousse::engineTime::userTimeToTime(const scalar theta) const
{
  return degToTime(theta);
}
mousse::scalar mousse::engineTime::timeToUserTime(const scalar t) const
{
  return timeToDeg(t);
}
