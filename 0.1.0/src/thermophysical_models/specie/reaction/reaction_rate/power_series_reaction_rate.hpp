// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::powerSeriesReactionRate
// Description
//   Power series reaction rate.
// SourceFiles
//   power_series_reaction_rate_i.hpp
#ifndef power_series_reaction_rate_hpp_
#define power_series_reaction_rate_hpp_
#include "scalar_field.hpp"
#include "type_info.hpp"
#include "fixed_list.hpp"
namespace mousse
{
class powerSeriesReactionRate
{
  // Private data
    scalar A_;
    scalar beta_;
    scalar Ta_;
    static const label nCoeff_ = 4;
    FixedList<scalar, nCoeff_> coeffs_;
public:
  // Constructors
    //- Construct from components
    inline powerSeriesReactionRate
    (
      const scalar A,
      const scalar beta,
      const scalar Ta,
      const FixedList<scalar, nCoeff_> coeffs
    );
    //- Construct from Istream
    inline powerSeriesReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline powerSeriesReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "powerSeries";
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
      const powerSeriesReactionRate&
    );
};
}  // namespace mousse
#include "power_series_reaction_rate_i.hpp"
#endif
