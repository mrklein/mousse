#ifndef CORE_DB_FUNCTION_OBJECTS_FUNCTION_OBJECT_FILE_HPP_
#define CORE_DB_FUNCTION_OBJECTS_FUNCTION_OBJECT_FILE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::functionObjectFile
// Description
//   Base class for output file data handling
// See Also
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject

#include "object_registry.hpp"
#include "ofstream.hpp"
#include "ptr_list.hpp"
#include "hash_set.hpp"
#include "iomanip.hpp"


namespace mousse {

class functionObjectFile
{
  // Private data

    //- Reference to the database
    const objectRegistry& obr_;

    //- Prefix
    const word prefix_;

    //- File names
    wordList names_;

    //- File pointer
    PtrList<OFstream> filePtrs_;

protected:

  // Protected Member Functions

    //- Initialise the output stream for writing
    virtual void initStream(Ostream& os) const;

    //- Return the base directory for output
    virtual fileName baseFileDir() const;

    //- Return the base directory for the current time value
    virtual fileName baseTimeDir() const;

    //- Create the output file
    virtual void createFiles();

    //- File header information
    virtual void writeFileHeader(const label i = 0);

    //- Write function
    virtual void write();

    //- Reset the list of names from a wordList
    virtual void resetNames(const wordList& names);

    //- Reset the list of names to a single name entry
    virtual void resetName(const word& name);

    //- Return the value width when writing to stream with optional offset
    virtual Omanip<int> valueWidth(const label offset = 0) const;

public:

  //- Directory prefix
  static const word outputPrefix;

  //- Additional characters for writing
  static label addChars;

  // Constructors

    //- Construct null
    functionObjectFile(const objectRegistry& obr, const word& prefix);

    //- Construct from components
    functionObjectFile
    (
      const objectRegistry& obr,
      const word& prefix,
      const word& name
    );

    //- Construct from components
    functionObjectFile
    (
      const objectRegistry& obr,
      const word& prefix,
      const wordList& names
    );

    //- Disallow default bitwise copy construct
    functionObjectFile(const functionObjectFile&) = delete;

    //- Disallow default bitwise assignment
    functionObjectFile& operator=(const functionObjectFile&) = delete;

  //- Destructor
  virtual ~functionObjectFile();

  // Member Functions

    //- Return const access to the names
    const wordList& names() const;

    //- Return access to the file (if only 1)
    OFstream& file();

    //- Return access to the files
    PtrList<OFstream>& files();

    //- Return file 'i'
    OFstream& file(const label i);

    //- Write a commented string to stream
    void writeCommented
    (
      Ostream& os,
      const string& str
    ) const;

    //- Write a tabbed string to stream
    void writeTabbed
    (
      Ostream& os,
      const string& str
    ) const;

    //- Write a commented header to stream
    void writeHeader
    (
      Ostream& os,
      const string& str
    ) const;

    //- Write a (commented) header property and value pair
    template<class Type>
    void writeHeaderValue
    (
      Ostream& os,
      const string& property,
      const Type& value
    ) const;

    //- Return width of character stream output
    label charWidth() const;

};

}  // namespace mousse

#include "function_object_file.ipp"

#endif
