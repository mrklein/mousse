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
public:
  // Constructors
    //- Construct from Istream
    valveBank
    (
      const polyMesh& mesh,
      Istream& is
    )
    {
      PtrList<entry> valveEntries{is};
      setSize(valveEntries.size());
      FOR_ALL(valveEntries, valveI)
      {
        valveI,
        set
        (
          new engineValve
          {
            valveEntries[valveI].keyword(),
            mesh,
            valveEntries[valveI].dict()
          }
        );
      }
    }
    //- Disallow default bitwise copy construct
    valveBank(const valveBank&) = delete;
    //- Disallow default bitwise assignment
    valveBank& operator=(const valveBank&) = delete;
  // Destructor - default
  // Member Functions
};
}  // namespace mousse
#endif
