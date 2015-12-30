// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileFormats::NASCore
// Description
//   Core routines used when reading/writing NASTRAN files.
// SourceFiles
//   nas_core.cpp
#ifndef nas_core_hpp_
#define nas_core_hpp_
#include "scalar.hpp"
#include "string.hpp"
namespace mousse
{
namespace fileFormats
{
class NASCore
{
public:
  // Public Member Functions
    //- Do weird things to extract number
    static scalar parseNASCoord(const string&);
  // Constructors
    //- Construct null
    NASCore();
};
}  // namespace fileFormats
}  // namespace mousse
#endif
