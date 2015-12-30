// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::opaqueSolid
// Description
//   Radiation for solid opaque solids - does nothing to energy equation source
//   terms (returns zeros) but creates absorptionEmissionModel and
//   scatterModel.
// SourceFiles
//   opaque_solid.cpp
#ifndef opaque_solid_hpp_
#define opaque_solid_hpp_
#include "radiation_model.hpp"
namespace mousse
{
namespace radiation
{
class opaqueSolid
:
  public radiationModel
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    opaqueSolid(const opaqueSolid&);
    //- Disallow default bitwise assignment
    void operator=(const opaqueSolid&);
public:
  //- Runtime type information
  TypeName("opaqueSolid");
  // Constructors
    //- Construct from components
    opaqueSolid(const volScalarField& T);
    //- Construct from components
    opaqueSolid(const dictionary& dict, const volScalarField& T);
  //- Destructor
  virtual ~opaqueSolid();
  // Member functions
    // Edit
      //- Solve radiation equation(s)
      void calculate();
      //- Read radiationProperties dictionary
      bool read();
      //- Source term component (for power of T^4)
      tmp<volScalarField> Rp() const;
      //- Source term component (constant)
      tmp<DimensionedField<scalar, volMesh> > Ru() const;
};
}  // namespace radiation
}  // namespace mousse
#endif
