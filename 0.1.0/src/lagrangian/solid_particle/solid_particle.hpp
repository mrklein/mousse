#ifndef LAGRANGIAN_SOLID_PARTICLE_SOLID_PARTICLE_HPP_
#define LAGRANGIAN_SOLID_PARTICLE_SOLID_PARTICLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidParticle
// Description
//   Simple solid spherical particle class with one-way coupling with the
//   continuous phase.

#include "particle.hpp"
#include "iostream.hpp"
#include "auto_ptr.hpp"
#include "interpolation_cell_point.hpp"
#include "contiguous.hpp"


namespace mousse {

class solidParticleCloud;


class solidParticle
:
  public particle
{
  // Private data
    //- Size in bytes of the fields
    static const std::size_t sizeofFields_;
    //- Diameter
    scalar d_;
    //- Velocity of parcel
    vector U_;
public:
  friend class Cloud<solidParticle>;
  //- Class used to pass tracking data to the trackToFace function
  class trackingData
  :
    public particle::TrackingData<solidParticleCloud>
  {
    // Interpolators for continuous phase fields
      const interpolationCellPoint<scalar>& rhoInterp_;
      const interpolationCellPoint<vector>& UInterp_;
      const interpolationCellPoint<scalar>& nuInterp_;
    //- Local gravitational or other body-force acceleration
    const vector& g_;
  public:
    // Constructors
      inline trackingData
      (
        solidParticleCloud& spc,
        const interpolationCellPoint<scalar>& rhoInterp,
        const interpolationCellPoint<vector>& UInterp,
        const interpolationCellPoint<scalar>& nuInterp,
        const vector& g
      );
    // Member functions
      inline const interpolationCellPoint<scalar>& rhoInterp() const;
      inline const interpolationCellPoint<vector>& UInterp() const;
      inline const interpolationCellPoint<scalar>& nuInterp() const;
      inline const vector& g() const;
  };
  // Constructors
    //- Construct from components
    inline solidParticle
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI,
      const scalar d,
      const vector& U
    );
    //- Construct from Istream
    solidParticle
    (
      const polyMesh& mesh,
      Istream& is,
      bool readFields = true
    );
    //- Construct and return a clone
    virtual autoPtr<particle> clone() const
    {
      return autoPtr<particle>{new solidParticle{*this}};
    }
    //- Factory class to read-construct particles used for
    //  parallel transfer
    class iNew
    {
      const polyMesh& mesh_;
    public:
      iNew(const polyMesh& mesh)
      :
        mesh_{mesh}
      {}
      autoPtr<solidParticle> operator()(Istream& is) const
      {
        return
          autoPtr<solidParticle>
          {
            new solidParticle{mesh_, is, true}
          };
      }
    };
  // Member Functions
    // Access
      //- Return diameter
      inline scalar d() const;
      //- Return velocity
      inline const vector& U() const;
    // Tracking
      //- Move
      bool move(trackingData&, const scalar);
    // Patch interactions
      //- Overridable function to handle the particle hitting a patch
      //  Executed before other patch-hitting functions
      bool hitPatch
      (
        const polyPatch&,
        trackingData& td,
        const label patchI,
        const scalar trackFraction,
        const tetIndices& tetIs
      );
      //- Overridable function to handle the particle hitting a
      //  processorPatch
      void hitProcessorPatch
      (
        const processorPolyPatch&,
        trackingData& td
      );
      //- Overridable function to handle the particle hitting a wallPatch
      void hitWallPatch
      (
        const wallPolyPatch&,
        trackingData& td,
        const tetIndices&
      );
      //- Overridable function to handle the particle hitting a polyPatch
      void hitPatch
      (
        const polyPatch&,
        trackingData& td
      );
      //- Transform the physical properties of the particle
      //  according to the given transformation tensor
      virtual void transformProperties(const tensor& T);
      //- Transform the physical properties of the particle
      //  according to the given separation vector
      virtual void transformProperties(const vector& separation);
      //- The nearest distance to a wall that
      //  the particle can be in the n direction
      virtual scalar wallImpactDistance(const vector& n) const;
  // I-O
    static void readFields(Cloud<solidParticle>& c);
    static void writeFields(const Cloud<solidParticle>& c);
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const solidParticle&);
};

template<> inline bool contiguous<solidParticle>() { return true; }

}  // namespace mousse


// Constructors 
inline mousse::solidParticle::trackingData::trackingData
(
  solidParticleCloud& spc,
  const interpolationCellPoint<scalar>& rhoInterp,
  const interpolationCellPoint<vector>& UInterp,
  const interpolationCellPoint<scalar>& nuInterp,
  const vector& g
)
:
  particle::TrackingData<solidParticleCloud>{spc},
  rhoInterp_{rhoInterp},
  UInterp_{UInterp},
  nuInterp_{nuInterp},
  g_{g}
{}


inline mousse::solidParticle::solidParticle
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const scalar d,
  const vector& U
)
:
  particle{mesh, position, cellI, tetFaceI, tetPtI},
  d_{d},
  U_{U}
{}


// Member Functions 
inline const mousse::interpolationCellPoint<mousse::scalar>&
mousse::solidParticle::trackingData::rhoInterp() const
{
  return rhoInterp_;
}


inline const mousse::interpolationCellPoint<mousse::vector>&
mousse::solidParticle::trackingData::UInterp() const
{
  return UInterp_;
}


inline const mousse::interpolationCellPoint<mousse::scalar>&
mousse::solidParticle::trackingData::nuInterp() const
{
  return nuInterp_;
}


inline const mousse::vector& mousse::solidParticle::trackingData::g() const
{
  return g_;
}


inline mousse::scalar mousse::solidParticle::d() const
{
  return d_;
}


inline const mousse::vector& mousse::solidParticle::U() const
{
  return U_;
}

#endif

