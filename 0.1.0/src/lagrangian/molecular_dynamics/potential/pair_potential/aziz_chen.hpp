#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_PAIR_POTENTIAL_AZIZ_CHEN_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_PAIR_POTENTIAL_AZIZ_CHEN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pairPotentials::azizChen
// Description
//   mousse::pairPotentials::azizChen
//   From:
//   \verbatim
//     \article{MA_Aziz_Chen,
//     author = {R. A. Aziz and H. H. Chen},
//     collaboration = {},
//     title = {An accurate intermolecular potential for argon},
//     publisher = {AIP},
//     year = {1977},
//     journal = {The Journal of Chemical Physics},
//     volume = {67},
//     number = {12},
//     pages = {5719-5726},
//     url = {http://link.aip.org/link/?JCP/67/5719/1},
//     doi = {10.1063/1.434827}
//     }
//   \endverbatim
// SourceFiles
//   aziz_chen.cpp
#include "pair_potential.hpp"
namespace mousse
{
namespace pairPotentials
{
class azizChen
:
  public pairPotential
{
  // Private data
    dictionary azizChenCoeffs_;
    scalar epsilon_;
    scalar rm_;
    scalar A_;
    scalar alpha_;
    scalar C6_;
    scalar C8_;
    scalar C10_;
    scalar D_;
    scalar gamma_;
public:
  //- Runtime type information
  TYPE_NAME("azizChen");
  // Constructors
    //- Construct from components
    azizChen
    (
      const word& name,
      const dictionary& pairPotentialProperties
    );
  //- Destructor
  ~azizChen()
  {}
  // Member Functions
    scalar unscaledEnergy(const scalar r) const;
    //- Read dictionary
    bool read(const dictionary& pairPotentialProperties);
};
}  // namespace pairPotentials
}  // namespace mousse
#endif
