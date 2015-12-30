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
  // Private Member Functions
    //- Disallow copy construct
    linearMotion(const linearMotion&);
    //- Disallow default bitwise assignment
    void operator=(const linearMotion&);
public:
  //- Runtime type information
  TypeName("linearMotion");
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
