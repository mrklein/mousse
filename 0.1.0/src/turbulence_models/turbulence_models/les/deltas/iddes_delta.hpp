#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_LES_DELTAS_IDDES_DELTA_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_LES_DELTAS_IDDES_DELTA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IDDESDelta
// Description
//   IDDESDelta used by the IDDES (improved low Re Spalart-Allmaras DES model)
//   The min and max delta are calculated using the double distance of the min or
//   max from the face centre to the cell centre.

#include "max_deltaxyz.hpp"


namespace mousse {
namespace LESModels {

class IDDESDelta
:
  public LESdelta
{
  // Private data
    maxDeltaxyz hmax_;
    scalar Cw_;
  // Private Member Functions
    //- Calculate the delta values
    void calcDelta();
public:
  //- Runtime type information
  TYPE_NAME("IDDESDelta");
  // Constructors
    //- Construct from name, mesh and IOdictionary
    IDDESDelta
    (
      const word& name,
      const turbulenceModel& turbulence,
      const dictionary&
    );
    //- Disallow default bitwise copy construct and assignment
    IDDESDelta(const IDDESDelta&) = delete;
    void operator=(const IDDESDelta&) = delete;
  // Destructor
    ~IDDESDelta()
    {}
  // Member Functions
    //- Read the LESdelta dictionary
    void read(const dictionary&);
    //- Return the hmax delta field
    const volScalarField& hmax() const { return hmax_; }
    // Correct values
    void correct();
};

}  // namespace LESModels
}  // namespace mousse

#endif

