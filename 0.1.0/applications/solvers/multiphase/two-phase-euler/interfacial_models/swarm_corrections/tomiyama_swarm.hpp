// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TomiyamaSwarm
// Description
//   Swarm correction of Tomiyama et al.
//   Reference:
//   \verbatim
//     "Drag Coefficients of Bubbles. 2nd Report. Drag Coefficient for a Swarm
//     of Bubbles and its Applicability to Transient Flow."
//     Tomiyama, A., Kataoka, I., Fukuda, T., and Sakaguchi, T.,
//     Nippon Kikai Gakkai Ronbunshu
//     Volume 61, Issue 588, 1995, pp. 2810-2817
//   \endverbatim
// SourceFiles
//   tomiyama_swarm.cpp
#ifndef TOMIYAMA_SWARM_HPP_
#define TOMIYAMA_SWARM_HPP_
#include "swarm_correction.hpp"
namespace mousse
{
namespace swarmCorrections
{
class TomiyamaSwarm
:
  public swarmCorrection
{
  // Private data
    //- Residual phase fraction
    const dimensionedScalar residualAlpha_;
    //- Constant exponent
    const dimensionedScalar l_;
public:
  //- Runtime type information
  TYPE_NAME("Tomiyama");
  // Constructors
    //- Construct from a dictionary and a phase pair
    TomiyamaSwarm
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~TomiyamaSwarm();
  // Member Functions
    //- Swarm correction coefficient
    virtual tmp<volScalarField> Cs() const;
};
}  // namespace swarmCorrections
}  // namespace mousse
#endif
