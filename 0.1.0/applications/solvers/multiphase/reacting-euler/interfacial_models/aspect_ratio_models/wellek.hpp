// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::aspectRatioModels::Wellek
// Description
//   Aspect ratio model of Wellek et al.
//   References:
//   \verbatim
//     "Implementation and Comparison of Correlations for interfacial Forces
//     in a Gas-Liquid System within an Euler-Euler Framework"
//     Otromke, M.,
//     PhD Thesis, April 2013
//   \endverbatim
//   \verbatim
//     "Shape of liquid drops moving in liquid media"
//     Wellek, R.M., Agrawal, A.K., Skelland, A.H.P.,
//     International Journal of Multiphase Flow
//     Volume 12, Issue 5, September 1966, pp. 854-862
//   \endverbatim
// SourceFiles
//   wellek.cpp
#ifndef wellek_hpp_
#define wellek_hpp_
#include "aspect_ratio_model.hpp"
namespace mousse
{
namespace aspectRatioModels
{
class Wellek
:
  public aspectRatioModel
{
public:
  //- Runtime type information
  TYPE_NAME("Wellek");
  // Constructors
    //- Construct from a dictionary and an ordered phase pair
    Wellek
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~Wellek();
  // Member Functions
    //- Aspect ratio
    virtual tmp<volScalarField> E() const;
};
}  // namespace aspectRatioModels
}  // namespace mousse
#endif
