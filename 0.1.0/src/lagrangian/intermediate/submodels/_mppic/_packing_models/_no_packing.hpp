// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PackingModel::NoPacking
// Description
// SourceFiles
//   _no_packing.cpp
#ifndef _no_packing_hpp_
#define _no_packing_hpp_
#include "_packing_model.hpp"
namespace mousse
{
namespace PackingModels
{
template<class CloudType>
class NoPacking
:
  public PackingModel<CloudType>
{
public:
  //- Runtime type information
  TypeName("none");
  // Constructors
    //- Construct from components
    NoPacking(const dictionary& dict, CloudType& owner);
    //- Construct copy
    NoPacking(const NoPacking<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<PackingModel<CloudType> > clone() const
    {
      return autoPtr<PackingModel<CloudType> >
      (
        new NoPacking<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoPacking();
  // Member Functions
    //- Calculate the velocity correction
    virtual vector velocityCorrection
    (
      typename CloudType::parcelType& p,
      const scalar deltaT
    ) const;
    //- Return the model 'active' status
    virtual bool active() const;
};
}  // namespace PackingModels
}  // namespace mousse
#ifdef NoRepository
#   include "_no_packing.cpp"
#endif
#endif
