// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "zone.hpp"
#include "iostream.hpp"
#include "demand_driven_data.hpp"
#include "hash_set.hpp"

// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(zone, 0);

}

// Protected Member Functions 
const mousse::Map<mousse::label>& mousse::zone::lookupMap() const
{
  if (!lookupMapPtr_)
  {
    calcLookupMap();
  }
  return *lookupMapPtr_;
}
void mousse::zone::calcLookupMap() const
{
  if (debug)
  {
    Info<< "void zone::calcLookupMap() const: "
      << "Calculating lookup map"
      << endl;
  }
  if (lookupMapPtr_)
  {
    FATAL_ERROR_IN("void zone::calcLookupMap() const")
      << "Lookup map already calculated" << nl
      << abort(FatalError);
  }
  const labelList& addr = *this;
  lookupMapPtr_ = new Map<label>(2*addr.size());
  Map<label>& lm = *lookupMapPtr_;
  FOR_ALL(addr, i)
  {
    lm.insert(addr[i], i);
  }
  if (debug)
  {
    Info<< "void zone::calcLookupMap() const: "
      << "Finished calculating lookup map"
      << endl;
  }
}
// Constructors 
mousse::zone::zone
(
  const word& name,
  const labelUList& addr,
  const label index
)
:
  labelList(addr),
  name_(name),
  index_(index),
  lookupMapPtr_(NULL)
{}
mousse::zone::zone
(
  const word& name,
  const Xfer<labelList>& addr,
  const label index
)
:
  labelList(addr),
  name_(name),
  index_(index),
  lookupMapPtr_(NULL)
{}
mousse::zone::zone
(
  const word& name,
  const dictionary& dict,
  const word& labelsName,
  const label index
)
:
  labelList(dict.lookup(labelsName)),
  name_(name),
  index_(index),
  lookupMapPtr_(NULL)
{}
mousse::zone::zone
(
  const zone& z,
  const labelUList& addr,
  const label index
)
:
  labelList(addr),
  name_(z.name()),
  index_(index),
  lookupMapPtr_(NULL)
{}
mousse::zone::zone
(
  const zone& z,
  const Xfer<labelList>& addr,
  const label index
)
:
  labelList(addr),
  name_(z.name()),
  index_(index),
  lookupMapPtr_(NULL)
{}
// Destructor 
mousse::zone::~zone()
{
  clearAddressing();
}
// Member Functions 
mousse::label mousse::zone::localID(const label globalCellID) const
{
  const Map<label>& lm = lookupMap();
  Map<label>::const_iterator lmIter = lm.find(globalCellID);
  if (lmIter == lm.end())
  {
    return -1;
  }
  else
  {
    return lmIter();
  }
}
void mousse::zone::clearAddressing()
{
  deleteDemandDrivenData(lookupMapPtr_);
}
bool mousse::zone::checkDefinition(const label maxSize, const bool report) const
{
  const labelList& addr = *this;
  bool hasError = false;
  // To check for duplicate entries
  labelHashSet elems(size());
  FOR_ALL(addr, i)
  {
    if (addr[i] < 0 || addr[i] >= maxSize)
    {
      hasError = true;
      if (report)
      {
        SERIOUS_ERROR_IN
        (
          "bool zone::checkDefinition("
          "const label maxSize, const bool report) const"
        )
        << "Zone " << name_
        << " contains invalid index label " << addr[i] << nl
        << "Valid index labels are 0.."
        << maxSize-1 << endl;
      }
      else
      {
        // w/o report - can stop checking now
        break;
      }
    }
    else if (!elems.insert(addr[i]))
    {
      if (report)
      {
        WARNING_IN
        (
          "bool zone::checkDefinition("
          "const label maxSize, const bool report) const"
        )
        << "Zone " << name_
        << " contains duplicate index label " << addr[i] << endl;
      }
    }
  }
  return hasError;
}
void mousse::zone::write(Ostream& os) const
{
  os  << nl << name_
    << nl << static_cast<const labelList&>(*this);
}
// Ostream Operator 
mousse::Ostream& mousse::operator<<(Ostream& os, const zone& z)
{
  z.write(os);
  os.check("Ostream& operator<<(Ostream& f, const zone& z");
  return os;
}
