// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hePsiThermo
// Description
//   Energy for a mixture based on compressibility
// SourceFiles
//   he_psi_thermo.cpp
#ifndef he_psi_thermo_hpp_
#define he_psi_thermo_hpp_
#include "psi_thermo.hpp"
#include "he_thermo.hpp"
namespace mousse
{
template<class BasicPsiThermo, class MixtureType>
class hePsiThermo
:
  public heThermo<BasicPsiThermo, MixtureType>
{
  // Private Member Functions
    //- Calculate the thermo variables
    void calculate();
    //- Construct as copy (not implemented)
    hePsiThermo(const hePsiThermo<BasicPsiThermo, MixtureType>&);
public:
  //- Runtime type information
  TypeName("hePsiThermo");
  // Constructors
    //- Construct from mesh and phase name
    hePsiThermo
    (
      const fvMesh&,
      const word& phaseName
    );
  //- Destructor
  virtual ~hePsiThermo();
  // Member functions
    //- Update properties
    virtual void correct();
};
}  // namespace mousse
#ifdef NoRepository
#   include "he_psi_thermo.cpp"
#endif
#endif
