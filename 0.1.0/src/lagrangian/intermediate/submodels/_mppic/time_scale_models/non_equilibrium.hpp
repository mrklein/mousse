#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TIME_SCALE_MODELS_NON_EQUILIBRIUM_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TIME_SCALE_MODELS_NON_EQUILIBRIUM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TimeScaleModels::nonEquilibrium
// Description
//   Non-Equlibrium model for the time scale over which properties of a
//   dispersed phase tend towards the mean value.
//   Reference:
//   \verbatim
//     "An improved collision damping time for MP-PIC calculations of dense
//     particle flows with applications to polydisperse sedimenting beds and
//     colliding particle jets"
//     P O'Rourke and D Snider
//     Chemical Engineering Science
//     Volume 65, Issue 22, Pages 6014-6028, November 2010
//   \endverbatim
// SourceFiles
//   non_equilibrium.cpp
#include "time_scale_model.hpp"
namespace mousse
{
namespace TimeScaleModels
{
class nonEquilibrium
:
  public TimeScaleModel
{
public:
  //- Runtime type information
  TYPE_NAME("nonEquilibrium");
  //- Constructors
    //- Construct from components
    nonEquilibrium(const dictionary& dict);
    //- Construct a copy
    nonEquilibrium(const nonEquilibrium& hc);
    //- Construct and return a clone
    virtual autoPtr<TimeScaleModel> clone() const
    {
      return autoPtr<TimeScaleModel>
      (
        new nonEquilibrium(*this)
      );
    }
  //- Destructor
  virtual ~nonEquilibrium();
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
