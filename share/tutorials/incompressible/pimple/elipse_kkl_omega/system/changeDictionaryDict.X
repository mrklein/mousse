// mousse: CFD toolbox

FoamFile
{
  version     2.0;
  format      ascii;
  class       dictionary;
  object      changeDictionaryDict;
}

dictionaryReplacement
{
  mirrorMeshDict
  {
    pointAndNormalDict
    {
      basePoint       (0 0 0);
      normalVector    (-1 0 0);
    }
  }
}

// vim: set ft=foam et sw=2 ts=2 sts=2:
