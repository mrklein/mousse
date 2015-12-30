// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LangmuirHinshelwoodReactionRate
// Description
//   Power series reaction rate.
// SourceFiles
//   langmuir_hinshelwood_reaction_rate_i.hpp
#ifndef langmuir_hinshelwood_reaction_rate_hpp_
#define langmuir_hinshelwood_reaction_rate_hpp_
#include "scalar_field.hpp"
#include "type_info.hpp"
namespace mousse
{
class LangmuirHinshelwoodReactionRate
{
  // Private data
    static const label n_ = 5;
    scalar A_[n_];
    scalar Ta_[n_];
    label co_;
    label c3h6_;
    label no_;
public:
  // Constructors
    //- Construct from components
    inline LangmuirHinshelwoodReactionRate
    (
      const scalar A[],
      const scalar Ta[],
      const label co,
      const label c3h6,
      const label no
    );
    //- Construct from Istream
    inline LangmuirHinshelwoodReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline LangmuirHinshelwoodReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "LangmuirHinshelwood";
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
      const LangmuirHinshelwoodReactionRate&
    );
};
}  // namespace mousse
#include "langmuir_hinshelwood_reaction_rate_i.hpp"
#endif
