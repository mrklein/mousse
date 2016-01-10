// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   debug.cpp
#ifndef debug_hpp_
#define debug_hpp_
namespace mousse
{
// Forward declaration of classes
class dictionary;
class Istream;
class Ostream;
class simpleRegIOobject;
class simpleObjectRegistry;
namespace debug
{
  //- The central control dictionary.
  //  Located in ~/.OpenFOAM/VERSION or $WM_PROJECT_DIR/etc
  //  \sa mousse::findEtcFile()
  dictionary& controlDict();
  //- The DebugSwitches sub-dictionary in the central controlDict.
  dictionary& debugSwitches();
  //- The InfoSwitches sub-dictionary in the central controlDict.
  dictionary& infoSwitches();
  //- The OptimisationSwitches sub-dictionary in the central controlDict.
  dictionary& optimisationSwitches();
  //- Lookup debug switch or add default value.
  int debugSwitch(const char* name, const int defaultValue=0);
  //- Lookup info switch or add default value.
  int infoSwitch(const char* name, const int defaultValue=0);
  //- Lookup optimisation switch or add default value.
  int optimisationSwitch(const char* name, const int defaultValue=0);
  //- Internal function to lookup a sub-dictionary from controlDict.
  dictionary& switchSet(const char* subDictName, dictionary*& subDictPtr);
  // Registered debug switches
    //- Register debug switch read/write object
    void addDebugObject(const char* name, simpleRegIOobject* obj);
    //- Register info switch read/write object
    void addInfoObject(const char* name, simpleRegIOobject* obj);
    //- Register optimisation switch read/write object
    void addOptimisationObject(const char* name, simpleRegIOobject* obj);
    //- Register DimensionSets read/write object
    void addDimensionSetObject(const char* name, simpleRegIOobject* obj);
    //- Register DimensionedConstant read/write object
    void addDimensionedConstantObject(const char* name, simpleRegIOobject*);
    //- Get access to registered debug switch objects
    simpleObjectRegistry& debugObjects();
    //- Get access to registered info switch objects
    simpleObjectRegistry& infoObjects();
    //- Get access to registered optimisation switch objects
    simpleObjectRegistry& optimisationObjects();
    //- Get access to registered dimensionSets switch objects
    simpleObjectRegistry& dimensionSetObjects();
    //- Get access to registered dimensionedConstant switch objects
    simpleObjectRegistry& dimensionedConstantObjects();
}  // namespace debug
}  // namespace mousse
#endif
