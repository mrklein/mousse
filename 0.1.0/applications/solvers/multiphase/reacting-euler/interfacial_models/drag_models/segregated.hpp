// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::segregated
// Description
//   Segregated drag model for use in regions with no obvious dispersed phase.
//   Reference:
//   \verbatim
//     "Towards the Numerical Simulation of Multi-scale Two-phase Flows",
//     Marschall, H.,
//     PhD Thesis, TU München, 2011
//   \endverbatim
// SourceFiles
//   segregated.cpp
#ifndef segregated_hpp_
#define segregated_hpp_
#include "drag_model.hpp"
namespace mousse
{
class phasePair;
namespace dragModels
{
class segregated
:
  public dragModel
{
  // Private data
    //- M coefficient
    const dimensionedScalar m_;
    //- N coefficient
    const dimensionedScalar n_;
public:
  //- Runtime type information
  TYPE_NAME("segregated");
  // Constructors
    //- Construct from components
    segregated
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~segregated();
  // Member Functions
    //- Drag coefficient
    virtual tmp<volScalarField> CdRe() const;
    //- The drag function used in the momentum equation
    virtual tmp<volScalarField> K() const;
    //- The drag function Kf used in the face-momentum equations
    virtual tmp<surfaceScalarField> Kf() const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
