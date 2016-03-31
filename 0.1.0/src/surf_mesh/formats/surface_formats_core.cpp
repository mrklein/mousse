// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_formats_core.hpp"
#include "time.hpp"
#include "ifstream.hpp"
#include "ofstream.hpp"
#include "sortable_list.hpp"
#include "surf_mesh.hpp"


// Static Data Members
mousse::word mousse::fileFormats::surfaceFormatsCore::nativeExt{"ofs"};


// Static Member Functions
mousse::string mousse::fileFormats::surfaceFormatsCore::getLineNoComment
(
  IFstream& is
)
{
  string line;
  do {
    is.getLine(line);
  } while ((line.empty() || line[0] == '#') && is.good());
  return line;
}


bool mousse::fileFormats::surfaceFormatsCore::checkSupport
(
  const wordHashSet& available,
  const word& ext,
  const bool verbose,
  const word& functionName
)
{
  if (available.found(ext)) {
    return true;
  } else if (verbose) {
    wordList toc = available.toc();
    SortableList<word> known{toc.xfer()};
    Info <<"Unknown file extension for " << functionName
      << " : " << ext << nl
      <<"Valid types: (";
    // compact output:
    FOR_ALL(known, i) {
      Info <<" " << known[i];
    }
    Info <<" )" << endl;
  }
  return false;
}


// Constructors 
mousse::fileFormats::surfaceFormatsCore::surfaceFormatsCore()
{}


// Destructor 
mousse::fileFormats::surfaceFormatsCore::~surfaceFormatsCore()
{}

