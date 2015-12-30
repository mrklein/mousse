/*
 * mousse: CFD toolbox
 * Copyright (C) 2011-2014 OpenFOAM Foundation
 * Copyright (C) 2016 mousse project
 * Application
 *   dir-to-string
 * Description
 *   converts a directory path into a string with appropriate capitalisation
 *   e.g. dir1/dir2 becomes dir1Dir2
 *
 * Usage
 *   echo dirName | dirToString
 *
 * e.g.
 *   using sh
 *  baseDirName=`echo $dir | sed 's%^\./%%' | $bin/dirToString`
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
    int c;
    int nextupper = 0;

    while ((c=getchar()) != EOF)
    {
        if (c == '/')
        {
            nextupper = 1;
        }
        else
        {
            if (nextupper)
            {
                putchar(toupper(c));
            }
            else
            {
                putchar(c);
            }

            nextupper = 0;
        }
    }

    return 0;
}

