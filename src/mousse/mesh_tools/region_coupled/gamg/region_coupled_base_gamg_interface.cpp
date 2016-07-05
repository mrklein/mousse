// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ami_interpolation.hpp"
#include "region_coupled_base_gamg_interface.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "map.hpp"
#include "poly_mesh.hpp"


// Constructors 
mousse::regionCoupledBaseGAMGInterface::regionCoupledBaseGAMGInterface
(
  const label index,
  const lduInterfacePtrsList& coarseInterfaces,
  const lduInterface& fineInterface,
  const labelField& localRestrictAddressing,
  const labelField& /*neighbourRestrictAddressing*/,
  const label /*fineLevelIndex*/,
  const label /*coarseComm*/
)
:
  GAMGInterface
  {
    index,
    coarseInterfaces
  },
  fineRegionCoupledLduInterface_
  {
    refCast<const regionCoupledLduInterface>(fineInterface)
  }
{
  // Construct face agglomeration from cell agglomeration
  {
    // From coarse face to cell
    DynamicList<label> dynFaceCells{localRestrictAddressing.size()};
    // From face to coarse face
    DynamicList<label> dynFaceRestrictAddressing
    {
      localRestrictAddressing.size()
    };
    Map<label> masterToCoarseFace{localRestrictAddressing.size()};
    FOR_ALL(localRestrictAddressing, ffi) {
      label curMaster = localRestrictAddressing[ffi];
      Map<label>::const_iterator fnd = masterToCoarseFace.find
      (
        curMaster
      );
      if (fnd == masterToCoarseFace.end()) {
        // New coarse face
        label coarseI = dynFaceCells.size();
        dynFaceRestrictAddressing.append(coarseI);
        dynFaceCells.append(curMaster);
        masterToCoarseFace.insert(curMaster, coarseI);
      } else {
        // Already have coarse face
        dynFaceRestrictAddressing.append(fnd());
      }
    }
    faceCells_.transfer(dynFaceCells);
    faceRestrictAddressing_.transfer(dynFaceRestrictAddressing);
  }
}


// Destructor
mousse::regionCoupledBaseGAMGInterface::~regionCoupledBaseGAMGInterface()
{}


// Member Functions 
mousse::tmp<mousse::labelField> mousse::regionCoupledBaseGAMGInterface::
internalFieldTransfer
(
  const Pstream::commsTypes,
  const labelUList& iF
) const
{
  tmp<labelField> tpnf{new labelField{iF}};
  return tpnf;
}

