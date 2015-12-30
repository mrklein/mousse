// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallBoundedStreamLineParticle
// Description
//   Particle class that samples fields as it passes through. Used in streamline
//   calculation.
// SourceFiles
//   wall_bounded_stream_line_particle.cpp
#ifndef wall_bounded_stream_line_particle_hpp_
#define wall_bounded_stream_line_particle_hpp_
#include "wall_bounded_particle.hpp"
#include "auto_ptr.hpp"
#include "interpolation.hpp"
#include "vector_list.hpp"
#include "info_proxy.hpp"
namespace mousse
{
class wallBoundedStreamLineParticleCloud;
class wallBoundedStreamLineParticle
:
  public wallBoundedParticle
{
public:
  //- Class used to pass tracking data to the trackToEdge function
  class trackingData
  :
    public wallBoundedParticle::TrackingData
    <
      Cloud<wallBoundedStreamLineParticle>
    >
  {
  public:
    const PtrList<interpolation<scalar> >& vsInterp_;
    const PtrList<interpolation<vector> >& vvInterp_;
    const label UIndex_;
    const bool trackForward_;
    const scalar trackLength_;
    DynamicList<vectorList>& allPositions_;
    List<DynamicList<scalarList> >& allScalars_;
    List<DynamicList<vectorList> >& allVectors_;
    // Constructors
      trackingData
      (
        Cloud<wallBoundedStreamLineParticle>& cloud,
        const PtrList<interpolation<scalar> >& vsInterp,
        const PtrList<interpolation<vector> >& vvInterp,
        const label UIndex,
        const bool trackForward,
        const scalar trackLength,
        const PackedBoolList& isWallPatch,
        DynamicList<List<point> >& allPositions,
        List<DynamicList<scalarList> >& allScalars,
        List<DynamicList<vectorList> >& allVectors
      )
      :
        wallBoundedParticle::TrackingData
        <
          Cloud<wallBoundedStreamLineParticle>
        >
        (
          cloud,
          isWallPatch
        ),
        vsInterp_(vsInterp),
        vvInterp_(vvInterp),
        UIndex_(UIndex),
        trackForward_(trackForward),
        trackLength_(trackLength),
        allPositions_(allPositions),
        allScalars_(allScalars),
        allVectors_(allVectors)
      {}
  };
private:
  // Private data
    //- Lifetime of particle. Particle dies when reaches 0.
    label lifeTime_;
    //- Sampled positions
    DynamicList<point> sampledPositions_;
    //- Sampled scalars
    List<DynamicList<scalar> > sampledScalars_;
    //- Sampled vectors
    List<DynamicList<vector> > sampledVectors_;
  // Private Member Functions
    vector interpolateFields
    (
      const trackingData& td,
      const point& position,
      const label cellI,
      const label faceI
    );
    vector sample(trackingData& td);
public:
  // Constructors
    //- Construct from components
    wallBoundedStreamLineParticle
    (
      const polyMesh& c,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI,
      const label meshEdgeStart,
      const label diagEdge,
      const label lifeTime
    );
    //- Construct from Istream
    wallBoundedStreamLineParticle
    (
      const polyMesh& c,
      Istream& is,
      bool readFields = true
    );
    //- Construct copy
    wallBoundedStreamLineParticle(const wallBoundedStreamLineParticle& p);
    //- Construct and return a clone
    autoPtr<particle> clone() const
    {
      return autoPtr<particle>(new wallBoundedStreamLineParticle(*this));
    }
    //- Factory class to read-construct particles used for
    //  parallel transfer
    class iNew
    {
      const polyMesh& mesh_;
    public:
      iNew(const polyMesh& mesh)
      :
        mesh_(mesh)
      {}
      autoPtr<wallBoundedStreamLineParticle> operator()
      (
        Istream& is
      ) const
      {
        return autoPtr<wallBoundedStreamLineParticle>
        (
          new wallBoundedStreamLineParticle(mesh_, is, true)
        );
      }
    };
  // Member Functions
    // Tracking
      //- Track all particles to their end point
      bool move(trackingData&, const scalar trackTime);
    // I-O
      //- Read
      static void readFields(Cloud<wallBoundedStreamLineParticle>&);
      //- Write
      static void writeFields
      (
        const Cloud<wallBoundedStreamLineParticle>&
      );
  // Ostream Operator
    friend Ostream& operator<<
    (
      Ostream&,
      const wallBoundedStreamLineParticle&
    );
};
}  // namespace mousse
#endif
