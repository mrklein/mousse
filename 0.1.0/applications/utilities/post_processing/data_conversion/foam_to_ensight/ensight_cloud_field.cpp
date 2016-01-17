// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ensight_cloud_field.hpp"
#include "time.hpp"
#include "iofield.hpp"
#include "ofstream.hpp"
#include "iomanip.hpp"
using namespace mousse;
// Global Functions 
template<class Type>
void ensightCloudField
(
  const IOobject& fieldObject,
  const fileName& postProcPath,
  const word& prepend,
  const label timeIndex,
  const word& cloudName,
  Ostream& ensightCaseFile,
  const bool dataExists
)
{
  if (dataExists)
  {
    Info<< "Converting cloud " << cloudName
      << " field " << fieldObject.name() << endl;
  }
  else
  {
    Info<< "Creating empty cloud " << cloudName
      << " field "  << fieldObject.name() << endl;
  }
  word timeFile = prepend + itoa(timeIndex);
  const Time& runTime = fieldObject.time();
  if (timeIndex == 0 && Pstream::master())
  {
    ensightCaseFile
      << pTraits<Type>::typeName << " per measured node:      1       ";
    ensightCaseFile.width(15);
    ensightCaseFile.setf(ios_base::left);
    ensightCaseFile
      << ("c" + fieldObject.name()).c_str()
      << (' ' + prepend + "****." + cloudName
       + "." + fieldObject.name()).c_str()
      << nl;
  }
  fileName ensightFileName
  (
    timeFile + "." + cloudName +"." + fieldObject.name()
  );
  OFstream ensightFile
  (
    postProcPath/ensightFileName,
    runTime.writeFormat(),
    runTime.writeVersion(),
    runTime.writeCompression()
  );
  ensightFile<< pTraits<Type>::typeName << " values" << nl;
  if (dataExists)
  {
    IOField<Type> vf(fieldObject);
    ensightFile.setf(ios_base::scientific, ios_base::floatfield);
    ensightFile.precision(5);
    label count = 0;
    FOR_ALL(vf, i)
    {
      Type v = vf[i];
      if (mag(v) < 1.0e-90)
      {
        v = pTraits<Type>::zero;
      }
      for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
      {
        ensightFile << setw(12) << component(v, cmpt);
        if (++count % 6 == 0)
        {
          ensightFile << nl;
        }
      }
    }
    if ((count % 6 != 0) || (count==0))
    {
      ensightFile << nl;
    }
  }
}
