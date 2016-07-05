// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::blended
// Description
//   Blends two drag models based on the phase fractions to handle
//   phase-inversion.
// SourceFiles
//   blended.cpp
#ifndef BLENDED_HPP_
#define BLENDED_HPP_
#include "drag_model.hpp"
namespace mousse
{
namespace dragModels
{
class blended
:
  public dragModel
{
  // Private data
    // The two drag models to be blended
    autoPtr<dragModel> dragModel1_;
    autoPtr<dragModel> dragModel2_;
public:
  //- Runtime type information
  TYPE_NAME("blended");
  // Constructors
    //- Construct from components
    blended
    (
      const dictionary& interfaceDict,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~blended();
  // Member Functions
    tmp<volScalarField> K(const volScalarField& Ur) const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
