// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParticleForce
// Description
//   Abstract base class for particle forces
// SourceFiles
//   _particle_force_i.hpp
//   _particle_force.cpp
//   _particle_force_new.cpp
#ifndef _particle_force_hpp_
#define _particle_force_hpp_
#include "dictionary.hpp"
#include "force_su_sp.hpp"
#include "fv_mesh.hpp"
namespace mousse
{
template<class CloudType>
class ParticleForce
{
  // Private data
    //- Reference to the owner cloud
    CloudType& owner_;
    //- Reference to the mesh database
    const fvMesh& mesh_;
    //- Force coefficients dictaionary
    const dictionary coeffs_;
public:
  //- Runtime type information
  TypeName("particleForce");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    ParticleForce,
    dictionary,
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    ),
    (owner, mesh, dict)
  );
  //- Convenience typedef for return type
  typedef VectorSpace<Vector<vector>, vector, 2> returnType;
  // Constructors
    //- Construct from mesh
    ParticleForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& forceType,
      const bool readCoeffs
    );
    //- Construct copy
    ParticleForce(const ParticleForce& pf);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new ParticleForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~ParticleForce();
  //- Selector
  static autoPtr<ParticleForce<CloudType> > New
  (
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& forceType
  );
  // Member Functions
    // Access
      //- Return const access to the cloud owner
      inline const CloudType& owner() const;
      //- Return refernce to the cloud owner
      inline CloudType& owner();
      //- Return the mesh database
      inline const fvMesh& mesh() const;
      //- Return the force coefficients dictionary
      inline const dictionary& coeffs() const;
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
      //- Return the added mass
      virtual scalar massAdd
      (
        const typename CloudType::parcelType& p,
        const scalar mass
      ) const;
};
}  // namespace mousse

#include "_particle_force_i.hpp"
#ifdef NoRepository
  #include "_particle_force.cpp"
#endif

#define makeParticleForceModel(CloudType)                                     \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  defineNamedTemplateTypeNameAndDebug                                         \
    (mousse::ParticleForce<kinematicCloudType>, 0);                           \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
      ParticleForce<kinematicCloudType>,                                      \
      dictionary                                                              \
    );                                                                        \
  }

#define makeParticleForceModelType(SS, CloudType)                             \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  defineNamedTemplateTypeNameAndDebug(mousse::SS<kinematicCloudType>, 0);     \
                                                                              \
  mousse::ParticleForce<kinematicCloudType>::                                 \
    adddictionaryConstructorToTable<mousse::SS<kinematicCloudType> >          \
      add##SS##CloudType##kinematicCloudType##ConstructorToTable_;

#endif
