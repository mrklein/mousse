// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coal_cloud_list.hpp"
// Constructors 
mousse::coalCloudList::coalCloudList
(
  const volScalarField& rho,
  const volVectorField& U,
  const dimensionedVector& g,
  const SLGThermo& slgThermo
)
:
  PtrList<coalCloud>(),
  mesh_(rho.mesh())
{
  IOdictionary props
  (
    IOobject
    (
      "coalCloudList",
      mesh_.time().constant(),
      mesh_,
      IOobject::MUST_READ
    )
  );
  const wordHashSet cloudNames(wordList(props.lookup("clouds")));
  setSize(cloudNames.size());
  label i = 0;
  FOR_ALL_CONST_ITER(wordHashSet, cloudNames, iter)
  {
    const word& name = iter.key();
    Info<< "creating cloud: " << name << endl;
    set
    (
      i++,
      new coalCloud
      (
        name,
        rho,
        U,
        g,
        slgThermo
      )
    );
    Info<< endl;
  }
}
// Member Functions 
void mousse::coalCloudList::evolve()
{
  FOR_ALL(*this, i)
  {
    operator[](i).evolve();
  }
}
