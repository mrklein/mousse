// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rotated_box_to_cell.hpp"
#include "poly_mesh.hpp"
#include "cell_modeller.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(rotatedBoxToCell, 0);
addToRunTimeSelectionTable(topoSetSource, rotatedBoxToCell, word);
addToRunTimeSelectionTable(topoSetSource, rotatedBoxToCell, istream);
}
mousse::topoSetSource::addToUsageTable mousse::rotatedBoxToCell::usage_
(
  rotatedBoxToCell::typeName,
  "\n    Usage: rotatedBoxToCell (originx originy originz)"
  " (ix iy iz) (jx jy jz) (kx ky kz)\n\n"
  "    Select all cells with cellCentre within parallelopiped\n\n"
);
// Private Member Functions 
void mousse::rotatedBoxToCell::combine(topoSet& set, const bool add) const
{
  // Define a cell for the box
  pointField boxPoints(8);
  boxPoints[0] = origin_;
  boxPoints[1] = origin_ + i_;
  boxPoints[2] = origin_ + i_ + j_;
  boxPoints[3] = origin_ + j_;
  boxPoints[4] = origin_ + k_;
  boxPoints[5] = origin_ + k_ + i_;
  boxPoints[6] = origin_ + k_ + i_ + j_;
  boxPoints[7] = origin_ + k_ + j_;
  labelList boxVerts(8);
  forAll(boxVerts, i)
  {
    boxVerts[i] = i;
  }
  const cellModel& hex = *(cellModeller::lookup("hex"));
  // Get outwards pointing faces.
  faceList boxFaces(cellShape(hex, boxVerts).faces());
  // Precalculate normals
  vectorField boxFaceNormals(boxFaces.size());
  forAll(boxFaces, i)
  {
    boxFaceNormals[i] = boxFaces[i].normal(boxPoints);
    //Pout<< "Face:" << i << " position:" << boxFaces[i].centre(boxPoints)
    //    << " normal:" << boxFaceNormals[i] << endl;
  }
  // Check whether cell centre is inside all faces of box.
  const pointField& ctrs = mesh_.cellCentres();
  forAll(ctrs, cellI)
  {
    bool inside = true;
    forAll(boxFaces, i)
    {
      const face& f = boxFaces[i];
      if (((ctrs[cellI] - boxPoints[f[0]]) & boxFaceNormals[i]) > 0)
      {
        inside = false;
        break;
      }
    }
    if (inside)
    {
      addOrDelete(set, cellI, add);
    }
  }
}
// Constructors 
// Construct from components
mousse::rotatedBoxToCell::rotatedBoxToCell
(
  const polyMesh& mesh,
  const vector& origin,
  const vector& i,
  const vector& j,
  const vector& k
)
:
  topoSetSource(mesh),
  origin_(origin),
  i_(i),
  j_(j),
  k_(k)
{}
// Construct from dictionary
mousse::rotatedBoxToCell::rotatedBoxToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  origin_(dict.lookup("origin")),
  i_(dict.lookup("i")),
  j_(dict.lookup("j")),
  k_(dict.lookup("k"))
{}
// Construct from Istream
mousse::rotatedBoxToCell::rotatedBoxToCell(const polyMesh& mesh, Istream& is)
:
  topoSetSource(mesh),
  origin_(is),
  i_(is),
  j_(is),
  k_(is)
{}
// Destructor 
mousse::rotatedBoxToCell::~rotatedBoxToCell()
{}
// Member Functions 
void mousse::rotatedBoxToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding cells with center within rotated box " << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing cells with center within rotated box " << endl;
    combine(set, false);
  }
}
