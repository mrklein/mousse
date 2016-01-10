// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
// Private Member Functions 
void mousse::edgeFaceCirculator::setEnd()
{
  faceLabel_ = -1;
  index_ = -1;
}
void mousse::edgeFaceCirculator::setFace
(
  const label faceI,
  const label cellI
)
{
  faceLabel_ = faceI;
  if (!isBoundaryEdge_ && !mesh_.isInternalFace(faceI))
  {
    FATAL_ERROR_IN
    (
      "edgeFaceCirculator::setFace(const label, const label)"
    )   << "Edge is not defined as boundary edge but still walked to"
      << " boundary face:" << faceI << " on cell:" << cellI
      << abort(FatalError);
  }
}
void mousse::edgeFaceCirculator::otherFace(const label cellI)
{
  const face& f = mesh_.faces()[faceLabel_];
  label v0 = f[index_];
  label v1 = f.nextLabel(index_);
  const cell& cFaces = mesh_.cells()[cellI];
  FOR_ALL(cFaces, i)
  {
    label faceB = cFaces[i];
    if (faceB != faceLabel_)
    {
      label fp = getMinIndex(mesh_.faces()[faceB], v0, v1);
      if (fp >= 0)
      {
        index_ = fp;
        setFace(faceB, cellI);
        return;
      }
    }
  }
  FATAL_ERROR_IN("edgeFaceCirculator::otherFace(const label)")
    << "Could not find next face stepping"
    << " through cell along edge." << endl
    << "face:" << faceLabel_ << " index in face:" << index_
    << " edge:" << mesh_.points()[v0] << mesh_.points()[v1]
    << abort(FatalError);
}
// Constructors 
//- Construct from components
mousse::edgeFaceCirculator::edgeFaceCirculator
(
  const primitiveMesh& mesh,
  const label faceLabel,
  const bool ownerSide,
  const label index,
  const bool isBoundaryEdge
)
:
  mesh_(mesh),
  faceLabel_(faceLabel),
  ownerSide_(ownerSide),
  index_(index),
  isBoundaryEdge_(isBoundaryEdge),
  startFaceLabel_(faceLabel_)
{}
//- Construct copy
mousse::edgeFaceCirculator::edgeFaceCirculator(const edgeFaceCirculator& circ)
:
  mesh_(circ.mesh_),
  faceLabel_(circ.faceLabel_),
  ownerSide_(circ.ownerSide_),
  index_(circ.index_),
  isBoundaryEdge_(circ.isBoundaryEdge_),
  startFaceLabel_(circ.startFaceLabel_)
{}
// Member Functions 
mousse::label mousse::edgeFaceCirculator::getMinIndex
(
  const face& f,
  const label v0,
  const label v1
)
{
  label fp = findIndex(f, v0);
  if (fp != -1)
  {
    label fpMin1 = f.rcIndex(fp);
    if (f[fpMin1] == v1)
    {
      fp = fpMin1;
    }
    else
    {
      label fpPlus1 = f.fcIndex(fp);
      if (f[fpPlus1] != v1)
      {
        fp = -1;
      }
    }
  }
  return fp;
}
mousse::label mousse::edgeFaceCirculator::faceLabel() const
{
  return faceLabel_;
}
bool mousse::edgeFaceCirculator::ownerSide() const
{
  return ownerSide_;
}
mousse::label mousse::edgeFaceCirculator::index() const
{
  return index_;
}
mousse::label mousse::edgeFaceCirculator::cellLabel() const
{
  if (ownerSide_)
  {
    return mesh_.faceOwner()[faceLabel_];
  }
  else if (mesh_.isInternalFace(faceLabel_))
  {
    return mesh_.faceNeighbour()[faceLabel_];
  }
  else
  {
    return -1;
  }
}
bool mousse::edgeFaceCirculator::sameOrder(const label v0, const label v1) const
{
  const face& f = mesh_.faces()[faceLabel_];
  label fp = getMinIndex(f, v0, v1);
  if (fp != index_)
  {
    FATAL_ERROR_IN
    (
      "edgeFaceCirculator::sameOrder(const label, const label) const"
    )   << "v0:" << v1 << " and v1:" << v1
      << " not on position:" << index_ << " on face:" << faceLabel_
      << " verts:" << f << " or not consecutive." << abort(FatalError);
  }
  // If we are neighbour the face would point into us so the min index would
  // be v0.
  return ownerSide_ != (f[index_] == v0);
}
void mousse::edgeFaceCirculator::setCanonical()
{
  if (isBoundaryEdge_)
  {
    // Boundary edge. Step until we're on boundary face and ownerSide
    label i = 0;
    while (true)
    {
      if (mesh_.isInternalFace(faceLabel_))
      {
        if (ownerSide_)
        {
          label cellI = mesh_.faceNeighbour()[faceLabel_];
          otherFace(cellI);
          // Maintain reverse direction of walking
          ownerSide_ = (mesh_.faceOwner()[faceLabel_] == cellI);
        }
        else
        {
          label cellI = mesh_.faceOwner()[faceLabel_];
          otherFace(cellI);
          // Maintain reverse direction of walking
          ownerSide_ = (mesh_.faceOwner()[faceLabel_] == cellI);
        }
      }
      else if (ownerSide_)
      {
        break;
      }
      else
      {
        label cellI = mesh_.faceOwner()[faceLabel_];
        otherFace(cellI);
        // Maintain reverse direction of walking
        ownerSide_ = (mesh_.faceOwner()[faceLabel_] == cellI);
      }
      i++;
      if (i >= 1000)
      {
        const face& f = mesh_.faces()[faceLabel_];
        FATAL_ERROR_IN("mousse::edgeFaceCirculator::setCanonical()")
          << "Walked " << i << " cells around edge "
          << mesh_.points()[f[index_]]
          << mesh_.points()[f.nextLabel(index_)]
          << " without reaching a boundary face."
          << " Are you sure this is a boundary edge?"
          << abort(FatalError);
      }
    }
    // Set up for correct walking
    ownerSide_ = true;
    startFaceLabel_ = faceLabel_;
  }
  else
  {
    // Internal edge. Walk until we hit minimum face label.
    label minFaceI = faceLabel_;
    bool minOwnerSide = ownerSide_;
    label minIndex = index_;
    while (true)
    {
      operator++();
      if (operator==(end()))
      {
        break;
      }
      if (!mesh_.isInternalFace(faceLabel_))
      {
        const face& f = mesh_.faces()[faceLabel_];
        FATAL_ERROR_IN("mousse::edgeFaceCirculator::setCanonical()")
          << "Reached boundary face " << faceLabel_
          << " when walking around internal edge "
          << mesh_.points()[f[index_]]
          << mesh_.points()[f.nextLabel(index_)]
          << "." << endl
          << "Are you sure this is an internal edge?"
          << abort(FatalError);
      }
      if (faceLabel_ < minFaceI)
      {
        minFaceI = faceLabel_;
        minOwnerSide = ownerSide_;
        minIndex = index_;
      }
    }
    faceLabel_ = minFaceI;
    ownerSide_ = minOwnerSide;
    index_ = minIndex;
    startFaceLabel_ = faceLabel_;
  }
}
void mousse::edgeFaceCirculator::operator=(const edgeFaceCirculator& circ)
{
  faceLabel_ = circ.faceLabel_;
  ownerSide_ = circ.ownerSide_;
  index_ = circ.index_;
  isBoundaryEdge_ = circ.isBoundaryEdge_;
  startFaceLabel_ = circ.startFaceLabel_;
}
bool mousse::edgeFaceCirculator::operator==(const edgeFaceCirculator& circ) const
{
  return faceLabel_ == circ.faceLabel_ && index_ == circ.index_;
  ////- Note: do I need full comparison? If not we now have that circulators
  ////  around same edge but in different direction are considered not equal
  //if (faceLabel_ == -1 && circ.faceLabel_ == -1)
  //{
  //    // both endConstIter
  //    return true;
  //}
  //
  //return
  //    faceLabel_ == circ.faceLabel_
  // && ownerSide_ == circ.ownerSide_
  // && index_ == circ.index_;
  // && startFaceLabel_ == circ.startFaceLabel_;
}
bool mousse::edgeFaceCirculator::operator!=(const edgeFaceCirculator& circ) const
{
  return !(*this == circ);
}
//- Step to next face.
mousse::edgeFaceCirculator&
mousse::edgeFaceCirculator::operator++()
{
  if (faceLabel_ == -1)
  {
    FATAL_ERROR_IN("edgeFaceCirculator::operator++()")
      << "Already reached end(). Cannot walk any further."
      << abort(FatalError);
  }
  else if (ownerSide_)
  {
    // Step to owner
    label cellI = mesh_.faceOwner()[faceLabel_];
    otherFace(cellI);
    // Maintain direction of walking
    ownerSide_ = (mesh_.faceOwner()[faceLabel_] != cellI);
    // Check for internal edge : ends on starting face.
    if (!isBoundaryEdge_ && faceLabel_ == startFaceLabel_)
    {
      setEnd();
    }
  }
  else if (mesh_.isInternalFace(faceLabel_))
  {
    // Step to neighbour
    label cellI = mesh_.faceNeighbour()[faceLabel_];
    otherFace(cellI);
    // Maintain direction of walking
    ownerSide_ = (mesh_.faceOwner()[faceLabel_] != cellI);
    // Check for internal edge : ends on starting face.
    if (!isBoundaryEdge_ && faceLabel_ == startFaceLabel_)
    {
      setEnd();
    }
  }
  else
  {
    // neighbour side of boundary face reached. Mark as endConstIter.
    setEnd();
  }
  return *this;
}
mousse::edgeFaceCirculator mousse::edgeFaceCirculator::begin() const
{
  edgeFaceCirculator iter
  (
    mesh_,
    faceLabel_,
    ownerSide_,
    index_,
    isBoundaryEdge_
  );
  if (isBoundaryEdge_)
  {
    iter.setCanonical();
  }
  return iter;
}
mousse::edgeFaceCirculator mousse::edgeFaceCirculator::cbegin() const
{
  edgeFaceCirculator iter
  (
    mesh_,
    faceLabel_,
    ownerSide_,
    index_,
    isBoundaryEdge_
  );
  if (isBoundaryEdge_)
  {
    iter.setCanonical();
  }
  return iter;
}
const mousse::edgeFaceCirculator& mousse::edgeFaceCirculator::end() const
{
  return endConstIter;
}
const mousse::edgeFaceCirculator& mousse::edgeFaceCirculator::cend() const
{
  return endConstIter;
}
