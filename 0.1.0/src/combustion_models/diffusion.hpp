// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::combustionModels::diffusion
// Description
//   Simple diffusion-based combustion model based on the principle mixed is
//   burnt. Additional parameter C is used to distribute the heat release rate
//   in time.
// SourceFiles
//   diffusion.cpp
#ifndef diffusion_hpp_
#define diffusion_hpp_
#include "single_step_combustion.hpp"
namespace mousse
{
namespace combustionModels
{
template<class CombThermoType, class ThermoType>
class diffusion
:
  public singleStepCombustion<CombThermoType, ThermoType>
{
  // Private data
    //- Model constant
    scalar C_;
    //- Name of oxidant - default is "O2"
    word oxidantName_;
  // Private Member Functions
    //- Disallow copy construct
    diffusion(const diffusion&);
    //- Disallow default bitwise assignment
    void operator=(const diffusion&);
public:
  //- Runtime type information
  TypeName("diffusion");
  // Constructors
    //- Construct from components
    diffusion
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
  //- Destructor
  virtual ~diffusion();
  // Member Functions
    // Evolution
      //- Correct combustion rate
      virtual void correct();
    // IO
      //- Update properties
      virtual bool read();
};
}  // namespace combustionModels
}  // namespace mousse
#ifdef NoRepository
#   include "diffusion.cpp"
#endif
#endif
