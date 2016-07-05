#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_LAGRANGIAN_WRITER_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_LAGRANGIAN_WRITER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::lagrangianWriter
// Description
//   Write fields (internal).

#include "ofstream.hpp"
#include "cloud.hpp"
#include "vol_fields.hpp"
#include "point_fields.hpp"
#include "vtk_mesh.hpp"


using namespace mousse;


namespace mousse {

class volPointInterpolation;


class lagrangianWriter
{
  const vtkMesh& vMesh_;
  const bool binary_;
  const fileName fName_;
  const word cloudName_;
  std::ofstream os_;
  label nParcels_;
public:
  // Constructors
    //- Construct from components
    lagrangianWriter
    (
      const vtkMesh&,
      const bool binary,
      const fileName&,
      const word&,
      const bool dummyCloud
    );
  // Member Functions
    std::ofstream& os()
    {
      return os_;
    }
    void writeParcelHeader(const label nFields);
    //- Write IOField
    template<class Type>
    void writeIOField(const wordList&);
};

}  // namespace mousse

#include "lagrangian_writer.ipp"

#endif
