#ifndef FINITE_VOLUME_CFD_TOOLS_GENERAL_SOLUTION_CONTROL_SIMPLE_CONTROL_HPP_
#define FINITE_VOLUME_CFD_TOOLS_GENERAL_SOLUTION_CONTROL_SIMPLE_CONTROL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::simpleControl
// Description
//   SIMPLE control class to supply convergence information/checks for
//   the SIMPLE loop.

#include "solution_control.hpp"


namespace mousse {

class simpleControl
:
  public solutionControl
{
protected:
  // Protected Data
    //- Initialised flag
    bool initialised_;
  // Protected Member Functions
    //- Read controls from fvSolution dictionary
    void read();
    //- Return true if all convergence checks are satisfied
    bool criteriaSatisfied();
public:
  // Static Data Members
    //- Run-time type information
    TYPE_NAME("simpleControl");
  // Constructors
    //- Construct from mesh
    simpleControl(fvMesh& mesh);
    //- Disallow default bitwise copy construct
    simpleControl(const simpleControl&) = delete;
    //- Disallow default bitwise assignment
    simpleControl& operator=(const simpleControl&) = delete;
  //- Destructor
  virtual ~simpleControl();
  // Member Functions
    // Solution control
      //- Loop loop
      virtual bool loop();
};
}  // namespace mousse
#endif
