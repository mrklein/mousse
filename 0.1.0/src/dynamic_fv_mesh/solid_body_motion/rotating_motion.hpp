#ifndef DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_ROTATING_MOTION_ROTATING_MOTION_HPP_
#define DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_ROTATING_MOTION_ROTATING_MOTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFunctions::rotatingMotion
// Description
//   SolidBodyMotionFvMesh 6DoF motion function.
//   The rotation is defined by an origin and axis of rotation and an angular
//   speed.

#include "solid_body_motion_function.hpp"
#include "primitive_fields.hpp"
#include "point.hpp"
#include "data_entry.hpp"
#include "auto_ptr.hpp"


namespace mousse {
namespace solidBodyMotionFunctions {

class rotatingMotion
:
  public solidBodyMotionFunction
{
  // Private data
    //- Origin of the axis
    const vector origin_;
    //- Axis vector
    const vector axis_;
    //- Angular velocty (rad/sec)
    autoPtr<DataEntry<scalar> > omega_;

public:
  //- Runtime type information
  TYPE_NAME("rotatingMotion");
  // Constructors
    //- Construct from components
    rotatingMotion
    (
      const dictionary& SBMFCoeffs,
      const Time& runTime
    );
    //- Construct and return a clone
    virtual autoPtr<solidBodyMotionFunction> clone() const
    {
      return autoPtr<solidBodyMotionFunction>
      {
        new rotatingMotion{SBMFCoeffs_, time_}
      };
    }
    //- Disallow copy construct
    rotatingMotion(const rotatingMotion&) = delete;
    //- Disallow default bitwise assignment
    rotatingMotion& operator=(const rotatingMotion&) = delete;
  //- Destructor
  virtual ~rotatingMotion();
  // Member Functions
    //- Return the solid-body motion transformation septernion
    virtual septernion transformation() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& SBMFCoeffs);
};

}  // namespace solidBodyMotionFunctions
}  // namespace mousse

#endif

