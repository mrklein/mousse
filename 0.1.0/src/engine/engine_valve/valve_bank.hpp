// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::valveBank
// Description
//   A list of valves.
#ifndef valve_bank_hpp_
#define valve_bank_hpp_
#include "ptr_list.hpp"
#include "engine_valve.hpp"
namespace mousse
{
class valveBank
:
  public PtrList<engineValve>
{
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    valveBank(const valveBank&);
    //- Disallow default bitwise assignment
    void operator=(const valveBank&);
public:
  // Constructors
    //- Construct from Istream
    valveBank
    (
      const polyMesh& mesh,
      Istream& is
    )
    {
      PtrList<entry> valveEntries(is);
      setSize(valveEntries.size());
      forAll(valveEntries, valveI)
      {
        valveI,
        set
        (
          new engineValve
          (
            valveEntries[valveI].keyword(),
            mesh,
            valveEntries[valveI].dict()
          )
        );
      }
    }
  // Destructor - default
  // Member Functions
};
}  // namespace mousse
#endif
