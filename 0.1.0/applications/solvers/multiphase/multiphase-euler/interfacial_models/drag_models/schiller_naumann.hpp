// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::SchillerNaumann
// Description
// SourceFiles
//   schiller_naumann.cpp
#ifndef SCHILLER_NAUMANN_HPP_
#define SCHILLER_NAUMANN_HPP_
#include "drag_model.hpp"
namespace mousse {
namespace dragModels {
class SchillerNaumann
:
  public dragModel
{
public:
  //- Runtime type information
  TYPE_NAME("SchillerNaumann");
  // Constructors
    //- Construct from components
    SchillerNaumann
    (
      const dictionary& interfaceDict,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~SchillerNaumann();
  // Member Functions
    tmp<volScalarField> K(const volScalarField& Ur) const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
