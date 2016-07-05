// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "actuation_disk_source.hpp"
#include "vol_fields.hpp"


//  Member Functions
template<class RhoFieldType>
void mousse::fv::actuationDiskSource::addActuationDiskAxialInertialResistance
(
  vectorField& Usource,
  const labelList& cells,
  const scalarField& Vcells,
  const RhoFieldType& rho,
  const vectorField& U
) const
{
  scalar a = 1.0 - Cp_/Ct_;
  vector uniDiskDir = diskDir_/mag(diskDir_);
  tensor E{tensor::zero};
  E.xx() = uniDiskDir.x();
  E.yy() = uniDiskDir.y();
  E.zz() = uniDiskDir.z();
  vector upU = vector(VGREAT, VGREAT, VGREAT);
  scalar upRho = VGREAT;
  if (upstreamCellId_ != -1) {
    upU =  U[upstreamCellId_];
    upRho = rho[upstreamCellId_];
  }
  reduce(upU, minOp<vector>());
  reduce(upRho, minOp<scalar>());
  scalar T = 2.0*upRho*diskArea_*mag(upU)*a*(1 - a);
  FOR_ALL(cells, i) {
    Usource[cells[i]] += ((Vcells[cells[i]]/V())*T*E) & upU;
  }
}

