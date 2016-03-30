// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "upstream.hpp"
#include "bool_list.hpp"
#include "token.hpp"


// Constructors 
mousse::UPstream::commsStruct::commsStruct()
:
  above_{-1},
  below_{0},
  allBelow_{0},
  allNotBelow_{0}
{}


mousse::UPstream::commsStruct::commsStruct
(
  const label above,
  const labelList& below,
  const labelList& allBelow,
  const labelList& allNotBelow
)
:
  above_{above},
  below_{below},
  allBelow_{allBelow},
  allNotBelow_{allNotBelow}
{}


mousse::UPstream::commsStruct::commsStruct
(
  const label nProcs,
  const label myProcID,
  const label above,
  const labelList& below,
  const labelList& allBelow
)
:
  above_{above},
  below_{below},
  allBelow_{allBelow},
  allNotBelow_{nProcs - allBelow.size() - 1}
{
  boolList inBelow(nProcs, false);
  FOR_ALL(allBelow, belowI) {
    inBelow[allBelow[belowI]] = true;
  }
  label notI = 0;
  FOR_ALL(inBelow, procI) {
    if ((procI != myProcID) && !inBelow[procI]) {
      allNotBelow_[notI++] = procI;
    }
  }
  if (notI != allNotBelow_.size()) {
    FATAL_ERROR_IN("commsStruct") << "problem!" << mousse::abort(FatalError);
  }
}


// Member Operators 
bool mousse::UPstream::commsStruct::operator==(const commsStruct& comm) const
{
  return
  (
    (above_ == comm.above())
    && (below_ == comm.below())
    && (allBelow_ == allBelow())
    && (allNotBelow_ == allNotBelow())
  );
}


bool mousse::UPstream::commsStruct::operator!=(const commsStruct& comm) const
{
  return !operator==(comm);
}


// Ostream Operator 
mousse::Ostream& mousse::operator<<(Ostream& os, const UPstream::commsStruct& comm)
{
  os << comm.above_ << token::SPACE
    << comm.below_ << token::SPACE
    << comm.allBelow_ << token::SPACE
    << comm.allNotBelow_;
  os.check
  (
    "Ostream& operator<<(Ostream&, const commsStruct&)"
  );
  return os;
}
