// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TimeScaleModels::equilibrium
// Description
//   Equlibrium model for the time scale over which properties of a dispersed
//   phase tend towards the mean value.
//   Reference:
//   \verbatim
//     "A model for collisional exchange in gas/liquid/solid fluidized beds"
//     P O'Rourke, P Zhao and D Snider
//     Chemical Engineering Science
//     Volume 64, Issue 8, Pages 1784-1797, April 2009
//   \endverbatim
// SourceFiles
//   equilibrium.cpp
#ifndef equilibrium_hpp_
#define equilibrium_hpp_
#include "time_scale_model.hpp"
namespace mousse
{
namespace TimeScaleModels
{
class equilibrium
:
  public TimeScaleModel
{
public:
  //- Runtime type information
  TYPE_NAME("equilibrium");
  //- Constructors
    //- Construct from components
    equilibrium(const dictionary& dict);
    //- Construct a copy
    equilibrium(const equilibrium& hc);
    //- Construct and return a clone
    virtual autoPtr<TimeScaleModel> clone() const
    {
      return autoPtr<TimeScaleModel>
      (
        new equilibrium(*this)
      );
    }
  //- Destructor
  virtual ~equilibrium();
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
