#ifndef CORE_DB_TIME_TIME_STATE_HPP_
#define CORE_DB_TIME_TIME_STATE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TimeState
// Description
//   The time value with time-stepping information, user-defined remapping, etc.
// SourceFiles
//   time_state.cpp
#include "dimensioned_scalar.hpp"
namespace mousse
{
class TimeState
:
  public dimensionedScalar
{
protected:
    label timeIndex_;
    scalar deltaT_;
    scalar deltaTSave_;
    scalar deltaT0_;
    bool deltaTchanged_;
    label outputTimeIndex_;
    //- Is outputTime because of primary?
    bool primaryOutputTime_;
    label secondaryOutputTimeIndex_;
    //- Is outputTime because of secondary?
    bool secondaryOutputTime_;
    bool outputTime_;
public:
  // Constructors
    TimeState();
  //- Destructor
  virtual ~TimeState();
  // Member functions
    // Access
      //- Convert the user-time (e.g. CA deg) to real-time (s).
      virtual scalar userTimeToTime(const scalar theta) const;
      //- Convert the real-time (s) into user-time (e.g. CA deg)
      virtual scalar timeToUserTime(const scalar t) const;
      //- Return current time value
      scalar timeOutputValue() const;
      //- Return current time index
      label timeIndex() const;
      //- Return time step value
      inline scalar deltaTValue() const
      {
        return deltaT_;
      }
      //- Return old time step value
      inline scalar deltaT0Value() const
      {
        return deltaT0_;
      }
      //- Return time step
      dimensionedScalar deltaT() const;
      //- Return old time step
      dimensionedScalar deltaT0() const;
    // Check
      //- Return true if this is an output time (primary or secondary)
      bool outputTime() const;
};
}  // namespace mousse
#endif
