// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SaffmanMeiLiftForce
// Description
//   Saffman-Mei particle lift force model applicable to spherical particles.
// SourceFiles
//   _saffman_mei_lift_force.cpp
#ifndef _saffman_mei_lift_force_hpp_
#define _saffman_mei_lift_force_hpp_
#include "_lift_force.hpp"
namespace mousse
{
template<class CloudType>
class SaffmanMeiLiftForce
:
  public LiftForce<CloudType>
{
protected:
  // Protected Member Functions
    //- Calculate the lift coefficient
    virtual scalar Cl
    (
      const typename CloudType::parcelType& p,
      const vector& curlUc,
      const scalar Re,
      const scalar muc
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("SaffmanMeiLiftForce");
  // Constructors
    //- Construct from mesh
    SaffmanMeiLiftForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& forceType = typeName
    );
    //- Construct copy
    SaffmanMeiLiftForce(const SaffmanMeiLiftForce& lf);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new SaffmanMeiLiftForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~SaffmanMeiLiftForce();
};
}  // namespace mousse
#ifdef NoRepository
  #include "_saffman_mei_lift_force.cpp"
#endif
#endif
