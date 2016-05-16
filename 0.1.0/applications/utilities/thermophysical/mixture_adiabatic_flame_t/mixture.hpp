#ifndef mixture_hpp_
#define mixture_hpp_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mixture

#include "error.hpp"
#include "list.hpp"
#include "substance.hpp"


namespace mousse {

class mixture
:
  public List<substance>
{
  // Private data
    word name_;
public:
  // Constructors
    //- Construct from Istream
    mixture(Istream& is)
    :
      List<substance>{is},
      name_{is}
    {
      scalar volTot = 0;
      for (label i = 0; i < size(); i++) {
        volTot += operator[](i).volFrac();
      }
      if (volTot > 1.001 || volTot < 0.999) {
        FatalErrorIn("mixture::mixture(istream& is)")
          << "Sum of volume fractions for Mixture " << name_
          << " = " << volTot << endl
          << "Should equal one."
          << abort(FatalError);
      }
    }
  // Member Functions
    // Access
      const word& name() const { return name_; }
};

}  // namespace mousse

#endif

