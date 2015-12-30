// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::cyclicAMIPolyPatch::interpolate
(
  const Field<Type>& fld,
  const UList<Type>& defaultValues
) const
{
  if (owner())
  {
    return AMI().interpolateToSource(fld, defaultValues);
  }
  else
  {
    return neighbPatch().AMI().interpolateToTarget(fld, defaultValues);
  }
}
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::cyclicAMIPolyPatch::interpolate
(
  const tmp<Field<Type> >& tFld,
  const UList<Type>& defaultValues
) const
{
  return interpolate(tFld(), defaultValues);
}
template<class Type, class CombineOp>
void mousse::cyclicAMIPolyPatch::interpolate
(
  const UList<Type>& fld,
  const CombineOp& cop,
  List<Type>& result,
  const UList<Type>& defaultValues
) const
{
  if (owner())
  {
    AMI().interpolateToSource
    (
      fld,
      cop,
      result,
      defaultValues
    );
  }
  else
  {
    neighbPatch().AMI().interpolateToTarget
    (
      fld,
      cop,
      result,
      defaultValues
    );
  }
}
