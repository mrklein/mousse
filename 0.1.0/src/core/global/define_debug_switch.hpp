#ifndef CORE_GLOBAL_DEFINE_DEBUG_SWITCH_HPP_
#define CORE_GLOBAL_DEFINE_DEBUG_SWITCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "simple_reg_ioobject.hpp"
#include "debug.hpp"
#include "label.hpp"


// definitions (debug information only)
namespace mousse {

//- Define the debug information, lookup as \a Name
template<class Type>
class RegisterDebugSwitch
:
  public ::mousse::simpleRegIOobject
{
public:
  //- The unique RegisterDebugSwitch object
  static const RegisterDebugSwitch registerDebugSwitch;
  RegisterDebugSwitch(const char* name)
  :
    ::mousse::simpleRegIOobject{mousse::debug::addDebugObject, name}
  {}
  virtual ~RegisterDebugSwitch()
  {}
  virtual void readData(mousse::Istream& is)
  {
    Type::debug = readLabel(is);
  }
  virtual void writeData(mousse::Ostream& os) const
  {
    os << Type::debug;
  }
};
}  // namespace mousse


#define REGISTER_TEMPLATE_DEBUG_SWITCH_WITH_NAME(Type,Name)                   \
  template<>                                                                  \
  const mousse::RegisterDebugSwitch<Type>                                     \
    mousse::RegisterDebugSwitch<Type>::registerDebugSwitch(Name)


//- Define the debug information, lookup as \a Name
#define REGISTER_DEBUG_SWITCH_WITH_NAME(Type,Tag,Name)                        \
  class add##Tag##ToDebug                                                     \
  :                                                                           \
    public ::mousse::simpleRegIOobject                                        \
  {                                                                           \
  public:                                                                     \
    add##Tag##ToDebug(const char* name)                                       \
    :                                                                         \
      ::mousse::simpleRegIOobject{mousse::debug::addDebugObject, name}        \
    {}                                                                        \
    virtual ~add##Tag##ToDebug()                                              \
    {}                                                                        \
    virtual void readData(mousse::Istream& is)                                \
    {                                                                         \
      Type::debug = readLabel(is);                                            \
    }                                                                         \
    virtual void writeData(mousse::Ostream& os) const                         \
    {                                                                         \
      os << Type::debug;                                                      \
    }                                                                         \
  };                                                                          \
  add##Tag##ToDebug add##Tag##ToDebug_(Name)


//- Define the debug information, lookup as \a Name
#define DEFINE_DEBUG_SWITCH_WITH_NAME(Type, Name, DebugSwitch)                \
  int Type::debug(::mousse::debug::debugSwitch(Name, DebugSwitch))


//- Define the debug information
#define DEFINE_DEBUG_SWITCH(Type, DebugSwitch)                                \
  DEFINE_DEBUG_SWITCH_WITH_NAME(Type, Type::typeName_(), DebugSwitch);        \
  REGISTER_DEBUG_SWITCH_WITH_NAME(Type, Type, Type::typeName_())


//- Define the debug information for templates, lookup as \a Name
#define DEFINE_TEMPLATE_DEBUG_SWITCH_WITH_NAME(Type, Name, DebugSwitch)       \
  template<>                                                                  \
  DEFINE_DEBUG_SWITCH_WITH_NAME(Type, Name, DebugSwitch);                     \
  REGISTER_TEMPLATE_DEBUG_SWITCH_WITH_NAME(Type, Name)


//- Define the debug information for templates sub-classes, lookup as \a Name
# define DEFINE_TEMPLATE2_DEBUG_SWITCH_WITH_NAME(Type, Name, DebugSwitch)     \
  template<>                                                                  \
  DEFINE_DEBUG_SWITCH_WITH_NAME(Type, Name, DebugSwitch);                     \
  REGISTER_TEMPLATE_DEBUG_SWITCH_WITH_NAME(Type, Name)


//- Define the debug information for templates
//  Useful with typedefs
#define DEFINE_TEMPLATE_DEBUG_SWITCH(Type, DebugSwitch)                       \
  DEFINE_TEMPLATE_DEBUG_SWITCH_WITH_NAME(Type, #Type, DebugSwitch)


//- Define the debug information directly for templates
#define DEFINE_NAMED_TEMPLATE_DEBUG_SWITCH(Type, DebugSwitch)                 \
  DEFINE_TEMPLATE_DEBUG_SWITCH_WITH_NAME(Type, Type::typeName_(), DebugSwitch)


// For templated sub-classes
//- Define the debug information for templates
//  Useful with typedefs
#define DEFINE_TEMPLATE2_DEBUG_SWITCH(Type, DebugSwitch)                      \
  DEFINE_TEMPLATE2_DEBUG_SWITCH_WITH_NAME(Type, #Type, DebugSwitch)


//- Define the debug information directly for templates
#define DEFINE_NAMED_TEMPLATE2_DEBUG_SWITCH(Type, DebugSwitch)                \
  DEFINE_TEMPLATE2_DEBUG_SWITCH_WITH_NAME(Type, Type::typeName_(), DebugSwitch)

#endif
