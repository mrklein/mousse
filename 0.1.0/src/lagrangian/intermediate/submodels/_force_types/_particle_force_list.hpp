// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParticleForceList
// Description
//   List of particle forces
// SourceFiles
//   _particle_force_list_i.hpp
//   _particle_force_list.cpp
#ifndef _particle_force_list_hpp_
#define _particle_force_list_hpp_
#include "_particle_force.hpp"
#include "force_su_sp.hpp"
namespace mousse
{
template<class CloudType>
class ParticleForceList
:
  public PtrList<ParticleForce<CloudType> >
{
  // Private data
    //- Reference to the owner cloud
    CloudType& owner_;
    //- Reference to the mesh database
    const fvMesh& mesh_;
    //- Forces dictionary
    const dictionary dict_;
    //- Calculate coupled forces flag
    bool calcCoupled_;
    //- Calculate non-coupled forces flag
    bool calcNonCoupled_;
public:
  // Constructors
    //- Null constructor
    ParticleForceList(CloudType& owner, const fvMesh& mesh);
    //- Construct from mesh
    ParticleForceList
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict,
      const bool readFields
    );
    //- Construct copy
    ParticleForceList(const ParticleForceList& pfl);
  //- Destructor
  virtual ~ParticleForceList();
  // Member Functions
    // Access
      //- Return const access to the cloud owner
      inline const CloudType& owner() const;
      //- Return refernce to the cloud owner
      inline CloudType& owner();
      //- Return the mesh database
      inline const fvMesh& mesh() const;
      //- Return the forces dictionary
      inline const dictionary& dict() const;
      //- Set the calcCoupled flag
      inline void setCalcCoupled(bool flag);
      //- Set the calcNonCoupled flag
      inline void setCalcNonCoupled(bool flag);
    // Evaluation
      //- Cache fields
      virtual void cacheFields(const bool store);
      //- Calculate the coupled force
      virtual forceSuSp calcCoupled
      (
        const typename CloudType::parcelType& p,
        const scalar dt,
        const scalar mass,
        const scalar Re,
        const scalar muc
      ) const;
      //- Calculate the non-coupled force
      virtual forceSuSp calcNonCoupled
      (
        const typename CloudType::parcelType& p,
        const scalar dt,
        const scalar mass,
        const scalar Re,
        const scalar muc
      ) const;
      //- Return the effective mass
      virtual scalar massEff
      (
        const typename CloudType::parcelType& p,
        const scalar mass
      ) const;
};
}  // namespace mousse
#include "_particle_force_list_i.hpp"
#ifdef NoRepository
  #include "_particle_force_list.cpp"
#endif
#endif
