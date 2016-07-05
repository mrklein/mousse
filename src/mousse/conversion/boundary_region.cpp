// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "boundary_region.hpp"
#include "iomap.hpp"
#include "ofstream.hpp"
#include "string_list_ops.hpp"


// Constructors 
mousse::boundaryRegion::boundaryRegion()
:
  Map<dictionary>{}
{}


mousse::boundaryRegion::boundaryRegion
(
  const objectRegistry& registry,
  const word& name,
  const fileName& instance
)
:
  Map<dictionary>{}
{
  readDict(registry, name, instance);
}


// Destructor 
mousse::boundaryRegion::~boundaryRegion()
{}


// Member Functions 
mousse::label mousse::boundaryRegion::append(const dictionary& dict)
{
  label maxId = -1;
  FOR_ALL_CONST_ITER(Map<dictionary>, *this, iter) {
    if (maxId < iter.key()) {
      maxId = iter.key();
    }
  }
  insert(++maxId, dict);
  return maxId;
}


mousse::Map<mousse::word> mousse::boundaryRegion::names() const
{
  Map<word> lookup;
  FOR_ALL_CONST_ITER(Map<dictionary>, *this, iter) {
    lookup.insert
    (
      iter.key(),
      iter().lookupOrDefault<word>
      (
        "Label",
        "boundaryRegion_" + mousse::name(iter.key())
      )
    );
  }
  return lookup;
}


mousse::Map<mousse::word> mousse::boundaryRegion::names
(
  const UList<wordRe>& patterns
) const
{
  Map<word> lookup;
  FOR_ALL_CONST_ITER(Map<dictionary>, *this, iter) {
    word lookupName = iter().lookupOrDefault<word>
    (
      "Label",
      "boundaryRegion_" + mousse::name(iter.key())
    );
    if (findStrings(patterns, lookupName)) {
      lookup.insert(iter.key(), lookupName);
    }
  }
  return lookup;
}


mousse::Map<mousse::word> mousse::boundaryRegion::boundaryTypes() const
{
  Map<word> lookup;
  FOR_ALL_CONST_ITER(Map<dictionary>, *this, iter) {
    lookup.insert
    (
      iter.key(),
      iter().lookupOrDefault<word>("BoundaryType", "patch")
    );
  }
  return lookup;
}


mousse::label mousse::boundaryRegion::findIndex(const word& name) const
{
  if (name.empty()) {
    return -1;
  }
  FOR_ALL_CONST_ITER(Map<dictionary>, *this, iter) {
    if (iter().lookupOrDefault<word>("Label", word::null) == name) {
      return iter.key();
    }
  }
  return -1;
}


mousse::word mousse::boundaryRegion::boundaryType(const word& name) const
{
  word bndType{"patch"};
  label id = this->findIndex(name);
  if (id >= 0) {
    operator[](id).readIfPresent<word>("BoundaryType", bndType);
  }
  return bndType;
}


void mousse::boundaryRegion::readDict
(
  const objectRegistry& registry,
  const word& name,
  const fileName& instance
)
{
  clear();
  // read constant/dictName
  IOMap<dictionary> ioObj
  {
    {
      name,
      instance,
      registry,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE,
      false
    }
  };
  if (ioObj.headerOk()) {
    *this = ioObj;
  } else {
    Info << "no constant/boundaryRegion information available" << endl;
  }
}


void mousse::boundaryRegion::writeDict
(
  const objectRegistry& registry,
  const word& name,
  const fileName& instance
) const
{
  // write constant/dictName
  IOMap<dictionary> ioObj
  {
    {
      name,
      instance,
      registry,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    }
  };
  ioObj.note() =
    "persistent data for thirdParty mesh <-> mousse translation";
  Info << "Writing " << ioObj.name() << " to " << ioObj.objectPath() << endl;
  OFstream os{ioObj.objectPath()};
  ioObj.writeHeader(os);
  os << *this;
}


// Member Operators 
void mousse::boundaryRegion::operator=(const boundaryRegion& rhs)
{
  Map<dictionary>::operator=(rhs);
}


void mousse::boundaryRegion::operator=(const Map<dictionary>& rhs)
{
  Map<dictionary>::operator=(rhs);
}


// Friend Functions 
void mousse::boundaryRegion::rename(const dictionary& mapDict)
{
  if (mapDict.empty()) {
    return;
  }
  // Use 1st pass to collect all the regions to be changed
  // and 2nd pass to relabel regions.
  // This avoid re-matching any renamed regions
  Map<word> mapping;
  FOR_ALL_CONST_ITER(dictionary, mapDict, iter) {
    word oldName{iter().stream()};
    label id = this->findIndex(oldName);
    if (id >= 0) {
      mapping.insert(id, iter().keyword());
    }
  }
  FOR_ALL_CONST_ITER(Map<word>, mapping, iter) {
    dictionary& dict = operator[](iter.key());
    Info << "rename patch: " << iter()
      << " <- " << word(dict.lookup("Label")) << nl;
    dict.set("Label", iter());
  }
}

