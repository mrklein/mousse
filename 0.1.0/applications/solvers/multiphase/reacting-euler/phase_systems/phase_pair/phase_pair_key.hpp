// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phasePairKey
// Description
// SourceFiles
#ifndef phase_pair_key_hpp_
#define phase_pair_key_hpp_
#include "pair.hpp"
namespace mousse
{
class phasePairKey
:
  public Pair<word>
{
public:
    class hash
    :
      public Hash<phasePairKey>
    {
    public:
      // Constructors
        // Construct null
        hash();
      // Member operators
        // Generate a hash from a phase pair key
        label operator()(const phasePairKey& key) const;
    };
private:
  // Private data
    //- Flag to indicate whether ordering is important
    bool ordered_;
public:
  // Constructors
    //- Construct null
    phasePairKey();
    //- Construct from names and the ordering flag
    phasePairKey
    (
      const word& name1,
      const word& name2,
      const bool ordered = false
    );
  // Destructor
  virtual ~phasePairKey();
  // Access
    //- Return the ordered flag
    bool ordered() const;
  // Friend Operators
    //- Test if keys are equal
    friend bool operator==(const phasePairKey& a, const phasePairKey& b);
    //- Test if keys are unequal
    friend bool operator!=(const phasePairKey& a, const phasePairKey& b);
    //- Read from stdin
    friend Istream& operator>>(Istream& is, phasePairKey& key);
    //- Write to stdout
    friend Ostream& operator<<(Ostream& os, const phasePairKey& key);
};
}  // namespace mousse
#endif
