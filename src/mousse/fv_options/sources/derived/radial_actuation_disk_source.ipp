// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "radial_actuation_disk_source.hpp"
#include "vol_fields.hpp"
#include "fv_matrix.hpp"
#include "fvm.hpp"
#include "mathematical_constants.hpp"


//  Member Functions
template<class RhoFieldType>
void mousse::fv::radialActuationDiskSource::
addRadialActuationDiskAxialInertialResistance
(
  vectorField& Usource,
  const labelList& cells,
  const scalarField& Vcells,
  const RhoFieldType& rho,
  const vectorField& U
) const
{
  scalar a = 1.0 - Cp_/Ct_;
  scalarField Tr{cells.size()};
  const vector uniDiskDir = diskDir_/mag(diskDir_);
  tensor E{tensor::zero};
  E.xx() = uniDiskDir.x();
  E.yy() = uniDiskDir.y();
  E.zz() = uniDiskDir.z();
  const Field<vector> zoneCellCentres{mesh().cellCentres(), cells};
  const Field<scalar> zoneCellVolumes{mesh().cellVolumes(), cells};
  const vector avgCentre = gSum(zoneCellVolumes*zoneCellCentres)/V();
  const scalar maxR = gMax(mag(zoneCellCentres - avgCentre));
  scalar intCoeffs =
    radialCoeffs_[0] + radialCoeffs_[1]*sqr(maxR)/2.0
    + radialCoeffs_[2]*pow4(maxR)/3.0;
  vector upU = vector{VGREAT, VGREAT, VGREAT};
  scalar upRho = VGREAT;
  if (upstreamCellId_ != -1) {
    upU =  U[upstreamCellId_];
    upRho = rho[upstreamCellId_];
  }
  reduce(upU, minOp<vector>());
  reduce(upRho, minOp<scalar>());
  scalar T = 2.0*upRho*diskArea_*mag(upU)*a*(1.0 - a);
  FOR_ALL(cells, i) {
    scalar r2 = magSqr(mesh().cellCentres()[cells[i]] - avgCentre);
    Tr[i] =
      T
      *(radialCoeffs_[0] + radialCoeffs_[1]*r2 + radialCoeffs_[2]*sqr(r2))
      /intCoeffs;
    Usource[cells[i]] += ((Vcells[cells[i]]/V_)*Tr[i]*E) & upU;
  }
  if (debug) {
    Info << "Source name: " << name() << nl
      << "Average centre: " << avgCentre << nl
      << "Maximum radius: " << maxR << endl;
  }
}

