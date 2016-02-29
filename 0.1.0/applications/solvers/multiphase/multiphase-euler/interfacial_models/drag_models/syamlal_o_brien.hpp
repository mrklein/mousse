// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModels::SyamlalOBrien
// Description
//   Syamlal, M., Rogers, W. and O'Brien, T. J. (1993) MFIX documentation,
//   Theory Guide. Technical Note DOE/METC-94/1004. Morgantown, West Virginia,
//   USA.
// SourceFiles
//   syamlal_o_brien.cpp
#ifndef SYAMLAL_O_BRIEN_HPP_
#define SYAMLAL_O_BRIEN_HPP_
#include "drag_model.hpp"
namespace mousse
{
namespace dragModels
{
class SyamlalOBrien
:
  public dragModel
{
public:
  //- Runtime type information
  TYPE_NAME("SyamlalOBrien");
  // Constructors
    //- Construct from components
    SyamlalOBrien
    (
      const dictionary& interfaceDict,
      const phaseModel& phase1,
      const phaseModel& phase2
    );
  //- Destructor
  virtual ~SyamlalOBrien();
  // Member Functions
    tmp<volScalarField> K(const volScalarField& Ur) const;
};
}  // namespace dragModels
}  // namespace mousse
#endif
