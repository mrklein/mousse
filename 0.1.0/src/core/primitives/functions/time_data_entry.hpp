// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TimeDataEntry
// Description
//   Light wrapper around DataEntry to provide a mechanism to update time-based
//   entries.
// SourceFiles
//   time_data_entry.cpp
#ifndef time_data_entry_hpp_
#define time_data_entry_hpp_
#include "data_entry.hpp"
#include "time.hpp"
namespace mousse
{
template<class Type>
class TimeDataEntry;
template<class Type>
Ostream& operator<<
(
  Ostream&,
  const TimeDataEntry<Type>&
);
template<class Type>
class TimeDataEntry
{
protected:
  // Protected data
    //- Reference to the time database
    const Time& time_;
    //- Name of the data entry
    const word name_;
    //- The underlying DataEntry
    autoPtr<DataEntry<Type> > entry_;
public:
  // Constructor
    //- Construct from entry name
    TimeDataEntry
    (
      const Time& t,
      const word& name,
      const dictionary& dict
    );
    //- Construct null from entry name
    TimeDataEntry
    (
      const Time& t,
      const word& entryName
    );
    //- Copy constructor
    TimeDataEntry(const TimeDataEntry<Type>& tde);
  //- Destructor
  virtual ~TimeDataEntry();
  // Member Functions
    // Access
      //- Reset entry by re-reading from dictionary
      void reset(const dictionary& dict);
      //- Return the name of the entry
      const word& name() const;
    // Evaluation
      //- Return value as a function of (scalar) independent variable
      virtual Type value(const scalar x) const;
      //- Integrate between two (scalar) values
      virtual Type integrate(const scalar x1, const scalar x2) const;
  // I/O
    //- Ostream Operator
    friend Ostream& operator<< <Type>
    (
      Ostream& os,
      const TimeDataEntry<Type>& de
    );
    //- Write in dictionary format
    virtual void writeData(Ostream& os) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "time_data_entry.cpp"
#endif
#endif
