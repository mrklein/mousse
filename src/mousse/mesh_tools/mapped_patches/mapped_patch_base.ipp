// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
void mousse::mappedPatchBase::distribute(List<Type>& lst) const
{
  switch (mode_) {
    case NEARESTPATCHFACEAMI:
    {
      lst = AMI().interpolateToSource(Field<Type>(lst.xfer()));
      break;
    }
    default:
    {
      map().distribute(lst);
    }
  }
}


template<class Type, class CombineOp>
void mousse::mappedPatchBase::distribute
(
  List<Type>& lst,
  const CombineOp& cop
) const
{
  switch (mode_) {
    case NEARESTPATCHFACEAMI:
    {
      lst = AMI().interpolateToSource
        (
          Field<Type>(lst.xfer()),
          cop
        );
      break;
    }
    default:
    {
      map().distribute
      (
        Pstream::defaultCommsType,
        map().schedule(),
        map().constructSize(),
        map().subMap(),
        map().constructMap(),
        lst,
        cop,
        pTraits<Type>::zero
      );
    }
  }
}


template<class Type>
void mousse::mappedPatchBase::reverseDistribute(List<Type>& lst) const
{
  switch (mode_) {
    case NEARESTPATCHFACEAMI:
    {
      lst = AMI().interpolateToTarget(Field<Type>(lst.xfer()));
      break;
    }
    default:
    {
      map().reverseDistribute(sampleSize(), lst);
      break;
    }
  }
}


template<class Type, class CombineOp>
void mousse::mappedPatchBase::reverseDistribute
(
  List<Type>& lst,
  const CombineOp& cop
) const
{
  switch (mode_) {
    case NEARESTPATCHFACEAMI:
    {
      lst = AMI().interpolateToTarget
        (
          Field<Type>(lst.xfer()),
          cop
        );
      break;
    }
    default:
    {
      label cSize = sampleSize();
      map().distribute
      (
        Pstream::defaultCommsType,
        map().schedule(),
        cSize,
        map().constructMap(),
        map().subMap(),
        lst,
        cop,
        pTraits<Type>::zero
      );
      break;
    }
  }
}
