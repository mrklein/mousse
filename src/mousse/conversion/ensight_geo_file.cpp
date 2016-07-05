// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ensight_geo_file.hpp"


// Constructors 
mousse::ensightGeoFile::ensightGeoFile
(
  const fileName& pathname,
  IOstream::streamFormat format
)
:
  ensightFile{pathname, format}
{
  writeBinaryHeader();
  write("Ensight Geometry File");  newline();
  write("=====================");  newline();
  write("node id assign");         newline();
  write("element id assign");      newline();
}


// Destructor 
mousse::ensightGeoFile::~ensightGeoFile()
{}


// Member Functions 
mousse::Ostream& mousse::ensightGeoFile::writeKeyword(const string& key)
{
  write(key);
  newline();
  return *this;
}

