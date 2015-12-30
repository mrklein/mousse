// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::noSoot
// Description
//   noSoot
// SourceFiles
//   no_soot.cpp
#ifndef no_soot_hpp_
#define no_soot_hpp_
#include "soot_model.hpp"
namespace mousse
{
namespace radiation
{
class noSoot
:
  public sootModel
{
public:
  //- Runtime type information
  TypeName("none");
  // Constructors
    //- Construct from components
    noSoot(const dictionary& dict, const fvMesh& mesh, const word&);
  //- Destructor
  virtual ~noSoot();
  // Member Functions
    // Edit
      //- Main update/correction routine
      void correct();
    // Access
      //- Return Ysoot
      const volScalarField& soot() const;
};
}  // namespace mousse
}  // namespace radiation
#endif
