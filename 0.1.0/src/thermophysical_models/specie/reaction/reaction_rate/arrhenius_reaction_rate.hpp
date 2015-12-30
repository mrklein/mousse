// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ArrheniusReactionRate
// Description
//   Arrhenius reaction rate given by:
//     k = A * T^beta * exp(-Ta/T)
// SourceFiles
//   arrhenius_reaction_rate_i.hpp
#ifndef arrhenius_reaction_rate_hpp_
#define arrhenius_reaction_rate_hpp_
#include "scalar_field.hpp"
#include "type_info.hpp"
namespace mousse
{
class ArrheniusReactionRate
{
  // Private data
    scalar A_;
    scalar beta_;
    scalar Ta_;
public:
  // Constructors
    //- Construct from components
    inline ArrheniusReactionRate
    (
      const scalar A,
      const scalar beta,
      const scalar Ta
    );
    //- Construct from Istream
    inline ArrheniusReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline ArrheniusReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "Arrhenius";
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
      const ArrheniusReactionRate&
    );
};
}  // namespace mousse
#include "arrhenius_reaction_rate_i.hpp"
#endif
