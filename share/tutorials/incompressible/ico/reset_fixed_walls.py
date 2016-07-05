#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Edit fixedWalls boundary description in-place."""

from __future__ import print_function
import sys
import re


FIX = """\
fixedWalls
    {
        type            fixedValue;
        value           uniform (0 0 0);
    }
    frontAndBack
"""


def reset_fixed_walls():
    filename = sys.argv[1]
    with open(filename, 'r') as f:
        src = f.read()
        fixer = re.compile('fixedWalls.*\{.*?\}.*frontAndBack', re.DOTALL)
        src = fixer.sub(FIX, src)
    with open(filename, 'w') as f:
        f.write(src)

if __name__ == '__main__':
    sys.exit(reset_fixed_walls())
