#ifndef DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_OSCILLATING_LINEAR_MOTION_OSCILLATING_LINEAR_MOTION_HPP_
#define DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_OSCILLATING_LINEAR_MOTION_OSCILLATING_LINEAR_MOTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFunctions::oscillatingLinearMotion
// Description
//   SolidBodyMotionFvMesh 6DoF motion function. Oscillating displacement.
// SourceFiles
//   oscillating_linear_motion.cpp
#include "solid_body_motion_function.hpp"
#include "primitive_fields.hpp"
#include "point.hpp"
namespace mousse
{
namespace solidBodyMotionFunctions
{
class oscillatingLinearMotion
:
  public solidBodyMotionFunction
{
  // Private data
    //- Amplitude
    vector amplitude_;
    //- Radial velocity
    scalar omega_;
public:
  //- Runtime type information
  TYPE_NAME("oscillatingLinearMotion");
  // Constructors
    //- Construct from components
    oscillatingLinearMotion
    (
      const dictionary& SBMFCoeffs,
      const Time& runTime
    );
    //- Construct and return a clone
    virtual autoPtr<solidBodyMotionFunction> clone() const
    {
      return autoPtr<solidBodyMotionFunction>
      {
        new oscillatingLinearMotion
        {
          SBMFCoeffs_,
          time_
        }
      };
    }
    //- Disallow copy construct
    oscillatingLinearMotion(const oscillatingLinearMotion&) = delete;
    //- Disallow default bitwise assignment
    oscillatingLinearMotion& operator=
    (
      const oscillatingLinearMotion&
    ) = delete;
  //- Destructor
  virtual ~oscillatingLinearMotion();
  // Member Functions
    //- Return the solid-body motion transformation septernion
    virtual septernion transformation() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& SBMFCoeffs);
};
}  // namespace solidBodyMotionFunctions
}  // namespace mousse
#endif
