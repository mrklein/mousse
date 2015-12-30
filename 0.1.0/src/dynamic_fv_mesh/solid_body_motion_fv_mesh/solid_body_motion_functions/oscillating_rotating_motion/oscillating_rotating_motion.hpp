// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFunctions::oscillatingRotatingMotion
// Description
//   SolidBodyMotionFvMesh 6DoF motion function. Oscillating rotation.
// SourceFiles
//   oscillating_rotating_motion.cpp
#ifndef oscillating_rotating_motion_hpp_
#define oscillating_rotating_motion_hpp_
#include "solid_body_motion_function.hpp"
#include "primitive_fields.hpp"
#include "point.hpp"
namespace mousse
{
namespace solidBodyMotionFunctions
{
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
  // Private Member Functions
    //- Disallow copy construct
    oscillatingRotatingMotion(const oscillatingRotatingMotion&);
    //- Disallow default bitwise assignment
    void operator=(const oscillatingRotatingMotion&);
public:
  //- Runtime type information
  TypeName("oscillatingRotatingMotion");
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
      (
        new oscillatingRotatingMotion
        (
          SBMFCoeffs_,
          time_
        )
      );
    }
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
