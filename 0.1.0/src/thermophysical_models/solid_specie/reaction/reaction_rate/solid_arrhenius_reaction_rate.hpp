// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidArrheniusReactionRate
// Description
//   Arrhenius reaction rate for solids
// SourceFiles
//   solid_arrhenius_reaction_rate_i.hpp
#ifndef solid_arrhenius_reaction_rate_hpp_
#define solid_arrhenius_reaction_rate_hpp_
#include "scalar_field.hpp"
#include "type_info.hpp"
namespace mousse
{
class solidArrheniusReactionRate
{
  // Private data
    scalar A_;
    scalar Ta_;
    scalar Tcrit_;
public:
  // Constructors
    //- Construct from components
    inline solidArrheniusReactionRate
    (
      const scalar A,
      const scalar Ta,
      const scalar Tcrit
    );
    //- Construct from Istream
    inline solidArrheniusReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline solidArrheniusReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  //- Destructor
  virtual ~solidArrheniusReactionRate()
  {}
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
      const solidArrheniusReactionRate&
    );
};
}  // namespace mousse
#include "solid_arrhenius_reaction_rate_i.hpp"
#endif
