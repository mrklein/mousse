#ifndef FV_OPTIONS_SOURCES_DERIVED_ROTOR_DISK_SOURCE_BLADE_MODEL_BLADE_MODEL_HPP_
#define FV_OPTIONS_SOURCES_DERIVED_ROTOR_DISK_SOURCE_BLADE_MODEL_BLADE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::bladeModel
// Description
//   Blade model class calculates:
//     Linear interpolated blade twist and chord based on radial position
//     Interpolation factor (for interpolating profile performance)
//   Input in list format:
//     data
//     (
//       (profile1 (radius1 twist1 chord1))
//       (profile1 (radius2 twist2 chord2))
//     );
//   where:
//     radius [m]
//     twist [deg], converted to [rad] internally
//     chord [m]

#include "list.hpp"
#include "dictionary.hpp"


namespace mousse {

class bladeModel
{
protected:
  // Protected data
    //- Corresponding profile name per section
    List<word> profileName_;
    //- Corresponding profile ID per section
    List<label> profileID_;
    //- Radius [m]
    List<scalar> radius_;
    //- Twist [deg] on input, converted to [rad]
    List<scalar> twist_;
    //- Chord [m]
    List<scalar> chord_;
    //- File name (optional)
    fileName fName_;
  // Protected Member Functions
    //- Return ture if file name is set
    bool readFromFile() const;
    //- Return the interpolation indices and gradient
    void interpolateWeights
    (
      const scalar& xIn,
      const List<scalar>& values,
      label& i1,
      label& i2,
      scalar& ddx
    ) const;
public:
  //- Constructor
  bladeModel(const dictionary& dict);
  //- Destructor
  virtual ~bladeModel();
  // Member functions
    // Access
      //- Return const access to the profile name list
      const List<word>& profileName() const;
      //- Return const access to the profile ID list
      const List<label>& profileID() const;
      //- Return const access to the radius list
      const List<scalar>& radius() const;
      //- Return const access to the twist list
      const List<scalar>& twist() const;
      //- Return const access to the chord list
      const List<scalar>& chord() const;
    // Edit
      //- Return non-const access to the profile ID list
      List<label>& profileID();
    // Evaluation
      //- Return the twist and chord for a given radius
      virtual void interpolate
      (
        const scalar radius,
        scalar& twist,
        scalar& chord,
        label& i1,
        label& i2,
        scalar& invDr
      ) const;
};

}  // namespace mousse

#endif

