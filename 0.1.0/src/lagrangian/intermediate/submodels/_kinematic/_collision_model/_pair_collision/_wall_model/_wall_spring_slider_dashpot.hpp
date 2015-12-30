// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::WallSpringSliderDashpot
// Description
//   Forces between particles and walls, interacting with a spring,
//   slider, damper model
#ifndef _wall_spring_slider_dashpot_hpp_
#define _wall_spring_slider_dashpot_hpp_
#include "_wall_model.hpp"
namespace mousse
{
template<class CloudType>
class WallSpringSliderDashpot
:
  public WallModel<CloudType>
{
  // Private data
    //- Effective Young's modulus value
    scalar Estar_;
    //- Effective shear modulus value
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
      scalar& rMin,
      scalar& rhoMax,
      scalar& vMagMax
    ) const;
    //- Calculate the wall interaction for a parcel at a given site
    void evaluateWall
    (
      typename CloudType::parcelType& p,
      const point& site,
      const WallSiteData<vector>& data,
      scalar pREff,
      scalar kN,
      bool cohesion
    ) const;
public:
  //- Runtime type information
  TypeName("wallSpringSliderDashpot");
  // Constructors
    //- Construct from dictionary
    WallSpringSliderDashpot(const dictionary& dict, CloudType& cloud);
  //- Destructor
  virtual ~WallSpringSliderDashpot();
  // Member Functions
    //- Return the volumeFactor
    inline scalar volumeFactor() const
    {
      return volumeFactor_;
    }
    //- Return the effective radius for a particle for the model
    virtual scalar pREff(const typename CloudType::parcelType& p) const;
    //- Whether the WallModel has a timestep limit that will
    //  require subCycling
    virtual bool controlsTimestep() const;
    //- For WallModels that control the timestep, calculate the
    //  number of subCycles needed to satisfy the minimum
    //  allowable timestep
    virtual label nSubCycles() const;
    //- Calculate the wall interaction for a parcel
    virtual void evaluateWall
    (
      typename CloudType::parcelType& p,
      const List<point>& flatSitePoints,
      const List<WallSiteData<vector> >& flatSiteData,
      const List<point>& sharpSitePoints,
      const List<WallSiteData<vector> >& sharpSiteData
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_wall_spring_slider_dashpot.cpp"
#endif
#endif
