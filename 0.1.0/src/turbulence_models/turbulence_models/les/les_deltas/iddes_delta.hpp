// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IDDESDelta
// Description
//   IDDESDelta used by the IDDES (improved low Re Spalart-Allmaras DES model)
//   The min and max delta are calculated using the double distance of the min or
//   max from the face centre to the cell centre.
// SourceFiles
//   iddes_delta.cpp
#ifndef IDDESDeltaDelta_H
#define IDDESDeltaDelta_H
#include "max_deltaxyz.hpp"
namespace mousse
{
namespace LESModels
{
class IDDESDelta
:
  public LESdelta
{
  // Private data
    maxDeltaxyz hmax_;
    scalar Cw_;
  // Private Member Functions
    //- Disallow default bitwise copy construct and assignment
    IDDESDelta(const IDDESDelta&);
    void operator=(const IDDESDelta&);
    //- Calculate the delta values
    void calcDelta();
public:
  //- Runtime type information
  TypeName("IDDESDelta");
  // Constructors
    //- Construct from name, mesh and IOdictionary
    IDDESDelta
    (
      const word& name,
      const turbulenceModel& turbulence,
      const dictionary&
    );
  // Destructor
    ~IDDESDelta()
    {}
  // Member Functions
    //- Read the LESdelta dictionary
    void read(const dictionary&);
    //- Return the hmax delta field
    const volScalarField& hmax() const
    {
      return hmax_;
    }
    // Correct values
    void correct();
};
}  // namespace LESModels
}  // namespace mousse
#endif
