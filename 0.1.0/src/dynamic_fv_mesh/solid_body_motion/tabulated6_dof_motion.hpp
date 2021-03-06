#ifndef DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_TABULATED6_DOF_MOTION_TABULATED6_DOF_MOTION_HPP_
#define DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_SOLID_BODY_MOTION_FUNCTIONS_TABULATED6_DOF_MOTION_TABULATED6_DOF_MOTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFunctions::tabulated6DoFMotion
// Description
//   Tabulated 6DoF motion function.
//   Obtained by interpolating tabulated data for surge (x-translation),
//   sway (y-translation), heave (z-translation), roll (rotation about x),
//   pitch (rotation about y) and yaw (rotation about z).

#include "solid_body_motion_function.hpp"
#include "primitive_fields.hpp"
#include "vector_2d.hpp"


namespace mousse {
namespace solidBodyMotionFunctions {

class tabulated6DoFMotion
:
  public solidBodyMotionFunction
{
  // Private data
    //- Time data file name read from dictionary
    fileName timeDataFileName_;
    //- Center of gravity read from dictionary
    vector CofG_;
    //- Type used to read in the translation and rotation "vectors"
    typedef Vector2D<vector> translationRotationVectors;
    //- Field of times
    scalarField times_;
    //- Field of translation and rotation "vectors"
    Field<translationRotationVectors> values_;
public:
  //- Runtime type information
  TYPE_NAME("tabulated6DoFMotion");
  // Constructors
    //- Construct from components
    tabulated6DoFMotion
    (
      const dictionary& SBMFCoeffs,
      const Time& runTime
    );
    //- Construct and return a clone
    virtual autoPtr<solidBodyMotionFunction> clone() const
    {
      return autoPtr<solidBodyMotionFunction>
      {
        new tabulated6DoFMotion{SBMFCoeffs_, time_}
      };
    }
    //- Disallow copy construct
    tabulated6DoFMotion(const tabulated6DoFMotion&) = delete;
    //- Disallow default bitwise assignment
    tabulated6DoFMotion& operator=(const tabulated6DoFMotion&) = delete;
  //- Destructor
  virtual ~tabulated6DoFMotion();
  // Member Functions
    //- Return the solid-body motion transformation septernion
    virtual septernion transformation() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& SBMFCoeffs);
};

}  // namespace solidBodyMotionFunctions
}  // namespace mousse

#endif

