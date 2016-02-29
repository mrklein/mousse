#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TIME_SCALE_MODELS_ISOTROPIC_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TIME_SCALE_MODELS_ISOTROPIC_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TimeScaleModels::isotropic
// Description
//   Model for the time scale over which the velocity field of a dispersed phase
//   tends towards an isotropic distribution.
//   Reference:
//   \verbatim
//     "Inclusion of collisional return-to-isotropy in the MP-PIC method"
//     P O'Rourke and D Snider
//     Chemical Engineering Science
//     Volume 80, Issue 0, Pages 39-54, December 2012
//   \endverbatim
// SourceFiles
//   isotropic.cpp
#include "time_scale_model.hpp"
namespace mousse
{
namespace TimeScaleModels
{
class isotropic
:
  public TimeScaleModel
{
public:
  //- Runtime type information
  TYPE_NAME("isotropic");
  //- Constructors
    //- Construct from components
    isotropic(const dictionary& dict);
    //- Construct a copy
    isotropic(const isotropic& hc);
    //- Construct and return a clone
    virtual autoPtr<TimeScaleModel> clone() const
    {
      return autoPtr<TimeScaleModel>
      (
        new isotropic(*this)
      );
    }
  //- Destructor
  virtual ~isotropic();
  //- Member Functions
    //- Time scale
    tmp<FieldField<Field, scalar> > oneByTau
    (
      const FieldField<Field, scalar>& alpha,
      const FieldField<Field, scalar>& r32,
      const FieldField<Field, scalar>& uSqr,
      const FieldField<Field, scalar>& f
    ) const;
};
}  // namespace TimeScaleModels
}  // namespace mousse
#endif
