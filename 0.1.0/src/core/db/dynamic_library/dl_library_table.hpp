// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dlLibraryTable
// Description
//   A table of dynamically loaded libraries
// SourceFiles
//   dl_library_table.cpp

#ifndef dl_library_table_hpp_
#define dl_library_table_hpp_

#include "label.hpp"
#include "dynamic_list.hpp"
#include "class_name.hpp"

namespace mousse
{
class dlLibraryTable
{
  // Private Member Functions
    DynamicList<void*> libPtrs_;
    DynamicList<fileName> libNames_;

public:

  // Declare name of the class and its debug switch
  CLASS_NAME("dlLibraryTable");

  // Constructors

    //- Construct null
    dlLibraryTable();

    //- Construct from dictionary and name of 'libs' entry giving
    //  the libraries to load
    dlLibraryTable(const dictionary&, const word&);

    //- Disallow default bitwise copy construct
    dlLibraryTable(const dlLibraryTable&) = delete;

    //- Disallow default bitwise assignment
    void operator=(const dlLibraryTable&) = delete;

  //- Destructor
  ~dlLibraryTable();

  // Member Functions

    //- Open the named library, optionally with warnings if problems occur
    bool open(const fileName& name, const bool verbose = true);

    //- Close the named library, optionally with warnings if problems occur
    bool close(const fileName& name, const bool verbose = true);

    //- Find the handle of the named library
    void* findLibrary(const fileName& name);

    //- Open all the libraries listed in the 'libsEntry' entry in the
    //  given dictionary if present
    bool open(const dictionary&, const word& libsEntry);

    //- Open all the libraries listed in the 'libsEntry' entry in the
    //  given dictionary if present and check the additions
    //  to the given constructor table
    template<class TablePtr>
    bool open
    (
      const dictionary&,
      const word& libsEntry,
      const TablePtr& tablePtr
    );

};

}  // namespace mousse

#ifdef NoRepository
#   include "dl_library_table_templates.cpp"
#endif
#endif
