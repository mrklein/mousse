// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "direct_fv_patch_field_mapper.hpp"
#include "calculated_fv_patch_field.hpp"
#include "weighted_fv_patch_field_mapper.hpp"
// Static Data Members
namespace mousse
{
  //- Helper class for list
  template<class Type>
  class ListPlusEqOp
  {
    public:
    void operator()(List<Type>& x, const List<Type> y) const
    {
      if (y.size())
      {
        if (x.size())
        {
          label sz = x.size();
          x.setSize(sz + y.size());
          FOR_ALL(y, i)
          {
            x[sz++] = y[i];
          }
        }
        else
        {
          x = y;
        }
      }
    }
  };
}
// Member Functions 
template<class Type>
void mousse::meshToMesh::add
(
  UList<Type>& fld,
  const label offset
) const
{
  FOR_ALL(fld, i)
  {
    fld[i] += offset;
  }
}
template<class Type, class CombineOp>
void mousse::meshToMesh::mapSrcToTgt
(
  const UList<Type>& srcField,
  const CombineOp& cop,
  List<Type>& result
) const
{
  if (result.size() != tgtToSrcCellAddr_.size())
  {
    FATAL_ERROR_IN
    (
      "void mousse::meshToMesh::mapSrcToTgt"
      "("
        "const UList<Type>&, "
        "const CombineOp&, "
        "List<Type>&"
      ") const"
    )   << "Supplied field size is not equal to target mesh size" << nl
      << "    source mesh    = " << srcToTgtCellAddr_.size() << nl
      << "    target mesh    = " << tgtToSrcCellAddr_.size() << nl
      << "    supplied field = " << result.size()
      << abort(FatalError);
  }
  multiplyWeightedOp<Type, CombineOp> cbop(cop);
  if (singleMeshProc_ == -1)
  {
    const mapDistribute& map = srcMapPtr_();
    List<Type> work(srcField);
    map.distribute(work);
    FOR_ALL(result, cellI)
    {
      const labelList& srcAddress = tgtToSrcCellAddr_[cellI];
      const scalarList& srcWeight = tgtToSrcCellWght_[cellI];
      if (srcAddress.size())
      {
//                result[cellI] = pTraits<Type>::zero;
        result[cellI] *= (1.0 - sum(srcWeight));
        FOR_ALL(srcAddress, i)
        {
          label srcI = srcAddress[i];
          scalar w = srcWeight[i];
          cbop(result[cellI], cellI, work[srcI], w);
        }
      }
    }
  }
  else
  {
    FOR_ALL(result, cellI)
    {
      const labelList& srcAddress = tgtToSrcCellAddr_[cellI];
      const scalarList& srcWeight = tgtToSrcCellWght_[cellI];
      if (srcAddress.size())
      {
//                result[cellI] = pTraits<Type>::zero;
        result[cellI] *= (1.0 - sum(srcWeight));
        FOR_ALL(srcAddress, i)
        {
          label srcI = srcAddress[i];
          scalar w = srcWeight[i];
          cbop(result[cellI], cellI, srcField[srcI], w);
        }
      }
    }
  }
}
template<class Type, class CombineOp>
mousse::tmp<mousse::Field<Type> > mousse::meshToMesh::mapSrcToTgt
(
  const Field<Type>& srcField,
  const CombineOp& cop
) const
{
  tmp<Field<Type> > tresult
  (
    new Field<Type>
    (
      tgtToSrcCellAddr_.size(),
      pTraits<Type>::zero
    )
  );
  mapSrcToTgt(srcField, cop, tresult());
  return tresult;
}
template<class Type, class CombineOp>
mousse::tmp<mousse::Field<Type> > mousse::meshToMesh::mapSrcToTgt
(
  const tmp<Field<Type> >& tsrcField,
  const CombineOp& cop
) const
{
  return mapSrcToTgt(tsrcField(), cop);
}
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::meshToMesh::mapSrcToTgt
(
  const Field<Type>& srcField
) const
{
  return mapSrcToTgt(srcField, plusEqOp<Type>());
}
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::meshToMesh::mapSrcToTgt
(
  const tmp<Field<Type> >& tsrcField
) const
{
  return mapSrcToTgt(tsrcField());
}
template<class Type, class CombineOp>
void mousse::meshToMesh::mapTgtToSrc
(
  const UList<Type>& tgtField,
  const CombineOp& cop,
  List<Type>& result
) const
{
  if (result.size() != srcToTgtCellAddr_.size())
  {
    FATAL_ERROR_IN
    (
      "void mousse::meshToMesh::mapTgtToSrc"
      "("
        "const UList<Type>&, "
        "const CombineOp&, "
        "List<Type>&"
      ") const"
    )   << "Supplied field size is not equal to source mesh size" << nl
      << "    source mesh    = " << srcToTgtCellAddr_.size() << nl
      << "    target mesh    = " << tgtToSrcCellAddr_.size() << nl
      << "    supplied field = " << result.size()
      << abort(FatalError);
  }
  multiplyWeightedOp<Type, CombineOp> cbop(cop);
  if (singleMeshProc_ == -1)
  {
    const mapDistribute& map = tgtMapPtr_();
    List<Type> work(tgtField);
    map.distribute(work);
    FOR_ALL(result, cellI)
    {
      const labelList& tgtAddress = srcToTgtCellAddr_[cellI];
      const scalarList& tgtWeight = srcToTgtCellWght_[cellI];
      if (tgtAddress.size())
      {
        result[cellI] *= (1.0 - sum(tgtWeight));
        FOR_ALL(tgtAddress, i)
        {
          label tgtI = tgtAddress[i];
          scalar w = tgtWeight[i];
          cbop(result[cellI], cellI, work[tgtI], w);
        }
      }
    }
  }
  else
  {
    FOR_ALL(result, cellI)
    {
      const labelList& tgtAddress = srcToTgtCellAddr_[cellI];
      const scalarList& tgtWeight = srcToTgtCellWght_[cellI];
      if (tgtAddress.size())
      {
        result[cellI] *= (1.0 - sum(tgtWeight));
        FOR_ALL(tgtAddress, i)
        {
          label tgtI = tgtAddress[i];
          scalar w = tgtWeight[i];
          cbop(result[cellI], cellI, tgtField[tgtI], w);
        }
      }
    }
  }
}
template<class Type, class CombineOp>
mousse::tmp<mousse::Field<Type> > mousse::meshToMesh::mapTgtToSrc
(
  const Field<Type>& tgtField,
  const CombineOp& cop
) const
{
  tmp<Field<Type> > tresult
  (
    new Field<Type>
    (
      srcToTgtCellAddr_.size(),
      pTraits<Type>::zero
    )
  );
  mapTgtToSrc(tgtField, cop, tresult());
  return tresult;
}
template<class Type, class CombineOp>
mousse::tmp<mousse::Field<Type> > mousse::meshToMesh::mapTgtToSrc
(
  const tmp<Field<Type> >& ttgtField,
  const CombineOp& cop
) const
{
  return mapTgtToSrc(ttgtField(), cop);
}
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::meshToMesh::mapTgtToSrc
(
  const Field<Type>& tgtField
) const
{
  return mapTgtToSrc(tgtField, plusEqOp<Type>());
}
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::meshToMesh::mapTgtToSrc
(
  const tmp<Field<Type> >& ttgtField
) const
{
  return mapTgtToSrc(ttgtField(), plusEqOp<Type>());
}
template<class Type, class CombineOp>
void mousse::meshToMesh::mapSrcToTgt
(
  const GeometricField<Type, fvPatchField, volMesh>& field,
  const CombineOp& cop,
  GeometricField<Type, fvPatchField, volMesh>& result
) const
{
  mapSrcToTgt(field, cop, result.internalField());
  const PtrList<AMIPatchToPatchInterpolation>& AMIList = patchAMIs();
  FOR_ALL(AMIList, i)
  {
    label srcPatchI = srcPatchID_[i];
    label tgtPatchI = tgtPatchID_[i];
    const fvPatchField<Type>& srcField = field.boundaryField()[srcPatchI];
    fvPatchField<Type>& tgtField = result.boundaryField()[tgtPatchI];
    // 2.3 does not do distributed mapping yet so only do if
    // running on single processor
    if (AMIList[i].singlePatchProc() != -1)
    {
      // Clone and map (since rmap does not do general mapping)
      tmp<fvPatchField<Type> > tnewTgt
      (
        fvPatchField<Type>::New
        (
          srcField,
          tgtField.patch(),
          result.dimensionedInternalField(),
          weightedFvPatchFieldMapper
          (
            AMIList[i].tgtAddress(),
            AMIList[i].tgtWeights()
          )
        )
      );
      // Transfer all mapped quantities (value and e.g. gradient) onto
      // tgtField. Value will get overwritten below.
      tgtField.rmap(tnewTgt(), identity(tgtField.size()));
    }
    tgtField == pTraits<Type>::zero;
    AMIList[i].interpolateToTarget
    (
      srcField,
      multiplyWeightedOp<Type, CombineOp>(cop),
      tgtField,
      UList<Type>::null()
    );
  }
  FOR_ALL(cuttingPatches_, i)
  {
    label patchI = cuttingPatches_[i];
    fvPatchField<Type>& pf = result.boundaryField()[patchI];
    pf == pf.patchInternalField();
  }
}
template<class Type, class CombineOp>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::meshToMesh::mapSrcToTgt
(
  const GeometricField<Type, fvPatchField, volMesh>& field,
  const CombineOp& cop
) const
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  const fvMesh& tgtMesh = static_cast<const fvMesh&>(tgtRegion_);
  const fvBoundaryMesh& tgtBm = tgtMesh.boundary();
  const typename fieldType::GeometricBoundaryField& srcBfld =
    field.boundaryField();
  PtrList<fvPatchField<Type> > tgtPatchFields(tgtBm.size());
  // constuct tgt boundary patch types as copy of 'field' boundary types
  // note: this will provide place holders for fields with additional
  // entries, but these values will need to be reset
  FOR_ALL(tgtPatchID_, i)
  {
    label srcPatchI = srcPatchID_[i];
    label tgtPatchI = tgtPatchID_[i];
    if (!tgtPatchFields.set(tgtPatchI))
    {
      tgtPatchFields.set
      (
        tgtPatchI,
        fvPatchField<Type>::New
        (
          srcBfld[srcPatchI],
          tgtMesh.boundary()[tgtPatchI],
          DimensionedField<Type, volMesh>::null(),
          directFvPatchFieldMapper
          (
            labelList(tgtMesh.boundary()[tgtPatchI].size(), -1)
          )
        )
      );
    }
  }
  // Any unset tgtPatchFields become calculated
  FOR_ALL(tgtPatchFields, tgtPatchI)
  {
    if (!tgtPatchFields.set(tgtPatchI))
    {
      // Note: use factory New method instead of direct generation of
      //       calculated so we keep constraints
      tgtPatchFields.set
      (
        tgtPatchI,
        fvPatchField<Type>::New
        (
          calculatedFvPatchField<Type>::typeName,
          tgtMesh.boundary()[tgtPatchI],
          DimensionedField<Type, volMesh>::null()
        )
      );
    }
  }
  tmp<fieldType> tresult
  (
    new fieldType
    (
      IOobject
      (
        type() + ":interpolate(" + field.name() + ")",
        tgtMesh.time().timeName(),
        tgtMesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      tgtMesh,
      field.dimensions(),
      Field<Type>(tgtMesh.nCells(), pTraits<Type>::zero),
      tgtPatchFields
    )
  );
  mapSrcToTgt(field, cop, tresult());
  return tresult;
}
template<class Type, class CombineOp>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::meshToMesh::mapSrcToTgt
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfield,
  const CombineOp& cop
) const
{
  return mapSrcToTgt(tfield(), cop);
}
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::meshToMesh::mapSrcToTgt
(
  const GeometricField<Type, fvPatchField, volMesh>& field
) const
{
  return mapSrcToTgt(field, plusEqOp<Type>());
}
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::meshToMesh::mapSrcToTgt
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfield
) const
{
  return mapSrcToTgt(tfield(), plusEqOp<Type>());
}
template<class Type, class CombineOp>
void mousse::meshToMesh::mapTgtToSrc
(
  const GeometricField<Type, fvPatchField, volMesh>& field,
  const CombineOp& cop,
  GeometricField<Type, fvPatchField, volMesh>& result
) const
{
  mapTgtToSrc(field, cop, result.internalField());
  const PtrList<AMIPatchToPatchInterpolation>& AMIList = patchAMIs();
  FOR_ALL(AMIList, i)
  {
    label srcPatchI = srcPatchID_[i];
    label tgtPatchI = tgtPatchID_[i];
    fvPatchField<Type>& srcField = result.boundaryField()[srcPatchI];
    const fvPatchField<Type>& tgtField = field.boundaryField()[tgtPatchI];
    // 2.3 does not do distributed mapping yet so only do if
    // running on single processor
    if (AMIList[i].singlePatchProc() != -1)
    {
      // Clone and map (since rmap does not do general mapping)
      tmp<fvPatchField<Type> > tnewSrc
      (
        fvPatchField<Type>::New
        (
          tgtField,
          srcField.patch(),
          result.dimensionedInternalField(),
          weightedFvPatchFieldMapper
          (
            AMIList[i].srcAddress(),
            AMIList[i].srcWeights()
          )
        )
      );
      // Transfer all mapped quantities (value and e.g. gradient) onto
      // srcField. Value will get overwritten below
      srcField.rmap(tnewSrc(), identity(srcField.size()));
    }
    srcField == pTraits<Type>::zero;
    AMIList[i].interpolateToSource
    (
      tgtField,
      multiplyWeightedOp<Type, CombineOp>(cop),
      srcField,
      UList<Type>::null()
    );
  }
  FOR_ALL(cuttingPatches_, i)
  {
    label patchI = cuttingPatches_[i];
    fvPatchField<Type>& pf = result.boundaryField()[patchI];
    pf == pf.patchInternalField();
  }
}
template<class Type, class CombineOp>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::meshToMesh::mapTgtToSrc
(
  const GeometricField<Type, fvPatchField, volMesh>& field,
  const CombineOp& cop
) const
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  const fvMesh& srcMesh = static_cast<const fvMesh&>(srcRegion_);
  const fvBoundaryMesh& srcBm = srcMesh.boundary();
  const typename fieldType::GeometricBoundaryField& tgtBfld =
    field.boundaryField();
  PtrList<fvPatchField<Type> > srcPatchFields(srcBm.size());
  // constuct src boundary patch types as copy of 'field' boundary types
  // note: this will provide place holders for fields with additional
  // entries, but these values will need to be reset
  FOR_ALL(srcPatchID_, i)
  {
    label srcPatchI = srcPatchID_[i];
    label tgtPatchI = tgtPatchID_[i];
    if (!srcPatchFields.set(tgtPatchI))
    {
      srcPatchFields.set
      (
        srcPatchI,
        fvPatchField<Type>::New
        (
          tgtBfld[srcPatchI],
          srcMesh.boundary()[tgtPatchI],
          DimensionedField<Type, volMesh>::null(),
          directFvPatchFieldMapper
          (
            labelList(srcMesh.boundary()[srcPatchI].size(), -1)
          )
        )
      );
    }
  }
  // Any unset srcPatchFields become calculated
  FOR_ALL(srcPatchFields, srcPatchI)
  {
    if (!srcPatchFields.set(srcPatchI))
    {
      // Note: use factory New method instead of direct generation of
      //       calculated so we keep constraints
      srcPatchFields.set
      (
        srcPatchI,
        fvPatchField<Type>::New
        (
          calculatedFvPatchField<Type>::typeName,
          srcMesh.boundary()[srcPatchI],
          DimensionedField<Type, volMesh>::null()
        )
      );
    }
  }
  tmp<fieldType> tresult
  (
    new fieldType
    (
      IOobject
      (
        type() + ":interpolate(" + field.name() + ")",
        srcMesh.time().timeName(),
        srcMesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      srcMesh,
      field.dimensions(),
      Field<Type>(srcMesh.nCells(), pTraits<Type>::zero),
      srcPatchFields
    )
  );
  mapTgtToSrc(field, cop, tresult());
  return tresult;
}
template<class Type, class CombineOp>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::meshToMesh::mapTgtToSrc
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfield,
  const CombineOp& cop
) const
{
  return mapTgtToSrc(tfield(), cop);
}
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::meshToMesh::mapTgtToSrc
(
  const GeometricField<Type, fvPatchField, volMesh>& field
) const
{
  return mapTgtToSrc(field, plusEqOp<Type>());
}
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::meshToMesh::mapTgtToSrc
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfield
) const
{
  return mapTgtToSrc(tfield(), plusEqOp<Type>());
}
