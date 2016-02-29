// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::GidaspowErgunWenYu
// Description
//   Gidaspow, Ergun, Wen and Yu drag model
//   Reference:
//   \verbatim
//     "Multiphase flow and fluidization",
//     Gidaspow, D.,
//     Academic Press, New York, 1994.
//   \endverbatim
// SourceFiles
//   gidaspow_ergun_wen_yu.cpp
#ifndef GIDASPOW_ERGUN_WEN_YU_HPP_
#define GIDASPOW_ERGUN_WEN_YU_HPP_
#include "drag_model.hpp"
namespace mousse
{
class phasePair;
namespace dragModels
{
class Ergun;
class WenYu;
class GidaspowErgunWenYu
:
  public dragModel
{
  // Private data
    //- Ergun drag model
    autoPtr<Ergun> Ergun_;
    //- Wen Yu drag model
    autoPtr<WenYu> WenYu_;
public:
  //- Runtime type information
  TYPE_NAME("GidaspowErgunWenYu");
  // Constructors
    //- Construct from a dictionary and an ordered phase pair
    GidaspowErgunWenYu
    (
      const dictionary& interfaceDict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~GidaspowErgunWenYu();
  // Member Functions
    //- Drag coefficient
    virtual tmp<volScalarField> CdRe() const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
