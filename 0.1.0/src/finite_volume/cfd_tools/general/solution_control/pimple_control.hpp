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
#ifndef pimple_control_hpp_
#define pimple_control_hpp_
#include "solution_control.hpp"
namespace mousse
{
class pimpleControl
:
  public solutionControl
{
  // Private member functions
    //- Disallow default bitwise copy construct
    pimpleControl(const pimpleControl&);
    //- Disallow default bitwise assignment
    void operator=(const pimpleControl&);
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
    TypeName("pimpleControl");
  // Constructors
    //- Construct from mesh and the name of control sub-dictionary
    pimpleControl(fvMesh& mesh, const word& dictName="PIMPLE");
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
#include "pimple_control_i.hpp"
#endif
