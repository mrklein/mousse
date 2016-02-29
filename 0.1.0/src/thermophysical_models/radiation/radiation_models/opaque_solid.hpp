#ifndef THERMOPHYSICAL_MODELS_RADIATION_RADIATION_MODELS_OPAQUE_SOLID_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_RADIATION_MODELS_OPAQUE_SOLID_HPP_

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
#include "radiation_model.hpp"
namespace mousse
{
namespace radiation
{
class opaqueSolid
:
  public radiationModel
{
public:
  //- Runtime type information
  TYPE_NAME("opaqueSolid");
  // Constructors
    //- Construct from components
    opaqueSolid(const volScalarField& T);
    //- Construct from components
    opaqueSolid(const dictionary& dict, const volScalarField& T);
    //- Disallow default bitwise copy construct
    opaqueSolid(const opaqueSolid&) = delete;
    //- Disallow default bitwise assignment
    opaqueSolid& operator=(const opaqueSolid&) = delete;
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
