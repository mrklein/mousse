// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingMixture
// Description
//   mousse::reactingMixture
// SourceFiles
//   reacting_mixture.cpp
#ifndef reacting_mixture_hpp_
#define reacting_mixture_hpp_
#include "species_table.hpp"
#include "chemistry_reader.hpp"
#include "multi_component_mixture.hpp"
namespace mousse
{
template<class ThermoType>
class reactingMixture
:
  public speciesTable,
  public autoPtr<chemistryReader<ThermoType> >,
  public multiComponentMixture<ThermoType>,
  public PtrList<Reaction<ThermoType> >
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    reactingMixture(const reactingMixture&);
    //- Disallow default bitwise assignment
    void operator=(const reactingMixture&);
public:
  //- The type of thermo package this mixture is instantiated for
  typedef ThermoType thermoType;
  // Constructors
    //- Construct from dictionary, mesh and phase name
    reactingMixture(const dictionary&, const fvMesh&, const word&);
  //- Destructor
  virtual ~reactingMixture()
  {}
  // Member functions
    //- Read dictionary
    void read(const dictionary&);
    label size() const
    {
      return PtrList<Reaction<ThermoType> >::size();
    }
    Reaction<ThermoType>& operator [] (const label i)
    {
      return PtrList<Reaction<ThermoType> >::operator[](i);
    }
    const Reaction<ThermoType>& operator [] (const label i) const
    {
      return PtrList<Reaction<ThermoType> >::operator[](i);
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "reacting_mixture.cpp"
#endif
#endif
