// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "euler_coordinate_rotation.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(EulerCoordinateRotation, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  coordinateRotation,
  EulerCoordinateRotation,
  dictionary
);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  coordinateRotation,
  EulerCoordinateRotation,
  objectRegistry
);

}


// Member Functions 
mousse::vector mousse::EulerCoordinateRotation::transform(const vector& st) const
{
  return (R_ & st);
}


mousse::vector mousse::EulerCoordinateRotation::invTransform
(
  const vector& st
) const
{
  return (Rtr_ & st);
}


mousse::tmp<mousse::vectorField> mousse::EulerCoordinateRotation::transform
(
  const vectorField&
) const
{
  NOT_IMPLEMENTED
  (
    "tmp<vectorField> mousse::EulerCoordinateRotation:: "
    "transform(const vectorField& st) const"
  );
  return tmp<vectorField>{NULL};
}


mousse::tmp<mousse::vectorField> mousse::EulerCoordinateRotation::invTransform
(
  const vectorField&
) const
{
  NOT_IMPLEMENTED
  (
    "tmp<vectorField>  mousse::EulerCoordinateRotation::"
    "invTransform(const vectorField& st) const"
  );
  return tmp<vectorField>{NULL};
}


const mousse::tensorField& mousse::EulerCoordinateRotation::Tr() const
{
  NOT_IMPLEMENTED
  (
    "const tensorField& EulerCoordinateRotation::Tr() const"
  );
  return NullObjectRef<tensorField>();
}


mousse::tmp<mousse::tensorField> mousse::EulerCoordinateRotation::transformTensor
(
  const tensorField&
) const
{
  NOT_IMPLEMENTED
  (
    "const tensorField& EulerCoordinateRotation::transformTensor() const"
  );
  return tmp<tensorField>{NULL};
}


mousse::tensor mousse::EulerCoordinateRotation::transformTensor
(
  const tensor& st
) const
{
  return (R_ & st & Rtr_);
}


mousse::tmp<mousse::tensorField> mousse::EulerCoordinateRotation::transformTensor
(
  const tensorField&,
  const labelList& /*cellMap*/
) const
{
  NOT_IMPLEMENTED
  (
    "tmp<mousse::tensorField> EulerCoordinateRotation::transformTensor "
    " const tensorField& st,"
    " const labelList& cellMap "
    ") const"
  );
  return tmp<tensorField>{NULL};
}


mousse::tmp<mousse::symmTensorField> mousse::EulerCoordinateRotation::
transformVector
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


mousse::symmTensor mousse::EulerCoordinateRotation::transformVector
(
  const vector& st
) const
{
  return transformPrincipal(R_, st);
}


// Private Member Functions 
void mousse::EulerCoordinateRotation::calcTransform
(
  const scalar phiAngle,
  const scalar thetaAngle,
  const scalar psiAngle,
  const bool inDegrees
)
{
  scalar phi   = phiAngle;
  scalar theta = thetaAngle;
  scalar psi   = psiAngle;
  if (inDegrees) {
    phi *= constant::mathematical::pi/180.0;
    theta *= constant::mathematical::pi/180.0;
    psi *= constant::mathematical::pi/180.0;
  }
  R_ =
  (
    tensor
    {
      cos(phi)*cos(psi) - sin(phi)*sin(psi)*cos(theta),
      -sin(phi)*cos(psi)*cos(theta) - cos(phi)*sin(psi),
      sin(phi)*sin(theta),
      cos(phi)*sin(psi)*cos(theta) + sin(phi)*cos(psi),
      cos(phi)*cos(psi)*cos(theta) - sin(phi)*sin(psi),
      -cos(phi)*sin(theta),
      sin(psi)*sin(theta),
      cos(psi)*sin(theta),
      cos(theta)
    }
  );
  Rtr_ = R_.T();
}


// Constructors 
mousse::EulerCoordinateRotation::EulerCoordinateRotation()
:
  R_{sphericalTensor::I},
  Rtr_{R_}
{}


mousse::EulerCoordinateRotation::EulerCoordinateRotation
(
  const vector& phiThetaPsi,
  const bool inDegrees
)
:
  R_{sphericalTensor::I},
  Rtr_{R_}
{
  calcTransform
  (
    phiThetaPsi.component(vector::X),
    phiThetaPsi.component(vector::Y),
    phiThetaPsi.component(vector::Z),
    inDegrees
  );
}


mousse::EulerCoordinateRotation::EulerCoordinateRotation
(
  const scalar phiAngle,
  const scalar thetaAngle,
  const scalar psiAngle,
  const bool inDegrees
)
:
  R_{sphericalTensor::I},
  Rtr_{R_}
{
  calcTransform(phiAngle, thetaAngle, psiAngle, inDegrees);
}


mousse::EulerCoordinateRotation::EulerCoordinateRotation
(
  const dictionary& dict
)
:
  R_{sphericalTensor::I},
  Rtr_{R_}
{
  vector rotation{dict.lookup("rotation")};
  calcTransform
  (
    rotation.component(vector::X),
    rotation.component(vector::Y),
    rotation.component(vector::Z),
    dict.lookupOrDefault("degrees", true)
  );
}


mousse::EulerCoordinateRotation::EulerCoordinateRotation
(
  const dictionary& dict,
  const objectRegistry&
)
:
  R_{sphericalTensor::I},
  Rtr_{R_}
{
  vector rotation{dict.lookup("rotation")};
  calcTransform
  (
    rotation.component(vector::X),
    rotation.component(vector::Y),
    rotation.component(vector::Z),
    dict.lookupOrDefault("degrees", true)
  );
}


void mousse::EulerCoordinateRotation::write(Ostream& os) const
{
  os.writeKeyword("e1") << e1() << token::END_STATEMENT << nl;
  os.writeKeyword("e2") << e2() << token::END_STATEMENT << nl;
  os.writeKeyword("e3") << e3() << token::END_STATEMENT << nl;
}

