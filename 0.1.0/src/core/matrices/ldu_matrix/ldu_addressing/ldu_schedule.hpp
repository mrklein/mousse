// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::lduScheduleEntry
// Description
//   Struct to hold the patch index and the initialisation flag for
//   the patch schedule

#ifndef ldu_schedule_hpp_
#define ldu_schedule_hpp_

#include "list.hpp"
#include "token.hpp"
#include "iostreams.hpp"
#include "dictionary.hpp"
#include "ipstream.hpp"
#include "opstream.hpp"

namespace mousse
{
// Forward declaration of friend functions and operators
struct lduScheduleEntry;
Ostream& operator<<(Ostream& os, const lduScheduleEntry& lb);
struct lduScheduleEntry
{
  label patch;
  bool init;
  friend bool operator!=
  (
    const lduScheduleEntry&,
    const lduScheduleEntry&
  )
  {
    return true;
  }
  friend Ostream& operator<<(Ostream& os, const lduScheduleEntry& lb)
  {
    os  << lb.patch << token::SPACE << lb.init;
    return os;
  }
};
typedef List<lduScheduleEntry> lduSchedule;
}  // namespace mousse
#endif
