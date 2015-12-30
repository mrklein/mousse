// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "starcd_coordinate_rotation.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(STARCDCoordinateRotation, 0);
  addToRunTimeSelectionTable
  (
    coordinateRotation,
    STARCDCoordinateRotation,
    dictionary
  );
  addToRunTimeSelectionTable
  (
    coordinateRotation,
    STARCDCoordinateRotation,
    objectRegistry
  );
}
// Member Functions 
mousse::vector mousse::STARCDCoordinateRotation::transform(const vector& st) const
{
  return (R_ & st);
}
mousse::vector mousse::STARCDCoordinateRotation::invTransform
(
  const vector& st
) const
{
  return (Rtr_ & st);
}
mousse::tmp<mousse::vectorField> mousse::STARCDCoordinateRotation::transform
(
  const vectorField& st
) const
{
  notImplemented
  (
    "tmp<vectorField> mousse::STARCDCoordinateRotation:: "
    "transform(const vectorField& st) const"
  );
  return tmp<vectorField>(NULL);
}
mousse::tmp<mousse::vectorField> mousse::STARCDCoordinateRotation::invTransform
(
  const vectorField& st
) const
{
  notImplemented
  (
    "tmp<vectorField>  mousse::STARCDCoordinateRotation::"
    "invTransform(const vectorField& st) const"
  );
  return tmp<vectorField>(NULL);
}
const mousse::tensorField& mousse::STARCDCoordinateRotation::Tr() const
{
  notImplemented
  (
    "const tensorField& STARCDCoordinateRotatio::Tr() const"
  );
  return NullObjectRef<tensorField>();
}
mousse::tmp<mousse::tensorField> mousse::STARCDCoordinateRotation::transformTensor
(
  const tensorField& st
) const
{
  notImplemented
  (
    "tmp<mousse::tensorField> STARCDCoordinateRotation::transformTensor()"
  );
  return tmp<tensorField>(NULL);
}
mousse::tensor mousse::STARCDCoordinateRotation::transformTensor
(
  const tensor& st
) const
{
  return (R_ & st & Rtr_);
}
mousse::tmp<mousse::tensorField> mousse::STARCDCoordinateRotation::transformTensor
(
  const tensorField& st,
  const labelList& cellMap
) const
{
  notImplemented
  (
    "tmp<mousse::tensorField> STARCDCoordinateRotation::transformTensor "
    " const tensorField& st,"
    " const labelList& cellMap "
    ") const"
  );
  return tmp<tensorField>(NULL);
}
mousse::tmp<mousse::symmTensorField> mousse::STARCDCoordinateRotation::
transformVector
(
  const vectorField& st
) const
{
  tmp<symmTensorField> tfld(new symmTensorField(st.size()));
  symmTensorField& fld = tfld();
  forAll(fld, i)
  {
    fld[i] = transformPrincipal(R_, st[i]);
  }
  return tfld;
}
mousse::symmTensor mousse::STARCDCoordinateRotation::transformVector
(
  const vector& st
) const
{
  return transformPrincipal(R_, st);
}
// Private Member Functions 
void mousse::STARCDCoordinateRotation::calcTransform
(
  const scalar rotZ,
  const scalar rotX,
  const scalar rotY,
  const bool inDegrees
)
{
  scalar x = rotX;
  scalar y = rotY;
  scalar z = rotZ;
  if (inDegrees)
  {
    x *= constant::mathematical::pi/180.0;
    y *= constant::mathematical::pi/180.0;
    z *= constant::mathematical::pi/180.0;
  }
  R_ =
  (
    tensor
    (
      cos(y)*cos(z) - sin(x)*sin(y)*sin(z),
      -cos(x)*sin(z),
      sin(x)*cos(y)*sin(z) + sin(y)*cos(z),
      cos(y)*sin(z) + sin(x)*sin(y)*cos(z),
      cos(x)*cos(z),
      sin(y)*sin(z) - sin(x)*cos(y)*cos(z),
      -cos(x)*sin(y),
      sin(x),
      cos(x)*cos(y)
    )
  );
  Rtr_ = R_.T();
}
// Constructors 
mousse::STARCDCoordinateRotation::STARCDCoordinateRotation()
:
  R_(sphericalTensor::I),
  Rtr_(R_)
{}
mousse::STARCDCoordinateRotation::STARCDCoordinateRotation
(
  const vector& rotZrotXrotY,
  const bool inDegrees
)
:
  R_(sphericalTensor::I),
  Rtr_(R_)
{
  calcTransform
  (
    rotZrotXrotY.component(vector::X),
    rotZrotXrotY.component(vector::Y),
    rotZrotXrotY.component(vector::Z),
    inDegrees
  );
}
mousse::STARCDCoordinateRotation::STARCDCoordinateRotation
(
  const scalar rotZ,
  const scalar rotX,
  const scalar rotY,
  const bool inDegrees
)
:
  R_(sphericalTensor::I),
  Rtr_(R_)
{
  calcTransform(rotZ, rotX, rotY, inDegrees);
}
mousse::STARCDCoordinateRotation::STARCDCoordinateRotation
(
  const dictionary& dict
)
:
  R_(sphericalTensor::I),
  Rtr_(R_)
{
  vector rotation(dict.lookup("rotation"));
  calcTransform
  (
    rotation.component(vector::X),
    rotation.component(vector::Y),
    rotation.component(vector::Z),
    dict.lookupOrDefault("degrees", true)
  );
}
mousse::STARCDCoordinateRotation::STARCDCoordinateRotation
(
  const dictionary& dict,
  const objectRegistry&
)
{
  vector rotation(dict.lookup("rotation"));
  calcTransform
  (
    rotation.component(vector::X),
    rotation.component(vector::Y),
    rotation.component(vector::Z),
    dict.lookupOrDefault("degrees", true)
  );
}
void mousse::STARCDCoordinateRotation::write(Ostream& os) const
{
  os.writeKeyword("e1") << e1() << token::END_STATEMENT << nl;
  os.writeKeyword("e2") << e2() << token::END_STATEMENT << nl;
  os.writeKeyword("e3") << e3() << token::END_STATEMENT << nl;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
