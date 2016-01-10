// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
// Member Functions 
inline const mousse::word& mousse::argList::executable() const
{
  return executable_;
}
inline const mousse::fileName& mousse::argList::rootPath() const
{
  return rootPath_;
}
inline const mousse::fileName& mousse::argList::caseName() const
{
  return case_;
}
inline const mousse::fileName& mousse::argList::globalCaseName() const
{
  return globalCase_;
}
inline const mousse::ParRunControl& mousse::argList::parRunControl() const
{
  return parRunControl_;
}
inline mousse::fileName mousse::argList::path() const
{
  return rootPath()/caseName();
}
inline const mousse::stringList& mousse::argList::args() const
{
  return args_;
}
inline mousse::stringList& mousse::argList::args()
{
  return args_;
}
inline const mousse::string& mousse::argList::arg(const label index) const
{
  return args_[index];
}
inline mousse::label mousse::argList::size() const
{
  return args_.size();
}
inline const mousse::HashTable<mousse::string>& mousse::argList::options() const
{
  return options_;
}
inline mousse::HashTable<mousse::string>& mousse::argList::options()
{
  return options_;
}
inline const mousse::string& mousse::argList::option(const word& opt) const
{
  return options_[opt];
}
inline bool mousse::argList::optionFound(const word& opt) const
{
  return options_.found(opt);
}
inline mousse::IStringStream mousse::argList::optionLookup(const word& opt) const
{
  return IStringStream(options_[opt]);
}
// Template Specializations 
namespace mousse
{
  // Template specialization for string
  template<>
  inline mousse::string
  mousse::argList::argRead<mousse::string>(const label index) const
  {
    return args_[index];
  }
  // Template specialization for word
  template<>
  inline mousse::word
  mousse::argList::argRead<mousse::word>(const label index) const
  {
    return args_[index];
  }
  // Template specialization for fileName
  template<>
  inline mousse::fileName
  mousse::argList::argRead<mousse::fileName>(const label index) const
  {
    return args_[index];
  }
  // Template specialization for string
  template<>
  inline mousse::string
  mousse::argList::optionRead<mousse::string>(const word& opt) const
  {
    return options_[opt];
  }
  // Template specialization for word
  template<>
  inline mousse::word
  mousse::argList::optionRead<mousse::word>(const word& opt) const
  {
    return options_[opt];
  }
  // Template specialization for fileName
  template<>
  inline mousse::fileName
  mousse::argList::optionRead<mousse::fileName>(const word& opt) const
  {
    return options_[opt];
  }
}
// Member Functions 
template<class T>
inline T mousse::argList::argRead(const label index) const
{
  T val;
  IStringStream(args_[index])() >> val;
  return val;
}
template<class T>
inline T mousse::argList::optionRead(const word& opt) const
{
  T val;
  optionLookup(opt)() >> val;
  return val;
}
template<class T>
inline bool mousse::argList::optionReadIfPresent
(
  const word& opt,
  T& val
) const
{
  if (optionFound(opt))
  {
    val = optionRead<T>(opt);
    return true;
  }
  else
  {
    return false;
  }
}
template<class T>
inline bool mousse::argList::optionReadIfPresent
(
  const word& opt,
  T& val,
  const T& deflt
) const
{
  if (optionReadIfPresent<T>(opt, val))
  {
    return true;
  }
  else
  {
    val = deflt;
    return false;
  }
}
template<class T>
inline T mousse::argList::optionLookupOrDefault
(
  const word& opt,
  const T& deflt
) const
{
  if (optionFound(opt))
  {
    return optionRead<T>(opt);
  }
  else
  {
    return deflt;
  }
}
// Member Operators 
inline const mousse::string& mousse::argList::operator[](const label index) const
{
  return args_[index];
}
inline const mousse::string& mousse::argList::operator[](const word& opt) const
{
  return options_[opt];
}
