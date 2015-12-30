// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::VirtualMassForce
// Description
//   Calculates particle virtual mass force
// SourceFiles
//   _virtual_mass_force.cpp
#ifndef _virtual_mass_force_hpp_
#define _virtual_mass_force_hpp_
#include "_pressure_gradient_force.hpp"
namespace mousse
{
template<class CloudType>
class VirtualMassForce
:
  public PressureGradientForce<CloudType>
{
  // Private data
    //- Virtual mass coefficient - typically 0.5
    scalar Cvm_;
public:
  //- Runtime type information
  TypeName("virtualMass");
  // Constructors
    //- Construct from mesh
    VirtualMassForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& forceType = typeName
    );
    //- Construct copy
    VirtualMassForce(const VirtualMassForce& pgf);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new VirtualMassForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~VirtualMassForce();
  // Member Functions
    // Evaluation
      //- Cache fields
      virtual void cacheFields(const bool store);
      //- Calculate the non-coupled force
      virtual forceSuSp calcCoupled
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
#ifdef NoRepository
  #include "_virtual_mass_force.cpp"
#endif
#endif
