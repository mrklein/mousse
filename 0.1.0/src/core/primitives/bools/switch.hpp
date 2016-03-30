#ifndef CORE_PRIMITIVES_BOOLS_SWITCH_HPP_
#define CORE_PRIMITIVES_BOOLS_SWITCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Switch
// Description
//   A simple wrapper around bool so that it can be read as a word:
//   true/false, on/off, yes/no, y/n, t/f, or none.
// SourceFiles
//   switch.cpp
//   switch_io.cpp

#include "bool.hpp"
#include "word.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
class Switch;
class dictionary;

Istream& operator>>(Istream&, Switch&);
Ostream& operator<<(Ostream&, const Switch&);


class Switch
{
  // Private data
    //- The logic and enumerated text representation stored as a single byte
    unsigned char switch_;
public:
  // Public data types
    // avoid issues with pre-processor defines
    #undef FALSE
    #undef TRUE
    #undef OFF
    #undef ON
    #undef NO
    #undef YES
    #undef NO_1
    #undef YES_1
    #undef FALSE_1
    #undef TRUE_1
    #undef NONE
    #undef PLACEHOLDER
    #undef INVALID
    //- The various text representations for a switch value.
    //  These also correspond to the entries in names.
    enum switchType
    {
      FALSE   = 0,  TRUE   = 1,
      OFF     = 2,  ON     = 3,
      NO      = 4,  YES    = 5,
      NO_1    = 6,  YES_1  = 7,
      FALSE_1 = 8,  TRUE_1 = 9,
      NONE    = 10, PLACEHOLDER = 11,
      INVALID
    };
  // Static data members
    //- The set of names corresponding to the switchType enumeration
    //  Includes an extra entry for "invalid".
    static const char* names[INVALID+1];
private:
  // Static Member Functions
    //- Return a switchType representation of a word
    static switchType asEnum(const std::string&, const bool allowInvalid);
public:
  // Constructors
    //- Construct null as false
    Switch()
    :
      switch_(Switch::FALSE)
    {}
    //- Construct from enumerated value
    Switch(const switchType sw)
    :
      switch_(sw)
    {}
    //- Construct from bool
    Switch(const bool b)
    :
      switch_(b ? Switch::TRUE : Switch::FALSE)
    {}
    //- Construct from integer values (treats integer as bool value)
    Switch(const int i)
    :
      switch_(i ? Switch::TRUE : Switch::FALSE)
    {}
    //- Construct from std::string, string, word
    //  Optionally allow bad words, and catch the error elsewhere
    Switch(const std::string& str, const bool allowInvalid=false)
    :
      switch_(asEnum(str, allowInvalid))
    {}
    //- Construct from character array
    //  Optionally allow bad words, and catch the error elsewhere
    Switch(const char* str, const bool allowInvalid=false)
    :
      switch_(asEnum(std::string(str), allowInvalid))
    {}
    //- Construct from Istream
    Switch(Istream& is);
    //- Construct from dictionary, supplying default value so that if the
    //  value is not found, it is added into the dictionary.
    static Switch lookupOrAddToDict
    (
      const word&,
      dictionary&,
      const Switch& defaultValue = false
    );
  // Member Functions
    //- Return true if the Switch has a valid value
    bool valid() const;
    //- Return a text representation of the Switch
    const char* asText() const;
    //- Update the value of the Switch if it is found in the dictionary
    bool readIfPresent(const word&, const dictionary&);
  // Member Operators
    //- Conversion to bool
    operator bool() const
    {
      return (switch_ & 0x1);
    }
    //- Assignment from enumerated value
    const Switch& operator=(const switchType sw)
    {
      switch_ = sw;
      return *this;
    }
    //- Assignment from bool
    const Switch& operator=(const bool b)
    {
      switch_ = (b ? Switch::TRUE : Switch::FALSE);
      return *this;
    }
  // IOstream Operators
    friend Istream& operator>>(Istream&, Switch&);
    friend Ostream& operator<<(Ostream&, const Switch&);
};

}  // namespace mousse

#endif
