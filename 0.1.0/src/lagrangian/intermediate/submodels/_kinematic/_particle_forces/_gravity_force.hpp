// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::GravityForce
// Description
//   Calculates particle gravity force
// SourceFiles
//   _gravity_force_i.hpp
//   _gravity_force.cpp
#ifndef _gravity_force_hpp_
#define _gravity_force_hpp_
#include "_particle_force.hpp"
namespace mousse
{
class fvMesh;
template<class CloudType>
class GravityForce
:
  public ParticleForce<CloudType>
{
  // Private data
    //- Reference to the acceleration due to gravity
    const vector& g_;
public:
  //- Runtime type information
  TYPE_NAME("gravity");
  // Constructors
    //- Construct from mesh
    GravityForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Construct copy
    GravityForce(const GravityForce& gf);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new GravityForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~GravityForce();
  // Member Functions
    // Access
      //- Return the acceleration due to gravity
      inline const vector& g() const;
    // Evaluation
      //- Calculate the non-coupled force
      virtual forceSuSp calcNonCoupled
      (
        const typename CloudType::parcelType& p,
        const scalar dt,
        const scalar mass,
        const scalar Re,
        const scalar muc
      ) const;
};
}  // namespace mousse
#include "_gravity_force_i.hpp"
#ifdef NoRepository
  #include "_gravity_force.cpp"
#endif
#endif
