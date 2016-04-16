#ifndef DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_OSCILLATING_ROTATING_MOTION_OSCILLATING_ROTATING_MOTION_HPP_
#define DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_OSCILLATING_ROTATING_MOTION_OSCILLATING_ROTATING_MOTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFunctions::oscillatingRotatingMotion
// Description
//   SolidBodyMotionFvMesh 6DoF motion function. Oscillating rotation.

#include "solid_body_motion_function.hpp"
#include "primitive_fields.hpp"
#include "point.hpp"


namespace mousse {
namespace solidBodyMotionFunctions{

class oscillatingRotatingMotion
:
  public solidBodyMotionFunction
{
  // Private data
    //- Centre of gravity
    point origin_;
    //- Amplitude
    vector amplitude_;
    //- Radial velocity
    scalar omega_;

public:
  //- Runtime type information
  TYPE_NAME("oscillatingRotatingMotion");
  // Constructors
    //- Construct from components
    oscillatingRotatingMotion
    (
      const dictionary& SBMFCoeffs,
      const Time& runTime
    );
    //- Construct and return a clone
    virtual autoPtr<solidBodyMotionFunction> clone() const
    {
      return autoPtr<solidBodyMotionFunction>
      {
        new oscillatingRotatingMotion{SBMFCoeffs_, time_}
      };
    }
    //- Disallow copy construct
    oscillatingRotatingMotion(const oscillatingRotatingMotion&) = delete;
    //- Disallow default bitwise assignment
    oscillatingRotatingMotion& operator=
    (
      const oscillatingRotatingMotion&
    ) = delete;
  //- Destructor
  virtual ~oscillatingRotatingMotion();
  // Member Functions
    //- Return the solid-body motion transformation septernion
    virtual septernion transformation() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& SBMFCoeffs);
};

}  // namespace solidBodyMotionFunctions
}  // namespace mousse

#endif

