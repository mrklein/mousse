#ifndef SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_SOLVERS_SIX_DOF_SOLVER_SIX_DOF_SOLVER_HPP_
#define SIX_DOF_RIGID_BODY_MOTION_SIX_DOF_SOLVERS_SIX_DOF_SOLVER_SIX_DOF_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sixDoFSolver

#include "six_dof_rigid_body_motion.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {
class sixDoFSolver {

protected:
  // Protected data
    //- The rigid body
    sixDoFRigidBodyMotion& body_;
  // Protected member functions
    //- Return the current centre of rotation
    inline point& centreOfRotation();
    //- Return the orientation
    inline tensor& Q();
    //- Return non-const access to vector
    inline vector& v();
    //- Return non-const access to acceleration
    inline vector& a();
    //- Return non-const access to angular momentum
    inline vector& pi();
    //- Return non-const access to torque
    inline vector& tau();
    //- Return the centre of rotation at previous time-step
    inline const point& centreOfRotation0() const;
    //- Return the orientation at previous time-step
    inline const tensor& Q0() const;
    //- Return the velocity at previous time-step
    inline const vector& v0() const;
    //- Return the acceleration at previous time-step
    inline const vector& a0() const;
    //- Return the angular momentum at previous time-step
    inline const vector& pi0() const;
    //- Return the torque at previous time-step
    inline const vector& tau0() const;
    //- Acceleration damping coefficient (for steady-state simulations)
    inline scalar aDamp() const;
    //- Translational constraint tensor
    inline tensor tConstraints() const;
    //- Rotational constraint tensor
    inline tensor rConstraints() const;
    //- Apply rotation tensors to Q0 for the given torque (pi) and deltaT
    //  and return the rotated Q and pi as a tuple
    inline Tuple2<tensor, vector> rotate
    (
      const tensor& Q0,
      const vector& pi,
      const scalar deltaT
    ) const;
    //- Update and relax accelerations from the force and torque
    inline void updateAcceleration
    (
      const vector& fGlobal,
      const vector& tauGlobal
    );
public:
  //- Runtime type information
  TYPE_NAME("sixDoFSolver");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      sixDoFSolver,
      dictionary,
      (
        const dictionary& dict,
        sixDoFRigidBodyMotion& body
      ),
      (dict, body)
    );
  // Constructors
    // Construct for given body
    sixDoFSolver(sixDoFRigidBodyMotion& body);
  //- Destructor
  virtual ~sixDoFSolver();
  // Selectors
    static autoPtr<sixDoFSolver> New
    (
      const dictionary& dict,
      sixDoFRigidBodyMotion& body
    );
  // Member Functions
    //- Drag coefficient
    virtual void solve
    (
      bool firstIter,
      const vector& fGlobal,
      const vector& tauGlobal,
      scalar deltaT,
      scalar deltaT0
    ) = 0;
};

}  // namespace mousse


// Protected Member Functions 
inline mousse::point& mousse::sixDoFSolver::centreOfRotation()
{
  return body_.motionState_.centreOfRotation();
}


inline mousse::tensor& mousse::sixDoFSolver::Q()
{
  return body_.motionState_.Q();
}


inline mousse::vector& mousse::sixDoFSolver::v()
{
  return body_.motionState_.v();
}


inline mousse::vector& mousse::sixDoFSolver::a()
{
  return body_.motionState_.a();
}


inline mousse::vector& mousse::sixDoFSolver::pi()
{
  return body_.motionState_.pi();
}


inline mousse::vector& mousse::sixDoFSolver::tau()
{
  return body_.motionState_.tau();
}


inline const mousse::point& mousse::sixDoFSolver::centreOfRotation0() const
{
  return body_.motionState0_.centreOfRotation();
}


inline const mousse::tensor& mousse::sixDoFSolver::Q0() const
{
  return body_.motionState0_.Q();
}


inline const mousse::vector& mousse::sixDoFSolver::v0() const
{
  return body_.motionState0_.v();
}


inline const mousse::vector& mousse::sixDoFSolver::a0() const
{
  return body_.motionState0_.a();
}


inline const mousse::vector& mousse::sixDoFSolver::pi0() const
{
  return body_.motionState0_.pi();
}


inline const mousse::vector& mousse::sixDoFSolver::tau0() const
{
  return body_.motionState0_.tau();
}


inline mousse::scalar mousse::sixDoFSolver::aDamp() const
{
  return body_.aDamp_;
}


inline mousse::tensor mousse::sixDoFSolver::tConstraints() const
{
  return body_.tConstraints_;
}


inline mousse::tensor mousse::sixDoFSolver::rConstraints() const
{
  return body_.rConstraints_;
}


//- Apply rotation tensors to Q0 for the given torque (pi) and deltaT
//  and return the rotated Q and pi as a tuple
inline mousse::Tuple2<mousse::tensor, mousse::vector> mousse::sixDoFSolver::rotate
(
  const tensor& Q0,
  const vector& pi,
  const scalar deltaT
) const
{
  return body_.rotate(Q0, pi, deltaT);
}


//- Update and relax accelerations from the force and torque
inline void mousse::sixDoFSolver::updateAcceleration
(
  const vector& fGlobal,
  const vector& tauGlobal
)
{
  body_.updateAcceleration(fGlobal, tauGlobal);
}

#endif
