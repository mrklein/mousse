// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PairSpringSliderDashpot
// Description
//   Pair forces between particles colliding with a spring, slider, damper model
#ifndef _pair_spring_slider_dashpot_hpp_
#define _pair_spring_slider_dashpot_hpp_
#include "_pair_model.hpp"
#include "_collision_record_list.hpp"
#include "mathematical_constants.hpp"
namespace mousse
{
template<class CloudType>
class PairSpringSliderDashpot
:
  public PairModel<CloudType>
{
  // Private data
    //- Effective Young's modulus value, assuming both particles have
    //  the same E value
    scalar Estar_;
    //- Effective shear modulus value, assuming both particles have
    //  the same Poisson's ratio and Young's modulus
    scalar Gstar_;
    //- alpha-coefficient, related to coefficient of restitution
    scalar alpha_;
    //- Spring power (b = 1 for linear, b = 3/2 for Hertzian)
    scalar b_;
    //- Coefficient of friction in for tangential sliding
    scalar mu_;
    //- Cohesion energy density [J/m^3]
    scalar cohesionEnergyDensity_;
    // Switch cohesion on and off
    bool cohesion_;
    //- The number of steps over which to resolve the minimum
    //  harmonic approximation of the collision period
    scalar collisionResolutionSteps_;
    //- Volume factor for determining the equivalent size of a
    //  parcel where nParticles is not 1.  The equivalent size of
    //  the parcel is
    //      parcelEquivVolume = volumeFactor*nParticles*p.volume()
    //  so
    //      parcelEquivD = cbrt(volumeFactor*nParticles)*p.d()
    //  + When volumeFactor = 1, the particles are compressed
    //    together so that the equivalent volume of the parcel is
    //    the sum of the constituent particles
    //  + When volumeFactor = 3*sqrt(2)/pi, the particles are
    //    close packed, but uncompressed.
    //  + When volumeFactor > 3*sqrt(2)/pi, the particles loosely
    //    grouped.
    // 3*sqrt(2)/pi = 1.350474 is the volume factor for close
    // packing, i.e pi/(3*sqrt(2)) is the maximum close packing
    // factor
    scalar volumeFactor_;
    //- Switch to control use of equivalent size particles.  Used
    //  because the calculation can be very expensive.
    bool useEquivalentSize_;
  // Private Member Functions
    //- Find the appropriate properties for determining the minimum
    //- Allowable timestep
    void findMinMaxProperties
    (
      scalar& RMin,
      scalar& rhoMax,
      scalar& vMagMax
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("pairSpringSliderDashpot");
  // Constructors
    //- Construct from dictionary
    PairSpringSliderDashpot(const dictionary& dict, CloudType& cloud);
  //- Destructor
  virtual ~PairSpringSliderDashpot();
  // Member Functions
    //- Return the volumeFactor
    inline scalar volumeFactor() const
    {
      return volumeFactor_;
    }
    // Return the area of overlap between two spheres of radii rA and rB,
    // centres separated by a distance rAB.  Assumes rAB < (rA + rB).
    inline scalar overlapArea(scalar rA, scalar rB, scalar rAB) const
    {
      // From:
      // http://mathworld.wolfram.com/Sphere-SphereIntersection.html
      return
        mathematical::pi/4.0
       /sqr(rAB)
       *(
          (-rAB + rA - rB)
         *(-rAB - rA + rB)
         *(-rAB + rA + rB)
         *( rAB + rA + rB)
        );
    }
    //- Whether the PairModel has a timestep limit that will
    //  require subCycling
    virtual bool controlsTimestep() const;
    //- For PairModels that control the timestep, calculate the
    //  number of subCycles needed to satisfy the minimum
    //  allowable timestep
    virtual label nSubCycles() const;
    //- Calculate the pair interaction between parcels
    virtual void evaluatePair
    (
      typename CloudType::parcelType& pA,
      typename CloudType::parcelType& pB
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_pair_spring_slider_dashpot.cpp"
#endif
#endif
