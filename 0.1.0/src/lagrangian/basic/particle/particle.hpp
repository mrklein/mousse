// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::particle
// Description
//   Base particle class
#ifndef particle_hpp_
#define particle_hpp_
#include "vector.hpp"
#include "_cloud.hpp"
#include "idl_list.hpp"
#include "point_field.hpp"
#include "face_list.hpp"
#include "ofstream.hpp"
#include "tetrahedron.hpp"
#include "fixed_list.hpp"
#include "poly_mesh_tet_decomposition.hpp"
#include "particle_macros.hpp"
#include "poly_mesh.hpp"
#include "time.hpp"
namespace mousse
{
// Forward declaration of classes
class particle;
class polyPatch;
class cyclicPolyPatch;
class processorPolyPatch;
class symmetryPlanePolyPatch;
class symmetryPolyPatch;
class wallPolyPatch;
class wedgePolyPatch;
// Forward declaration of friend functions and operators
Ostream& operator<<
(
  Ostream&,
  const particle&
);
bool operator==(const particle&, const particle&);
bool operator!=(const particle&, const particle&);
class particle
:
  public IDLList<particle>::link
{
  // Private member data
    //- Size in bytes of the position data
    static const std::size_t sizeofPosition_;
    //- Size in bytes of the fields
    static const std::size_t sizeofFields_;
public:
  template<class CloudType>
  class TrackingData
  {
    // Private data
      //- Reference to the cloud containing (this) particle
      CloudType& cloud_;
  public:
    // Public data
      typedef CloudType cloudType;
      //- Flag to switch processor
      bool switchProcessor;
      //- Flag to indicate whether to keep particle (false = delete)
      bool keepParticle;
    // Constructor
    TrackingData(CloudType& cloud)
    :
      cloud_(cloud)
    {}
    // Member functions
      //- Return a reference to the cloud
      CloudType& cloud()
      {
        return cloud_;
      }
  };
protected:
  // Protected data
    //- Reference to the polyMesh database
    const polyMesh& mesh_;
    //- Position of particle
    vector position_;
    //- Index of the cell it is in
    label cellI_;
    //- Face index if the particle is on a face otherwise -1
    label faceI_;
    //- Fraction of time-step completed
    scalar stepFraction_;
    //- Index of the face that owns the decomposed tet that the
    //  particle is in
    label tetFaceI_;
    //- Index of the point on the face that defines the decomposed
    //  tet that the particle is in.  Relative to the face base
    //  point.
    label tetPtI_;
    //- Originating processor id
    label origProc_;
    //- Local particle id on originating processor
    label origId_;
  // Private Member Functions
    //- Find the tet tri faces between position and tet centre
    void findTris
    (
      const vector& position,
      DynamicList<label>& faceList,
      const tetPointRef& tet,
      const FixedList<vector, 4>& tetAreas,
      const FixedList<label, 4>& tetPlaneBasePtIs,
      const scalar tol
    ) const;
    //- Find the lambda value for the line to-from across the
    //  given tri face, where p = from + lambda*(to - from)
    inline scalar tetLambda
    (
      const vector& from,
      const vector& to,
      const label triI,
      const vector& tetArea,
      const label tetPlaneBasePtI,
      const label cellI,
      const label tetFaceI,
      const label tetPtI,
      const scalar tol
    ) const;
    //- Find the lambda value for a moving tri face
    inline scalar movingTetLambda
    (
      const vector& from,
      const vector& to,
      const label triI,
      const vector& tetArea,
      const label tetPlaneBasePtI,
      const label cellI,
      const label tetFaceI,
      const label tetPtI,
      const scalar tol
    ) const;
    //- Modify the tet owner data by crossing triI
    inline void tetNeighbour(label triI);
    //- Cross the from the given face across the given edge of the
    //  given cell to find the resulting face and tetPtI
    inline void crossEdgeConnectedFace
    (
      const label& cellI,
      label& tetFaceI,
      label& tetPtI,
      const edge& e
    );
    //- Hit wall faces in the current cell if the
    //- wallImpactDistance is non-zero.  They may not be in
    //- Different tets to the current.
    template<class CloudType>
    inline void hitWallFaces
    (
      const CloudType& td,
      const vector& from,
      const vector& to,
      scalar& lambdaMin,
      tetIndices& closestTetIs
    );
  // Patch interactions
    //- Overridable function to handle the particle hitting a face
    template<class TrackData>
    void hitFace(TrackData& td);
    //- Overridable function to handle the particle hitting a
    //  patch.  Executed before other patch-hitting functions.
    //  trackFraction is passed in to allow mesh motion to
    //  interpolate in time to the correct face state.
    template<class TrackData>
    bool hitPatch
    (
      const polyPatch&,
      TrackData& td,
      const label patchI,
      const scalar trackFraction,
      const tetIndices& tetIs
    );
    //- Overridable function to handle the particle hitting a wedgePatch
    template<class TrackData>
    void hitWedgePatch(const wedgePolyPatch&, TrackData& td);
    //- Overridable function to handle the particle hitting a
    //  symmetryPlanePatch
    template<class TrackData>
    void hitSymmetryPlanePatch
    (
      const symmetryPlanePolyPatch&,
      TrackData& td
    );
    //- Overridable function to handle the particle hitting a
    //  symmetryPatch
    template<class TrackData>
    void hitSymmetryPatch(const symmetryPolyPatch&, TrackData& td);
    //- Overridable function to handle the particle hitting a cyclicPatch
    template<class TrackData>
    void hitCyclicPatch(const cyclicPolyPatch&, TrackData& td);
    //- Overridable function to handle the particle hitting a cyclicAMIPatch
    template<class TrackData>
    void hitCyclicAMIPatch
    (
      const cyclicAMIPolyPatch&,
      TrackData& td,
      const vector& direction
    );
    //- Overridable function to handle the particle hitting a
    //  processorPatch
    template<class TrackData>
    void hitProcessorPatch(const processorPolyPatch&, TrackData& td);
    //- Overridable function to handle the particle hitting a wallPatch
    template<class TrackData>
    void hitWallPatch
    (
      const wallPolyPatch&,
      TrackData& td,
      const tetIndices& tetIs
    );
    //- Overridable function to handle the particle hitting a
    //  general patch
    template<class TrackData>
    void hitPatch(const polyPatch&, TrackData& td);
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("particle");
    //- String representation of properties
    DefinePropertyList("(Px Py Pz) cellI tetFaceI tetPtI origProc origId");
    //- Cumulative particle counter - used to provode unique ID
    static label particleCount_;
    //- Fraction of distance to tet centre to move a particle to
    // 'rescue' it from a tracking problem
    static const scalar trackingCorrectionTol;
    //- Fraction of the cell volume to use in determining tolerance values
    //  for the denominator and numerator of lambda
    static const scalar lambdaDistanceToleranceCoeff;
    //- Minimum stepFraction tolerance
    static const scalar minStepFractionTol;
  // Constructors
    //- Construct from components
    particle
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI
    );
    //- Construct from components, tetFaceI_ and tetPtI_ are not
    //  supplied so they will be deduced by a search
    particle
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      bool doCellFacePt = true
    );
    //- Construct from Istream
    particle(const polyMesh& mesh, Istream&, bool readFields = true);
    //- Construct as a copy
    particle(const particle& p);
    //- Construct as a copy with refernce to a new mesh
    particle(const particle& p, const polyMesh& mesh);
    //- Construct a clone
    virtual autoPtr<particle> clone() const
    {
      return autoPtr<particle>(new particle(*this));
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
      autoPtr<particle> operator()(Istream& is) const
      {
        return autoPtr<particle>(new particle(mesh_, is, true));
      }
    };
  //- Destructor
  virtual ~particle()
  {}
  // Member Functions
    // Access
      //- Get unique particle creation id
      inline label getNewParticleID() const;
      //- Return the mesh database
      inline const polyMesh& mesh() const;
      //- Return current particle position
      inline const vector& position() const;
      //- Return current particle position
      inline vector& position();
      //- Return current cell particle is in
      inline label& cell();
      //- Return current cell particle is in
      inline label cell() const;
      //- Return current tet face particle is in
      inline label& tetFace();
      //- Return current tet face particle is in
      inline label tetFace() const;
      //- Return current tet face particle is in
      inline label& tetPt();
      //- Return current tet face particle is in
      inline label tetPt() const;
      //- Return the indices of the current tet that the
      //  particle occupies.
      inline tetIndices currentTetIndices() const;
      //- Return the geometry of the current tet that the
      //  particle occupies.
      inline tetPointRef currentTet() const;
      //- Return the normal of the tri on tetFaceI_ for the
      //  current tet.
      inline vector normal() const;
      //- Return the normal of the tri on tetFaceI_ for the
      //  current tet at the start of the timestep, i.e. based
      //  on oldPoints
      inline vector oldNormal() const;
      //- Return current face particle is on otherwise -1
      inline label& face();
      //- Return current face particle is on otherwise -1
      inline label face() const;
      //- Return the impact model to be used, soft or hard (default).
      inline bool softImpact() const;
      //- Return the particle current time
      inline scalar currentTime() const;
    // Check
      //- Check the stored cell value (setting if necessary) and
      //  initialise the tetFace and tetPt values
      inline void initCellFacePt();
      //- Is the particle on the boundary/(or outside the domain)?
      inline bool onBoundary() const;
      //- Is this global face an internal face?
      inline bool internalFace(const label faceI) const;
      //- Is this global face a boundary face?
      inline bool boundaryFace(const label faceI) const;
      //- Which patch is particle on
      inline label patch(const label faceI) const;
      //- Which face of this patch is this particle on
      inline label patchFace
      (
        const label patchI,
        const label faceI
      ) const;
      //- Return the fraction of time-step completed
      inline scalar& stepFraction();
      //-  Return the fraction of time-step completed
      inline scalar stepFraction() const;
      //- Return const access to the originating processor id
      inline label origProc() const;
      //- Return the originating processor id for manipulation
      inline label& origProc();
      //- Return const access to  the particle id on originating processor
      inline label origId() const;
      //- Return the particle id on originating processor for manipulation
      inline label& origId();
    // Track
      //- Track particle to end of trajectory
      //  or until it hits the boundary.
      //  On entry 'stepFraction()' should be set to the fraction of the
      //  time-step at which the tracking starts and on exit it contains
      //  the fraction of the time-step completed.
      //  Returns the boundary face index if the track stops at the
      //  boundary, -1 otherwise.
      template<class TrackData>
      label track(const vector& endPosition, TrackData& td);
      //- Track particle to a given position and returns 1.0 if the
      //  trajectory is completed without hitting a face otherwise
      //  stops at the face and returns the fraction of the trajectory
      //  completed.
      //  on entry 'stepFraction()' should be set to the fraction of the
      //  time-step at which the tracking starts.
      template<class TrackData>
      scalar trackToFace(const vector& endPosition, TrackData& td);
      //- Return the index of the face to be used in the interpolation
      //  routine
      inline label faceInterpolation() const;
  // Transformations
    //- Transform the physical properties of the particle
    //  according to the given transformation tensor
    virtual void transformProperties(const tensor& T);
    //- Transform the physical properties of the particle
    //  according to the given separation vector
    virtual void transformProperties(const vector& separation);
    //- The nearest distance to a wall that
    //  the particle can be in the n direction
    virtual scalar wallImpactDistance(const vector& n) const;
  // Parallel transfer
    //- Convert global addressing to the processor patch
    //  local equivalents
    template<class TrackData>
    void prepareForParallelTransfer(const label patchI, TrackData& td);
    //- Convert processor patch addressing to the global equivalents
    //  and set the cellI to the face-neighbour
    template<class TrackData>
    void correctAfterParallelTransfer(const label patchI, TrackData& td);
  // I-O
    //- Read the fields associated with the owner cloud
    template<class CloudType>
    static void readFields(CloudType& c);
    //- Write the fields associated with the owner cloud
    template<class CloudType>
    static void writeFields(const CloudType& c);
    //- Write the particle position and cell
    void writePosition(Ostream&) const;
  // Friend Operators
    friend Ostream& operator<<(Ostream&, const particle&);
    friend bool operator==(const particle& pA, const particle& pB);
    friend bool operator!=(const particle& pA, const particle& pB);
};
}  // namespace mousse

// Private Member Functions 
inline void mousse::particle::findTris
(
  const vector& position,
  DynamicList<label>& faceList,
  const tetPointRef& tet,
  const FixedList<vector, 4>& tetAreas,
  const FixedList<label, 4>& tetPlaneBasePtIs,
  const scalar tol
) const
{
  faceList.clear();
  const point Ct = tet.centre();
  for (label i = 0; i < 4; i++)
  {
    scalar lambda = tetLambda
    (
      Ct,
      position,
      i,
      tetAreas[i],
      tetPlaneBasePtIs[i],
      cellI_,
      tetFaceI_,
      tetPtI_,
      tol
    );
    if ((lambda > 0.0) && (lambda < 1.0))
    {
      faceList.append(i);
    }
  }
}
inline mousse::scalar mousse::particle::tetLambda
(
  const vector& from,
  const vector& to,
  const label triI,
  const vector& n,
  const label tetPlaneBasePtI,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const scalar tol
) const
{
  const pointField& pPts = mesh_.points();
  if (mesh_.moving())
  {
    return movingTetLambda
    (
      from,
      to,
      triI,
      n,
      tetPlaneBasePtI,
      cellI,
      tetFaceI,
      tetPtI,
      tol
    );
  }
  const point& base = pPts[tetPlaneBasePtI];
  scalar lambdaNumerator = (base - from) & n;
  scalar lambdaDenominator = (to - from) & n;
  // n carries the area of the tet faces, so the dot product with a
  // delta-length has the units of volume.  Comparing the component of each
  // delta-length in the direction of n times the face area to a fraction of
  // the cell volume.
  if (mag(lambdaDenominator) < tol)
  {
    if (mag(lambdaNumerator) < tol)
    {
      // Track starts on the face, and is potentially
      // parallel to it.  +-tol/+-tol is not a good
      // comparison, return 0.0, in anticipation of tet
      // centre correction.
      return 0.0;
    }
    else
    {
      if (mag((to - from)) < tol/mag(n))
      {
        // 'Zero' length track (compared to the tolerance, which is
        // based on the cell volume, divided by the tet face area), not
        // along the face, face cannot be crossed.
        return GREAT;
      }
      else
      {
        // Trajectory is non-zero and parallel to face
        lambdaDenominator = sign(lambdaDenominator)*SMALL;
      }
    }
  }
  return lambdaNumerator/lambdaDenominator;
}
inline mousse::scalar mousse::particle::movingTetLambda
(
  const vector& from,
  const vector& to,
  const label triI,
  const vector& n,
  const label tetPlaneBasePtI,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const scalar tol
) const
{
  const pointField& pPts = mesh_.points();
  const pointField& oldPPts = mesh_.oldPoints();
  // Base point of plane at end of motion
  const point& b = pPts[tetPlaneBasePtI];
  // n: Normal of plane at end of motion
  // Base point of plane at start of timestep
  const point& b00 = oldPPts[tetPlaneBasePtI];
  // Base point of plane at start of tracking portion (cast forward by
  // stepFraction)
  point b0 = b00 + stepFraction_*(b - b00);
  // Normal of plane at start of tracking portion
  vector n0 = vector::zero;
  {
    tetIndices tetIs(cellI, tetFaceI, tetPtI, mesh_);
    // Tet at timestep start
    tetPointRef tet00 = tetIs.oldTet(mesh_);
    // Tet at timestep end
    tetPointRef tet = tetIs.tet(mesh_);
    point tet0PtA = tet00.a() + stepFraction_*(tet.a() - tet00.a());
    point tet0PtB = tet00.b() + stepFraction_*(tet.b() - tet00.b());
    point tet0PtC = tet00.c() + stepFraction_*(tet.c() - tet00.c());
    point tet0PtD = tet00.d() + stepFraction_*(tet.d() - tet00.d());
    // Tracking portion start tet (cast forward by stepFraction)
    tetPointRef tet0(tet0PtA, tet0PtB, tet0PtC, tet0PtD);
    switch (triI)
    {
      case 0:
      {
        n0 = tet0.Sa();
        break;
      }
      case 1:
      {
        n0 = tet0.Sb();
        break;
      }
      case 2:
      {
        n0 = tet0.Sc();
        break;
      }
      case 3:
      {
        n0 = tet0.Sd();
        break;
      }
      default:
      {
        break;
      }
    }
  }
  if (mag(n0) < SMALL)
  {
    // If the old normal is zero (for example in layer addition)
    // then use the current normal;
    n0 = n;
  }
  scalar lambdaNumerator = 0;
  scalar lambdaDenominator = 0;
  vector dP = to - from;
  vector dN = n - n0;
  vector dB = b - b0;
  vector dS = from - b0;
  if (mag(dN) > SMALL)
  {
    scalar a = (dP - dB) & dN;
    scalar b = ((dP - dB) & n0) + (dS & dN);
    scalar c = dS & n0;
    if (mag(a) > SMALL)
    {
      // Solve quadratic for lambda
      scalar discriminant = sqr(b) - 4.0*a*c;
      if (discriminant < 0)
      {
        // Imaginary roots only - face not crossed
        return GREAT;
      }
      else
      {
        scalar q = -0.5*(b + sign(b)*mousse::sqrt(discriminant));
        if (mag(q) < VSMALL)
        {
          // If q is zero, then l1 = q/a is the required
          // value of lambda, and is zero.
          return 0.0;
        }
        scalar l1 = q/a;
        scalar l2 = c/q;
        // There will be two roots, a big one and a little
        // one, choose the little one.
        if (mag(l1) < mag(l2))
        {
          return l1;
        }
        else
        {
          return l2;
        }
      }
    }
    {
      // When a is zero, solve the first order polynomial
      lambdaNumerator = -c;
      lambdaDenominator = b;
    }
  }
  else
  {
    // When n = n0 is zero, there is no plane rotation, solve the
    // first order polynomial
    lambdaNumerator = -(dS & n0);
    lambdaDenominator = ((dP - dB) & n0);
  }
  if (mag(lambdaDenominator) < tol)
  {
    if (mag(lambdaNumerator) < tol)
    {
      // Track starts on the face, and is potentially
      // parallel to it.  +-tol)/+-tol is not a good
      // comparison, return 0.0, in anticipation of tet
      // centre correction.
      return 0.0;
    }
    else
    {
      if (mag((to - from)) < tol/mag(n))
      {
        // Zero length track, not along the face, face
        // cannot be crossed.
        return GREAT;
      }
      else
      {
        // Trajectory is non-zero and parallel to face
        lambdaDenominator = sign(lambdaDenominator)*SMALL;
      }
    }
  }
  return lambdaNumerator/lambdaDenominator;
}
inline void mousse::particle::tetNeighbour(label triI)
{
  const labelList& pOwner = mesh_.faceOwner();
  const faceList& pFaces = mesh_.faces();
  bool own = (pOwner[tetFaceI_] == cellI_);
  const mousse::face& f = pFaces[tetFaceI_];
  label tetBasePtI = mesh_.tetBasePtIs()[tetFaceI_];
  if (tetBasePtI == -1)
  {
    FATAL_ERROR_IN
    (
      "inline void mousse::particle::tetNeighbour(label triI)"
    )
    << "No base point for face " << tetFaceI_ << ", " << f
    << ", produces a valid tet decomposition."
    << abort(FatalError);
  }
  label facePtI = (tetPtI_ + tetBasePtI) % f.size();
  label otherFacePtI = f.fcIndex(facePtI);
  switch (triI)
  {
    case 0:
    {
      // Crossing this triangle changes tet to that in the
      // neighbour cell over tetFaceI
      // Modification of cellI_ will happen by other indexing,
      // tetFaceI_ and tetPtI don't change.
      break;
    }
    case 1:
    {
      crossEdgeConnectedFace
      (
        cellI_,
        tetFaceI_,
        tetPtI_,
        mousse::edge(f[facePtI], f[otherFacePtI])
      );
      break;
    }
    case 2:
    {
      if (own)
      {
        if (tetPtI_ < f.size() - 2)
        {
          tetPtI_ = f.fcIndex(tetPtI_);
        }
        else
        {
          crossEdgeConnectedFace
          (
            cellI_,
            tetFaceI_,
            tetPtI_,
            mousse::edge(f[tetBasePtI], f[otherFacePtI])
          );
        }
      }
      else
      {
        if (tetPtI_ > 1)
        {
          tetPtI_ = f.rcIndex(tetPtI_);
        }
        else
        {
          crossEdgeConnectedFace
          (
            cellI_,
            tetFaceI_,
            tetPtI_,
            mousse::edge(f[tetBasePtI], f[facePtI])
          );
        }
      }
      break;
    }
    case 3:
    {
      if (own)
      {
        if (tetPtI_ > 1)
        {
          tetPtI_ = f.rcIndex(tetPtI_);
        }
        else
        {
          crossEdgeConnectedFace
          (
            cellI_,
            tetFaceI_,
            tetPtI_,
            mousse::edge(f[tetBasePtI], f[facePtI])
          );
        }
      }
      else
      {
        if (tetPtI_ < f.size() - 2)
        {
          tetPtI_ = f.fcIndex(tetPtI_);
        }
        else
        {
          crossEdgeConnectedFace
          (
            cellI_,
            tetFaceI_,
            tetPtI_,
            mousse::edge(f[tetBasePtI], f[otherFacePtI])
          );
        }
      }
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "inline void "
        "mousse::particle::tetNeighbour(label triI)"
      )
      << "Tet tri face index error, can only be 0..3, supplied "
      << triI << abort(FatalError);
      break;
    }
  }
}
inline void mousse::particle::crossEdgeConnectedFace
(
  const label& cellI,
  label& tetFaceI,
  label& tetPtI,
  const edge& e
)
{
  const faceList& pFaces = mesh_.faces();
  const cellList& pCells = mesh_.cells();
  const mousse::face& f = pFaces[tetFaceI];
  const mousse::cell& thisCell = pCells[cellI];
  FOR_ALL(thisCell, cFI)
  {
    // Loop over all other faces of this cell and
    // find the one that shares this edge
    label fI = thisCell[cFI];
    if (tetFaceI == fI)
    {
      continue;
    }
    const mousse::face& otherFace = pFaces[fI];
    label edDir = otherFace.edgeDirection(e);
    if (edDir == 0)
    {
      continue;
    }
    else if (f == pFaces[fI])
    {
      // This is a necessary condition if using duplicate baffles
      // (so coincident faces). We need to make sure we don't cross into
      // the face with the same vertices since we might enter a tracking
      // loop where it never exits. This test should be cheap
      // for most meshes so can be left in for 'normal' meshes.
      continue;
    }
    else
    {
      //Found edge on other face
      tetFaceI = fI;
      label eIndex = -1;
      if (edDir == 1)
      {
        // Edge is in the forward circulation of this face, so
        // work with the start point of the edge
        eIndex = findIndex(otherFace, e.start());
      }
      else
      {
        // edDir == -1, so the edge is in the reverse
        // circulation of this face, so work with the end
        // point of the edge
        eIndex = findIndex(otherFace, e.end());
      }
      label tetBasePtI = mesh_.tetBasePtIs()[fI];
      if (tetBasePtI == -1)
      {
        FATAL_ERROR_IN
        (
          "inline void "
          "mousse::particle::crossEdgeConnectedFace"
          "("
            "const label& cellI,"
            "label& tetFaceI,"
            "label& tetPtI,"
            "const edge& e"
          ")"
        )
        << "No base point for face " << fI << ", " << f
        << ", produces a decomposition that has a minimum "
        << "volume greater than tolerance."
        << abort(FatalError);
      }
      // Find eIndex relative to the base point on new face
      eIndex -= tetBasePtI;
      if (neg(eIndex))
      {
        eIndex = (eIndex + otherFace.size()) % otherFace.size();
      }
      if (eIndex == 0)
      {
        // The point is the base point, so this is first tet
        // in the face circulation
        tetPtI = 1;
      }
      else if (eIndex == otherFace.size() - 1)
      {
        // The point is the last before the base point, so
        // this is the last tet in the face circulation
        tetPtI = otherFace.size() - 2;
      }
      else
      {
        tetPtI = eIndex;
      }
      break;
    }
  }
}
// Member Functions 
inline mousse::label mousse::particle::getNewParticleID() const
{
  label id = particleCount_++;
  if (id == labelMax)
  {
    WARNING_IN("particle::getNewParticleID() const")
      << "Particle counter has overflowed. This might cause problems"
      << " when reconstructing particle tracks." << endl;
  }
  return id;
}
inline const mousse::polyMesh& mousse::particle::mesh() const
{
  return mesh_;
}
inline const mousse::vector& mousse::particle::position() const
{
  return position_;
}
inline mousse::vector& mousse::particle::position()
{
  return position_;
}
inline mousse::label mousse::particle::cell() const
{
  return cellI_;
}
inline mousse::label& mousse::particle::cell()
{
  return cellI_;
}
inline mousse::label mousse::particle::tetFace() const
{
  return tetFaceI_;
}
inline mousse::label& mousse::particle::tetFace()
{
  return tetFaceI_;
}
inline mousse::label mousse::particle::tetPt() const
{
  return tetPtI_;
}
inline mousse::label& mousse::particle::tetPt()
{
  return tetPtI_;
}
inline mousse::tetIndices mousse::particle::currentTetIndices() const
{
  return tetIndices(cellI_, tetFaceI_, tetPtI_, mesh_);
}
inline mousse::tetPointRef mousse::particle::currentTet() const
{
  return currentTetIndices().tet(mesh_);
}
inline mousse::vector mousse::particle::normal() const
{
  return currentTetIndices().faceTri(mesh_).normal();
}
inline mousse::vector mousse::particle::oldNormal() const
{
  return currentTetIndices().oldFaceTri(mesh_).normal();
}
inline mousse::label mousse::particle::face() const
{
  return faceI_;
}
inline mousse::label& mousse::particle::face()
{
  return faceI_;
}
inline void mousse::particle::initCellFacePt()
{
  if (cellI_ == -1)
  {
    mesh_.findCellFacePt
    (
      position_,
      cellI_,
      tetFaceI_,
      tetPtI_
    );
    if (cellI_ == -1)
    {
      FATAL_ERROR_IN("void mousse::particle::initCellFacePt()")
        << "cell, tetFace and tetPt search failure at position "
        << position_ << abort(FatalError);
    }
  }
  else
  {
    mesh_.findTetFacePt(cellI_, position_, tetFaceI_, tetPtI_);
    if (tetFaceI_ == -1 || tetPtI_ == -1)
    {
      label oldCellI = cellI_;
      mesh_.findCellFacePt
      (
        position_,
        cellI_,
        tetFaceI_,
        tetPtI_
      );
      if (cellI_ == -1 || tetFaceI_ == -1 || tetPtI_ == -1)
      {
        // The particle has entered this function with a cell
        // number, but hasn't been able to find a cell to
        // occupy.
        if (!mesh_.pointInCellBB(position_, oldCellI, 0.1))
        {
          // If the position is not inside the (slightly
          // extended) bound-box of the cell that it thought
          // it should be in, then this is considered an
          // error.
          FATAL_ERROR_IN("void mousse::particle::initCellFacePt()")
            << "    cell, tetFace and tetPt search failure at "
            << "position " << position_ << nl
            << "    for requested cell " << oldCellI << nl
            << "    If this is a restart or "
             "reconstruction/decomposition etc. it is likely that"
             " the write precision is not sufficient.\n"
             "    Either increase 'writePrecision' or "
             "set 'writeFormat' to 'binary'"
            << abort(FatalError);
        }
        // The position is in the (slightly extended)
        // bound-box of the cell.  This situation may arise
        // because the face decomposition of the cell is not
        // the same as when the particle acquired the cell
        // index.  For example, it has been read into a mesh
        // that has made a different face base-point decision
        // for a boundary face and now this particle is in a
        // position that is not in the mesh.  Gradually move
        // the particle towards the centre of the cell that it
        // thought that it was in.
        cellI_ = oldCellI;
        point newPosition = position_;
        const point& cC = mesh_.cellCentres()[cellI_];
        label trap(1.0/trackingCorrectionTol + 1);
        label iterNo = 0;
        do
        {
          newPosition += trackingCorrectionTol*(cC - position_);
          mesh_.findTetFacePt
          (
            cellI_,
            newPosition,
            tetFaceI_,
            tetPtI_
          );
          iterNo++;
        } while (tetFaceI_ < 0  && iterNo <= trap);
        if (tetFaceI_ == -1)
        {
          FATAL_ERROR_IN("void mousse::particle::initCellFacePt()")
            << "cell, tetFace and tetPt search failure at position "
            << position_ << abort(FatalError);
        }
        if (debug)
        {
          WARNING_IN("void mousse::particle::initCellFacePt()")
            << "Particle moved from " << position_
            << " to " << newPosition
            << " in cell " << cellI_
            << " tetFace " << tetFaceI_
            << " tetPt " << tetPtI_ << nl
            << "    (A fraction of "
            << 1.0 - mag(cC - newPosition)/mag(cC - position_)
            << " of the distance to the cell centre)"
            << " because a decomposition tetFace and tetPt "
            << "could not be found."
            << endl;
        }
        position_ = newPosition;
      }
      if (debug && cellI_ != oldCellI)
      {
        WARNING_IN("void mousse::particle::initCellFacePt()")
          << "Particle at position " << position_
          << " searched for a cell, tetFace and tetPt." << nl
          << "    Found"
          << " cell " << cellI_
          << " tetFace " << tetFaceI_
          << " tetPt " << tetPtI_ << nl
          << "    This is a different cell to that which was supplied"
          << " (" << oldCellI << ")." << nl
          << endl;
      }
    }
  }
}
inline bool mousse::particle::onBoundary() const
{
  return faceI_ != -1 && faceI_ >= mesh_.nInternalFaces();
}
inline mousse::scalar& mousse::particle::stepFraction()
{
  return stepFraction_;
}
inline mousse::scalar mousse::particle::stepFraction() const
{
  return stepFraction_;
}
inline mousse::label mousse::particle::origProc() const
{
  return origProc_;
}
inline mousse::label& mousse::particle::origProc()
{
  return origProc_;
}
inline mousse::label mousse::particle::origId() const
{
  return origId_;
}
inline mousse::label& mousse::particle::origId()
{
  return origId_;
}
inline bool mousse::particle::softImpact() const
{
  return false;
}
inline mousse::scalar mousse::particle::currentTime() const
{
  return
    mesh_.time().value()
   + stepFraction_*mesh_.time().deltaTValue();
}
inline bool mousse::particle::internalFace(const label faceI) const
{
  return mesh_.isInternalFace(faceI);
}
bool mousse::particle::boundaryFace(const label faceI) const
{
  return !internalFace(faceI);
}
inline mousse::label mousse::particle::patch(const label faceI) const
{
  return mesh_.boundaryMesh().whichPatch(faceI);
}
inline mousse::label mousse::particle::patchFace
(
  const label patchI,
  const label faceI
) const
{
  return mesh_.boundaryMesh()[patchI].whichFace(faceI);
}
inline mousse::label mousse::particle::faceInterpolation() const
{
  return faceI_;
}

#ifdef NoRepository
#   include "particle_templates.cpp"
#endif
#endif
