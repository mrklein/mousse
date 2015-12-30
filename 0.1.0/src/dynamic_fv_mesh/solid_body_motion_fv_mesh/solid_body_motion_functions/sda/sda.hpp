// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFunctions::SDA
// Description
//   Ship design analysis (SDA) 3DoF motion function.
//   Comprising sinusoidal roll (rotation about x), heave (z-translation)
//   and sway (y-translation) motions with changing amplitude and phase.
// See Also
//   mousse::solidBodyMotionFunctions::tabulated6DoFMotion
// SourceFiles
//   sda.cpp
#ifndef sda_hpp_
#define sda_hpp_
#include "solid_body_motion_function.hpp"
namespace mousse
{
namespace solidBodyMotionFunctions
{
class SDA
:
  public solidBodyMotionFunction
{
  // Private data
    //- Center of gravity
    vector CofG_;
    //- Model scale ratio
    scalar lamda_;
    //- Max roll amplitude [rad]
    scalar rollAmax_;
    //- Min roll amplitude [rad]
    scalar rollAmin_;
    //- Heave amplitude [m]
    scalar heaveA_;
    //- Sway amplitude [m]
    scalar swayA_;
    //- Damping Coefficient [-]
    scalar Q_;
    //- Time Period for liquid [sec]
    scalar Tp_;
    //- Natural Period of Ship [sec]
    scalar Tpn_;
    //- Reference time step [sec]
    scalar dTi_;
    //- Incr. in Tp/unit 'dTi'[-]
    scalar dTp_;
  // Private Member Functions
    //- Disallow copy construct
    SDA(const SDA&);
    //- Disallow default bitwise assignment
    void operator=(const SDA&);
public:
  //- Runtime type information
  TypeName("SDA");
  // Constructors
    //- Construct from components
    SDA
    (
      const dictionary& SBMFCoeffs,
      const Time& runTime
    );
    //- Construct and return a clone
    virtual autoPtr<solidBodyMotionFunction> clone() const
    {
      return autoPtr<solidBodyMotionFunction>
      (
        new SDA
        (
          SBMFCoeffs_,
          time_
        )
      );
    }
  //- Destructor
  virtual ~SDA();
  // Member Functions
    //- Return the solid-body motion transformation septernion
    virtual septernion transformation() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& SBMFCoeffs);
};
}  // namespace solidBodyMotionFunctions
}  // namespace mousse
#endif
