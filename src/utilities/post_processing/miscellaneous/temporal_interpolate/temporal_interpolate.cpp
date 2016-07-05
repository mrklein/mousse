// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time_selector.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "vol_mesh.hpp"
#include "surface_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "point_fields.hpp"
#include "_read_fields.hpp"
#include "interpolation_weights.hpp"
#include "uniform_interpolate.hpp"


using namespace mousse;


class fieldInterpolator
{
  Time& runTime_;
  const fvMesh& mesh_;
  const IOobjectList& objects_;
  const HashSet<word>& selectedFields_;
  instant ti_;
  instant ti1_;
  const interpolationWeights& interpolator_;
  const wordList& timeNames_;
  int divisions_;
public:
  fieldInterpolator
  (
    Time& runTime,
    const fvMesh& mesh,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields,
    const instant& ti,
    const instant& ti1,
    const interpolationWeights& interpolator,
    const wordList& timeNames,
    int divisions
  )
  :
    runTime_{runTime},
    mesh_{mesh},
    objects_{objects},
    selectedFields_{selectedFields},
    ti_{ti},
    ti1_{ti1},
    interpolator_{interpolator},
    timeNames_{timeNames},
    divisions_{divisions}
  {}
  template<class GeoFieldType> void interpolate();
};


template<class GeoFieldType>
void fieldInterpolator::interpolate()
{
  const word& fieldClassName = GeoFieldType::typeName;
  IOobjectList fields = objects_.lookupClass(fieldClassName);
  if (!fields.size())
    return;
  Info << "    " << fieldClassName << "s:";
  FOR_ALL_CONST_ITER(IOobjectList, fields, fieldIter) {
    if (!selectedFields_.empty()
        && !selectedFields_.found(fieldIter()->name()))
      continue;
    Info << " " << fieldIter()->name() << '(';
    scalar deltaT = (ti1_.value() - ti_.value())/(divisions_ + 1);
    for (int j=0; j<divisions_; j++) {
      instant timej = instant(ti_.value() + (j + 1)*deltaT);
      runTime_.setTime(instant(timej.name()), 0);
      Info << timej.name();
      if (j < divisions_-1) {
        Info << " ";
      }
      // Calculate times to read and weights
      labelList indices;
      scalarField weights;
      interpolator_.valueWeights
      (
        runTime_.value(),
        indices,
        weights
      );
      const wordList selectedTimeNames
      {
        UIndirectList<word>{timeNames_, indices}()
      };
      ReadFields<GeoFieldType>
      (
        fieldIter()->name(),
        mesh_,
        selectedTimeNames
      );
      GeoFieldType fieldj
      (
        uniformInterpolate<GeoFieldType>
        (
          {
            fieldIter()->name(),
            runTime_.timeName(),
            fieldIter()->db(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
          },
          fieldIter()->name(),
          selectedTimeNames,
          weights
        )
      );
      fieldj.write();
    }
    Info << ')';
  }
  Info << endl;
}


int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.inc"
  argList::addOption
  (
    "fields",
    "list",
    "specify a list of fields to be interpolated. Eg, '(U T p)' - "
    "regular expressions not currently supported"
  );
  argList::addOption
  (
    "divisions",
    "integer",
    "specify number of temporal sub-divisions to create (default = 1)."
  );
  argList::addOption
  (
    "interpolationType",
    "word",
    "specify type of interpolation (linear or spline)"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  HashSet<word> selectedFields;
  if (args.optionFound("fields")) {
    args.optionLookup("fields")() >> selectedFields;
  }
  if (selectedFields.size()) {
    Info << "Interpolating fields " << selectedFields << nl << endl;
  } else {
    Info << "Interpolating all fields" << nl << endl;
  }
  int divisions = 1;
  if (args.optionFound("divisions")) {
    args.optionLookup("divisions")() >> divisions;
  }
  Info << "Using " << divisions << " per time interval" << nl << endl;
  const word interpolationType =
    args.optionLookupOrDefault<word>("interpolationType", "linear");
  Info << "Using interpolation " << interpolationType << nl << endl;
  instantList timeDirs = timeSelector::select0(runTime, args);
  scalarField timeVals{timeDirs.size()};
  wordList timeNames{timeDirs.size()};
  FOR_ALL(timeDirs, i) {
    timeVals[i] = timeDirs[i].value();
    timeNames[i] = timeDirs[i].name();
  }
  autoPtr<interpolationWeights> interpolatorPtr
  {
    interpolationWeights::New
    (
      interpolationType,
      timeVals
    )
  };
  #include "create_named_mesh.inc"
  Info << "Interpolating fields for times:" << endl;
  for (label timei = 0; timei < timeDirs.size() - 1; timei++) {
    runTime.setTime(timeDirs[timei], timei);
    // Read objects in time directory
    IOobjectList objects{mesh, runTime.timeName()};
    fieldInterpolator interpolator
    {
      runTime,
      mesh,
      objects,
      selectedFields,
      timeDirs[timei],
      timeDirs[timei+1],
      interpolatorPtr(),
      timeNames,
      divisions
    };
    // Interpolate vol fields
    interpolator.interpolate<volScalarField>();
    interpolator.interpolate<volVectorField>();
    interpolator.interpolate<volSphericalTensorField>();
    interpolator.interpolate<volSymmTensorField>();
    interpolator.interpolate<volTensorField>();
    // Interpolate surface fields
    interpolator.interpolate<surfaceScalarField>();
    interpolator.interpolate<surfaceVectorField>();
    interpolator.interpolate<surfaceSphericalTensorField>();
    interpolator.interpolate<surfaceSymmTensorField>();
    interpolator.interpolate<surfaceTensorField>();
  }
  Info << "End\n" << endl;
  return 0;
}

