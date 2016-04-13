// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cylindrical.hpp"
#include "axes_rotation.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "poly_mesh.hpp"
#include "tensor_io_field.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cylindrical, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  coordinateRotation,
  cylindrical,
  dictionary
);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  coordinateRotation,
  cylindrical,
  objectRegistry
);

}


// Private Member Functions
void mousse::cylindrical::init
(
  const objectRegistry& obr,
  const List<label>& cells
)
{
  const polyMesh& mesh = refCast<const polyMesh>(obr);
  const vectorField& cc = mesh.cellCentres();
  if (cells.size()) {
    Rptr_.reset(new tensorField{cells.size()});
    tensorField& R = Rptr_();
    FOR_ALL(cells, i) {
      label cellI = cells[i];
      vector dir = cc[cellI] - origin_;
      dir /= mag(dir) + VSMALL;
      R[i] = axesRotation(e3_, dir).R();
    }
  } else {
    Rptr_.reset(new tensorField{mesh.nCells()});
    tensorField& R = Rptr_();
    FOR_ALL(cc, cellI) {
      vector dir = cc[cellI] - origin_;
      dir /= mag(dir) + VSMALL;
      R[cellI] = axesRotation(e3_, dir).R();
    }
  }
}


// Constructors
mousse::cylindrical::cylindrical
(
  const dictionary& dict,
  const objectRegistry& obr
)
:
  Rptr_{},
  origin_{point::zero},
  e3_{vector::zero}
{
  // If origin is specified in the coordinateSystem
  if (dict.parent().found("origin")) {
    dict.parent().lookup("origin") >> origin_;
  }
  // rotation axis
  dict.lookup("e3") >> e3_;
  init(obr);
}


mousse::cylindrical::cylindrical
(
  const objectRegistry& obr,
  const vector& axis,
  const point& origin
)
:
  Rptr_{},
  origin_{origin},
  e3_{axis}
{
  init(obr);
}


mousse::cylindrical::cylindrical
(
  const objectRegistry& obr,
  const vector& axis,
  const point& origin,
  const List<label>& cells
)
:
  Rptr_{},
  origin_{origin},
  e3_{axis}
{
  init(obr, cells);
}


mousse::cylindrical::cylindrical(const dictionary&)
:
  Rptr_{},
  origin_{},
  e3_{}
{
  FATAL_ERROR_IN("cylindrical(const dictionary&)")
    << " cylindrical can not be constructed from dictionary "
    << " use the construtctor : "
       "("
       "  const dictionary&, const objectRegistry&"
       ")"
    << exit(FatalIOError);
}


mousse::cylindrical::cylindrical(const tensorField& R)
:
  Rptr_{},
  origin_{vector::zero},
  e3_{vector::zero}
{
  Rptr_() = R;
}


// Member Functions
void mousse::cylindrical::clear()
{
  if (!Rptr_.empty()) {
    Rptr_.clear();
  }
}


void mousse::cylindrical::updateCells
(
  const polyMesh& mesh,
  const labelList& cells
)
{
  const vectorField& cc = mesh.cellCentres();
  tensorField& R = Rptr_();
  FOR_ALL(cells, i) {
    label cellI = cells[i];
    vector dir = cc[cellI] - origin_;
    dir /= mag(dir) + VSMALL;
    R[cellI] = axesRotation(e3_, dir).R();
  }
}


mousse::tmp<mousse::vectorField> mousse::cylindrical::transform
(
  const vectorField& vf
) const
{
  if (Rptr_->size() != vf.size()) {
    FATAL_ERROR_IN
    (
      "tmp<vectorField> cylindrical::transform(const vectorField&)"
    )
    << "vectorField st has different size to tensorField "
    << abort(FatalError);
  }
  return (Rptr_() & vf);
}


mousse::vector mousse::cylindrical::transform(const vector&) const
{
  NOT_IMPLEMENTED
  (
    "vector cylindrical::transform(const vector&) const"
  );
  return vector::zero;
}


mousse::vector mousse::cylindrical::transform
(
  const vector& v,
  const label cmptI
) const
{
  return (Rptr_()[cmptI] & v);
}


mousse::tmp<mousse::vectorField> mousse::cylindrical::invTransform
(
  const vectorField& vf
) const
{
  return (Rptr_().T() & vf);
}


mousse::vector mousse::cylindrical::invTransform(const vector&) const
{
  NOT_IMPLEMENTED
  (
    "vector cylindrical::invTransform(const vector&) const"
  );
  return vector::zero;
}


mousse::vector mousse::cylindrical::invTransform
(
  const vector& v,
  const label cmptI
) const
{
  return (Rptr_()[cmptI].T() & v);
}


mousse::tmp<mousse::tensorField> mousse::cylindrical::transformTensor
(
  const tensorField& tf
) const
{
  if (Rptr_->size() != tf.size())
  {
    FATAL_ERROR_IN
    (
      "tmp<tensorField> cylindrical::transformTensor"
      "("
        "const tensorField&"
      ")"
    )
    << "tensorField st has different size to tensorField Tr"
    << abort(FatalError);
  }
  return (Rptr_() & tf & Rptr_().T());
}


mousse::tensor mousse::cylindrical::transformTensor
(
  const tensor&
) const
{
  NOT_IMPLEMENTED
  (
    "tensor cylindrical::transformTensor(const tensor&) const"
  );
  return tensor::zero;
}


mousse::tmp<mousse::tensorField> mousse::cylindrical::transformTensor
(
  const tensorField& tf,
  const labelList& cellMap
) const
{
  if (cellMap.size() != tf.size())
  {
    FATAL_ERROR_IN
    (
      "tmp<tensorField> cylindrical::transformTensor"
      "("
        "const tensorField&, "
        "const labelList&"
      ")"
    )
    << "tensorField tf has different size to tensorField Tr"
    << abort(FatalError);
  }
  const tensorField& R = Rptr_();
  const tensorField Rtr(R.T());
  tmp<tensorField> tt{new tensorField{cellMap.size()}};
  tensorField& t = tt();
  FOR_ALL(cellMap, i) {
    const label cellI = cellMap[i];
    t[i] = R[cellI] & tf[i] & Rtr[cellI];
  }
  return tt;
}


mousse::tmp<mousse::symmTensorField> mousse::cylindrical::transformVector
(
  const vectorField& vf
) const
{
  if (Rptr_->size() != vf.size()) {
    FATAL_ERROR_IN("cylindrical::transformVector(const vectorField&)")
      << "tensorField vf has different size to tensorField Tr"
      << abort(FatalError);
  }
  tmp<symmTensorField> tfld{new symmTensorField{Rptr_->size()}};
  symmTensorField& fld = tfld();
  const tensorField& R = Rptr_();
  FOR_ALL(fld, i) {
    fld[i] = transformPrincipal(R[i], vf[i]);
  }
  return tfld;
}


mousse::symmTensor mousse::cylindrical::transformVector
(
  const vector&
) const
{
  NOT_IMPLEMENTED
  (
    "tensor cylindrical::transformVector(const vector&) const"
  );
  return symmTensor::zero;
}


void mousse::cylindrical::write(Ostream& os) const
{
  os.writeKeyword("e3") << e3() << token::END_STATEMENT << nl;
}
