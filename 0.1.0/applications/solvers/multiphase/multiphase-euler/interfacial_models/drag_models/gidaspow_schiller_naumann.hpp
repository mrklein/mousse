// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::GidaspowSchillerNaumann
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
// SourceFiles
//   gidaspow_schiller_naumann.cpp
#ifndef GIDASPOW_SCHILLER_NAUMANN_HPP_
#define GIDASPOW_SCHILLER_NAUMANN_HPP_
#include "drag_model.hpp"
namespace mousse
{
namespace dragModels
{
class GidaspowSchillerNaumann
:
  public dragModel
{
public:
  //- Runtime type information
  TYPE_NAME("GidaspowSchillerNaumann");
  // Constructors
    //- Construct from components
    GidaspowSchillerNaumann
    (
      const dictionary& interfaceDict,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~GidaspowSchillerNaumann();
  // Member Functions
    tmp<volScalarField> K(const volScalarField& Ur) const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
