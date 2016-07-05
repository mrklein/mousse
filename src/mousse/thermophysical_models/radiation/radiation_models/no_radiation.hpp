#ifndef THERMOPHYSICAL_MODELS_RADIATION_RADIATION_MODELS_NO_RADIATION_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_RADIATION_MODELS_NO_RADIATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::noRadiation
// Description
//   No radiation - does nothing to energy equation source terms
//   (returns zeros)

#include "radiation_model.hpp"


namespace mousse {
namespace radiation {

class noRadiation
:
  public radiationModel
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    noRadiation(const volScalarField& T);
    //- Construct from components
    noRadiation(const dictionary& dict, const volScalarField& T);
    //- Disallow default bitwise copy construct
    noRadiation(const noRadiation&) = delete;
    //- Disallow default bitwise assignment
    noRadiation& operator=(const noRadiation&) = delete;
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

