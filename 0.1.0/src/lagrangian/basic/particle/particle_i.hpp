// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "time.hpp"
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
    FatalErrorIn
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
      FatalErrorIn
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
  forAll(thisCell, cFI)
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
        FatalErrorIn
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
    WarningIn("particle::getNewParticleID() const")
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
      FatalErrorIn("void mousse::particle::initCellFacePt()")
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
          FatalErrorIn("void mousse::particle::initCellFacePt()")
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
          FatalErrorIn("void mousse::particle::initCellFacePt()")
            << "cell, tetFace and tetPt search failure at position "
            << position_ << abort(FatalError);
        }
        if (debug)
        {
          WarningIn("void mousse::particle::initCellFacePt()")
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
        WarningIn("void mousse::particle::initCellFacePt()")
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
