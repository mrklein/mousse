// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::regionModels::regionModel::mapRegionPatchField
(
  const regionModel& nbrRegion,
  const label regionPatchI,
  const label nbrPatchI,
  const Field<Type>& nbrField,
  const bool flip
) const
{
  int oldTag = UPstream::msgType();
  UPstream::msgType() = oldTag + 1;
  const AMIPatchToPatchInterpolation& ami =
    interRegionAMI(nbrRegion, regionPatchI, nbrPatchI, flip);
  tmp<Field<Type> > tresult(ami.interpolateToSource(nbrField));
  UPstream::msgType() = oldTag;
  return tresult;
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::regionModels::regionModel::mapRegionPatchField
(
  const regionModel& nbrRegion,
  const word& fieldName,
  const label regionPatchI,
  const bool flip
) const
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  const fvMesh& nbrRegionMesh = nbrRegion.regionMesh();
  if (nbrRegionMesh.foundObject<fieldType>(fieldName))
  {
    const label nbrPatchI = nbrCoupledPatchID(nbrRegion, regionPatchI);
    int oldTag = UPstream::msgType();
    UPstream::msgType() = oldTag + 1;
    const AMIPatchToPatchInterpolation& ami =
      interRegionAMI(nbrRegion, regionPatchI, nbrPatchI, flip);
    const fieldType& nbrField =
      nbrRegionMesh.lookupObject<fieldType>(fieldName);
    const Field<Type>& nbrFieldp = nbrField.boundaryField()[nbrPatchI];
    tmp<Field<Type> > tresult(ami.interpolateToSource(nbrFieldp));
    UPstream::msgType() = oldTag;
    return tresult;
  }
  else
  {
    const polyPatch& p = regionMesh().boundaryMesh()[regionPatchI];
    return
      tmp<Field<Type> >
      (
        new Field<Type>
        (
          p.size(),
          pTraits<Type>::zero
        )
      );
  }
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::regionModels::regionModel::mapRegionPatchInternalField
(
  const regionModel& nbrRegion,
  const word& fieldName,
  const label regionPatchI,
  const bool flip
) const
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  const fvMesh& nbrRegionMesh = nbrRegion.regionMesh();
  if (nbrRegionMesh.foundObject<fieldType>(fieldName))
  {
    const label nbrPatchI = nbrCoupledPatchID(nbrRegion, regionPatchI);
    int oldTag = UPstream::msgType();
    UPstream::msgType() = oldTag + 1;
    const AMIPatchToPatchInterpolation& ami =
      interRegionAMI(nbrRegion, regionPatchI, nbrPatchI, flip);
    const fieldType& nbrField =
      nbrRegionMesh.lookupObject<fieldType>(fieldName);
    const fvPatchField<Type>& nbrFieldp =
      nbrField.boundaryField()[nbrPatchI];
    tmp<Field<Type> > tresult
    (
      ami.interpolateToSource(nbrFieldp.patchInternalField())
    );
    UPstream::msgType() = oldTag;
    return tresult;
  }
  else
  {
    const polyPatch& p = regionMesh().boundaryMesh()[regionPatchI];
    return
      tmp<Field<Type> >
      (
        new Field<Type>
        (
          p.size(),
          pTraits<Type>::zero
        )
      );
  }
}
template<class Type>
void mousse::regionModels::regionModel::toPrimary
(
  const label regionPatchI,
  List<Type>& regionField
) const
{
  FOR_ALL(intCoupledPatchIDs_, i)
  {
    if (intCoupledPatchIDs_[i] == regionPatchI)
    {
      const mappedPatchBase& mpb =
        refCast<const mappedPatchBase>
        (
          regionMesh().boundaryMesh()[regionPatchI]
        );
      mpb.reverseDistribute(regionField);
      return;
    }
  }
  FATAL_ERROR_IN("const void toPrimary(const label, List<Type>&) const")
    << "Region patch ID " << regionPatchI << " not found in region mesh"
    << abort(FatalError);
}
template<class Type>
void mousse::regionModels::regionModel::toRegion
(
  const label regionPatchI,
  List<Type>& primaryField
) const
{
  FOR_ALL(intCoupledPatchIDs_, i)
  {
    if (intCoupledPatchIDs_[i] == regionPatchI)
    {
      const mappedPatchBase& mpb =
        refCast<const mappedPatchBase>
        (
          regionMesh().boundaryMesh()[regionPatchI]
        );
      mpb.distribute(primaryField);
      return;
    }
  }
  FATAL_ERROR_IN("const void toRegion(const label, List<Type>&) const")
    << "Region patch ID " << regionPatchI << " not found in region mesh"
    << abort(FatalError);
}
template<class Type, class CombineOp>
void mousse::regionModels::regionModel::toPrimary
(
  const label regionPatchI,
  List<Type>& regionField,
  const CombineOp& cop
) const
{
  FOR_ALL(intCoupledPatchIDs_, i)
  {
    if (intCoupledPatchIDs_[i] == regionPatchI)
    {
      const mappedPatchBase& mpb =
        refCast<const mappedPatchBase>
        (
          regionMesh().boundaryMesh()[regionPatchI]
        );
      mpb.reverseDistribute(regionField, cop);
      return;
    }
  }
  FATAL_ERROR_IN
  (
    "const void toPrimary"
    "("
      "const label, "
      "List<Type>&, "
      "const CombineOp&"
    ") const"
  )   << "Region patch ID " << regionPatchI << " not found in region mesh"
    << abort(FatalError);
}
template<class Type, class CombineOp>
void mousse::regionModels::regionModel::toRegion
(
  const label regionPatchI,
  List<Type>& primaryField,
  const CombineOp& cop
) const
{
  FOR_ALL(intCoupledPatchIDs_, i)
  {
    if (intCoupledPatchIDs_[i] == regionPatchI)
    {
      const mappedPatchBase& mpb =
        refCast<const mappedPatchBase>
        (
          regionMesh().boundaryMesh()[regionPatchI]
        );
      mpb.distribute(primaryField, cop);
      return;
    }
  }
  FATAL_ERROR_IN
  (
    "const void toRegion(const label, List<Type>&, const CombineOp&) const"
  )   << "Region patch ID " << regionPatchI << " not found in region mesh"
    << abort(FatalError);
}
