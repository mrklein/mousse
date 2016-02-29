#ifndef FINITE_VOLUME_CFD_TOOLS_GENERAL_SOLUTION_CONTROL_PISO_CONTROL_HPP_
#define FINITE_VOLUME_CFD_TOOLS_GENERAL_SOLUTION_CONTROL_PISO_CONTROL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pisoControl
// Description
//   Specialization of the pimpleControl class for PISO control.
#include "pimple_control.hpp"
namespace mousse
{
class pisoControl
:
  public pimpleControl
{
public:
  // Static Data Members
    //- Run-time type information
    TYPE_NAME("pisoControl");
  // Constructors
    //- Construct from mesh and the name of control sub-dictionary
    pisoControl(fvMesh& mesh, const word& dictName="PISO");
    //- Disallow default bitwise copy construct
    pisoControl(const pisoControl&) = delete;
    //- Disallow default bitwise assignment
    pisoControl& operator=(const pisoControl&) = delete;
  //- Destructor
  virtual ~pisoControl();
};
}  // namespace mousse
#endif
