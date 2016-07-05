// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "axes_rotation.hpp"
#include "dictionary.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(axesRotation, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(coordinateRotation, axesRotation, dictionary);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  coordinateRotation,
  axesRotation,
  objectRegistry
);

}


// Private Member Functions
void mousse::axesRotation::calcTransform
(
  const vector& axis1,
  const vector& axis2,
  const axisOrder& order
)
{
  vector a = axis1/mag(axis1);
  vector b = axis2;
  b = b - (b & a)*a;
  if (mag(b) < SMALL) {
    FATAL_ERROR_IN("axesRotation::calcTransform()")
      << "axis1, axis2 appear co-linear: "
      << axis1 << ", " << axis2 << endl
      << abort(FatalError);
  }
  b = b/mag(b);
  vector c = a^b;
  tensor Rtr;
  switch (order) {
    case e1e2:
    {
      Rtr = tensor(a, b, c);
      break;
    }
    case e2e3:
    {
      Rtr = tensor(c, a, b);
      break;
    }
    case e3e1:
    {
      Rtr = tensor(b, c, a);
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "axesRotation::calcTransform"
        "("
          "const vector&,"
          "const vector&,"
          "const axisOrder&"
        ")"
      )
      << "Unhandled axes specifictation" << endl
      << abort(FatalError);
      Rtr = tensor::zero;
      break;
    }
  }
  // Global->local transformation
  Rtr_ = Rtr;
  // Local->global transformation
  R_ = Rtr.T();
}


// Constructors
mousse::axesRotation::axesRotation()
:
  R_{sphericalTensor::I},
  Rtr_{R_}
{}


mousse::axesRotation::axesRotation
(
  const vector& axis,
  const vector& dir
)
:
  R_{sphericalTensor::I},
  Rtr_{R_}
{
  calcTransform(axis, dir, e3e1);
}


mousse::axesRotation::axesRotation
(
  const dictionary& dict
)
:
  R_{sphericalTensor::I},
  Rtr_{R_}
{
  operator=(dict);
}


mousse::axesRotation::axesRotation
(
  const dictionary& dict,
  const objectRegistry&
)
:
  R_{sphericalTensor::I},
  Rtr_{R_}
{
  operator=(dict);
}


mousse::axesRotation::axesRotation(const tensor& R)
:
  R_{R},
  Rtr_{R_.T()}
{}


// Member Functions
const mousse::tensorField& mousse::axesRotation::Tr() const
{
  NOT_IMPLEMENTED
  (
    "const mousse::tensorField& axesRotation::Tr() const"
  );
  return NullObjectRef<tensorField>();
}


mousse::tmp<mousse::vectorField> mousse::axesRotation::transform
(
  const vectorField& st
) const
{
  return (R_ & st);
}


mousse::vector mousse::axesRotation::transform(const vector& st) const
{
  return (R_ & st);
}


mousse::tmp<mousse::vectorField> mousse::axesRotation::invTransform
(
  const vectorField& st
) const
{
  return (Rtr_ & st);
}


mousse::vector mousse::axesRotation::invTransform(const vector& st) const
{
  return (Rtr_ & st);
}


mousse::tmp<mousse::tensorField> mousse::axesRotation::transformTensor
(
  const tensorField&
) const
{
  NOT_IMPLEMENTED
  (
    "const tensorField& axesRotation::transformTensor() const"
  );
  return tmp<tensorField>(NULL);
}


mousse::tensor mousse::axesRotation::transformTensor
(
  const tensor& st
) const
{
  return (R_ & st & Rtr_);
}


mousse::tmp<mousse::tensorField> mousse::axesRotation::transformTensor
(
  const tensorField&,
  const labelList& /*cellMap*/
) const
{
  NOT_IMPLEMENTED
  (
    "tmp<mousse::tensorField> axesRotation::transformTensor "
    "("
      "const tensorField&,"
      "const labelList&"
    ") const"
  );
  return tmp<tensorField>{NULL};
}


mousse::tmp<mousse::symmTensorField> mousse::axesRotation::transformVector
(
  const vectorField& st
) const
{
  tmp<symmTensorField> tfld{new symmTensorField{st.size()}};
  symmTensorField& fld = tfld();
  FOR_ALL(fld, i) {
    fld[i] = transformPrincipal(R_, st[i]);
  }
  return tfld;
}


mousse::symmTensor mousse::axesRotation::transformVector
(
  const vector& st
) const
{
  return transformPrincipal(R_, st);
}


// Member Operators
void mousse::axesRotation::operator=(const dictionary& dict)
{
  if (debug) {
    Pout << "axesRotation::operator=(const dictionary&) : "
      << "assign from " << dict << endl;
  }
  vector axis1, axis2;
  axisOrder order{e3e1};
  if (dict.readIfPresent("e1", axis1) && dict.readIfPresent("e2", axis2)) {
    order = e1e2;
  } else if (dict.readIfPresent("e2", axis1)&& dict.readIfPresent("e3", axis2)) {
    order = e2e3;
  } else if (dict.readIfPresent("e3", axis1)&& dict.readIfPresent("e1", axis2)) {
    order = e3e1;
  } else if (dict.found("axis") || dict.found("direction")) {
    // Both "axis" and "direction" are required
    // If one is missing the appropriate error message will be generated
    order = e3e1;
    dict.lookup("axis") >> axis1;
    dict.lookup("direction") >> axis2;
  } else {
    FATAL_ERROR_IN
    (
      "axesRotation::operator=(const dictionary&) "
    )
    << "not entry of the type (e1, e2) or (e2, e3) or (e3, e1) "
    << "found "
    << exit(FatalError);
  }
  calcTransform(axis1, axis2, order);
}


void mousse::axesRotation::write(Ostream& os) const
{
  os.writeKeyword("e1") << e1() << token::END_STATEMENT << nl;
  os.writeKeyword("e2") << e2() << token::END_STATEMENT << nl;
  os.writeKeyword("e3") << e3() << token::END_STATEMENT << nl;
}

