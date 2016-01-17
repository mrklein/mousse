// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ensight_output_functions.hpp"
#include "passive_particle.hpp"
#include "iofield.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "ofstream.hpp"
#include "iomanip.hpp"
// Global Functions 
void mousse::ensightCaseEntry
(
  OFstream& caseFile,
  const string& ensightType,
  const word& fieldName,
  const fileName& dataMask,
  const fileName& local,
  const label cloudNo,
  const label timeSet
)
{
  caseFile.setf(ios_base::left);
  fileName dirName{dataMask};
  if (local.size())
  {
    dirName = dirName/local;
  }
  if (cloudNo >= 0)
  {
    label ts = 1;
    if (timeSet > ts)
    {
      ts = timeSet;
    }
    // prefix variables with 'c' (cloud)
    caseFile
      << ensightType.c_str()
      << " per measured node: " << ts << " "
      << setw(15)
      << ("c" + mousse::name(cloudNo) + fieldName).c_str()
      << " "
      << (dirName/fieldName).c_str()
      << nl;
  }
  else
  {
    caseFile
      << ensightType.c_str()
      << " per element: "
      << setw(15) << fieldName
      << " "
      << (dirName/fieldName).c_str()
      << nl;
  }
}
void mousse::ensightParticlePositions
(
  const polyMesh& mesh,
  const fileName& dataDir,
  const fileName& subDir,
  const word& cloudName,
  IOstream::streamFormat format
)
{
  Cloud<passiveParticle> parcels{mesh, cloudName, false};
  fileName cloudDir = subDir/cloud::prefix/cloudName;
  fileName postFileName = cloudDir/"positions";
  // the ITER/lagrangian subdirectory must exist
  mkDir(dataDir/cloudDir);
  ensightFile os{dataDir/postFileName, format};
  // tag binary format (just like geometry files)
  os.writeBinaryHeader();
  os.write(postFileName);
  os.newline();
  os.write("particle coordinates");
  os.newline();
  os.write(parcels.size(), 8);   // unusual width
  os.newline();
  // binary write is Ensight6 - first ids, then positions
  if (format == IOstream::BINARY)
  {
    FOR_ALL(parcels, i)
    {
      os.write(i+1);
    }
    FOR_ALL_CONST_ITER(Cloud<passiveParticle>, parcels, elmnt)
    {
      const vector& p = elmnt().position();
      os.write(p.x());
      os.write(p.y());
      os.write(p.z());
    }
  }
  else
  {
    label nParcels = 0;
    FOR_ALL_CONST_ITER(Cloud<passiveParticle>, parcels, elmnt)
    {
      const vector& p = elmnt().position();
      os.write(++nParcels, 8);    // unusual width
      os.write(p.x());
      os.write(p.y());
      os.write(p.z());
      os.newline();
    }
  }
}
template<class Type>
void mousse::ensightLagrangianField
(
  const IOobject& fieldObject,
  const fileName& dataDir,
  const fileName& subDir,
  const word& cloudName,
  IOstream::streamFormat format
)
{
  Info<< " " << fieldObject.name() << flush;
  fileName cloudDir = subDir/cloud::prefix/cloudName;
  fileName postFileName = cloudDir/fieldObject.name();
  string title = postFileName + " with " + pTraits<Type>::typeName + " values";
  ensightFile os{dataDir/postFileName, format};
  os.write(title);
  os.newline();
  IOField<Type> field{fieldObject};
  // 6 values per line
  label count = 0;
  FOR_ALL(field, i)
  {
    Type val = field[i];
    if (mag(val) < 1.0e-90)
    {
      val = pTraits<Type>::zero;
    }
    for (direction cmpt=0; cmpt < pTraits<Type>::nComponents; cmpt++)
    {
      os.write( component(val, cmpt) );
    }
    count += pTraits<Type>::nComponents;
    if (count % 6 == 0)
    {
      os.newline();
    }
  }
  // add final newline if required
  if (count % 6)
  {
    os.newline();
  }
}
//- Write generalized field components
template<class Type>
void mousse::ensightVolField
(
  const ensightParts& partsList,
  const IOobject& fieldObject,
  const fvMesh& mesh,
  const fileName& dataDir,
  const fileName& subDir,
  IOstream::streamFormat format
)
{
  Info<< " " << fieldObject.name() << flush;
  fileName postFileName = subDir/fieldObject.name();
  ensightFile os{dataDir/postFileName, format};
  os.write(postFileName);
  os.newline();
  // ie, volField<Type>
  partsList.writeField
  (
    os,
    GeometricField<Type, fvPatchField, volMesh>
    (
      fieldObject,
      mesh
    )
  );
}
