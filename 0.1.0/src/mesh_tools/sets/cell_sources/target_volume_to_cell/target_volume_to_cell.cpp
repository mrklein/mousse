// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "target_volume_to_cell.hpp"
#include "poly_mesh.hpp"
#include "global_mesh_data.hpp"
#include "plane.hpp"
#include "cell_set.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "pstream_reduce_ops.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(targetVolumeToCell, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, targetVolumeToCell, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, targetVolumeToCell, istream);
}

mousse::topoSetSource::addToUsageTable mousse::targetVolumeToCell::usage_
(
  targetVolumeToCell::typeName,
  "\n    Usage: targetVolumeToCell (nx ny nz)\n\n"
  "    Adjust plane until obtained selected volume\n\n"
);

// Private Member Functions
mousse::scalar mousse::targetVolumeToCell::volumeOfSet
(
  const PackedBoolList& selected
) const
{
  scalar sumVol = 0.0;
  FOR_ALL(selected, cellI)
  {
    if (selected[cellI])
    {
      sumVol += mesh_.cellVolumes()[cellI];
    }
  }
  return returnReduce(sumVol, sumOp<scalar>());
}
mousse::label mousse::targetVolumeToCell::selectCells
(
  const scalar normalComp,
  const PackedBoolList& maskSet,
  PackedBoolList& selected
) const
{
  selected.setSize(mesh_.nCells());
  selected = false;
  label nSelected = 0;
  FOR_ALL(mesh_.cellCentres(), cellI)
  {
    const point& cc = mesh_.cellCentres()[cellI];
    if (maskSet[cellI] && ((cc&n_) < normalComp))
    {
      selected[cellI] = true;
      nSelected++;
    }
  }
  return returnReduce(nSelected, sumOp<label>());
}
void mousse::targetVolumeToCell::combine(topoSet& set, const bool add) const
{
  if (vol_ <= 0)
  {
    // Select no cells
    return;
  }
  PackedBoolList maskSet(mesh_.nCells(), 1);
  label nTotCells = mesh_.globalData().nTotalCells();
  if (maskSetName_.size())
  {
    // Read cellSet
    Info<< "    Operating on subset defined by cellSet " << maskSetName_
      << endl;
    maskSet = 0;
    cellSet subset(mesh_, maskSetName_);
    FOR_ALL_CONST_ITER(cellSet, subset, iter)
    {
      maskSet[iter.key()] = 1;
    }
    nTotCells = returnReduce(subset.size(), sumOp<label>());
  }
  // Get plane for min,max volume.
  // Planes all have base (0 0 0) and fixed normal so work only on normal
  // component.
  scalar maxComp = -GREAT;
  label maxCells = 0;
  //scalar maxVol = 0;
  scalar minComp = GREAT;
  {
    const boundBox& bb = mesh_.bounds();
    pointField points(bb.points());
    //label minPointI = -1;
    label maxPointI = -1;
    FOR_ALL(points, pointI)
    {
      scalar c = (points[pointI]&n_);
      if (c > maxComp)
      {
        maxComp = c;
        maxPointI = pointI;
      }
      else if (c < minComp)
      {
        minComp = c;
        //minPointI = pointI;
      }
    }
    PackedBoolList maxSelected(mesh_.nCells());
    maxCells = selectCells(maxComp, maskSet, maxSelected);
    //maxVol = volumeOfSet(maxSelected);
    // Check that maxPoint indeed selects all cells
    if (maxCells != nTotCells)
    {
      WARNING_IN("targetVolumeToCell::combine(topoSet&, const bool) const")
        << "Plane " << plane(points[maxPointI], n_)
        << " selects " << maxCells
        << " cells instead of all " << nTotCells
        << " cells. Results might be wrong." << endl;
    }
  }
  // Bisection
  // ~~~~~~~~~
  PackedBoolList selected(mesh_.nCells());
  label nSelected = -1;
  scalar selectedVol = 0.0;
  //scalar selectedComp = 0.0;
  scalar low = minComp;
  scalar high = maxComp;
  const scalar tolerance = SMALL*100*(maxComp-minComp);
  while ((high-low) > tolerance)
  {
    scalar mid = 0.5*(low + high);
    nSelected = selectCells(mid, maskSet, selected);
    selectedVol = volumeOfSet(selected);
    //Pout<< "High:" << high << " low:" << low << " mid:" << mid << nl
    //    << "    nSelected:" << nSelected << nl
    //    << "    vol      :" << selectedVol << nl
    //    << endl;
    if (selectedVol < vol_)
    {
      low = mid;
      PackedBoolList highSelected(mesh_.nCells());
      label nHigh = selectCells(high, maskSet, selected);
      if (nSelected == nHigh)
      {
        break;
      }
    }
    else
    {
      high = mid;
      PackedBoolList lowSelected(mesh_.nCells());
      label nLow = selectCells(low, maskSet, selected);
      if (nSelected == nLow)
      {
        break;
      }
    }
  }
  nSelected = selectCells(high, maskSet, selected);
  selectedVol = volumeOfSet(selected);
  if (selectedVol < vol_)
  {
    //selectedComp = high;
  }
  else
  {
    nSelected = selectCells(low, maskSet, selected);
    selectedVol = volumeOfSet(selected);
    if (selectedVol < vol_)
    {
      //selectedComp = low;
    }
    else
    {
      WARNING_IN("targetVolumeToCell::combine(topoSet&, const bool) const")
        << "Did not converge onto plane. " << nl
        << "high plane:"
        << plane(high*n_, n_)
        << nl
        << "low plane :"
        << plane(low*n_, n_)
        << endl;
    }
  }
  Info<< "    Selected " << nSelected << " with actual volume " << selectedVol
    << endl;
  FOR_ALL(selected, cellI)
  {
    if (selected[cellI])
    {
      addOrDelete(set, cellI, add);
    }
  }
}
// Constructors
// Construct from components
mousse::targetVolumeToCell::targetVolumeToCell
(
  const polyMesh& mesh,
  const scalar vol,
  const vector& n
)
:
  topoSetSource(mesh),
  vol_(vol),
  n_(n)
{}
// Construct from dictionary
mousse::targetVolumeToCell::targetVolumeToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  vol_(readScalar(dict.lookup("volume"))),
  n_(dict.lookup("normal")),
  maskSetName_(dict.lookupOrDefault<word>("set", ""))
{}
// Construct from Istream
mousse::targetVolumeToCell::targetVolumeToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  vol_(readScalar(checkIs(is))),
  n_(checkIs(is))
{}

// Destructor
mousse::targetVolumeToCell::~targetVolumeToCell()
{}

// Member Functions
void mousse::targetVolumeToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding cells up to target volume " << vol_
      << " out of total volume " << gSum(mesh_.cellVolumes()) << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing cells up to target volume " << vol_
      << " out of total volume " << gSum(mesh_.cellVolumes()) << endl;
    combine(set, false);
  }
}
