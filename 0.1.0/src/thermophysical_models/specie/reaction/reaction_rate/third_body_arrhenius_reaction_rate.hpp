// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thirdBodyArrheniusReactionRate
// Description
//   Arrhenius reaction rate enhanced by third-body interation.
// SourceFiles
//   third_body_arrhenius_reaction_rate_i.hpp
#ifndef third_body_arrhenius_reaction_rate_hpp_
#define third_body_arrhenius_reaction_rate_hpp_
#include "arrhenius_reaction_rate.hpp"
#include "third_body_efficiencies.hpp"
namespace mousse
{
class thirdBodyArrheniusReactionRate
:
  ArrheniusReactionRate
{
  // Private data
    thirdBodyEfficiencies thirdBodyEfficiencies_;
public:
  // Constructors
    //- Construct from components
    inline thirdBodyArrheniusReactionRate
    (
      const scalar A,
      const scalar beta,
      const scalar Ta,
      const thirdBodyEfficiencies& tbes
    );
    //- Construct from Istream
    inline thirdBodyArrheniusReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline thirdBodyArrheniusReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "thirdBodyArrhenius";
    }
    inline scalar operator()
    (
      const scalar p,
      const scalar T,
      const scalarField& c
    ) const;
    //- Write to stream
    inline void write(Ostream& os) const;
  // Ostream Operator
    inline friend Ostream& operator<<
    (
      Ostream&,
      const thirdBodyArrheniusReactionRate&
    );
};
}  // namespace mousse
#include "third_body_arrhenius_reaction_rate_i.hpp"
#endif
