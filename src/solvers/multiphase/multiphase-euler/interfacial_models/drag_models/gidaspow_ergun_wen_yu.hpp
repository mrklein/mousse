// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::GidaspowErgunWenYu
// Description
//   D. Gidaspow, Multiphase flow and fluidization,
//     Academic Press, New York, 1994.
// SourceFiles
//   gidaspow_ergun_wen_yu.cpp
#ifndef GIDASPOW_ERGUN_WEN_YU_HPP_
#define GIDASPOW_ERGUN_WEN_YU_HPP_
#include "drag_model.hpp"
namespace mousse
{
namespace dragModels
{
class GidaspowErgunWenYu
:
  public dragModel
{
public:
  //- Runtime type information
  TYPE_NAME("GidaspowErgunWenYu");
  // Constructors
    //- Construct from components
    GidaspowErgunWenYu
    (
      const dictionary& interfaceDict,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~GidaspowErgunWenYu();
  // Member Functions
    tmp<volScalarField> K(const volScalarField& Ur) const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
