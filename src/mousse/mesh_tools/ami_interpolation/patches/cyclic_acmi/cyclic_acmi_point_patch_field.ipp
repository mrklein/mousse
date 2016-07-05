// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_acmi_point_patch_field.hpp"
#include "swap.hpp"
#include "transform_field.hpp"
#include "point_fields.hpp"


// Constructors 
template<class Type>
mousse::cyclicACMIPointPatchField<Type>::cyclicACMIPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  coupledPointPatchField<Type>{p, iF},
  cyclicACMIPatch_{refCast<const cyclicACMIPointPatch>(p)},
  ppiPtr_{NULL},
  nbrPpiPtr_{NULL}
{}


template<class Type>
mousse::cyclicACMIPointPatchField<Type>::cyclicACMIPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  coupledPointPatchField<Type>{p, iF, dict},
  cyclicACMIPatch_{refCast<const cyclicACMIPointPatch>(p)},
  ppiPtr_{NULL},
  nbrPpiPtr_{NULL}
{
  if (!isType<cyclicACMIPointPatch>(p)) {
    FATAL_IO_ERROR_IN
    (
      "cyclicACMIPointPatchField<Type>::cyclicACMIPointPatchField\n"
      "(\n"
      "    const pointPatch&,\n"
      "    const DimensionedField<Type, pointMesh>&,\n"
      "    const dictionary&\n"
      ")\n",
      dict
    )
    << "patch " << this->patch().index() << " not cyclicACMI type. "
    << "Patch type = " << p.type()
    << exit(FatalIOError);
  }
}


template<class Type>
mousse::cyclicACMIPointPatchField<Type>::cyclicACMIPointPatchField
(
  const cyclicACMIPointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  coupledPointPatchField<Type>{ptf, p, iF, mapper},
  cyclicACMIPatch_{refCast<const cyclicACMIPointPatch>(p)},
  ppiPtr_{NULL},
  nbrPpiPtr_{NULL}
{
  if (!isType<cyclicACMIPointPatch>(this->patch())) {
    FATAL_ERROR_IN
    (
      "cyclicACMIPointPatchField<Type>::cyclicACMIPointPatchField\n"
      "(\n"
      "    const cyclicACMIPointPatchField<Type>&,\n"
      "    const pointPatch&,\n"
      "    const DimensionedField<Type, pointMesh>&,\n"
      "    const pointPatchFieldMapper&\n"
      ")\n"
    )
    << "Field type does not correspond to patch type for patch "
    << this->patch().index() << "." << endl
    << "Field type: " << typeName << endl
    << "Patch type: " << this->patch().type()
    << exit(FatalError);
  }
}


template<class Type>
mousse::cyclicACMIPointPatchField<Type>::cyclicACMIPointPatchField
(
  const cyclicACMIPointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  coupledPointPatchField<Type>{ptf, iF},
  cyclicACMIPatch_{ptf.cyclicACMIPatch_},
  ppiPtr_{NULL},
  nbrPpiPtr_{NULL}
{}


// Member Functions 
template<class Type>
void mousse::cyclicACMIPointPatchField<Type>::swapAddSeparated
(
  const Pstream::commsTypes,
  Field<Type>& pField
) const
{
  if (!cyclicACMIPatch_.cyclicACMIPatch().owner())
    return;
  // We inplace modify pField. To prevent the other side (which gets
  // evaluated at a later date) using already changed values we do
  // all swaps on the side that gets evaluated first.
  // Get neighbouring pointPatch
  const cyclicACMIPointPatch& nbrPatch = cyclicACMIPatch_.neighbPatch();
  // Get neighbouring pointPatchField
  const GeometricField<Type, pointPatchField, pointMesh>& fld =
    refCast<const GeometricField<Type, pointPatchField, pointMesh> >
    (
      this->dimensionedInternalField()
    );
  const cyclicACMIPointPatchField<Type>& nbr =
    refCast<const cyclicACMIPointPatchField<Type> >
    (
      fld.boundaryField()[nbrPatch.index()]
    );
  Field<Type> ptFld{this->patchInternalField(pField)};
  Field<Type> nbrPtFld{nbr.patchInternalField(pField)};
  if (doTransform()) {
    const tensor& forwardT = this->forwardT()[0];
    const tensor& reverseT = this->reverseT()[0];
    transform(ptFld, reverseT, ptFld);
    transform(nbrPtFld, forwardT, nbrPtFld);
  }

  // convert point field to face field, AMI interpolate, then
  // face back to point
  {
    // add neighbour side contribution to owner
    Field<Type> nbrFcFld{nbrPpi().pointToFaceInterpolate(nbrPtFld)};
    const cyclicAMIPolyPatch& cami = cyclicACMIPatch_.cyclicACMIPatch();
    // interpolate to owner
    nbrFcFld = cami.interpolate(nbrFcFld);
    // add to internal field
    this->addToInternalField
    (
      pField,
      ppi().faceToPointInterpolate(nbrFcFld)()
    );
  }

  {
    // add owner side contribution to neighbour
    Field<Type> fcFld{ppi().pointToFaceInterpolate(ptFld)};
    const cyclicAMIPolyPatch& cami = cyclicACMIPatch_.cyclicACMIPatch();
    // interpolate to neighbour
    fcFld = cami.neighbPatch().cyclicAMIPolyPatch::interpolate(fcFld);
    // add to internal field
    nbr.addToInternalField
    (
      pField,
      nbrPpi().faceToPointInterpolate(fcFld)()
    );
  }
}

