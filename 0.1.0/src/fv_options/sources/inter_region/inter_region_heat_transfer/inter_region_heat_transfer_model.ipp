// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::fv::interRegionHeatTransferModel::interpolate
(
  const interRegionHeatTransferModel& nbrModel,
  const Field<Type>& field
) const
{
  if (master_) {
    return meshInterp().mapTgtToSrc(field);
  } else {
    return (nbrModel.meshInterp().mapSrcToTgt(field));
  }
}


template<class Type>
mousse::tmp<mousse::Field<Type>>
mousse::fv::interRegionHeatTransferModel::interpolate
(
  const Field<Type>& field
) const
{
  return interpolate(nbrModel(), field);
}


template<class Type>
void mousse::fv::interRegionHeatTransferModel::interpolate
(
  const interRegionHeatTransferModel& nbrModel,
  const Field<Type>& field,
  Field<Type>& result
) const
{
  if (master_) {
    meshInterp().mapTgtToSrc(field, plusEqOp<scalar>(), result);
  } else {
    nbrModel.meshInterp().mapSrcToTgt(field, plusEqOp<scalar>(), result);
  }
}


template<class Type>
void mousse::fv::interRegionHeatTransferModel::interpolate
(
  const Field<Type>& field,
  Field<Type>& result
) const
{
  return interpolate(nbrModel(), field, result);
}

