// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diameterModels::IATE
// Description
//   IATE (Interfacial Area Transport Equation) bubble diameter model.
//   Solves for the interfacial curvature per unit volume of the phase rather
//   than interfacial area per unit volume to avoid stability issues relating to
//   the consistency requirements between the phase fraction and interfacial area
//   per unit volume.  In every other respect this model is as presented in the
//   paper:
//   \verbatim
//     "Development of Interfacial Area Transport Equation"
//     Ishii, M., Kim, S. and Kelly, J.,
//     Nuclear Engineering and Technology, Vol.37 No.6 December 2005
//   \endverbatim
// SourceFiles
//   iate.cpp
#ifndef iate_hpp_
#define iate_hpp_
#include "diameter_model.hpp"
namespace mousse
{
namespace diameterModels
{
// Forward declaration of classes
class IATEsource;
class IATE
:
  public diameterModel
{
  // Private data
    //- Interfacial curvature (alpha*interfacial area)
    volScalarField kappai_;
    //- Maximum diameter used for stabilisation in the limit kappai->0
    dimensionedScalar dMax_;
    //- Minimum diameter used for stabilisation in the limit kappai->inf
    dimensionedScalar dMin_;
    //- Residual phase fraction
    dimensionedScalar residualAlpha_;
    //- The Sauter-mean diameter of the phase
    volScalarField d_;
    //- IATE sources
    PtrList<IATEsource> sources_;
  // Private member functions
    tmp<volScalarField> dsm() const;
public:
  friend class IATEsource;
  //- Runtime type information
  TYPE_NAME("IATE");
  // Constructors
    //- Construct from components
    IATE
    (
      const dictionary& diameterProperties,
      const phaseModel& phase
    );
  //- Destructor
  virtual ~IATE();
  // Member Functions
    //- Return the interfacial curvature
    const volScalarField& kappai() const
    {
      return kappai_;
    }
    //- Return the interfacial area
    tmp<volScalarField> a() const
    {
      return phase_*kappai_;
    }
    //- Return the Sauter-mean diameter
    virtual tmp<volScalarField> d() const
    {
      return d_;
    }
    //- Correct the diameter field
    virtual void correct();
    //- Read phaseProperties dictionary
    virtual bool read(const dictionary& phaseProperties);
};
}  // namespace diameterModels
}  // namespace mousse
#endif
