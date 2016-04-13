#ifndef FINITE_VOLUME_CFD_TOOLS_GENERAL_SOLUTION_CONTROL_PIMPLE_CONTROL_HPP_
#define FINITE_VOLUME_CFD_TOOLS_GENERAL_SOLUTION_CONTROL_PIMPLE_CONTROL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pimpleControl
// Description
//   PIMPLE control class to supply convergence information/checks for
//   the PIMPLE loop.
//   May also be used to for PISO-based algorithms as PISO controls are a
//   sub-set of PIMPLE controls.

#include "solution_control.hpp"


namespace mousse {

class pimpleControl
:
  public solutionControl
{
protected:
  // Protected data
    // Solution controls
      //- Maximum number of PIMPLE correctors
      label nCorrPIMPLE_;
      //- Maximum number of PISO correctors
      label nCorrPISO_;
      //- Current PISO corrector
      label corrPISO_;
      //- Flag to indicate whether to only solve turbulence on final iter
      bool turbOnFinalIterOnly_;
      //- Converged flag
      bool converged_;
  // Protected Member Functions
    //- Read controls from fvSolution dictionary
    virtual void read();
    //- Return true if all convergence checks are satisfied
    virtual bool criteriaSatisfied();
public:
  // Static Data Members
    //- Run-time type information
    TYPE_NAME("pimpleControl");
  // Constructors
    //- Construct from mesh and the name of control sub-dictionary
    pimpleControl(fvMesh& mesh, const word& dictName="PIMPLE");
    //- Disallow default bitwise copy construct
    pimpleControl(const pimpleControl&) = delete;
    //- Disallow default bitwise assignment
    pimpleControl& operator=(const pimpleControl&) = delete;
  //- Destructor
  virtual ~pimpleControl();
  // Member Functions
    // Access
      //- Maximum number of PIMPLE correctors
      inline label nCorrPIMPLE() const;
      //- Maximum number of PISO correctors
      inline label nCorrPISO() const;
      //- Current PISO corrector index
      inline label corrPISO() const;
    // Solution control
      //- PIMPLE loop
      virtual bool loop();
      //- Pressure corrector loop
      inline bool correct();
      //- Helper function to identify when to store the intial residuals
      inline bool storeInitialResiduals() const;
      //- Helper function to identify first PIMPLE (outer) iteration
      inline bool firstIter() const;
      //- Helper function to identify final PIMPLE (outer) iteration
      inline bool finalIter() const;
      //- Helper function to identify final inner iteration
      inline bool finalInnerIter() const;
      //- Helper function to identify whether to solve for turbulence
      inline bool turbCorr() const;
};
}  // namespace mousse


// Member Functions 
inline mousse::label mousse::pimpleControl::nCorrPIMPLE() const
{
  return nCorrPIMPLE_;
}


inline mousse::label mousse::pimpleControl::nCorrPISO() const
{
  return nCorrPISO_;
}


inline mousse::label mousse::pimpleControl::corrPISO() const
{
  return corrPISO_;
}


inline bool mousse::pimpleControl::correct()
{
  corrPISO_++;
  if (debug) {
    Info << algorithmName_ << " correct: corrPISO = " << corrPISO_ << endl;
  }
  if (corrPISO_ <= nCorrPISO_) {
    return true;
  } else {
    corrPISO_ = 0;
    return false;
  }
}


inline bool mousse::pimpleControl::storeInitialResiduals() const
{
  // Start from second PIMPLE iteration
  return (corr_ == 2) && (corrPISO_ == 0) && (corrNonOrtho_ == 0);
}


inline bool mousse::pimpleControl::firstIter() const
{
  return corr_ == 1;
}


inline bool mousse::pimpleControl::finalIter() const
{
  return converged_ || (corr_ == nCorrPIMPLE_);
}


inline bool mousse::pimpleControl::finalInnerIter() const
{
  return (finalIter() && corrPISO_ == nCorrPISO_
          && corrNonOrtho_ == nNonOrthCorr_ + 1);
}


inline bool mousse::pimpleControl::turbCorr() const
{
  return !turbOnFinalIterOnly_ || finalIter();
}

#endif
