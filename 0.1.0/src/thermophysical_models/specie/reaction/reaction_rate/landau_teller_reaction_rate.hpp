// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LandauTellerReactionRate
// Description
//   Landau-Teller reaction rate.
// SourceFiles
//   landau_teller_reaction_rate_i.hpp
#ifndef landau_teller_reaction_rate_hpp_
#define landau_teller_reaction_rate_hpp_
#include "scalar_field.hpp"
#include "type_info.hpp"
namespace mousse
{
class LandauTellerReactionRate
{
  // Private data
    scalar A_;
    scalar beta_;
    scalar Ta_;
    scalar B_;
    scalar C_;
public:
  // Constructors
    //- Construct from components
    inline LandauTellerReactionRate
    (
      const scalar A,
      const scalar beta,
      const scalar Ta,
      const scalar B,
      const scalar C
    );
    //- Construct from Istream
    inline LandauTellerReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline LandauTellerReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "LandauTeller";
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
      const LandauTellerReactionRate&
    );
};
}  // namespace mousse
#include "landau_teller_reaction_rate_i.hpp"
#endif
