// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PackingModels::Implicit
// Description
//   Implicit model for applying an inter-particle stress to the particles.
//   The time evolution of particulate volume fraction is solved for implicitly
//   on the eulerian mesh. The computed flux is then applied to the lagrangian
//   field. The gravity force can optionally be applied to the particles as part
//   of this model, using the keyword "applyGravity".
// SourceFiles
//   _implicit.cpp
#ifndef _implicit_hpp_
#define _implicit_hpp_
#include "_packing_model.hpp"
namespace mousse
{
namespace PackingModels
{
template<class CloudType>
class Implicit
:
  public PackingModel<CloudType>
{
private:
  //- Private data
    //- Volume fraction field
    volScalarField alpha_;
    //- Correction flux
    tmp<surfaceScalarField> phiCorrect_;
    //- Correction cell-centred velocity
    tmp<volVectorField> uCorrect_;
    //- Flag to indicate whether gravity is applied
    Switch applyGravity_;
    //- Minimum stable volume fraction
    scalar alphaMin_;
    //- Minimum stable density
    scalar rhoMin_;
public:
  //- Runtime type information
  TypeName("implicit");
  // Constructors
    //- Construct from components
    Implicit(const dictionary& dict, CloudType& owner);
    //- Construct copy
    Implicit(const Implicit<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<PackingModel<CloudType> > clone() const
    {
      return autoPtr<PackingModel<CloudType> >
      (
        new Implicit<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~Implicit();
  // Member Functions
    //- Calculate the inter particles stresses
    virtual void cacheFields(const bool store);
    //- Calculate the velocity correction
    virtual vector velocityCorrection
    (
      typename CloudType::parcelType& p,
      const scalar deltaT
    ) const;
};
}  // namespace PackingModels
}  // namespace mousse
#ifdef NoRepository
#   include "_implicit.cpp"
#endif
#endif
