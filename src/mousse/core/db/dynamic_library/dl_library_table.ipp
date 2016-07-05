// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dl_library_table.hpp"
#include "dictionary.hpp"
#include "file_name_list.hpp"


// Member Functions 
template<class TablePtr>
bool mousse::dlLibraryTable::open
(
  const dictionary& dict,
  const word& libsEntry,
  const TablePtr& tablePtr
)
{
  if (dict.found(libsEntry)) {
    fileNameList libNames{dict.lookup(libsEntry)};
    bool allOpened = (libNames.size() > 0);
    FOR_ALL(libNames, i) {
      const fileName& libName = libNames[i];
      label nEntries = 0;
      if (tablePtr) {
        nEntries = tablePtr->size();
      }
      bool opened = dlLibraryTable::open(libName);
      allOpened = opened && allOpened;
      if (!opened) {
        WARNING_IN
        (
          "dlLibraryTable::open"
          "(const dictionary&, const word&, "
          "const TablePtr&)"
        )
        << "Could not open library " << libName
        << endl << endl;
      } else if (debug && (!tablePtr || tablePtr->size() <= nEntries)) {
        WARNING_IN
        (
          "dlLibraryTable::open"
          "(const dictionary&, const word&, "
          "const TablePtr&)"
        )
        << "library " << libName
        << " did not introduce any new entries"
        << endl << endl;
      }
    }
    return allOpened;
  } else {
    return false;
  }
}
