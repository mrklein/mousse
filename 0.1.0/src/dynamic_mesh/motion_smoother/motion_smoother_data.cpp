// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "motion_smoother_data.hpp"


// Constructors 
mousse::motionSmootherData::motionSmootherData(const pointMesh& pMesh)
:
  displacement_
  {
    {
      "displacement",
      pMesh.time().timeName(),
      pMesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    pMesh
  },
  scale_
  {
    {
      "scale",
      pMesh.time().timeName(),
      pMesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    pMesh,
    {"scale", dimless, 1.0}
  },
  oldPoints_(pMesh().points())
{}


mousse::motionSmootherData::motionSmootherData
(
  const pointVectorField& displacement
)
:
  displacement_
  {
    {
      "displacement",
      displacement.time().timeName(),
      displacement.mesh()(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    displacement
  },
  scale_
  {
    {
      "scale",
      displacement.time().timeName(),
      displacement.mesh()(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    displacement.mesh(),
    {"scale", dimless, 1.0}
  },
  oldPoints_{displacement.mesh()().points()}
{}


// Member Functions 
mousse::pointVectorField& mousse::motionSmootherData::displacement()
{
  return displacement_;
}


const mousse::pointVectorField& mousse::motionSmootherData::displacement() const
{
  return displacement_;
}


const mousse::pointScalarField& mousse::motionSmootherData::scale() const
{
  return scale_;
}


const mousse::pointField& mousse::motionSmootherData::oldPoints() const
{
  return oldPoints_;
}

