// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionProperties
// Description
//   Simple class to hold region information for coupled region simulations.
//   Gives per physics ('fluid', 'solid') the names of the regions. There
//   is no assumption on this level that one region should only have one
//   set of physics.
// SourceFiles
//   region_properties.cpp
#ifndef region_properties_hpp_
#define region_properties_hpp_
#include "time.hpp"
#include "hash_table.hpp"
namespace mousse
{
class regionProperties
:
  public HashTable<wordList>
{
    //- Disallow default bitwise copy construct
    regionProperties(const regionProperties&);
    //- Disallow default bitwise assignment
    void operator=(const regionProperties&);
public:
  // Constructors
    //- Construct from components
    regionProperties(const Time& runTime);
  //- Destructor
  ~regionProperties();
};
}  // namespace mousse
#endif
