#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_RIGID_BODY_MOTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFRigidBodyMotion
// Description
//   Six degree of freedom motion for a rigid body.
//   Angular momentum stored in body fixed reference frame.  Reference
//   orientation of the body (where Q = I) must align with the cartesian axes
//   such that the Inertia tensor is in principle component form.  Can add
//   restraints (e.g. a spring) and constraints (e.g. motion may only be on a
//   plane).
//   The time-integrator for the motion is run-time selectable with options for
//   symplectic (explicit), Crank-Nicolson and Newmark schemes.

#include "six_dof_rigid_body_motion_state.hpp"
#include "point_field.hpp"
#include "six_dof_rigid_body_motion_restraint.hpp"
#include "six_dof_rigid_body_motion_constraint.hpp"
#include "tuple2.hpp"


namespace mousse {

// Forward declarations
class sixDoFSolver;


class sixDoFRigidBodyMotion
{
  friend class sixDoFSolver;
  // Private data
    //- Motion state data object
    sixDoFRigidBodyMotionState motionState_;
    //- Motion state data object for previous time-step
    sixDoFRigidBodyMotionState motionState0_;
    //- Motion restraints
    PtrList<sixDoFRigidBodyMotionRestraint> restraints_;
    //- Motion constaints
    PtrList<sixDoFRigidBodyMotionConstraint> constraints_;
    //- Translational constraint tensor
    tensor tConstraints_;
    //- Rotational constraint tensor
    tensor rConstraints_;
    //- Centre of mass of initial state
    point initialCentreOfMass_;
    //- Centre of rotation of initial state
    point initialCentreOfRotation_;
    //- Orientation of initial state
    tensor initialQ_;
    //- Mass of the body
    scalar mass_;
    //- Moment of inertia of the body in reference configuration
    //  (Q = I)
    diagTensor momentOfInertia_;
    //- Acceleration relaxation coefficient
    scalar aRelax_;
    //- Acceleration damping coefficient (for steady-state simulations)
    scalar aDamp_;
    //- Switch to turn reporting of motion data on and off
    Switch report_;
    //- Motion solver
    autoPtr<sixDoFSolver> solver_;
  // Private Member Functions
    //- Calculate the rotation tensor around the body reference
    //  frame x-axis by the given angle
    inline tensor rotationTensorX(scalar deltaT) const;
    //- Calculate the rotation tensor around the body reference
    //  frame y-axis by the given angle
    inline tensor rotationTensorY(scalar deltaT) const;
    //- Calculate the rotation tensor around the body reference
    //  frame z-axis by the given angle
    inline tensor rotationTensorZ(scalar deltaT) const;
    //- Apply rotation tensors to Q0 for the given torque (pi) and deltaT
    //  and return the rotated Q and pi as a tuple
    inline Tuple2<tensor, vector> rotate
    (
      const tensor& Q0,
      const vector& pi,
      const scalar deltaT
    ) const;
    //- Apply the restraints to the object
    void applyRestraints();
    //- Update and relax accelerations from the force and torque
    void updateAcceleration(const vector& fGlobal, const vector& tauGlobal);
    // Access functions retained as private because of the risk of
    // confusion over what is a body local frame vector and what is global
    // Access
      //- Return the restraints
      inline const PtrList<sixDoFRigidBodyMotionRestraint>&
        restraints() const;
      //- Return the constraints
      inline const PtrList<sixDoFRigidBodyMotionConstraint>&
        constraints() const;
      //- Return the initial centre of rotation
      inline const point& initialCentreOfRotation() const;
      //- Return the initial orientation
      inline const tensor& initialQ() const;
      //- Return the orientation
      inline const tensor& Q() const;
      //- Return the current acceleration
      inline const vector& a() const;
      //- Return the current angular momentum
      inline const vector& pi() const;
      //- Return the current torque
      inline const vector& tau() const;
    // Edit
      //- Return the centre of rotation
      inline point& initialCentreOfRotation();
      //- Return initial orientation
      inline tensor& initialQ();
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
public:
  // Constructors
    //- Construct null
    sixDoFRigidBodyMotion();
    //- Construct from constant and state dictionaries
    sixDoFRigidBodyMotion
    (
      const dictionary& dict,
      const dictionary& stateDict
    );
    //- Construct as copy
    sixDoFRigidBodyMotion(const sixDoFRigidBodyMotion&);
  //- Destructor
  ~sixDoFRigidBodyMotion();
  // Member Functions
    // Access
      //- Return the mass
      inline scalar mass() const;
      //- Return the inertia tensor
      inline const diagTensor& momentOfInertia() const;
      //- Return the motion state
      inline const sixDoFRigidBodyMotionState& state() const;
      //- Return the current centre of rotation
      inline const point& centreOfRotation() const;
      //- Return the initial centre of mass
      inline const point& initialCentreOfMass() const;
      //- Return the current centre of mass
      inline point centreOfMass() const;
      //- Return the orientation tensor, Q.
      //  globalVector = Q & bodyLocalVector
      //  bodyLocalVector = Q.T() & globalVector
      inline const tensor& orientation() const;
      //- Return the angular velocity in the global frame
      inline vector omega() const;
      //- Return the current velocity
      inline const vector& v() const;
      inline vector momentArm() const;
      //- Return the report Switch
      inline bool report() const;
    // Edit
      //- Store the motion state at the beginning of the time-step
      inline void newTime();
      //- Return non-const access to the centre of rotation
      inline point& centreOfRotation();
    // Constraints and Restraints
      //- Add restraints to the motion, public to allow external
      //  addition of restraints after construction
      void addRestraints(const dictionary& dict);
      //- Add restraints to the motion, public to allow external
      //  addition of restraints after construction
      void addConstraints(const dictionary& dict);
    // Update state
      //- Symplectic integration of velocities, orientation and position.
      //  Changes to Crank-Nicolson integration for subsequent iterations.
      void update
      (
        bool firstIter,
        const vector& fGlobal,
        const vector& tauGlobal,
        scalar deltaT,
        scalar deltaT0
      );
      //- Report the status of the motion
      void status() const;
    // Transformations
      //- Return the velocity of a position
      inline point velocity(const point& pt) const;
      //- Transform the given initial state point by the current motion
      //  state
      inline point transform(const point& initialPoints) const;
      //- Transform the given initial state pointField by the current
      //  motion state
      tmp<pointField> transform(const pointField& initialPoints) const;
      //- Transform the given initial state pointField by the current
      //  motion state scaled by the given scale
      tmp<pointField> transform
      (
        const pointField& initialPoints,
        const scalarField& scale
      ) const;
    //- Write
    void write(Ostream&) const;
    //- Read coefficients dictionary and update system parameters,
    //  constraints and restraints but not the current state
    bool read(const dictionary& dict);
};

}  // namespace mousse


// Private Member Functions 
inline mousse::tensor mousse::sixDoFRigidBodyMotion::rotationTensorX
(
  scalar phi
) const
{
  return {1,                0,                 0,
          0, mousse::cos(phi), -mousse::sin(phi),
          0, mousse::sin(phi),  mousse::cos(phi)};
}


inline mousse::tensor mousse::sixDoFRigidBodyMotion::rotationTensorY
(
  scalar phi
) const
{
  return { mousse::cos(phi), 0, mousse::sin(phi),
                          0, 1,                0,
          -mousse::sin(phi), 0, mousse::cos(phi)};
}


inline mousse::tensor mousse::sixDoFRigidBodyMotion::rotationTensorZ
(
  scalar phi
) const
{
  return {mousse::cos(phi), -mousse::sin(phi), 0,
          mousse::sin(phi), mousse::cos(phi), 0,
          0, 0, 1};
}


inline mousse::Tuple2<mousse::tensor, mousse::vector>
mousse::sixDoFRigidBodyMotion::rotate
(
  const tensor& Q0,
  const vector& pi0,
  const scalar deltaT
) const
{
  Tuple2<tensor, vector> Qpi{Q0, pi0};
  tensor& Q = Qpi.first();
  vector& pi = Qpi.second();
  tensor R = rotationTensorX(0.5*deltaT*pi.x()/momentOfInertia_.xx());
  pi = pi & R;
  Q = Q & R;
  R = rotationTensorY(0.5*deltaT*pi.y()/momentOfInertia_.yy());
  pi = pi & R;
  Q = Q & R;
  R = rotationTensorZ(deltaT*pi.z()/momentOfInertia_.zz());
  pi = pi & R;
  Q = Q & R;
  R = rotationTensorY(0.5*deltaT*pi.y()/momentOfInertia_.yy());
  pi = pi & R;
  Q = Q & R;
  R = rotationTensorX(0.5*deltaT*pi.x()/momentOfInertia_.xx());
  pi = pi & R;
  Q = Q & R;
  return Qpi;
}


inline const mousse::PtrList<mousse::sixDoFRigidBodyMotionRestraint>&
mousse::sixDoFRigidBodyMotion::restraints() const
{
  return restraints_;
}


inline const mousse::PtrList<mousse::sixDoFRigidBodyMotionConstraint>&
mousse::sixDoFRigidBodyMotion::constraints() const
{
  return constraints_;
}


inline const mousse::point&
mousse::sixDoFRigidBodyMotion::initialCentreOfRotation() const
{
  return initialCentreOfRotation_;
}


inline const mousse::tensor&
mousse::sixDoFRigidBodyMotion::initialQ() const
{
  return initialQ_;
}


inline const mousse::tensor& mousse::sixDoFRigidBodyMotion::Q() const
{
  return motionState_.Q();
}


inline const mousse::vector& mousse::sixDoFRigidBodyMotion::v() const
{
  return motionState_.v();
}


inline const mousse::vector& mousse::sixDoFRigidBodyMotion::a() const
{
  return motionState_.a();
}


inline const mousse::vector& mousse::sixDoFRigidBodyMotion::pi() const
{
  return motionState_.pi();
}


inline const mousse::vector& mousse::sixDoFRigidBodyMotion::tau() const
{
  return motionState_.tau();
}


inline mousse::point& mousse::sixDoFRigidBodyMotion::initialCentreOfRotation()
{
  return initialCentreOfRotation_;
}


inline mousse::tensor& mousse::sixDoFRigidBodyMotion::initialQ()
{
  return initialQ_;
}


inline mousse::tensor& mousse::sixDoFRigidBodyMotion::Q()
{
  return motionState_.Q();
}


inline mousse::vector& mousse::sixDoFRigidBodyMotion::v()
{
  return motionState_.v();
}


inline mousse::vector& mousse::sixDoFRigidBodyMotion::a()
{
  return motionState_.a();
}


inline mousse::vector& mousse::sixDoFRigidBodyMotion::pi()
{
  return motionState_.pi();
}


inline mousse::vector& mousse::sixDoFRigidBodyMotion::tau()
{
  return motionState_.tau();
}


// Member Functions 
inline mousse::scalar mousse::sixDoFRigidBodyMotion::mass() const
{
  return mass_;
}


inline const mousse::diagTensor&
mousse::sixDoFRigidBodyMotion::momentOfInertia() const
{
  return momentOfInertia_;
}


inline const mousse::sixDoFRigidBodyMotionState&
mousse::sixDoFRigidBodyMotion::state() const
{
  return motionState_;
}


inline const mousse::point& mousse::sixDoFRigidBodyMotion::centreOfRotation() const
{
  return motionState_.centreOfRotation();
}


inline const mousse::point&
mousse::sixDoFRigidBodyMotion::initialCentreOfMass() const
{
  return initialCentreOfMass_;
}


inline mousse::point mousse::sixDoFRigidBodyMotion::centreOfMass() const
{
  return transform(initialCentreOfMass_);
}


inline mousse::vector mousse::sixDoFRigidBodyMotion::momentArm() const
{
  return centreOfMass() - motionState_.centreOfRotation();
}


inline const mousse::tensor&
mousse::sixDoFRigidBodyMotion::orientation() const
{
  return Q();
}


inline mousse::vector mousse::sixDoFRigidBodyMotion::omega() const
{
  return  Q() & (inv(momentOfInertia_) & pi());
}


inline bool mousse::sixDoFRigidBodyMotion::report() const
{
  return report_;
}


inline void mousse::sixDoFRigidBodyMotion::newTime()
{
  motionState0_ = motionState_;
}


inline mousse::point& mousse::sixDoFRigidBodyMotion::centreOfRotation()
{
  return motionState_.centreOfRotation();
}


inline mousse::point mousse::sixDoFRigidBodyMotion::velocity
(
  const point& pt
) const
{
  return (omega() ^ (pt - centreOfRotation())) + v();
}


inline mousse::point mousse::sixDoFRigidBodyMotion::transform
(
  const point& initialPoint
) const
{
  return (centreOfRotation()
          + (Q() & initialQ_.T() & (initialPoint - initialCentreOfRotation_)));
}

#endif

