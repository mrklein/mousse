// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::TomiyamaCorrelated
// Description
//   Correlation of Tomiyama et al.
//   Reference:
//   \verbatim
//     "Terminal velocity of single bubbles in surface tension force dominant
//     regime"
//     Tomiyama, T., Celata, G.P., Hosokawa, S., Yoshida, S.,
//     International Journal of Multiphase Flow
//     Volume 28, Issue 9, September 2002, pp. 1497-1519
//   \endverbatim
// SourceFiles
//   tomiyama_correlated.cpp
#ifndef tomiyama_correlated_hpp_
#define tomiyama_correlated_hpp_
#include "drag_model.hpp"
namespace mousse
{
class phasePair;
namespace dragModels
{
class TomiyamaCorrelated
:
  public dragModel
{
  // Private data
    //- Coefficient
    const dimensionedScalar A_;
public:
  //- Runtime type information
  TYPE_NAME("TomiyamaCorrelated");
  // Constructors
    //- Construct from a dictionary and a phase pair
    TomiyamaCorrelated
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~TomiyamaCorrelated();
  // Member Functions
    //- Drag coefficient
    virtual tmp<volScalarField> CdRe() const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
