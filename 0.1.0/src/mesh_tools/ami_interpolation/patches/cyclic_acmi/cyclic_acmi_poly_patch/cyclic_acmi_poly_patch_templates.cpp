// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::cyclicACMIPolyPatch::interpolate
(
  const Field<Type>& fldCouple,
  const Field<Type>& fldNonOverlap
) const
{
  // Note: do not scale AMI field as face areas have already been taken into
  // account
  if (owner())
  {
    return
      AMI().interpolateToSource(fldCouple)
     + (1.0 - AMI().srcWeightsSum())*fldNonOverlap;
  }
  else
  {
    return
      neighbPatch().AMI().interpolateToTarget(fldCouple)
     + (1.0 - neighbPatch().AMI().tgtWeightsSum())*fldNonOverlap;
  }
}
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::cyclicACMIPolyPatch::interpolate
(
  const tmp<Field<Type> >& tFldCouple,
  const tmp<Field<Type> >& tFldNonOverlap
) const
{
  return interpolate(tFldCouple(), tFldNonOverlap());
}
template<class Type, class CombineOp>
void mousse::cyclicACMIPolyPatch::interpolate
(
  const UList<Type>& fldCouple,
  const UList<Type>& fldNonOverlap,
  const CombineOp& cop,
  List<Type>& result
) const
{
  // Note: do not scale AMI field as face areas have already been taken into
  // account
  if (owner())
  {
    AMI().interpolateToSource(fldCouple, cop, result);
    result += (1.0 - AMI().srcWeightsSum())*fldNonOverlap;
  }
  else
  {
    neighbPatch().AMI().interpolateToTarget(fldCouple, cop, result);
    result += (1.0 - neighbPatch().AMI().tgtWeightsSum())*fldNonOverlap;
  }
}
