#ifndef MESH_TOOLS_COORDINATE_SYSTEMS_COORDINATE_ROTATION_EULER_COORDINATE_ROTATION_HPP_
#define MESH_TOOLS_COORDINATE_SYSTEMS_COORDINATE_ROTATION_EULER_COORDINATE_ROTATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::EulerCoordinateRotation
// Description
//   A coordinateRotation defined in the z-x-y Euler convention.
//   The 3 rotations are defined in the Euler convention
//   (around Z, around X' and around Z').
//   For reference and illustration, see
//   http://mathworld.wolfram.com/EulerAngles.html
//   Note, however, that it is the reverse transformation
//   (local->global) that is defined here.
//   - the rotation angles are in degrees, unless otherwise explictly specified:
//   \verbatim
//     coordinateRotation
//     {
//       type        EulerRotation
//       degrees     false;
//       rotation    (0 0 3.141592654);
//     }
//   \endverbatim
#include "coordinate_rotation.hpp"
namespace mousse
{
class EulerCoordinateRotation
:
  public coordinateRotation
{
  // Private Member Data
    //- Local-to-global transformation tensor
    tensor R_;
    //- Global-to-Local transformation tensor
    tensor Rtr_;
  // Private Member Functions
    //- Calculate transformation tensor
    void calcTransform
    (
      const scalar phiAngle,
      const scalar thetaAngle,
      const scalar psiAngle,
      const bool inDegrees=true
    );
public:
  //- Runtime type information
  TYPE_NAME("EulerRotation");
  // Constructors
    //- Construct null
    EulerCoordinateRotation();
    //- Construct from rotation vector
    EulerCoordinateRotation
    (
      const vector& phiThetaPsi,
      const bool inDegrees=true
    );
    //- Construct from components of rotation vector
    EulerCoordinateRotation
    (
      const scalar phiAngle,
      const scalar thetaAngle,
      const scalar psiAngle,
      const bool inDegrees=true
    );
    //- Construct from dictionary
    EulerCoordinateRotation(const dictionary&);
    //- Construct from dictionary and mesh
    EulerCoordinateRotation(const dictionary&, const objectRegistry&);
  // Member Functions
    //- Reset rotation to an identity rotation
    virtual void clear()
    {
      R_ = sphericalTensor::I;
      Rtr_ = sphericalTensor::I;
    }
    //- Update the rotation for a list of cells
    virtual void updateCells(const polyMesh&, const labelList&)
    {
      // do nothing
    }
    //- Return local-to-global transformation tensor
    virtual const tensor& R() const
    {
      return R_;
    }
    //- Return global-to-local transformation tensor
    virtual const tensor& Rtr() const
    {
      return Rtr_;
    };
    //- Return local Cartesian x-axis
    virtual const vector e1() const
    {
      return R_.x();
    }
    //- Return local Cartesian y-axis
    virtual const vector e2() const
    {
      return R_.y();
    }
    //- Return local Cartesian z-axis
    virtual const vector e3() const
    {
      return R_.z();
    }
    //- Return transformation tensor field
    virtual const tensorField& Tr() const;
    //- Transform vectorField using transformation tensor field
    virtual tmp<vectorField> transform(const vectorField& st) const;
    //- Transform vector using transformation tensor
    virtual vector transform(const vector& st) const;
    //- Inverse transform vectorField using transformation tensor field
    virtual tmp<vectorField> invTransform(const vectorField& st) const;
    //- Inverse transform vector using transformation tensor
    virtual vector invTransform(const vector& st) const;
    //- Transform tensor field using transformation tensorField
    virtual tmp<tensorField> transformTensor(const tensorField& st) const;
    //- Transform tensor using transformation tensorField
    virtual tensor transformTensor(const tensor& st) const;
    //- Transform tensor sub-field using transformation tensorField
    virtual tmp<tensorField> transformTensor
    (
      const tensorField& st,
      const labelList& cellMap
    ) const;
    //- Transform vectorField using transformation tensorField and return
    // symmetrical tensorField
    virtual tmp<symmTensorField> transformVector
    (
      const vectorField& st
    ) const;
    //- Transform vector using transformation tensor and return
    // symmetrical tensor
    virtual symmTensor transformVector(const vector& st) const;
  // Write
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
