// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::WenYu
// Description
//   Wen and Yu drag model
//   Reference:
//   \verbatim
//     "Eulerian Two-Phase Flow Theory Applied to Fluidization"
//     Enwald, H., Peirano, E., Almstedt, A-E.,
//     Int. J. Multiphase Flow, Vol. 22, Suppl, 1996, pp. 21-66
//     Eq. 86-87, p. 40
//   \endverbatim
// SourceFiles
//   wen_yu.cpp
#ifndef WEN_YU_HPP_
#define WEN_YU_HPP_
#include "drag_model.hpp"
namespace mousse
{
class phasePair;
namespace dragModels
{
class WenYu
:
  public dragModel
{
  // Private data
    //- Residual Reynolds Number
    const dimensionedScalar residualRe_;
public:
  //- Runtime type information
  TYPE_NAME("WenYu");
  // Constructors
    //- Construct from a dictionary and a phase pair
    WenYu
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~WenYu();
  // Member Functions
    //- Drag coefficient
    virtual tmp<volScalarField> CdRe() const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
