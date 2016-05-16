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
#include "writer.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  #include "create_fields.inc"

  fileName vtkPath{runTime.path()/"VTK"};
  mkDir(vtkPath);
  Info << "Scanning times to determine track data for cloud " << cloudName
    << nl << endl;
  labelList maxIds{Pstream::nProcs(), -1};
  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    Info << "    Reading particle positions" << endl;
    passiveParticleCloud myCloud{mesh, cloudName};
    Info << "    Read " << returnReduce(myCloud.size(), sumOp<label>())
      << " particles" << endl;
    FOR_ALL_CONST_ITER(passiveParticleCloud, myCloud, iter) {
      label origId = iter().origId();
      label origProc = iter().origProc();
      if (origProc >= maxIds.size()) {
        // Expand size
        maxIds.setSize(origProc+1, -1);
      }
      maxIds[origProc] = max(maxIds[origProc], origId);
    }
  }
  label maxNProcs = returnReduce(maxIds.size(), maxOp<label>());
  Info << "Detected particles originating from " << maxNProcs
    << " processors." << nl << endl;
  maxIds.setSize(maxNProcs, -1);
  Pstream::listCombineGather(maxIds, maxEqOp<label>());
  Pstream::listCombineScatter(maxIds);
  labelList numIds = maxIds + 1;
  Info << nl << "Particle statistics:" << endl;
  FOR_ALL(maxIds, procI) {
    Info << "    Found " << numIds[procI] << " particles originating"
      << " from processor " << procI << endl;
  }
  Info << nl << endl;
  // calc starting ids for particles on each processor
  List<label> startIds{numIds.size(), 0};
  for (label i = 0; i < numIds.size() - 1; i++) {
    startIds[i+1] += startIds[i] + numIds[i];
  }
  label nParticle = startIds.last() + numIds[startIds.size()-1];
  // number of tracks to generate
  label nTracks = nParticle/sampleFrequency;
  // storage for all particle tracks
  List<DynamicList<vector>> allTracks{nTracks};
  Info << "\nGenerating " << nTracks << " particle tracks for cloud "
    << cloudName << nl << endl;
  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    List<pointField> allPositions{Pstream::nProcs()};
    List<labelField> allOrigIds{Pstream::nProcs()};
    List<labelField> allOrigProcs{Pstream::nProcs()};
    // Read particles. Will be size 0 if no particles.
    Info << "    Reading particle positions" << endl;
    passiveParticleCloud myCloud{mesh, cloudName};
    // collect the track data on all processors that have positions
    allPositions[Pstream::myProcNo()].setSize(myCloud.size(), point::zero);
    allOrigIds[Pstream::myProcNo()].setSize(myCloud.size(), 0);
    allOrigProcs[Pstream::myProcNo()].setSize(myCloud.size(), 0);
    label i = 0;
    FOR_ALL_CONST_ITER(passiveParticleCloud, myCloud, iter) {
      allPositions[Pstream::myProcNo()][i] = iter().position();
      allOrigIds[Pstream::myProcNo()][i] = iter().origId();
      allOrigProcs[Pstream::myProcNo()][i] = iter().origProc();
      i++;
    }
    // collect the track data on the master processor
    Pstream::gatherList(allPositions);
    Pstream::gatherList(allOrigIds);
    Pstream::gatherList(allOrigProcs);
    Info << "    Constructing tracks" << nl << endl;
    if (!Pstream::master())
      continue;
    FOR_ALL(allPositions, procI) {
      FOR_ALL(allPositions[procI], i) {
        label globalId =
          startIds[allOrigProcs[procI][i]] + allOrigIds[procI][i];
        if (globalId % sampleFrequency == 0) {
          label trackId = globalId/sampleFrequency;
          if (allTracks[trackId].size() < maxPositions) {
            allTracks[trackId].append(allPositions[procI][i]);
          }
        }
      }
    }
  }
  if (Pstream::master()) {
    PtrList<coordSet> tracks{allTracks.size()};
    FOR_ALL(allTracks, trackI) {
      tracks.set
      (
        trackI,
        new coordSet
        {
          "track" + mousse::name(trackI),
          "distance"
        }
      );
      tracks[trackI].transfer(allTracks[trackI]);
    }
    autoPtr<writer<scalar>> scalarFormatterPtr =
      writer<scalar>::New(setFormat);
    fileName vtkFile
    {
      scalarFormatterPtr().getFileName(tracks[0], wordList{0})
    };
    OFstream vtkTracks{vtkPath/"particleTracks." + vtkFile.ext()};
    Info << "\nWriting particle tracks in " << setFormat
      << " format to " << vtkTracks.name()
      << nl << endl;
    scalarFormatterPtr().write
    (
      true,           // writeTracks
      tracks,
      wordList{0},
      List<List<scalarField>>{0},
      vtkTracks
    );
  }
  return 0;
}

