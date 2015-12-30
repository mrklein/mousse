// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ignition_site.hpp"
#include "time.hpp"
#include "vol_fields.hpp"
// Private Member Functions 
void mousse::ignitionSite::findIgnitionCells(const fvMesh& mesh)
{
  // Bit tricky: generate C and V before shortcutting if cannot find
  // cell locally. mesh.C generation uses parallel communication.
  const volVectorField& centres = mesh.C();
  const scalarField& vols = mesh.V();
  label ignCell = mesh.findCell(location_);
  if (ignCell == -1)
  {
    return;
  }
  scalar radius = diameter_/2.0;
  cells_.setSize(1);
  cellVolumes_.setSize(1);
  cells_[0] = ignCell;
  cellVolumes_[0] = vols[ignCell];
  scalar minDist = GREAT;
  label nIgnCells = 1;
  forAll(centres, celli)
  {
    scalar dist = mag(centres[celli] - location_);
    if (dist < minDist)
    {
      minDist = dist;
    }
    if (dist < radius && celli != ignCell)
    {
      cells_.setSize(nIgnCells+1);
      cellVolumes_.setSize(nIgnCells+1);
      cells_[nIgnCells] = celli;
      cellVolumes_[nIgnCells] = vols[celli];
      nIgnCells++;
    }
  }
  if (cells_.size())
  {
    Pout<< "Found ignition cells:" << endl << cells_ << endl;
  }
}
// Member Functions 
const mousse::labelList& mousse::ignitionSite::cells() const
{
  if (mesh_.changing() && timeIndex_ != db_.timeIndex())
  {
    const_cast<ignitionSite&>(*this).findIgnitionCells(mesh_);
  }
  timeIndex_ = db_.timeIndex();
  return cells_;
}
bool mousse::ignitionSite::igniting() const
{
  scalar curTime = db_.value();
  scalar deltaT = db_.deltaTValue();
  return
  (
    (curTime - deltaT >= time_)
    &&
    (curTime - deltaT < time_ + max(duration_, deltaT) + SMALL)
  );
}
bool mousse::ignitionSite::ignited() const
{
  scalar curTime = db_.value();
  scalar deltaT = db_.deltaTValue();
  return(curTime - deltaT >= time_);
}
// Member Operators 
void mousse::ignitionSite::operator=(const ignitionSite& is)
{
  location_ = is.location_;
  diameter_ = is.diameter_;
  time_ = is.time_;
  duration_ = is.duration_;
  strength_ = is.strength_;
  cells_ = is.cells_;
  cellVolumes_ = is.cellVolumes_;
}
