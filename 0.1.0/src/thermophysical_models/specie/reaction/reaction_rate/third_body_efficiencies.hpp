// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thirdBodyEfficiencies
// Description
//   Third body efficiencies
// SourceFiles
//   third_body_efficiencies_i.hpp
#ifndef third_body_efficiencies_hpp_
#define third_body_efficiencies_hpp_
#include "scalar_list.hpp"
#include "species_table.hpp"
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
#include "third_body_efficiencies_i.hpp"
#endif
