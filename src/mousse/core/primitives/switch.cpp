// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "switch.hpp"
#include "error.hpp"
#include "dictionary.hpp"


// Static Data Members
// NB: values chosen such that bitwise '&' 0x1 yields the bool value
// INVALID is also evaluates to false, but don't rely on that
const char* mousse::Switch::names[mousse::Switch::INVALID+1] =
{
  "false", "true",
  "off",   "on",
  "no",    "yes",
  "n",     "y",
  "f",     "t",
  "none",  "true",  // is there a reasonable counterpart to "none"?
  "invalid"
};


// Static Member Functions
mousse::Switch::switchType mousse::Switch::asEnum
(
  const std::string& str,
  const bool allowInvalid
)
{
  for (int sw = 0; sw < Switch::INVALID; ++sw) {
    if (str == names[sw]) {
      // handle aliases
      switch (sw) {
        case Switch::NO_1:
        case Switch::NONE:
        {
          return Switch::NO;
          break;
        }
        case Switch::YES_1:
        {
          return Switch::YES;
          break;
        }
        case Switch::FALSE_1:
        {
          return Switch::FALSE;
          break;
        }
        case Switch::TRUE_1:
        {
          return Switch::TRUE;
          break;
        }
        default:
        {
          return switchType(sw);
          break;
        }
      }
    }
  }
  if (!allowInvalid) {
    FATAL_ERROR_IN("Switch::asEnum(const std::string&, const bool)")
      << "unknown switch word " << str << nl
      << abort(FatalError);
  }
  return Switch::INVALID;
}


mousse::Switch mousse::Switch::lookupOrAddToDict
(
  const word& name,
  dictionary& dict,
  const Switch& defaultValue
)
{
  return dict.lookupOrAddDefault<Switch>(name, defaultValue);
}


// Member Functions
bool mousse::Switch::valid() const
{
  return switch_ <= Switch::NONE;
}


const char* mousse::Switch::asText() const
{
  return names[switch_];
}


bool mousse::Switch::readIfPresent(const word& name, const dictionary& dict)
{
  return dict.readIfPresent<Switch>(name, *this);
}

