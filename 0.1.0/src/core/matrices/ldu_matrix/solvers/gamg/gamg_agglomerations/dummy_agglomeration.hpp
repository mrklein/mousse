// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dummyAgglomeration
// Description
//   Agglomerate without combining cells. Used for testing.
// SourceFiles
//   dummy_agglomeration.cpp

#ifndef dummy_agglomeration_hpp_
#define dummy_agglomeration_hpp_

#include "gamg_agglomeration.hpp"

namespace mousse
{
class dummyAgglomeration
:
  public GAMGAgglomeration
{

  // Private data

    //- Preset number of levels
    label nLevels_;

public:

  //- Runtime type information
  TYPE_NAME("dummy");

  // Constructors

    //- Construct given mesh and controls
    dummyAgglomeration
    (
      const lduMesh& mesh,
      const dictionary& controlDict
    );

    //- Disallow default bitwise copy construct
    dummyAgglomeration(const dummyAgglomeration&) = delete;

    //- Disallow default bitwise assignment
    dummyAgglomeration& operator=(const dummyAgglomeration&) = delete;

};

}  // namespace mousse

#endif
