// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "clock.hpp"
#include "string.hpp"
#include <sstream>
#include <iomanip>


// Static Data Members
const char *mousse::clock::monthNames[] =
{
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};

// Static Member Functions
time_t mousse::clock::getTime()
{
  return ::time(reinterpret_cast<time_t*>(0));
}


const struct tm mousse::clock::rawDate()
{
  time_t t = getTime();
  struct tm *timeStruct = localtime(&t);
  return *timeStruct;
}


mousse::string mousse::clock::dateTime()
{
  std::ostringstream osBuffer;
  time_t t = getTime();
  struct tm *timeStruct = localtime(&t);
  osBuffer
    << std::setfill('0')
    << std::setw(4) << timeStruct->tm_year + 1900
    << '-' << std::setw(2) << timeStruct->tm_mon + 1
    << '-' << std::setw(2) << timeStruct->tm_mday
    << 'T'
    << std::setw(2) << timeStruct->tm_hour
    << ':' << std::setw(2) << timeStruct->tm_min
    << ':' << std::setw(2) << timeStruct->tm_sec;
  return osBuffer.str();
}


mousse::string mousse::clock::date()
{
  std::ostringstream osBuffer;
  time_t t = getTime();
  struct tm *timeStruct = localtime(&t);
  osBuffer
    << monthNames[timeStruct->tm_mon]
    << ' ' << std::setw(2) << std::setfill('0') << timeStruct->tm_mday
    << ' ' << std::setw(4) << timeStruct->tm_year + 1900;
  return osBuffer.str();
}


mousse::string mousse::clock::clockTime()
{
  std::ostringstream osBuffer;
  time_t t = getTime();
  struct tm *timeStruct = localtime(&t);
  osBuffer
    << std::setfill('0')
    << std::setw(2) << timeStruct->tm_hour
    << ':' << std::setw(2) << timeStruct->tm_min
    << ':' << std::setw(2) << timeStruct->tm_sec;
  return osBuffer.str();
}


// Constructors 
mousse::clock::clock()
:
  startTime_{getTime()},
  lastTime_{startTime_},
  newTime_{startTime_}
{}


// Member Functions 
time_t mousse::clock::elapsedClockTime() const
{
  newTime_ = getTime();
  return newTime_ - startTime_;
}


time_t mousse::clock::clockTimeIncrement() const
{
  lastTime_ = newTime_;
  newTime_ = getTime();
  return newTime_ - lastTime_;
}

