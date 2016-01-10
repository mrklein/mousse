// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef class_name_hpp_
#define class_name_hpp_

#include "define_debug_switch.hpp"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// declarations (without debug information)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//- Add typeName information from argument \a TypeNameString to a class.
//  Without debug information
#define CLASS_NAME_NO_DEBUG(TypeNameString)                                   \
  static const char* typeName_() { return TypeNameString; }                   \
  static const ::mousse::word typeName

//- Add typeName information from argument \a TypeNameString to a namespace.
//  Without debug information.
#define NAMESPACE_NAME_NO_DEBUG(TypeNameString)                               \
  inline const char* typeName_() { return TypeNameString; }                   \
  extern const ::mousse::word typeName

//- Add typeName information from argument \a TemplateNameString to a
//  template class.  Without debug information.
#define TEMPLATE_NAME_NO_DEBUG(TemplateNameString)                            \
class TemplateNameString##Name                                                \
{                                                                             \
public:                                                                       \
  TemplateNameString##Name() {}                                               \
  CLASS_NAME_NO_DEBUG(#TemplateNameString);                                   \
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// declarations (with debug information)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//- Add typeName information from argument \a TypeNameString to a class.
//  Also declares debug information.
#define CLASS_NAME(TypeNameString)                                            \
  CLASS_NAME_NO_DEBUG(TypeNameString);                                        \
  static int debug

//- Add typeName information from argument \a TypeNameString to a namespace.
//  Also declares debug information.
#define NAMESPACE_NAME(TypeNameString)                                        \
  NAMESPACE_NAME_NO_DEBUG(TypeNameString);                                    \
  extern int debug

//- Add typeName information from argument \a TypeNameString to a
//  template class.  Also declares debug information.
#define TEMPLATE_NAME(TemplateNameString)                                     \
class TemplateNameString##Name                                                \
{                                                                             \
public:                                                                       \
  TemplateNameString##Name() {}                                               \
  CLASS_NAME(#TemplateNameString);                                            \
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// definitions (without debug information)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//- Define the typeName, with alternative lookup as \a Name
#define DEFINE_TYPE_NAME_WITH_NAME(Type, Name)                                \
  const ::mousse::word Type::typeName(Name)

//- Define the typeName
#define DEFINE_TYPE_NAME(Type)                                                \
  DEFINE_TYPE_NAME_WITH_NAME(Type, Type::typeName_())

//- Define the typeName as \a Name for template classes
#define DEFINE_TEMPLATE_TYPE_NAME_WITH_NAME(Type, Name)                       \
  template<>                                                                  \
  DEFINE_TYPE_NAME_WITH_NAME(Type, Name)

//- Define the typeName as \a Name for template sub-classes
#define DEFINE_TEMPLATE2_TYPE_NAME_WITH_NAME(Type, Name)                      \
  template<>                                                                  \
  DEFINE_TYPE_NAME_WITH_NAME(Type, Name)

//- Define the typeName for template classes, useful with typedefs
#define DEFINE_TEMPLATE_TYPE_NAME(Type)                                       \
  DEFINE_TEMPLATE_TYPE_NAME_WITH_NAME(Type, #Type)

//- Define the typeName directly for template classes
#define DEFINE_NAMED_TEMPLATE_TYPE_NAME(Type)                                 \
  DEFINE_TEMPLATE_TYPE_NAME_WITH_NAME(Type, Type::typeName_())

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// definitions (with debug information)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//- Define the typeName and debug information
#define DEFINE_TYPE_NAME_AND_DEBUG(Type, DebugSwitch)                         \
  DEFINE_TYPE_NAME(Type);                                                     \
  DEFINE_DEBUG_SWITCH(Type, DebugSwitch)

//- Define the typeName and debug information, lookup as \a Name
#define DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME(Type, Name, DebugSwitch)\
  DEFINE_TEMPLATE_TYPE_NAME_WITH_NAME(Type, Name);                            \
  DEFINE_TEMPLATE_DEBUG_SWITCH_WITH_NAME(Type, Name, DebugSwitch)

//- Define the typeName and debug information for templates, useful
//  with typedefs
#define DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(Type, DebugSwitch)                \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME(Type, #Type, DebugSwitch)

//- Define the typeName and debug information for templates
#define DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(Type, DebugSwitch)          \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME(Type);                                      \
  DEFINE_NAMED_TEMPLATE_DEBUG_SWITCH(Type, DebugSwitch)

// for templated sub-classes
//- Define the typeName and debug information, lookup as \a Name
#define DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG_WITH_NAME(Type, Name, DebugSwitch)\
  DEFINE_TEMPLATE2_TYPE_NAME_WITH_NAME(Type, Name);                           \
  DEFINE_TEMPLATE2_DEBUG_SWITCH_WITH_NAME(Type, Name, DebugSwitch)

//- Define the typeName and debug information for templates, useful
//  with typedefs
#define DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG(Type, DebugSwitch)               \
  DEFINE_TEMPLATE2_TYPE_NAME_AND_DEBUG_WITH_NAME(Type, #Type, DebugSwitch)

#endif
