#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TDRAG_TNON_SPHERE_DRAG_FORCE_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TDRAG_TNON_SPHERE_DRAG_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NonSphereDragForce
// Description
//   Drag model for non-spherical particles.
//   Takes the form of
//     24.0/Re*(1.0 + a_*pow(Re, b_)) + Re*c_/(Re + d_);
//   Where a(phi), b(phi), c(phi) and d(phi) are model coefficients, with phi
//   defined as:
//        area of sphere with same volume as particle
//     phi = -------------------------------------------
//            actual particle area
//   Equation used is Eqn (11) of reference below - good to within 2 to 4 % of
//   RMS values from experiment.
//   H and L also give a simplified model with greater error compared to
//   results from experiment - Eqn 12 - but since phi is presumed
//   constant, it offers little benefit.
//   Reference:
//   \verbatim
//     "Drag coefficient and terminal velocity of spherical and nonspherical
//     particles"
//     A. Haider and O. Levenspiel,
//     Powder Technology
//     Volume 58, Issue 1, May 1989, Pages 63-70
//   \endverbatim
#include "_particle_force.hpp"
namespace mousse
{
template<class CloudType>
class NonSphereDragForce
:
  public ParticleForce<CloudType>
{
protected:
  // Protected Data
    //- Ratio of surface of sphere having same volume as particle to
    //  actual surface area of particle (0 < phi <= 1)
    scalar phi_;
    // Model coefficients
      scalar a_;
      scalar b_;
      scalar c_;
      scalar d_;
  // Private Member Functions
    //- Drag coefficient multiplied by Reynolds number
    scalar CdRe(const scalar Re) const;
public:
  //- Runtime type information
  TYPE_NAME("nonSphereDrag");
  // Constructors
    //- Construct from mesh
    NonSphereDragForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Construct copy
    NonSphereDragForce(const NonSphereDragForce<CloudType>& df);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new NonSphereDragForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NonSphereDragForce();
  // Member Functions
    // Evaluation
      //- Calculate the non-coupled force
      virtual forceSuSp calcCoupled
      (
        const typename CloudType::parcelType& p,
        const scalar dt,
        const scalar mass,
        const scalar Re,
        const scalar muc
      ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_non_sphere_drag_force.cpp"
#endif
#endif
