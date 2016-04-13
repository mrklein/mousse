// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "cloud.hpp"
#include "iodictionary.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "time_selector.hpp"
#include "ofstream.hpp"
#include "passive_particle_cloud.hpp"
#include "sortable_list.hpp"
#include "ioobject_list.hpp"
#include "ptr_list.hpp"
#include "field.hpp"
#include "steady_particle_tracks_templates.hpp"

using namespace mousse;

namespace mousse
{
label validateFields
(
  const List<word>& userFields,
  const IOobjectList& cloudObjs
)
{
  List<bool> ok{userFields.size(), false};
  FOR_ALL(userFields, i)
  {
    ok[i] = ok[i] || fieldOk<label>(cloudObjs, userFields[i]);
    ok[i] = ok[i] || fieldOk<scalar>(cloudObjs, userFields[i]);
    ok[i] = ok[i] || fieldOk<vector>(cloudObjs, userFields[i]);
    ok[i] = ok[i] || fieldOk<sphericalTensor>(cloudObjs, userFields[i]);
    ok[i] = ok[i] || fieldOk<symmTensor>(cloudObjs, userFields[i]);
    ok[i] = ok[i] || fieldOk<tensor>(cloudObjs, userFields[i]);
  }
  label nOk = 0;
  FOR_ALL(ok, i)
  {
    if (ok[i])
    {
      nOk++;
    }
    else
    {
      Info << "\n*** Warning: user specified field '" << userFields[i]
        << "' unavailable" << endl;
    }
  }
  return nOk;
}
template<>
void writeVTK(OFstream& os, const label& value)
{
  os << value;
}
template<>
void writeVTK(OFstream& os, const scalar& value)
{
  os << value;
}
}
int main(int argc, char *argv[])
{
  argList::noParallel();
  timeSelector::addOptions();
  #include "add_region_option.inc"
  #include "add_dict_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  #include "create_fields.inc"

  fileName vtkPath{runTime.path()/"VTK"};
  mkDir(vtkPath);
  typedef HashTable<label, labelPair, labelPair::Hash<>> trackTableType;
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    fileName vtkTimePath{runTime.path()/"VTK"/runTime.timeName()};
    mkDir(vtkTimePath);
    Info << "    Reading particle positions" << endl;
    PtrList<passiveParticle> particles{0};

    // transfer particles to (more convenient) list
    {
      passiveParticleCloud ppc{mesh, cloudName};
      Info << "\n    Read " << returnReduce(ppc.size(), sumOp<label>())
        << " particles" << endl;
      particles.setSize(ppc.size());
      label i = 0;
      FOR_ALL_ITER(passiveParticleCloud, ppc, iter)
      {
        particles.set(i++, ppc.remove(&iter()));
      }
      // myCloud should now be empty
    }
    List<label> particleToTrack{particles.size()};
    label nTracks = 0;

    {
      trackTableType trackTable;
      FOR_ALL(particles, i)
      {
        const label origProc = particles[i].origProc();
        const label origId = particles[i].origId();
        const trackTableType::const_iterator& iter =
          trackTable.find(labelPair(origProc, origId));
        if (iter == trackTable.end())
        {
          particleToTrack[i] = nTracks;
          trackTable.insert(labelPair(origProc, origId), nTracks);
          nTracks++;
        }
        else
        {
          particleToTrack[i] = iter();
        }
      }
    }
    if (nTracks == 0)
    {
      Info << "\n    No track data" << endl;
    }
    else
    {
      Info << "\n    Generating " << nTracks << " tracks" << endl;
      // determine length of each track
      labelList trackLengths{nTracks, 0};
      FOR_ALL(particleToTrack, i)
      {
        const label trackI = particleToTrack[i];
        trackLengths[trackI]++;
      }
      // particle "age" property used to sort the tracks
      List<SortableList<scalar>> agePerTrack{nTracks};
      List<List<label>> particleMap{nTracks};
      FOR_ALL(trackLengths, i)
      {
        const label length = trackLengths[i];
        agePerTrack[i].setSize(length);
        particleMap[i].setSize(length);
      }
      // store the particle age per track
      IOobjectList cloudObjs
      {
        mesh,
        runTime.timeName(),
        cloud::prefix/cloudName
      };

      // TODO: gather age across all procs
      {
        tmp<scalarField> tage = readParticleField<scalar>("age", cloudObjs);
        const scalarField& age = tage();
        List<label> trackSamples{nTracks, 0};
        FOR_ALL(particleToTrack, i)
        {
          const label trackI = particleToTrack[i];
          const label sampleI = trackSamples[trackI];
          agePerTrack[trackI][sampleI] = age[i];
          particleMap[trackI][sampleI] = i;
          trackSamples[trackI]++;
        }
        tage.clear();
      }
      if (Pstream::master())
      {
        OFstream os{vtkTimePath/"particleTracks.vtk"};
        Info << "\n    Writing particle tracks to " << os.name() << endl;
        label nPoints = sum(trackLengths);
        os << "# vtk DataFile Version 2.0" << nl
          << "particleTracks" << nl
          << "ASCII" << nl
          << "DATASET POLYDATA" << nl
          << "POINTS " << nPoints << " float" << nl;
        Info << "\n    Writing points" << endl;

        {
          FOR_ALL(agePerTrack, i)
          {
            agePerTrack[i].sort();
            const labelList& ids = agePerTrack[i].indices();
            labelList& particleIds = particleMap[i];

            {
              // update addressing
              List<label> sortedIds{ids};
              FOR_ALL(sortedIds, j)
              {
                sortedIds[j] = particleIds[ids[j]];
              }
              particleIds = sortedIds;
            }

            FOR_ALL(ids, j)
            {
              const label localId = particleIds[j];
              const vector& pos = particles[localId].position();
              os << pos.x() << ' ' << pos.y() << ' ' << pos.z()
                << nl;
            }
          }
        }

        // write track (line) connectivity to file
        Info << "\n    Writing track lines" << endl;
        os << "\nLINES " << nTracks << ' ' << nPoints + nTracks << nl;

        // Write ids of track points to file
        {
          label globalPtI = 0;
          FOR_ALL(particleMap, i)
          {
            os << particleMap[i].size() << nl;
            FOR_ALL(particleMap[i], j)
            {
              os << ' ' << globalPtI++;
              if (((j + 1) % 10 == 0) && (j != 0))
              {
                os << nl;
              }
            }
            os << nl;
          }
        }
        const label nFields = validateFields(userFields, cloudObjs);
        os << "POINT_DATA " << nPoints << nl
          << "FIELD attributes " << nFields << nl;
        Info<< "\n    Processing fields" << nl << endl;
        processFields<label>(os, particleMap, userFields, cloudObjs);
        processFields<scalar>(os, particleMap, userFields, cloudObjs);
        processFields<vector>(os, particleMap, userFields, cloudObjs);
        processFields<sphericalTensor>(os, particleMap, userFields, cloudObjs);
        processFields<symmTensor>(os, particleMap, userFields, cloudObjs);
        processFields<tensor>(os, particleMap, userFields, cloudObjs);
      }
    }
    Info << endl;
  }
  Info << "\ndone" << endl;
  return 0;
}
