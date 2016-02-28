// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::WenYu
// Description
//   H, Enwald, E. Peirano, A-E Almstedt
//   'Eulerian Two-Phase Flow Theory Applied to Fluidization'
//   Int. J. Multiphase Flow, Vol. 22, Suppl, pp. 21-66 (1996)
//   Eq. 86-87, p. 40
//   This is identical to the Wen and Yu, Rowe model Table 3.6 p.56  in
//   the Ph.D. thesis of Berend van Wachem
//   'Derivation, Implementation and Validation
//           of
//      Computer Simulation Models
//     for Gas-Solid Fluidized Beds'
//   NB: The difference between the Gidaspow-version is the void-fraction
//     in the Re-number
// SourceFiles
//   wen_yu.cpp
#ifndef WEN_YU_HPP_
#define WEN_YU_HPP_
#include "drag_model.hpp"
namespace mousse
{
namespace dragModels
{
class WenYu
:
  public dragModel
{
public:
  //- Runtime type information
  TYPE_NAME("WenYu");
  // Constructors
    //- Construct from components
    WenYu
    (
      const dictionary& interfaceDict,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~WenYu();
  // Member Functions
    tmp<volScalarField> K(const volScalarField& Ur) const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
