#ifndef SAMPLING_SAMPLED_SURFACE_MAKE_SURFACE_WRITER_METHODS_HPP_
#define SAMPLING_SAMPLED_SURFACE_MAKE_SURFACE_WRITER_METHODS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_writer.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

#define MAKE_SURFACE_WRITER_TYPE(ThisClass)                                   \
  DEFINE_TYPE_NAME_AND_DEBUG(ThisClass, 0);                                   \
  ADD_TO_RUN_TIME_SELECTION_TABLE(surfaceWriter, ThisClass, word)


#define DEFINE_SURFACE_WRITER_WRITE_FIELD(ThisClass, FieldType)               \
  void ThisClass::write                                                       \
  (                                                                           \
    const fileName& outputDir,                                                \
    const fileName& surfaceName,                                              \
    const pointField& points,                                                 \
    const faceList& faces,                                                    \
    const word& fieldName,                                                    \
    const Field<FieldType>& values,                                           \
    const bool isNodeValues,                                                  \
    const bool verbose                                                        \
  ) const                                                                     \
  {                                                                           \
    writeTemplate                                                             \
    (                                                                         \
      outputDir,                                                              \
      surfaceName,                                                            \
      points,                                                                 \
      faces,                                                                  \
      fieldName,                                                              \
      values,                                                                 \
      isNodeValues,                                                           \
      verbose                                                                 \
    );                                                                        \
  }


#define DEFINE_SURFACE_WRITER_WRITE_FIELDS(ThisClass)                         \
  DEFINE_SURFACE_WRITER_WRITE_FIELD(ThisClass, scalar);                       \
  DEFINE_SURFACE_WRITER_WRITE_FIELD(ThisClass, vector);                       \
  DEFINE_SURFACE_WRITER_WRITE_FIELD(ThisClass, sphericalTensor);              \
  DEFINE_SURFACE_WRITER_WRITE_FIELD(ThisClass, symmTensor);                   \
  DEFINE_SURFACE_WRITER_WRITE_FIELD(ThisClass, tensor)

}  // namespace mousse

#endif

