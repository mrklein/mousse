// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "block_descriptor.hpp"
// Constructors 
mousse::blockDescriptor::blockDescriptor
(
  const cellShape& bshape,
  const pointField& blockPointField,
  const curvedEdgeList& edges,
  const Vector<label>& meshDensity,
  const UList<gradingDescriptors>& expand,
  const word& zoneName
)
:
  blockPointField_(blockPointField),
  curvedEdges_(edges),
  blockShape_(bshape),
  meshDensity_(meshDensity),
  edgePoints_(12),
  edgeWeights_(12),
  expand_(expand),
  zoneName_(zoneName)
{
  if (expand_.size() != 12)
  {
    FatalErrorIn
    (
      "blockDescriptor::blockDescriptor"
      "(const cellShape&, const pointField& blockPointField, "
      "const curvedEdgeList&, const Vector<label>& meshDensity, "
      "const scalarList& expand, const word& zoneName)"
    )   << "Unknown definition of expansion ratios"
      << exit(FatalError);
  }
  // Create a list of edges
  makeBlockEdges();
}
mousse::blockDescriptor::blockDescriptor
(
  const pointField& blockPointField,
  const curvedEdgeList& edges,
  Istream& is
)
:
  blockPointField_(blockPointField),
  curvedEdges_(edges),
  blockShape_(is),
  meshDensity_(),
  edgePoints_(12),
  edgeWeights_(12),
  expand_
  (
    12,
    gradingDescriptors()
  ),
  zoneName_()
{
  // Examine next token
  token t(is);
  // Optional zone name
  if (t.isWord())
  {
    zoneName_ = t.wordToken();
    // Examine next token
    is >> t;
  }
  is.putBack(t);
  if (t.isPunctuation())
  {
    // New-style: read a list of 3 values
    if (t.pToken() == token::BEGIN_LIST)
    {
      is >> meshDensity_;
    }
    else
    {
      FatalIOErrorIn
      (
        "blockDescriptor::blockDescriptor"
        "(const pointField&, const curvedEdgeList&, Istream&)",
        is
      )   << "incorrect token while reading n, expected '(', found "
        << t.info()
        << exit(FatalIOError);
    }
  }
  else
  {
    // Old-style: read three labels
    is  >> meshDensity_.x()
      >> meshDensity_.y()
      >> meshDensity_.z();
  }
  is >> t;
  if (!t.isWord())
  {
    is.putBack(t);
  }
  List<gradingDescriptors> expRatios(is);
  if (expRatios.size() == 1)
  {
    // Identical in x/y/z-directions
    expand_ = expRatios[0];
  }
  else if (expRatios.size() == 3)
  {
    // x-direction
    expand_[0]  = expRatios[0];
    expand_[1]  = expRatios[0];
    expand_[2]  = expRatios[0];
    expand_[3]  = expRatios[0];
    // y-direction
    expand_[4]  = expRatios[1];
    expand_[5]  = expRatios[1];
    expand_[6]  = expRatios[1];
    expand_[7]  = expRatios[1];
    // z-direction
    expand_[8]  = expRatios[2];
    expand_[9]  = expRatios[2];
    expand_[10] = expRatios[2];
    expand_[11] = expRatios[2];
  }
  else if (expRatios.size() == 12)
  {
    expand_ = expRatios;
  }
  else
  {
    FatalErrorIn
    (
      "blockDescriptor::blockDescriptor"
      "(const pointField&, const curvedEdgeList&, Istream&)"
    )   << "Unknown definition of expansion ratios: " << expRatios
      << exit(FatalError);
  }
  // Create a list of edges
  makeBlockEdges();
}
// Destructor 
mousse::blockDescriptor::~blockDescriptor()
{}
// Member Functions 
const mousse::pointField& mousse::blockDescriptor::blockPointField() const
{
  return blockPointField_;
}
const mousse::cellShape& mousse::blockDescriptor::blockShape() const
{
  return blockShape_;
}
const mousse::List< mousse::List< mousse::point > >&
mousse::blockDescriptor::blockEdgePoints() const
{
  return edgePoints_;
}
const mousse::scalarListList& mousse::blockDescriptor::blockEdgeWeights() const
{
  return edgeWeights_;
}
const mousse::Vector<mousse::label>& mousse::blockDescriptor::meshDensity() const
{
  return meshDensity_;
}
const mousse::word& mousse::blockDescriptor::zoneName() const
{
  return zoneName_;
}
mousse::label mousse::blockDescriptor::nPoints() const
{
  return
  (
    (meshDensity_.x() + 1)
   * (meshDensity_.y() + 1)
   * (meshDensity_.z() + 1)
  );
}
mousse::label mousse::blockDescriptor::nCells() const
{
  return
  (
    meshDensity_.x()
   * meshDensity_.y()
   * meshDensity_.z()
  );
}
const mousse::point& mousse::blockDescriptor::blockPoint(const label i) const
{
  return blockPointField_[blockShape_[i]];
}
// Friend Operators
mousse::Ostream& mousse::operator<<(Ostream& os, const blockDescriptor& bd)
{
  const cellShape& bshape = bd.blockShape();
  const labelList& blockLabels = bshape;
  os  << bshape.model().name() << " (";
  forAll(blockLabels, labelI)
  {
    if (labelI)
    {
      os  << ' ';
    }
    os  << blockLabels[labelI];
  }
  os  << ')';
  if (bd.zoneName().size())
  {
    os  << ' ' << bd.zoneName();
  }
  os  << ' '  << bd.meshDensity()
    << " simpleGrading (";
  const List<gradingDescriptors>& expand = bd.expand_;
  // Can we use a compact notation?
  if
  (
    // x-direction
    (
      expand[0] == expand[1]
    && expand[0] == expand[2]
    && expand[0] == expand[3]
    )
  && // y-direction
    (
      expand[4] == expand[5]
    && expand[4] == expand[6]
    && expand[4] == expand[7]
    )
  && // z-direction
    (
      expand[8] == expand[9]
    && expand[8] == expand[10]
    && expand[8] == expand[11]
    )
  )
  {
    os  << expand[0] << ' ' << expand[4] << ' ' << expand[8];
  }
  else
  {
    forAll(expand, edgeI)
    {
      if (edgeI)
      {
        os  << ' ';
      }
      os  << expand[edgeI];
    }
  }
  os  << ")";
  return os;
}
