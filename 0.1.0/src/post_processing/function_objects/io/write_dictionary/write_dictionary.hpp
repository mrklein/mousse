// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::writeDictionary
// Group
//   grpUtilitiesFunctionObjects
// Description
//   This function object writes dictionaries on start-up, and on change
// SourceFiles
//   write_dictionary.cpp
//   i_owrite_dictionary.hpp
#ifndef write_dictionary_hpp_
#define write_dictionary_hpp_
#include "word_list.hpp"
#include "run_time_selection_tables.hpp"
#include "sha1_digest.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class writeDictionary
{
protected:
  // Private data
    //- Name of this set of writeDictionary
    word name_;
    //- Reference to the database
    const objectRegistry& obr_;
    //- Names of dictionaries to monitor
    wordList dictNames_;
    //- List of changed dictionaries (only those registered to database)
    List<SHA1Digest> digests_;
  // Private Member Functions
    //- Helper function to write the dictionary if found at location
    bool tryDirectory
    (
      const label dictI,
      const word& location,
      bool& firstDict
    );
public:
  //- Runtime type information
  TYPE_NAME("writeDictionary");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    writeDictionary
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    writeDictionary(const writeDictionary&) = delete;
    //- Disallow default bitwise assignment
    writeDictionary& operator=(const writeDictionary&) = delete;
  //- Destructor
  virtual ~writeDictionary();
  // Member Functions
    //- Return name of the writeDictionary
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the writeDictionary data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Write the writeDictionary
    virtual void write();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh
    virtual void movePoints(const polyMesh&)
    {}
};
}  // namespace mousse
#endif
