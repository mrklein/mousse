// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "debug.hpp"
#include "dictionary.hpp"
#include "ifstream.hpp"
#include "os_specific.hpp"
#include "ostream.hpp"
#include "demand_driven_data.hpp"
#include "simple_object_registry.hpp"


namespace mousse {
namespace debug {

//! \cond ignoreDocumentation - local scope
dictionary* controlDictPtr_{NULL};
dictionary* debugSwitchesPtr_{NULL};
dictionary* infoSwitchesPtr_{NULL};
dictionary* optimisationSwitchesPtr_{NULL};

// Debug switch read and write callback tables.
simpleObjectRegistry* debugObjectsPtr_{NULL};
simpleObjectRegistry* infoObjectsPtr_{NULL};
simpleObjectRegistry* optimisationObjectsPtr_{NULL};
simpleObjectRegistry* dimensionSetObjectsPtr_{NULL};
simpleObjectRegistry* dimensionedConstantObjectsPtr_{NULL};

// to ensure controlDictPtr_ is deleted at the end of the run
class deleteControlDictPtr
{
public:
  deleteControlDictPtr()
  {}
  ~deleteControlDictPtr()
  {
    deleteDemandDrivenData(debugObjectsPtr_);
    deleteDemandDrivenData(infoObjectsPtr_);
    deleteDemandDrivenData(optimisationObjectsPtr_);
    deleteDemandDrivenData(dimensionSetObjectsPtr_);
    deleteDemandDrivenData(dimensionedConstantObjectsPtr_);
    debugSwitchesPtr_ = NULL;
    infoSwitchesPtr_ = NULL;
    optimisationSwitchesPtr_ = NULL;
    deleteDemandDrivenData(controlDictPtr_);
  }
};

deleteControlDictPtr deleteControlDictPtr_;
//! \endcond

}  // namespace debug

}  // namespace mousse


mousse::dictionary& mousse::debug::controlDict()
{
  if (!controlDictPtr_) {
    fileNameList controlDictFiles = findEtcFiles("controlDict", true);
    controlDictPtr_ = new dictionary();
    FOR_ALL_REVERSE(controlDictFiles, cdfi) {
      IFstream ifs{controlDictFiles[cdfi]};
      if (!ifs.good()) {
        SAFE_FATAL_IO_ERROR_IN
        (
          "debug::controlDict()",
          ifs,
          "Cannot open controlDict"
        );
      }
      controlDictPtr_->merge(dictionary(ifs));
    }
  }
  return *controlDictPtr_;
}
mousse::dictionary& mousse::debug::switchSet
(
  const char* subDictName,
  dictionary*& subDictPtr
)
{
  if (!subDictPtr)
  {
    entry* ePtr = controlDict().lookupEntryPtr
    (
      subDictName, false, false
    );
    if (!ePtr || !ePtr->isDict())
    {
      cerr<< "debug::switchSet(const char*, dictionary*&):\n"
        << "    Cannot find " <<  subDictName << " in dictionary "
        << controlDict().name().c_str()
        << std::endl << std::endl;
      ::exit(1);
    }
    subDictPtr = &ePtr->dict();
  }
  return *subDictPtr;
}


mousse::dictionary& mousse::debug::debugSwitches()
{
  return switchSet("DebugSwitches", debugSwitchesPtr_);
}


mousse::dictionary& mousse::debug::infoSwitches()
{
  return switchSet("InfoSwitches", infoSwitchesPtr_);
}


mousse::dictionary& mousse::debug::optimisationSwitches()
{
  return switchSet("OptimisationSwitches", optimisationSwitchesPtr_);
}


int mousse::debug::debugSwitch(const char* name, const int defaultValue)
{
  return debugSwitches().lookupOrAddDefault
  (
    name, defaultValue, false, false
  );
}


int mousse::debug::infoSwitch(const char* name, const int defaultValue)
{
  return infoSwitches().lookupOrAddDefault
  (
    name, defaultValue, false, false
  );
}


int mousse::debug::optimisationSwitch(const char* name, const int defaultValue)
{
  return optimisationSwitches().lookupOrAddDefault
  (
    name, defaultValue, false, false
  );
}


void mousse::debug::addDebugObject(const char* name, simpleRegIOobject* obj)
{
  simpleObjectRegistryEntry* ptr = debugObjects().lookupPtr(name);
  if (ptr) {
    ptr->append(obj);
  } else {
    debugObjects().append
    (
      name,
      new simpleObjectRegistryEntry{List<simpleRegIOobject*>{1, obj}}
    );
  }
}


void mousse::debug::addInfoObject(const char* name, simpleRegIOobject* obj)
{
  simpleObjectRegistryEntry* ptr = infoObjects().lookupPtr(name);
  if (ptr) {
    ptr->append(obj);
  } else {
    infoObjects().append
    (
      name,
      new simpleObjectRegistryEntry{List<simpleRegIOobject*>{1, obj}}
    );
  }
}


void mousse::debug::addOptimisationObject
(
  const char* name,
  simpleRegIOobject* obj
)
{
  simpleObjectRegistryEntry* ptr = optimisationObjects().lookupPtr(name);
  if (ptr) {
    ptr->append(obj);
  } else {
    optimisationObjects().append
    (
      name,
      new simpleObjectRegistryEntry{List<simpleRegIOobject*>{1, obj}}
    );
  }
}


void mousse::debug::addDimensionSetObject
(
  const char* name,
  simpleRegIOobject* obj
)
{
  simpleObjectRegistryEntry* ptr = dimensionSetObjects().lookupPtr(name);
  if (ptr) {
    ptr->append(obj);
  } else {
    dimensionSetObjects().append
    (
      name,
      new simpleObjectRegistryEntry{List<simpleRegIOobject*>{1, obj}}
    );
  }
}


void mousse::debug::addDimensionedConstantObject
(
  const char* name,
  simpleRegIOobject* obj
)
{
  simpleObjectRegistryEntry* ptr = dimensionedConstantObjects().lookupPtr
  (
    name
  );
  if (ptr) {
    ptr->append(obj);
  } else {
    dimensionedConstantObjects().append
    (
      name,
      new simpleObjectRegistryEntry{List<simpleRegIOobject*>{1, obj}}
    );
  }
}


mousse::simpleObjectRegistry& mousse::debug::debugObjects()
{
  if (!debugObjectsPtr_) {
    debugObjectsPtr_ = new simpleObjectRegistry{1000};
  }
  return *debugObjectsPtr_;
}


mousse::simpleObjectRegistry& mousse::debug::infoObjects()
{
  if (!infoObjectsPtr_) {
    infoObjectsPtr_ = new simpleObjectRegistry{100};
  }
  return *infoObjectsPtr_;
}


mousse::simpleObjectRegistry& mousse::debug::optimisationObjects()
{
  if (!optimisationObjectsPtr_) {
    optimisationObjectsPtr_ = new simpleObjectRegistry{100};
  }
  return *optimisationObjectsPtr_;
}


mousse::simpleObjectRegistry& mousse::debug::dimensionSetObjects()
{
  if (!dimensionSetObjectsPtr_) {
    dimensionSetObjectsPtr_ = new simpleObjectRegistry{100};
  }
  return *dimensionSetObjectsPtr_;
}


mousse::simpleObjectRegistry& mousse::debug::dimensionedConstantObjects()
{
  if (!dimensionedConstantObjectsPtr_) {
    dimensionedConstantObjectsPtr_ = new simpleObjectRegistry{100};
  }
  return *dimensionedConstantObjectsPtr_;
}

