// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RegisterSwitch
// Description
//   Class and registration macros for InfoSwitches and OptimisationSwitches
//   to support reading from system/controlDict and dynamic update.

#ifndef register_switch_hpp_
#define register_switch_hpp_

#include "simple_reg_ioobject.hpp"

namespace mousse
{
template<class Type>
class RegisterSwitch
:
  public simpleRegIOobject
{
  Type& optSwitch_;
public:
  RegisterSwitch
  (
    void (*registryFn)(const char* name, simpleRegIOobject*),
    const char* name,
    Type& optSwitch
  )
  :
    simpleRegIOobject(registryFn, name),
    optSwitch_(optSwitch)
  {}
  virtual ~RegisterSwitch()
  {}
  virtual void readData(mousse::Istream& is)
  {
    is >> optSwitch_;
  }
  virtual void writeData(mousse::Ostream& os) const
  {
    os << optSwitch_;
  }
};
}  // namespace mousse

#define CONCAT(x, y) x ## y
#define CONCAT2(x, y) CONCAT(x, y)
#define FILE_UNIQUE(x) CONCAT2(x, __LINE__)

#define REGISTER_OPT_SWITCH(Name, Type, Switch)                               \
  static mousse::RegisterSwitch<Type> FILE_UNIQUE(_addToOpt_)                 \
    (mousse::debug::addOptimisationObject, Name, Switch)

#define REGISTER_INFO_SWITCH(Name, Type, Switch)                              \
  static mousse::RegisterSwitch<Type> FILE_UNIQUE(_addToOpt_)                 \
    (mousse::debug::addInfoObject, Name, Switch)
#endif
