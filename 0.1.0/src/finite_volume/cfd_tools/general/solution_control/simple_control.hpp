// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::simpleControl
// Description
//   SIMPLE control class to supply convergence information/checks for
//   the SIMPLE loop.
#ifndef simple_control_hpp_
#define simple_control_hpp_
#include "solution_control.hpp"
namespace mousse
{
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
    //- Disallow default bitwise copy construct
    simpleControl(const simpleControl&);
    //- Disallow default bitwise assignment
    void operator=(const simpleControl&);
public:
  // Static Data Members
    //- Run-time type information
    TypeName("simpleControl");
  // Constructors
    //- Construct from mesh
    simpleControl(fvMesh& mesh);
  //- Destructor
  virtual ~simpleControl();
  // Member Functions
    // Solution control
      //- Loop loop
      virtual bool loop();
};
}  // namespace mousse
#endif
