// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::GidaspowSchillerNaumann
// Description
//   Gidaspow, Schiller and Naumann drag model
//   References:
//   \verbatim
//     "Eulerian Two-Phase Flow Theory Applied to Fluidization"
//     Enwald, H., Peirano, E., Almstedt, A-E.,
//     Int. J. Multiphase Flow, Vol. 22, Suppl, 1996, pp. 21-66
//     Eq. 86-87, p. 40
//     This is identical to the Wen and Yu, Rowe model Table 3.6 p.56  in
//     "Derivation, Implementation and Validation of Computer Simulation Models
//     for Gas-Solid Fluidized Beds",
//     Berend van Wachem
//     Ph.D. thesis.
//   \endverbatim
// SourceFiles
//   gidaspow_schiller_naumann.cpp
#ifndef GIDASPOW_SCHILLER_NAUMANN_HPP_
#define GIDASPOW_SCHILLER_NAUMANN_HPP_
#include "drag_model.hpp"
namespace mousse
{
class phasePair;
namespace dragModels
{
class GidaspowSchillerNaumann
:
  public dragModel
{
  // Private data
    //- Residual Reynolds Number
    const dimensionedScalar residualRe_;
public:
  //- Runtime type information
  TYPE_NAME("GidaspowSchillerNaumann");
  // Constructors
    //- Construct from a dictionary and a phase pair
    GidaspowSchillerNaumann
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~GidaspowSchillerNaumann();
  // Member Functions
    //- Drag coefficient
    virtual tmp<volScalarField> CdRe() const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
