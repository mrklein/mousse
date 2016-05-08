#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_STREAM_LINE_STREAM_LINE_PARTICLE_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_STREAM_LINE_STREAM_LINE_PARTICLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::streamLineParticle
// Description
//   Particle class that samples fields as it passes through. Used in streamline
//   calculation.

#include "particle.hpp"
#include "auto_ptr.hpp"
#include "interpolation.hpp"
#include "vector_list.hpp"


namespace mousse {

class streamLineParticleCloud;


class streamLineParticle
:
  public particle
{
public:
  //- Class used to pass tracking data to the trackToFace function
  class trackingData
  :
    public particle::TrackingData<Cloud<streamLineParticle>>
  {
  public:
    const PtrList<interpolation<scalar>>& vsInterp_;
    const PtrList<interpolation<vector>>& vvInterp_;
    const label UIndex_;
    const bool trackForward_;
    const label nSubCycle_;
    const scalar trackLength_;
    DynamicList<vectorList>& allPositions_;
    List<DynamicList<scalarList>>& allScalars_;
    List<DynamicList<vectorList>>& allVectors_;
    // Constructors
      trackingData
      (
        Cloud<streamLineParticle>& cloud,
        const PtrList<interpolation<scalar>>& vsInterp,
        const PtrList<interpolation<vector>>& vvInterp,
        const label UIndex,
        const bool trackForward,
        const label nSubCycle,
        const scalar trackLength,
        DynamicList<List<point>>& allPositions,
        List<DynamicList<scalarList>>& allScalars,
        List<DynamicList<vectorList>>& allVectors
      )
      :
        particle::TrackingData<Cloud<streamLineParticle>>{cloud},
        vsInterp_{vsInterp},
        vvInterp_{vvInterp},
        UIndex_{UIndex},
        trackForward_{trackForward},
        nSubCycle_{nSubCycle},
        trackLength_{trackLength},
        allPositions_{allPositions},
        allScalars_{allScalars},
        allVectors_{allVectors}
      {}
  };
private:
  // Private data
    //- Lifetime of particle. Particle dies when reaches 0.
    label lifeTime_;
    //- Sampled positions
    DynamicList<point> sampledPositions_;
    //- Sampled scalars
    List<DynamicList<scalar>> sampledScalars_;
    //- Sampled vectors
    List<DynamicList<vector>> sampledVectors_;
  // Private Member Functions
    //- Estimate dt to cross from current face to next one in nSubCycle
    //  steps.
    scalar calcSubCycleDeltaT
    (
      trackingData& td,
      const scalar dt,
      const vector& U
    ) const;
    void constrainVelocity
    (
      trackingData& td,
      const scalar dt,
      vector& U
    );
    //- Interpolate all quantities; return interpolated velocity.
    vector interpolateFields
    (
      const trackingData&,
      const point&,
      const label cellI,
      const label faceI
    );
public:
  // Constructors
    //- Construct from components
    streamLineParticle
    (
      const polyMesh& c,
      const vector& position,
      const label cellI,
      const label lifeTime
    );
    //- Construct from Istream
    streamLineParticle
    (
      const polyMesh& c,
      Istream& is,
      bool readFields = true
    );
    //- Construct copy
    streamLineParticle(const streamLineParticle& p);
    //- Construct and return a clone
    autoPtr<particle> clone() const
    {
      return autoPtr<particle>{new streamLineParticle{*this}};
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
      autoPtr<streamLineParticle> operator()(Istream& is) const
      {
        return
          autoPtr<streamLineParticle>
          {
            new streamLineParticle{mesh_, is, true}
          };
      }
    };
  // Member Functions
    // Tracking
      //- Track all particles to their end point
      bool move(trackingData&, const scalar trackTime);
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
      //- Overridable function to handle the particle hitting a wedge
      void hitWedgePatch
      (
        const wedgePolyPatch&,
        trackingData& td
      );
      //- Overridable function to handle the particle hitting a
      //  symmetry plane
      void hitSymmetryPlanePatch
      (
        const symmetryPlanePolyPatch&,
        trackingData& td
      );
      //- Overridable function to handle the particle hitting a
      //  symmetry patch
      void hitSymmetryPatch
      (
        const symmetryPolyPatch&,
        trackingData& td
      );
      //- Overridable function to handle the particle hitting a cyclic
      void hitCyclicPatch
      (
        const cyclicPolyPatch&,
        trackingData& td
      );
      //- Overridable function to handle the particle hitting a
      //- processorPatch
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
    // I-O
      //- Read
      static void readFields(Cloud<streamLineParticle>&);
      //- Write
      static void writeFields(const Cloud<streamLineParticle>&);
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const streamLineParticle&);
};

}  // namespace mousse

#endif

