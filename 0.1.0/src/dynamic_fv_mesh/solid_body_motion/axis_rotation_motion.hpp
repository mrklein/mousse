#ifndef DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_AXIS_ROTATION_MOTION_AXIS_ROTATION_MOTION_HPP_
#define DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_AXIS_ROTATION_MOTION_AXIS_ROTATION_MOTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFunctions::axisRotationMotion
// Description
//   Constant velocity rotation around CoG. Similar to rotatingMotion but
//   motion specified as rotation vector.

#include "solid_body_motion_function.hpp"
#include "primitive_fields.hpp"
#include "point.hpp"


namespace mousse {
namespace solidBodyMotionFunctions {

class axisRotationMotion
:
  public solidBodyMotionFunction
{
  // Private data
    //- Origin
    point origin_;
    //- Rotational velocity (deg/s)
    vector radialVelocity_;
public:
  //- Runtime type information
  TYPE_NAME("axisRotationMotion");
  // Constructors
    //- Construct from components
    axisRotationMotion
    (
      const dictionary& SBMFCoeffs,
      const Time& runTime
    );
    //- Construct and return a clone
    virtual autoPtr<solidBodyMotionFunction> clone() const
    {
      return autoPtr<solidBodyMotionFunction>
      {
        new axisRotationMotion{SBMFCoeffs_, time_}
      };
    }
    //- Disallow copy construct
    axisRotationMotion(const axisRotationMotion&) = delete;
    //- Disallow default bitwise assignment
    axisRotationMotion& operator=(const axisRotationMotion&) = delete;
  //- Destructor
  virtual ~axisRotationMotion();
  // Member Functions
    //- Return the solid-body motion transformation septernion
    virtual septernion transformation() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& SBMFCoeffs);
};

}  // namespace solidBodyMotionFunctions
}  // namespace mousse

#endif

