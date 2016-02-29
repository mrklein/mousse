// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::interface
// Description
//   Drag between phase separated by a VoF resolved interface.
// SourceFiles
//   interface.cpp
#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_
#include "drag_model.hpp"
namespace mousse
{
namespace dragModels
{
class interface
:
  public dragModel
{
public:
  //- Runtime type information
  TYPE_NAME("interface");
  // Constructors
    //- Construct from components
    interface
    (
      const dictionary& interfaceDict,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~interface();
  // Member Functions
    tmp<volScalarField> K(const volScalarField& Ur) const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
