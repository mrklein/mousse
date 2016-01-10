// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::demandDrivenEntry
// Description
//   Class for demand-driven dictionary entries
//   Holds a reference to a dictionary, which is then queried if the value
//   is requested and has not already been cached
// SourceFiles
//   demand_driven_entry.cpp
//   demand_driven_entry_i.hpp
#ifndef demand_driven_entry_hpp_
#define demand_driven_entry_hpp_
#include "dictionary.hpp"
namespace mousse
{
template<class Type>
class demandDrivenEntry
{
private:
  // Private data
    //- Reference to the dictionary
    const dictionary& dict_;
    //- Keyword to look up
    const word keyword_;
    //- Value
    mutable Type value_;
    //- Flag to say that the value has been stored
    mutable bool stored_;
public:
  //- Constructors
    //- Construct from dictionary and value - cannot be re-read
    demandDrivenEntry
    (
      const dictionary& dict,
      const Type& value
    );
    //- Construct from dictionary and keyword
    demandDrivenEntry
    (
      const dictionary& dict,
      const word& keyword
    );
    //- Construct from dictionary, keyword and default value
    demandDrivenEntry
    (
      const dictionary& dict,
      const word& keyword,
      const Type& defaultValue,
      const bool readIfPresent = true
    );
    //- Copy constructor
    demandDrivenEntry(const demandDrivenEntry& dde);
  // Public Member Functions
    //- Initialise
    inline void initialise() const;
    //- Return the value
    inline const Type& value() const;
    //- Set the value
    inline void setValue(const Type& value);
    //- Reset the demand-driven entry
    inline void reset();
};
}  // namespace mousse
#include "demand_driven_entry_i.hpp"
#ifdef NoRepository
  #include "demand_driven_entry.cpp"
#endif
#endif
