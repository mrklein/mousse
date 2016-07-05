// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::heatTransferModels::RanzMarshall
// Description
// SourceFiles
//   ranz_marshall.cpp
#ifndef RANZ_MARSHALL_HPP_
#define RANZ_MARSHALL_HPP_
#include "heat_transfer_model.hpp"
namespace mousse
{
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
      const dictionary& interfaceDict,
      const volScalarField& alpha1,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~RanzMarshall();
  // Member Functions
    tmp<volScalarField> K(const volScalarField& Ur) const;
};
}  // namespace heatTransferModels
}  // namespace mousse
#endif
