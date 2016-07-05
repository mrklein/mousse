#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

# File extensions for process
EXTS = ['.hpp', '.ipp', '.cpp', '.inc']

# Dictionary of file name -> full path pairs for lookup acceleration
FIXES = {}


def make_backup(fn):
    print('Backing up {0} ... '.format(fn), end='')
    print('OK')


def fix_includes(fn, root=None, backup=False):
    lines = []
    if backup:
        make_backup(fn)
        """
    with open(fn, 'r') as f:
        for l in f:
            if l.strip().startswith('#include'):
                print(l)
                lines.append(l)
            else:
                lines.append(l)
    with open(fn, 'w') as f:
        f.write(lines.join(os.linesep))
        """


def run():
    from argparse import ArgumentParser
    from sys import argv
    from os import walk
    from os.path import splitext, join

    parser = ArgumentParser()
    parser.add_argument('-i', '--inplace', action='store_false', dest='backup',
            help='do not create file backup')
    parser.add_argument('-r', '--root', action='store', dest='root',
            help='include root folder', nargs=1)
    parser.add_argument('path', nargs='+',
            help='list of paths where to look for files to fix')
    args = parser.parse_args()

    for path in args.path:
        for root, dirs, files in walk(path, topdown=False, followlinks=False):
            for name in files:
                fn, fext = splitext(name)
                if fext in EXTS:
                    fix_includes(join(root, name), args.root, args.backup)
    return 0


if __name__ == '__main__':
    import sys
    sys.exit(run())
