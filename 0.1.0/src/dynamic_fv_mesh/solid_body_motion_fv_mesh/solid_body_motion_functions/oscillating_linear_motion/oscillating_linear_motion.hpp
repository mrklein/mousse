// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFunctions::oscillatingLinearMotion
// Description
//   SolidBodyMotionFvMesh 6DoF motion function. Oscillating displacement.
// SourceFiles
//   oscillating_linear_motion.cpp
#ifndef oscillating_linear_motion_hpp_
#define oscillating_linear_motion_hpp_
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
  // Private Member Functions
    //- Disallow copy construct
    oscillatingLinearMotion(const oscillatingLinearMotion&);
    //- Disallow default bitwise assignment
    void operator=(const oscillatingLinearMotion&);
public:
  //- Runtime type information
  TypeName("oscillatingLinearMotion");
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
      (
        new oscillatingLinearMotion
        (
          SBMFCoeffs_,
          time_
        )
      );
    }
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
