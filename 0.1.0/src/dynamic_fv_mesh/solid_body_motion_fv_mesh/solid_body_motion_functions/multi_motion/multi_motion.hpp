#ifndef DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_MULTI_MOTION_MULTI_MOTION_HPP_
#define DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_MULTI_MOTION_MULTI_MOTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFunctions::multiMotion
// Description
//   Combination of SolidBodyMotionFvMesh 6DoF motion functions.
// SourceFiles
//   multi_motion.cpp
#include "solid_body_motion_function.hpp"
#include "primitive_fields.hpp"
#include "point.hpp"
namespace mousse
{
namespace solidBodyMotionFunctions
{
class multiMotion
:
  public solidBodyMotionFunction
{
  // Private data
    //- Motions to combine
    PtrList<solidBodyMotionFunction> SBMFs_;

public:
  //- Runtime type information
  TYPE_NAME("multiMotion");
  // Constructors
    //- Construct from components
    multiMotion
    (
      const dictionary& SBMFCoeffs,
      const Time& runTime
    );
    //- Construct and return a clone
    virtual autoPtr<solidBodyMotionFunction> clone() const
    {
      return autoPtr<solidBodyMotionFunction>
      {
        new multiMotion
        {
          SBMFCoeffs_,
          time_
        }
      };
    }
    //- Disallow copy construct
    multiMotion(const multiMotion&) = delete;
    //- Disallow default bitwise assignment
    multiMotion& operator=(const multiMotion&) = delete;
  //- Destructor
  virtual ~multiMotion();
  // Member Functions
    //- Return the solid-body motion transformation septernion
    virtual septernion transformation() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& SBMFCoeffs);
};
}  // namespace solidBodyMotionFunctions
}  // namespace mousse
#endif
