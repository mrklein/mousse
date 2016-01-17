// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solution_control.hpp"

// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(solutionControl, 0);
}

// Protected Member Functions 
void mousse::solutionControl::read(const bool absTolOnly)
{
  const dictionary& solutionDict = this->dict();
  // Read solution controls
  nNonOrthCorr_ =
    solutionDict.lookupOrDefault<label>("nNonOrthogonalCorrectors", 0);
  momentumPredictor_ =
    solutionDict.lookupOrDefault("momentumPredictor", true);
  transonic_ = solutionDict.lookupOrDefault("transonic", false);
  consistent_ = solutionDict.lookupOrDefault("consistent", false);
  // Read residual information
  const dictionary residualDict
  (
    solutionDict.subOrEmptyDict("residualControl")
  );
  DynamicList<fieldData> data(residualControl_);
  FOR_ALL_CONST_ITER(dictionary, residualDict, iter)
  {
    const word& fName = iter().keyword();
    const label fieldI = applyToField(fName, false);
    if (fieldI == -1)
    {
      fieldData fd;
      fd.name = fName.c_str();
      if (absTolOnly)
      {
        fd.absTol = readScalar(residualDict.lookup(fName));
        fd.relTol = -1;
        fd.initialResidual = -1;
      }
      else
      {
        if (iter().isDict())
        {
          const dictionary& fieldDict(iter().dict());
          fd.absTol = readScalar(fieldDict.lookup("tolerance"));
          fd.relTol = readScalar(fieldDict.lookup("relTol"));
          fd.initialResidual = 0.0;
        }
        else
        {
          FATAL_ERROR_IN("bool mousse::solutionControl::read()")
            << "Residual data for " << iter().keyword()
            << " must be specified as a dictionary"
            << exit(FatalError);
        }
      }
      data.append(fd);
    }
    else
    {
      fieldData& fd = data[fieldI];
      if (absTolOnly)
      {
        fd.absTol = readScalar(residualDict.lookup(fName));
      }
      else
      {
        if (iter().isDict())
        {
          const dictionary& fieldDict(iter().dict());
          fd.absTol = readScalar(fieldDict.lookup("tolerance"));
          fd.relTol = readScalar(fieldDict.lookup("relTol"));
        }
        else
        {
          FATAL_ERROR_IN("bool mousse::solutionControl::read()")
            << "Residual data for " << iter().keyword()
            << " must be specified as a dictionary"
            << exit(FatalError);
        }
      }
    }
  }
  residualControl_.transfer(data);
  if (debug)
  {
    FOR_ALL(residualControl_, i)
    {
      const fieldData& fd = residualControl_[i];
      Info<< "residualControl[" << i << "]:" << nl
        << "    name     : " << fd.name << nl
        << "    absTol   : " << fd.absTol << nl
        << "    relTol   : " << fd.relTol << nl
        << "    iniResid : " << fd.initialResidual << endl;
    }
  }
}

void mousse::solutionControl::read()
{
  read(false);
}

mousse::label mousse::solutionControl::applyToField
(
  const word& fieldName,
  const bool useRegEx
) const
{
  FOR_ALL(residualControl_, i)
  {
    if (useRegEx && residualControl_[i].name.match(fieldName))
    {
      return i;
    }
    else if (residualControl_[i].name == fieldName)
    {
      return i;
    }
  }
  return -1;
}

void mousse::solutionControl::storePrevIterFields() const
{
//    storePrevIter<label>();
  storePrevIter<scalar>();
  storePrevIter<vector>();
  storePrevIter<sphericalTensor>();
  storePrevIter<symmTensor>();
  storePrevIter<tensor>();
}

// Constructors 
mousse::solutionControl::solutionControl(fvMesh& mesh, const word& algorithmName)
:
  IOobject
  {
    "solutionControl",
    mesh.time().timeName(),
    mesh
  },
  mesh_{mesh},
  residualControl_{},
  algorithmName_{algorithmName},
  nNonOrthCorr_{0},
  momentumPredictor_{true},
  transonic_{false},
  consistent_{false},
  corr_{0},
  corrNonOrtho_{0}
{}

// Destructor 
mousse::solutionControl::~solutionControl()
{}
