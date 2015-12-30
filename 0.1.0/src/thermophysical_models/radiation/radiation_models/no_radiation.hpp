// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::noRadiation
// Description
//   No radiation - does nothing to energy equation source terms
//   (returns zeros)
// SourceFiles
//   no_radiation.cpp
#ifndef no_radiation_hpp_
#define no_radiation_hpp_
#include "radiation_model.hpp"
namespace mousse
{
namespace radiation
{
class noRadiation
:
  public radiationModel
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    noRadiation(const noRadiation&);
    //- Disallow default bitwise assignment
    void operator=(const noRadiation&);
public:
  //- Runtime type information
  TypeName("none");
  // Constructors
    //- Construct from components
    noRadiation(const volScalarField& T);
    //- Construct from components
    noRadiation(const dictionary& dict, const volScalarField& T);
  //- Destructor
  virtual ~noRadiation();
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
