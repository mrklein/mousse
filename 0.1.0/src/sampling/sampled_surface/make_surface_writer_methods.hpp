// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_surface_writer_methods_hpp_
#define make_surface_writer_methods_hpp_
#include "surface_writer.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
#define makeSurfaceWriterType(ThisClass)                                      \
  DEFINE_TYPE_NAME_AND_DEBUG(ThisClass, 0);                                     \
  ADD_TO_RUN_TIME_SELECTION_TABLE(surfaceWriter, ThisClass, word)
#define defineSurfaceWriterWriteField(ThisClass, FieldType)                   \
  void ThisClass::write                                                     \
  (                                                                         \
    const fileName& outputDir,                                            \
    const fileName& surfaceName,                                          \
    const pointField& points,                                             \
    const faceList& faces,                                                \
    const word& fieldName,                                                \
    const Field<FieldType>& values,                                       \
    const bool isNodeValues,                                              \
    const bool verbose                                                    \
  ) const                                                                   \
  {                                                                         \
    writeTemplate                                                         \
    (                                                                     \
      outputDir,                                                        \
      surfaceName,                                                      \
      points,                                                           \
      faces,                                                            \
      fieldName,                                                        \
      values,                                                           \
      isNodeValues,                                                     \
      verbose                                                           \
    );                                                                    \
  }
#define defineSurfaceWriterWriteFields(ThisClass)                             \
  defineSurfaceWriterWriteField(ThisClass, scalar);                         \
  defineSurfaceWriterWriteField(ThisClass, vector);                         \
  defineSurfaceWriterWriteField(ThisClass, sphericalTensor);                \
  defineSurfaceWriterWriteField(ThisClass, symmTensor);                     \
  defineSurfaceWriterWriteField(ThisClass, tensor)
}  // namespace mousse
#endif
