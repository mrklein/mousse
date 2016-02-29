#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_THIRD_BODY_EFFICIENCIES_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_THIRD_BODY_EFFICIENCIES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thirdBodyEfficiencies
// Description
//   Third body efficiencies
#include "scalar_list.hpp"
#include "species_table.hpp"
#include "tuple2.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class thirdBodyEfficiencies;
Ostream& operator<<(Ostream&, const thirdBodyEfficiencies&);
class thirdBodyEfficiencies
:
  public scalarList
{
  // Private data
    const speciesTable& species_;
public:
  // Constructors
    //- Construct from components
    inline thirdBodyEfficiencies
    (
      const speciesTable& species,
      const scalarList& efficiencies
    );
    //- Construct from Istream
    inline thirdBodyEfficiencies
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline thirdBodyEfficiencies
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member functions
    //- Calculate and return M, the concentration of the third-bodies
    inline scalar M(const scalarList& c) const;
    //- Write to stream
    inline void write(Ostream& os) const;
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const thirdBodyEfficiencies&);
};
}  // namespace mousse

// Constructors 
inline mousse::thirdBodyEfficiencies::thirdBodyEfficiencies
(
  const speciesTable& species,
  const scalarList& efficiencies
)
:
  scalarList{efficiencies},
  species_{species}
{
  if (size() != species_.size())
  {
    FATAL_ERROR_IN
    (
      "thirdBodyEfficiencies::thirdBodyEfficiencies"
      "(const speciesTable& species, const scalarList& efficiencies)"
    )
    << "number of efficiencies = " << size()
    << " is not equal to the number of species " << species_.size()
    << exit(FatalError);
  }
}
inline mousse::thirdBodyEfficiencies::thirdBodyEfficiencies
(
  const speciesTable& species,
  Istream& is
)
:
  scalarList{species.size()},
  species_{species}
{
  is.readBeginList
  (
    "thirdBodyEfficiencies::thirdBodyEfficiencies"
    "(const speciesTable& species, Istream& is)"
  );
  scalar defaultEff = readScalar(is);
  scalarList::operator=(defaultEff);
  token t;
  while ((is >> t) && !t.isPunctuation())
  {
    if (t.isWord())
    {
      operator[](species[t.wordToken()]) = readScalar(is);
    }
    else
    {
      FATAL_IO_ERROR_IN
      (
        "thirdBodyEfficiencies::thirdBodyEfficiencies"
        "(const speciesTable& species, Istream& is)",
        is
      )
      << "expected <word>, found " << t.info()
      << exit(FatalIOError);
    }
  }
  if (t.pToken() != token::END_LIST)
  {
    FATAL_IO_ERROR_IN
    (
      "thirdBodyEfficiencies::thirdBodyEfficiencies"
      "(const speciesTable& species, Istream& is)",
      is
    )
    << "expected ')', found " << t.info()
    << exit(FatalIOError);
  }
  if (size() != species_.size())
  {
    FATAL_IO_ERROR_IN
    (
      "thirdBodyEfficiencies::thirdBodyEfficiencies"
      "(const speciesTable& species, Istream& is)",
      is
    )
    << "number of efficiencies = " << size()
    << " is not equal to the number of species " << species_.size()
    << exit(FatalIOError);
  }
}
inline mousse::thirdBodyEfficiencies::thirdBodyEfficiencies
(
  const speciesTable& species,
  const dictionary& dict
)
:
  scalarList{species.size()},
  species_{species}
{
  if (dict.found("coeffs"))
  {
    List<Tuple2<word, scalar> > coeffs(dict.lookup("coeffs"));
    if (coeffs.size() != species_.size())
    {
      FATAL_ERROR_IN
      (
        "thirdBodyEfficiencies::thirdBodyEfficiencies"
        "(const speciesTable&, const dictionary&)"
      )
      << "number of efficiencies = " << coeffs.size()
      << " is not equat to the number of species " << species_.size()
      << exit(FatalIOError);
    }
    FOR_ALL(coeffs, i)
    {
      operator[](species[coeffs[i].first()]) = coeffs[i].second();
    }
  }
  else
  {
    scalar defaultEff = readScalar(dict.lookup("defaultEfficiency"));
    scalarList::operator=(defaultEff);
  }
}
// Member functions 
inline mousse::scalar mousse::thirdBodyEfficiencies::M(const scalarList& c) const
{
  scalar M = 0.0;
  FOR_ALL(*this, i)
  {
    M += operator[](i)*c[i];
  }
  return M;
}
inline void mousse::thirdBodyEfficiencies::write(Ostream& os) const
{
  List<Tuple2<word, scalar> > coeffs(species_.size());
  FOR_ALL(coeffs, i)
  {
    coeffs[i].first() = species_[i];
    coeffs[i].second() = operator[](i);
  }
  os.writeKeyword("coeffs") << coeffs << token::END_STATEMENT << nl;
}
// Ostream Operator 
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const thirdBodyEfficiencies& tbes
)
{
  scalarList orderedTbes = tbes;
  sort(orderedTbes);
  scalar val = orderedTbes[0];
  label count = 1;
  scalar valMaxCount = val;
  label maxCount = 1;
  for (label i=1; i<orderedTbes.size(); i++)
  {
    if (equal(orderedTbes[i], val))
    {
      count++;
    }
    else
    {
      if (count > maxCount)
      {
        maxCount = count;
        valMaxCount = val;
      }
      count = 1;
      val = orderedTbes[i];
    }
  }
  if (count > maxCount)
  {
    maxCount = count;
    valMaxCount = val;
  }
  os << token::BEGIN_LIST << valMaxCount;
  FOR_ALL(tbes, i)
  {
    if (notEqual(tbes[i], valMaxCount))
    {
      os  << token::SPACE << tbes.species_[i]
        << token::SPACE << tbes[i];
    }
  }
  os << token::END_LIST;
  return os;
}
#endif
