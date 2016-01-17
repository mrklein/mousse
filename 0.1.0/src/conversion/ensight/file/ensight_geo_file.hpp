// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightGeoFile
// Description
//   Specialized Ensight output with extra geometry file header
#ifndef ensight_geo_file_hpp_
#define ensight_geo_file_hpp_
#include "ensight_file.hpp"
namespace mousse
{
class ensightGeoFile
:
  public ensightFile
{
public:
  // Constructors
    //- Construct from pathname
    ensightGeoFile
    (
      const fileName& pathname,
      IOstream::streamFormat format=IOstream::BINARY
    );
    //- Disallow default bitwise assignment
    ensightGeoFile& operator=(const ensightGeoFile&) = delete;
    //- Disallow default copy constructor
    ensightGeoFile(const ensightGeoFile&) = delete;
  //- Destructor
  ~ensightGeoFile();
  // Output
    //- Write keyword with trailing newline
    virtual Ostream& writeKeyword(const string& key);
};
}  // namespace mousse
#endif
