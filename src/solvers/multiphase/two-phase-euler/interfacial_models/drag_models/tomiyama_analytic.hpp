// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::TomiyamaAnalytic
// Description
//   Analytical drag model of Tomiyama et al.
//   Reference:
//   \verbatim
//     "Drag Coefficients of Bubbles. 1st Report. Drag Coefficients of a
//     Single Bubble in a Stagnant Liquid."
//     Tomiyama, A., Kataoka, I., and Sakaguchi, T.,
//     Nippon Kikai Gakkai Ronbunshu
//     Volume 61, Issue 587, 1995, pp. 2357-2364
//   \endverbatim
// SourceFiles
//   tomiyama_analytic.cpp
#ifndef TOMIYAMA_ANALYTIC_HPP_
#define TOMIYAMA_ANALYTIC_HPP_
#include "drag_model.hpp"
namespace mousse
{
class phasePair;
namespace dragModels
{
class TomiyamaAnalytic
:
  public dragModel
{
  // Private data
    //- Residual Reynolds Number
    const dimensionedScalar residualRe_;
    //- Residual Eotvos number
    const dimensionedScalar residualEo_;
    //- Residual aspect ratio
    const dimensionedScalar residualE_;
public:
  //- Runtime type information
  TYPE_NAME("TomiyamaAnalytic");
  // Constructors
    //- Construct from a dictionary and a phase pair
    TomiyamaAnalytic
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~TomiyamaAnalytic();
  // Member Functions
    // Drag coefficient
    virtual tmp<volScalarField> CdRe() const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
