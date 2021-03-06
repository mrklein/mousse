#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_STATE_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_STATE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotionState
// Description
//   Holds the motion state of sixDoF object.  Wrapped up together
//   to allow rapid scatter to other processors.  The processors must all
//   maintain exactly the same state data to avoid any drift or inconsistency.

#include "vector.hpp"
#include "point.hpp"
#include "diag_tensor.hpp"
#include "tensor.hpp"
#include "dictionary.hpp"


namespace mousse {

// Forward declaration of classes
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class sixDoFRigidBodyMotionState;
Istream& operator>>(Istream&, sixDoFRigidBodyMotionState&);
Ostream& operator<<(Ostream&, const sixDoFRigidBodyMotionState&);


class sixDoFRigidBodyMotionState
{
  // Private data
    //- Current position of the centre of mass of the body
    point centreOfRotation_;
    //- Orientation, stored as the rotation tensor to transform
    //  from the body to the global reference frame, i.e.:
    //  globalVector = Q_ & bodyLocalVector
    //  bodyLocalVector = Q_.T() & globalVector
    tensor Q_;
    // Linear velocity of body
    vector v_;
    // Total linear acceleration of body
    vector a_;
    //- Angular momentum of body, in body local reference frame
    vector pi_;
    //- Total torque on body, in body local reference frame
    vector tau_;
public:
  // Constructors
    //- Construct null
    sixDoFRigidBodyMotionState();
    //- Construct from dictionary
    sixDoFRigidBodyMotionState(const dictionary& dict);
    //- Construct as copy
    sixDoFRigidBodyMotionState(const sixDoFRigidBodyMotionState&);
  //- Destructor
  ~sixDoFRigidBodyMotionState();
  // Member Functions
    // Access
      //- Return access to the centre of mass
      inline const point& centreOfRotation() const;
      //- Return access to the orientation
      inline const tensor& Q() const;
      //- Return access to velocity
      inline const vector& v() const;
      //- Return access to acceleration
      inline const vector& a() const;
      //- Return access to angular momentum
      inline const vector& pi() const;
      //- Return access to torque
      inline const vector& tau() const;
    // Edit
      //- Return non-const access to the centre of mass
      inline point& centreOfRotation();
      //- Return non-const access to the orientation
      inline tensor& Q();
      //- Return non-const access to vector
      inline vector& v();
      //- Return non-const access to acceleration
      inline vector& a();
      //- Return non-const access to angular momentum
      inline vector& pi();
      //- Return non-const access to torque
      inline vector& tau();
    //- Write to dictionary
    void write(dictionary& dict) const;
    //- Write to stream
    void write(Ostream&) const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, sixDoFRigidBodyMotionState&);
    friend Ostream& operator<<(Ostream&, const sixDoFRigidBodyMotionState&);
};

}  // namespace mousse


// Member Functions 
inline const mousse::point&
mousse::sixDoFRigidBodyMotionState::centreOfRotation() const
{
  return centreOfRotation_;
}


inline const mousse::tensor& mousse::sixDoFRigidBodyMotionState::Q() const
{
  return Q_;
}


inline const mousse::vector& mousse::sixDoFRigidBodyMotionState::v() const
{
  return v_;
}


inline const mousse::vector& mousse::sixDoFRigidBodyMotionState::a() const
{
  return a_;
}


inline const mousse::vector& mousse::sixDoFRigidBodyMotionState::pi() const
{
  return pi_;
}


inline const mousse::vector& mousse::sixDoFRigidBodyMotionState::tau() const
{
  return tau_;
}


inline mousse::point& mousse::sixDoFRigidBodyMotionState::centreOfRotation()
{
  return centreOfRotation_;
}


inline mousse::tensor& mousse::sixDoFRigidBodyMotionState::Q()
{
  return Q_;
}


inline mousse::vector& mousse::sixDoFRigidBodyMotionState::v()
{
  return v_;
}


inline mousse::vector& mousse::sixDoFRigidBodyMotionState::a()
{
  return a_;
}


inline mousse::vector& mousse::sixDoFRigidBodyMotionState::pi()
{
  return pi_;
}


inline mousse::vector& mousse::sixDoFRigidBodyMotionState::tau()
{
  return tau_;
}

#endif

