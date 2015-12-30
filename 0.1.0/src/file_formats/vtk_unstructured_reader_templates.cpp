// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vtk_unstructured_reader.hpp"
#include "label_io_field.hpp"
#include "scalar_io_field.hpp"
#include "string_io_list.hpp"
#include "cell_modeller.hpp"
#include "vector_io_field.hpp"
// Private Member Functions 
template<class T>
void mousse::vtkUnstructuredReader::readBlock
(
  Istream& inFile,
  const label n,
  List<T>& lst
) const
{
  lst.setSize(n);
  forAll(lst, i)
  {
    inFile >> lst[i];
  }
}
template<class Type>
void mousse::vtkUnstructuredReader::printFieldStats
(
  const objectRegistry& obj
) const
{
  wordList fieldNames(obj.names(Type::typeName));
  if (fieldNames.size() > 0)
  {
    Info<< "Read " << fieldNames.size() << " " << Type::typeName
      << " fields:" << endl;
    Info<< "Size\tName" << nl
      << "----\t----" << endl;
    forAll(fieldNames, i)
    {
      Info<< obj.lookupObject<Type>(fieldNames[i]).size()
        << "\t" << fieldNames[i]
        << endl;
    }
    Info<< endl;
  }
}
