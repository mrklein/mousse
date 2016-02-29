#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TISOTROPY_MODELS_TNO_ISOTROPY_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TISOTROPY_MODELS_TNO_ISOTROPY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IsotropyModels::NoIsotropy
// Description
// SourceFiles
//   _no_isotropy.cpp
#include "_isotropy_model.hpp"
namespace mousse
{
namespace IsotropyModels
{
template<class CloudType>
class NoIsotropy
:
  public IsotropyModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    NoIsotropy(const dictionary& dict, CloudType& owner);
    //- Construct copy
    NoIsotropy(const NoIsotropy<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<IsotropyModel<CloudType> > clone() const
    {
      return autoPtr<IsotropyModel<CloudType> >
      (
        new NoIsotropy<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoIsotropy();
  //- Member Functions
    //- Calculate velocities
    virtual void calculate();
    //- Return the model 'active' status
    virtual bool active() const;
};
}  // namespace IsotropyModels
}  // namespace mousse
#ifdef NoRepository
#   include "_no_isotropy.cpp"
#endif
#endif
