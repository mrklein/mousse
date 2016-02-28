// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::heatTransferModels::RanzMarshall
// Description
//   Ranz-Marshall correlation for turbulent heat transfer from the surface of a
//   sphere to the surrounding fluid.
// SourceFiles
//   ranz_marshall.cpp
#ifndef ranz_marshall_hpp_
#define ranz_marshall_hpp_
#include "heat_transfer_model.hpp"
namespace mousse
{
class phasePair;
namespace heatTransferModels
{
class RanzMarshall
:
  public heatTransferModel
{
public:
  //- Runtime type information
  TYPE_NAME("RanzMarshall");
  // Constructors
    //- Construct from components
    RanzMarshall
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~RanzMarshall();
  // Member Functions
    //- The heat transfer function K used in the enthalpy equation
    tmp<volScalarField> K(const scalar residualAlpha) const;
};
}  // namespace heatTransferModels
}  // namespace mousse
#endif
