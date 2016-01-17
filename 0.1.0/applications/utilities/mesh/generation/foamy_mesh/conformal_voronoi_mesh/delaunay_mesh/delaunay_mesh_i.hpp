// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Triangulation>
inline const mousse::Time& mousse::DelaunayMesh<Triangulation>::time() const
{
  return runTime_;
}
template<class Triangulation>
void mousse::DelaunayMesh<Triangulation>::timeCheck
(
  const string& description,
  const bool check
) const
{
  if (check)
  {
    Info<< nl << "--- [ cpuTime "
      << time().elapsedCpuTime() << " s, "
      << "delta " << time().cpuTimeIncrement()<< " s";
    if (description != word::null)
    {
      Info<< ", " << description << " ";
    }
    else
    {
      Info<< " ";
    }
    Info<< "] --- " << endl;
  }
}
template<class Triangulation>
inline mousse::label mousse::DelaunayMesh<Triangulation>::getNewCellIndex() const
{
  label id = cellCount_++;
  if (id == labelMax)
  {
    WarningIn
    (
      "mousse::DelaunayMesh<Triangulation>::getNewCellIndex() const"
    )   << "Cell counter has overflowed." << endl;
  }
  return id;
}
template<class Triangulation>
inline mousse::label mousse::DelaunayMesh<Triangulation>::getNewVertexIndex() const
{
  label id = vertexCount_++;
  if (id == labelMax)
  {
    WarningIn
    (
      "mousse::DelaunayMesh<Triangulation>::getNewVertexIndex() const"
    )   << "Vertex counter has overflowed." << endl;
  }
  return id;
}
template<class Triangulation>
mousse::label mousse::DelaunayMesh<Triangulation>::cellCount() const
{
  return cellCount_;
}
template<class Triangulation>
mousse::label mousse::DelaunayMesh<Triangulation>::vertexCount() const
{
  return vertexCount_;
}
template<class Triangulation>
void mousse::DelaunayMesh<Triangulation>::resetCellCount()
{
  cellCount_ = 0;
}
template<class Triangulation>
void mousse::DelaunayMesh<Triangulation>::resetVertexCount()
{
  vertexCount_ = 0;
}
