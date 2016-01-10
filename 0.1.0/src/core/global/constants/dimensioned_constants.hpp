// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   dimensioned_constants.cpp

#ifndef dimensioned_constants_hpp_
#define dimensioned_constants_hpp_

#include "dictionary.hpp"
#include "dimensioned_scalar.hpp"

namespace mousse
{
dictionary& dimensionedConstants();
dimensionedScalar dimensionedConstant(const word& group, const word& varName);
template<class T>
T dimensionedConstant
(
  const word& group,
  const word& varName,
  const T& defaultValue
)
{
  dictionary& dict = dimensionedConstants();
  const word unitSet(dict.lookup("unitSet"));
  dictionary& unitDict(dict.subDict(unitSet + "Coeffs"));
  if (unitDict.found(group))
  {
    dictionary& groupDict = unitDict.subDict(group);
    if (groupDict.found(varName))
    {
      return pTraits<T>(groupDict.lookup(varName));
    }
    else
    {
      groupDict.add(varName, defaultValue);
      return defaultValue;
    }
  }
  else
  {
    unitDict.add(group, dictionary::null);
    unitDict.subDict(group).add(varName, defaultValue);
    return defaultValue;
  }
}

//- Defined dimensioned constant , lookup as \a Name
#define DEFINE_DIMENSIONED_CONSTANT(Group,Switch,Tag,Name)                    \
  const mousse::dimensionedScalar Switch;                                     \
  class add##Tag##ToDimensionedConstant                                       \
  :                                                                           \
    public mousse::simpleRegIOobject                                          \
  {                                                                           \
  public:                                                                     \
    add##Tag##ToDimensionedConstant(const char* name)                         \
    :                                                                         \
      mousse::simpleRegIOobject                                               \
      (mousse::debug::addDimensionedConstantObject,name)                      \
    {                                                                         \
      mousse::dimensionedScalar ds                                            \
      (                                                                       \
        mousse::dimensionedConstant                                           \
        (                                                                     \
          Group,                                                              \
          Name                                                                \
        )                                                                     \
      );                                                                      \
      mousse::dimensionedScalar& s = const_cast<mousse::dimensionedScalar&>   \
      (                                                                       \
        Switch                                                                \
      );                                                                      \
      s.dimensions().reset(ds.dimensions());                                  \
      s = ds;                                                                 \
    }                                                                         \
    virtual ~add##Tag##ToDimensionedConstant()                                \
    {}                                                                        \
    virtual void readData(mousse::Istream&)                                   \
    {                                                                         \
      const_cast<mousse::dimensionedScalar&>(Switch) =                        \
        mousse::dimensionedConstant                                           \
        (                                                                     \
          Group,                                                              \
          Name                                                                \
        );                                                                    \
    }                                                                         \
    virtual void writeData(mousse::Ostream& os) const                         \
    {                                                                         \
      os << Switch;                                                           \
    }                                                                         \
  };                                                                          \
  add##Tag##ToDimensionedConstant add##Tag##ToDimensionedConstant_(Name)

//- Defined dimensioned constant with default , lookup as \a Name
#define DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT\
(Group,Switch,DefaultExpr,Tag,Name)                                           \
  const mousse::dimensionedScalar Switch;                                     \
  class add##Tag##ToDimensionedConstantWithDefault                            \
  :                                                                           \
    public mousse::simpleRegIOobject                                          \
  {                                                                           \
  public:                                                                     \
    add##Tag##ToDimensionedConstantWithDefault(const char* name)              \
    :                                                                         \
      mousse::simpleRegIOobject                                               \
      (mousse::debug::addDimensionedConstantObject,name)                      \
    {                                                                         \
      mousse::dimensionedScalar ds                                            \
      (                                                                       \
        mousse::dimensionedConstant                                           \
        (                                                                     \
          Group,                                                              \
          Name,                                                               \
          mousse::dimensionedScalar(Name,DefaultExpr)                         \
        )                                                                     \
      );                                                                      \
      mousse::dimensionedScalar& s = const_cast<mousse::dimensionedScalar&>   \
      (                                                                       \
        Switch                                                                \
      );                                                                      \
      s.dimensions().reset(ds.dimensions());                                  \
      s = ds;                                                                 \
    }                                                                         \
    virtual ~add##Tag##ToDimensionedConstantWithDefault()                     \
    {}                                                                        \
    virtual void readData(mousse::Istream&)                                   \
    {                                                                         \
      const_cast<mousse::dimensionedScalar&>(Switch) =                        \
        mousse::dimensionedConstant                                           \
        (                                                                     \
          Group,                                                              \
          Name,                                                               \
          mousse::dimensionedScalar(Name,DefaultExpr)                         \
        );                                                                    \
    }                                                                         \
    virtual void writeData(mousse::Ostream& os) const                         \
    {                                                                         \
      os << Switch;                                                           \
    }                                                                         \
  };                                                                          \
  add##Tag##ToDimensionedConstantWithDefault                                  \
    add##Tag##ToDimensionedConstantWithDefault_(Name)
}  // namespace mousse
#endif
