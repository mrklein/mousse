// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotentials::maitlandSmith
// Description
//   mousse::pairPotentials::maitlandSmith
//   From:
//   \verbatim
//     @ARTICLE{MA_Maitland_Smith,
//     author = {{Maitland}, G.~C. and {Smith}, E.~B.},
//     title = {A simplified representation of
//     intermolecular potential energy},
//     journal = {Chemical Physics Letters},
//     year = 1973,
//     month = oct,
//     volume = 22,
//     pages = {443-446},
//     adsurl = {http://adsabs.harvard.edu/abs/1973CPL....22..443M},
//     adsnote = {Provided by the SAO/NASA Astrophysics Data System}
//     }
//   \endverbatim
//   Parameters for other monoatomics from:
//   \verbatim
//     @BOOK{MD_Maitland_Rigby_Smith_Wakeham,
//     AUTHOR =       {Geoffrey C. Maitland and Maurice Rigby and
//     E. Brian Smith and William A. Wakeham},
//     TITLE =        {Intermolecular Forces: Their Origin and Determination},
//     PUBLISHER =    {Oxford University Press},
//     YEAR =         {1981}
//     }
//   \endverbatim
// SourceFiles
//   maitland_smith.cpp
#ifndef maitland_smith_hpp_
#define maitland_smith_hpp_
#include "pair_potential.hpp"
namespace mousse
{
namespace pairPotentials
{
class maitlandSmith
:
  public pairPotential
{
  // Private data
    dictionary maitlandSmithCoeffs_;
    scalar m_;
    scalar gamma_;
    scalar rm_;
    scalar epsilon_;
public:
  //- Runtime type information
  TypeName("maitlandSmith");
  // Constructors
    //- Construct from components
    maitlandSmith
    (
      const word& name,
      const dictionary& pairPotentialProperties
    );
  //- Destructor
  ~maitlandSmith()
  {}
  // Member Functions
    scalar unscaledEnergy(const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& pairPotentialProperties);
};
}  // namespace pairPotentials
}  // namespace mousse
#endif
