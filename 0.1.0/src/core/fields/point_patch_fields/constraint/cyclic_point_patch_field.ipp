// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_point_patch_field.hpp"
#include "swap.hpp"
#include "transform_field.hpp"
#include "point_fields.hpp"


// Constructors 
template<class Type>
mousse::cyclicPointPatchField<Type>::cyclicPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF
)
:
  coupledPointPatchField<Type>{p, iF},
  cyclicPatch_{refCast<const cyclicPointPatch>(p)}
{}


template<class Type>
mousse::cyclicPointPatchField<Type>::cyclicPointPatchField
(
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const dictionary& dict
)
:
  coupledPointPatchField<Type>{p, iF, dict},
  cyclicPatch_{refCast<const cyclicPointPatch>(p)}
{
  if (!isType<cyclicPointPatch>(p)) {
    FATAL_IO_ERROR_IN
    (
      "cyclicPointPatchField<Type>::cyclicPointPatchField\n"
      "(\n"
      "    const pointPatch& p,\n"
      "    const Field<Type>& field,\n"
      "    const dictionary& dict\n"
      ")\n",
      dict
    )
    << "patch " << this->patch().index() << " not cyclic type. "
    << "Patch type = " << p.type()
    << exit(FatalIOError);
  }
}


template<class Type>
mousse::cyclicPointPatchField<Type>::cyclicPointPatchField
(
  const cyclicPointPatchField<Type>& ptf,
  const pointPatch& p,
  const DimensionedField<Type, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  coupledPointPatchField<Type>{ptf, p, iF, mapper},
  cyclicPatch_{refCast<const cyclicPointPatch>(p)}
{
  if (!isType<cyclicPointPatch>(this->patch()))
  {
    FATAL_ERROR_IN
    (
      "cyclicPointPatchField<Type>::cyclicPointPatchField\n"
      "(\n"
      "    const cyclicPointPatchField<Type>& ptf,\n"
      "    const pointPatch& p,\n"
      "    const DimensionedField<Type, pointMesh>& iF,\n"
      "    const pointPatchFieldMapper& mapper\n"
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
mousse::cyclicPointPatchField<Type>::cyclicPointPatchField
(
  const cyclicPointPatchField<Type>& ptf,
  const DimensionedField<Type, pointMesh>& iF
)
:
  coupledPointPatchField<Type>{ptf, iF},
  cyclicPatch_{ptf.cyclicPatch_}
{}


// Member Functions 
template<class Type>
void mousse::cyclicPointPatchField<Type>::swapAddSeparated
(
  const Pstream::commsTypes,
  Field<Type>& pField
) const
{
  // Get neighbouring pointPatch
  const cyclicPointPatch& nbrPatch = cyclicPatch_.neighbPatch();
  if (cyclicPatch_.cyclicPatch().owner()) {
    // We inplace modify pField. To prevent the other side (which gets
    // evaluated at a later date) using already changed values we do
    // all swaps on the side that gets evaluated first.
    // Get neighbouring pointPatchField
    const GeometricField<Type, pointPatchField, pointMesh>& fld =
      refCast<const GeometricField<Type, pointPatchField, pointMesh>>
      (
        this->dimensionedInternalField()
      );
    const cyclicPointPatchField<Type>& nbr =
      refCast<const cyclicPointPatchField<Type>>
      (
        fld.boundaryField()[nbrPatch.index()]
      );
    Field<Type> pf{this->patchInternalField(pField)};
    Field<Type> nbrPf{nbr.patchInternalField(pField)};
    const edgeList& pairs = cyclicPatch_.transformPairs();
    if (doTransform()) {
      // Transform both sides.
      FOR_ALL(pairs, pairi) {
        label pointi = pairs[pairi][0];
        label nbrPointi = pairs[pairi][1];
        Type tmp = pf[pointi];
        pf[pointi] = transform(forwardT()[0], nbrPf[nbrPointi]);
        nbrPf[nbrPointi] = transform(reverseT()[0], tmp);
      }
    } else {
      FOR_ALL(pairs, pairi) {
        Swap(pf[pairs[pairi][0]], nbrPf[pairs[pairi][1]]);
      }
    }
    this->addToInternalField(pField, pf);
    nbr.addToInternalField(pField, nbrPf);
  }
}
