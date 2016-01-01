// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_table.hpp"
#include "iomap.hpp"
#include "ofstream.hpp"
#include "word_list.hpp"
#include "string_list_ops.hpp"
// Static Data Members
const char* const mousse::cellTable::defaultMaterial_ = "fluid";
// Private Member Functions 
mousse::Map<mousse::label> mousse::cellTable::zoneMap() const
{
  Map<label> lookup;
  label zoneI = 0;
  forAllConstIter(Map<dictionary>, *this, iter)
  {
    lookup.insert(iter.key(), zoneI++);
  }
  return lookup;
}
mousse::wordList mousse::cellTable::namesList() const
{
  Map<word> lookup = names();
  wordList lst(lookup.size());
  label zoneI = 0;
  forAllConstIter(Map<word>, lookup, iter)
  {
    lst[zoneI++] = iter();
  }
  return lst;
}
void mousse::cellTable::addDefaults()
{
  forAllIter(Map<dictionary>, *this, iter)
  {
    if (!iter().found("MaterialType"))
    {
      iter().add("MaterialType", defaultMaterial_);
    }
  }
}
void mousse::cellTable::setEntry
(
  const label id,
  const word& keyWord,
  const word& value
)
{
  dictionary dict;
  dict.add(keyWord, value);
  iterator iter = find(id);
  if (iter != end())
  {
    iter().merge(dict);
  }
  else
  {
    insert(id, dict);
  }
}
// Constructors 
mousse::cellTable::cellTable()
:
  Map<dictionary>()
{}
mousse::cellTable::cellTable
(
  const objectRegistry& registry,
  const word& name,
  const fileName& instance
)
:
  Map<dictionary>()
{
  readDict(registry, name, instance);
}
// Destructor 
mousse::cellTable::~cellTable()
{}
// Member Functions 
mousse::label mousse::cellTable::append(const dictionary& dict)
{
  label maxId = -1;
  forAllConstIter(Map<dictionary>, *this, iter)
  {
    if (maxId < iter.key())
    {
      maxId = iter.key();
    }
  }
  insert(++maxId, dict);
  return maxId;
}
mousse::Map<mousse::word> mousse::cellTable::names() const
{
  Map<word> lookup;
  forAllConstIter(Map<dictionary>, *this, iter)
  {
    lookup.insert
    (
      iter.key(),
      iter().lookupOrDefault<word>
      (
        "Label",
        "cellTable_" + mousse::name(iter.key())
      )
    );
  }
  return lookup;
}
mousse::Map<mousse::word> mousse::cellTable::names
(
  const UList<wordRe>& patterns
) const
{
  Map<word> lookup;
  forAllConstIter(Map<dictionary>, *this, iter)
  {
    word lookupName = iter().lookupOrDefault<word>
    (
      "Label",
      "cellTable_" + mousse::name(iter.key())
    );
    if (findStrings(patterns, lookupName))
    {
      lookup.insert(iter.key(), lookupName);
    }
  }
  return lookup;
}
mousse::word mousse::cellTable::name(const label id) const
{
  word theName("cellTable_" + mousse::name(id));
  const_iterator iter = find(id);
  if (iter != end())
  {
    iter().readIfPresent("Label", theName);
  }
  return theName;
}
mousse::label mousse::cellTable::findIndex(const word& name) const
{
  if (name.empty())
  {
    return -1;
  }
  forAllConstIter(Map<dictionary>, *this, iter)
  {
    if (iter().lookupOrDefault<word>("Label", word::null) == name)
    {
      return iter.key();
    }
  }
  return -1;
}
mousse::Map<mousse::word> mousse::cellTable::materialTypes() const
{
  Map<word> lookup;
  forAllConstIter(Map<dictionary>, *this, iter)
  {
    lookup.insert
    (
      iter.key(),
      iter().lookupOrDefault<word>("MaterialType", defaultMaterial_)
    );
  }
  return lookup;
}
mousse::Map<mousse::word> mousse::cellTable::selectType(const word& matl) const
{
  Map<word> lookup;
  forAllConstIter(Map<dictionary>, *this, iter)
  {
    if
    (
      matl
    == iter().lookupOrDefault<word>("MaterialType", defaultMaterial_)
    )
    {
      lookup.insert
      (
        iter.key(),
        iter().lookupOrDefault<word>
        (
          "Label",
          "cellTable_" + mousse::name(iter.key())
        )
      );
    }
  }
  return lookup;
}
mousse::Map<mousse::word> mousse::cellTable::fluids() const
{
  return selectType("fluid");
}
mousse::Map<mousse::word> mousse::cellTable::solids() const
{
  return selectType("solid");
}
mousse::Map<mousse::word> mousse::cellTable::shells() const
{
  return selectType("shell");
}
void mousse::cellTable::setMaterial(const label id, const word& matlType)
{
  setEntry(id, "MaterialType", matlType);
}
void mousse::cellTable::setName(const label id, const word& name)
{
  setEntry(id, "Label", name);
}
void mousse::cellTable::setName(const label id)
{
  iterator iter = find(id);
  if (iter == end() || !iter().found("Label"))
  {
    setName(id, "cellTable_" + mousse::name(id));
  }
}
void mousse::cellTable::readDict
(
  const objectRegistry& registry,
  const word& name,
  const fileName& instance
)
{
  clear();
  // read constant/dictName
  IOMap<dictionary> ioObj
  (
    IOobject
    (
      name,
      instance,
      registry,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE,
      false
    )
  );
  if (ioObj.headerOk())
  {
    *this = ioObj;
    addDefaults();
  }
  else
  {
    Info<< "no constant/cellTable information available" << endl;
  }
}
void mousse::cellTable::writeDict
(
  const objectRegistry& registry,
  const word& name,
  const fileName& instance
) const
{
  // write constant/dictName
  IOMap<dictionary> ioObj
  (
    IOobject
    (
      name,
      instance,
      registry,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    )
  );
  ioObj.note() =
    "persistent data for thirdParty mesh <-> mousse translation";
  Info<< "Writing " << ioObj.name() << " to " << ioObj.objectPath() << endl;
  OFstream os(ioObj.objectPath());
  ioObj.writeHeader(os);
  os << *this;
}
// Member Operators 
void mousse::cellTable::operator=(const cellTable& rhs)
{
  Map<dictionary>::operator=(rhs);
  addDefaults();
}
void mousse::cellTable::operator=(const Map<dictionary>& rhs)
{
  Map<dictionary>::operator=(rhs);
  addDefaults();
}
void mousse::cellTable::operator=(const polyMesh& mesh)
{
  Map<dictionary> zoneDict;
  // create cellTableId and cellTable based on cellZones
  label nZoneCells = 0;
  wordList zoneNames = mesh.cellZones().names();
  label unZonedType = zoneNames.size() + 1;
  // do cell zones
  forAll(mesh.cellZones(), zoneI)
  {
    const cellZone& cZone = mesh.cellZones()[zoneI];
    nZoneCells += cZone.size();
    dictionary dict;
    dict.add("Label", zoneNames[zoneI]);
    zoneDict.insert(zoneI + 1, dict);
  }
  // collect unzoned cells
  // special case: no zones at all - do entire mesh
  if (nZoneCells == 0)
  {
    zoneDict.clear();
    unZonedType = 1;
  }
  if (mesh.nCells() > nZoneCells)
  {
    zoneDict.insert
    (
      unZonedType,
      dictionary(IStringStream("Label cells;")())
    );
  }
  Map<dictionary>::operator=(zoneDict);
  addDefaults();
}
// Friend Functions 
void mousse::cellTable::addCellZones
(
  polyMesh& mesh,
  const labelList& tableIds
) const
{
  Map<label> typeToZone = zoneMap();
  List<DynamicList<label> > zoneCells(size());
  forAll(tableIds, cellI)
  {
    Map<label>::const_iterator iter = typeToZone.find(tableIds[cellI]);
    if (iter != typeToZone.end())
    {
      zoneCells[iter()].append(cellI);
    }
  }
  // track which zones were actually used
  labelList zoneUsed(zoneCells.size());
  wordList  zoneNames(namesList());
  label nZone = 0;
  forAll(zoneCells, zoneI)
  {
    zoneCells[zoneI].shrink();
    if (zoneCells[zoneI].size())
    {
      zoneUsed[nZone++] = zoneI;
    }
  }
  zoneUsed.setSize(nZone);
  cellZoneMesh& czMesh = mesh.cellZones();
  czMesh.clear();
  if (nZone <= 1)
  {
    Info<< "cellZones not used" << endl;
    return;
  }
  czMesh.setSize(nZone);
  forAll(zoneUsed, zoneI)
  {
    const label origZoneI = zoneUsed[zoneI];
    Info<< "cellZone " << zoneI
      << " (size: "  << zoneCells[origZoneI].size()
      << ") name: "  << zoneNames[origZoneI] << endl;
    czMesh.set
    (
      zoneI,
      new cellZone
      (
        zoneNames[origZoneI],
        zoneCells[origZoneI],
        zoneI,
        czMesh
      )
    );
  }
  czMesh.writeOpt() = IOobject::AUTO_WRITE;
}
void mousse::cellTable::combine(const dictionary& mapDict, labelList& tableIds)
{
  if (mapDict.empty())
  {
    return;
  }
  Map<word> origNames(names());
  labelList mapping(identity(max(origNames.toc()) + 1));
  bool remap = false;
  forAllConstIter(dictionary, mapDict, iter)
  {
    wordReList patterns(iter().stream());
    // find all matches
    Map<word> matches;
    forAllConstIter(Map<word>, origNames, namesIter)
    {
      if (findStrings(patterns, namesIter()))
      {
        matches.insert(namesIter.key(), namesIter());
      }
    }
    if (matches.size())
    {
      label targetId = this->findIndex(iter().keyword());
      Info<< "combine cellTable: " << iter().keyword();
      if (targetId < 0)
      {
        // not found - reuse 1st element but with different name
        targetId = min(matches.toc());
        operator[](targetId).set("Label", iter().keyword());
        Info<< " = (";
      }
      else
      {
        Info<< " += (";
      }
      // the mapping and name for targetId is already okay
      matches.erase(targetId);
      origNames.erase(targetId);
      // remove matched names, leaving targetId on 'this'
      this->erase(matches);
      origNames.erase(matches);
      forAllConstIter(Map<word>, matches, matchIter)
      {
        mapping[matchIter.key()] = targetId;
        Info<< " " << matchIter();
      }
      Info<< " )" << endl;
      remap = true;
    }
  }
  if (remap)
  {
    inplaceRenumber(mapping, tableIds);
  }
}
