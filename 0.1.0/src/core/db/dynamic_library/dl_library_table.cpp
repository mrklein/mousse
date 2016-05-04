// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dl_library_table.hpp"
#include "os_specific.hpp"
#include "int.hpp"
#include "dictionary.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(dlLibraryTable, 0);

}


// Constructors
mousse::dlLibraryTable::dlLibraryTable()
{}


mousse::dlLibraryTable::dlLibraryTable
(
  const dictionary& dict,
  const word& libsEntry
)
{
  open(dict, libsEntry);
}


// Destructor
mousse::dlLibraryTable::~dlLibraryTable()
{
  FOR_ALL_REVERSE(libPtrs_, i) {
    if (libPtrs_[i]) {
      if (debug) {
        Info
          << "dlLibraryTable::~dlLibraryTable() : closing "
          << libNames_[i]
          << " with handle " << uintptr_t(libPtrs_[i]) << endl;
      }
      dlClose(libPtrs_[i]);
    }
  }
}


// Member Functions
bool mousse::dlLibraryTable::open
(
  const fileName& functionLibName,
  const bool verbose
)
{
  if (functionLibName.size()) {
    void* functionLibPtr = dlOpen(functionLibName, verbose);
    if (debug) {
      Info
        << "dlLibraryTable::open : opened " << functionLibName
        << " resulting in handle " << uintptr_t(functionLibPtr) << endl;
    }
    if (!functionLibPtr) {
      if (verbose) {
        WARNING_IN
        (
          "dlLibraryTable::open(const fileName&, const bool)"
        )
        << "could not load " << functionLibName
        << endl;
      }
      return false;
    } else {
      libPtrs_.append(functionLibPtr);
      libNames_.append(functionLibName);
      return true;
    }
  }
  return false;
}


bool mousse::dlLibraryTable::close
(
  const fileName& functionLibName,
  const bool verbose
)
{
  label index = -1;
  FOR_ALL_REVERSE(libNames_, i) {
    if (libNames_[i] == functionLibName) {
      index = i;
      break;
    }
  }
  if (index != -1) {
    if (debug) {
      Info << "dlLibraryTable::close : closing " << functionLibName
        << " with handle " << uintptr_t(libPtrs_[index]) << endl;
    }
    bool ok = dlClose(libPtrs_[index]);
    libPtrs_[index] = nullptr;
    libNames_[index] = fileName::null;
    if (!ok) {
      if (verbose) {
        WARNING_IN
        (
          "dlLibraryTable::close(const fileName&)"
        )
        << "could not close " << functionLibName
        << endl;
      }
      return false;
    }
    return true;
  }
  return false;
}


void* mousse::dlLibraryTable::findLibrary(const fileName& functionLibName)
{
  label index = -1;
  FOR_ALL_REVERSE(libNames_, i) {
    if (libNames_[i] == functionLibName) {
      index = i;
      break;
    }
  }
  if (index != -1) {
    return libPtrs_[index];
  }
  return nullptr;
}


bool mousse::dlLibraryTable::open
(
  const dictionary& dict,
  const word& libsEntry
)
{
  if (dict.found(libsEntry)) {
    fileNameList libNames{dict.lookup(libsEntry)};
    bool allOpened = !libNames.empty();
    FOR_ALL(libNames, i) {
      allOpened = dlLibraryTable::open(libNames[i]) && allOpened;
    }
    return allOpened;
  } else {
    return false;
  }
}

