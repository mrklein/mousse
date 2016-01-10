// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFunctions::linearMotion
// Description
//   SolidBodyMotionFvMesh 6DoF motion function. Constant velocity displacement.
// SourceFiles
//   linear_motion.cpp
#ifndef linear_motion_hpp_
#define linear_motion_hpp_
#include "solid_body_motion_function.hpp"
#include "primitive_fields.hpp"
#include "point.hpp"
namespace mousse
{
namespace solidBodyMotionFunctions
{
class linearMotion
:
  public solidBodyMotionFunction
{
  // Private data
    //- Linear velocity
    vector velocity_;

public:
  //- Runtime type information
  TYPE_NAME("linearMotion");
  // Constructors
    //- Construct from components
    linearMotion
    (
      const dictionary& SBMFCoeffs,
      const Time& runTime
    );
    //- Construct and return a clone
    virtual autoPtr<solidBodyMotionFunction> clone() const
    {
      return autoPtr<solidBodyMotionFunction>
      (
        new linearMotion
        (
          SBMFCoeffs_,
          time_
        )
      );
    }
    //- Disallow copy construct
    linearMotion(const linearMotion&) = delete;
    //- Disallow default bitwise assignment
    linearMotion& operator=(const linearMotion&) = delete;
  //- Destructor
  virtual ~linearMotion();
  // Member Functions
    //- Return the solid-body motion transformation septernion
    virtual septernion transformation() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& SBMFCoeffs);
};
}  // namespace solidBodyMotionFunctions
}  // namespace mousse
#endif
