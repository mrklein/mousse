#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Reduce files indent, optionally rename into snake_case."""

from __future__ import print_function
from argparse import ArgumentParser


def _get_files(path, recurse=False):
    from os import listdir, walk
    from os.path import exists
    if not exists(path):
        raise OSError
    if not recurse:
        for itm in listdir(path):
            yield path, itm
    else:
        for root, _, files in walk(path, topdown=False, followlinks=False):
            for itm in files:
                yield root, itm


def _reduce_indent(line, factor):
    """Reduce :line: indent by a factor :factor:."""
    indent = len(line) - len(line.lstrip())
    indent //= factor
    return ' '*indent + line.lstrip()


def _is_foam_dictionary(path):
    """Check if :path: is Foam dictionary file."""
    from os.path import exists, isfile
    from re import compile, DOTALL
    if not exists(path):
        return False
    if not isfile(path):
        return False
    foam_file = compile('FoamFile.*?\{.*?version\s+(.+);.*\}', DOTALL)
    with open(path, 'r') as f:
        src = f.read()
        return foam_file.search(src) is not None


def _snake_case(name, separator="_"):
    import re
    _underscorer1 = re.compile(r'(.)([A-Z][a-z]+)')
    _underscorer2 = re.compile('([a-z0-9])([A-Z])')
    subbed = _underscorer1.sub(r'\1_\2', name)
    return _underscorer2.sub(r'\1_\2', subbed).lower()


def _squize_file(path, everything=False, level=2):
    from os.path import exists, isfile
    from re import compile, M, S
    from os import linesep

    if not exists(path):
        raise OSError
    if not isfile(path):
        raise OSError
    if not everything and not _is_foam_dictionary(path):
        return

    with open(path, 'r') as f:
        src = f.read()
        # Useless star comments
        r1 = compile(r'\/\/ *(\* )+\/\/\n', M)
        # Head comment=
        r2 = compile(r'\/\*-+\*- C\+\+ -\*-+\*\\(.*?)\\*-+\*\/\n', M | S)
        # Another star comments
        r3 = compile(r'\/\/ \*+ \/\/\n', M)
        r3v = compile(r'\/\/ \*+ \/\/', M)

        src = r1.sub(r'', src)
        src = r2.sub(r'', src)
        src = r3.sub(r'', src)
        src = r3v.sub(r'', src)

    lines = src.split(linesep)
    reduced_indent = []
    for l in lines:
        if len(l) < 1:
            continue
        reduced_indent.append(_reduce_indent(l, level))

    with open(path, 'w') as f:
        for l in reduced_indent:
            f.write('{0}\n'.format(l))


def _squize():
    from os import rename
    from os.path import exists, join, basename, dirname, isfile
    parser = ArgumentParser()
    parser.add_argument("-r", "--recurse", help="Recurse into sub-folders",
                        action="store_true", dest="recurse")
    parser.add_argument("-n", "--rename", help="Rename files into snake_case",
                        action="store_true", dest="rename")
    parser.add_argument("-a", "--all", help="Process all files",
                        action="store_true", dest="all")
    parser.add_argument("-l", "--level",
                        help="Indent reduction level (1 - no reduction)",
                        action="store", type=int, dest="level", default=2)
    parser.add_argument("paths", help="Path to fix", action="store",
                        type=str, nargs="*", default=["."])
    args = parser.parse_args()
    for path in args.paths:
        if not exists(path):
            continue

        if isfile(path):
            _squize_file(path, args.all, args.level)
            if args.rename:
                try:
                    name = basename(path)
                    folder = dirname(path)
                    rename(path, join(folder, _snake_case(name)))
                except OSError:
                    print('Can not rename {0}'.format(path))
        else:
            for p, i in _get_files(path, recurse=args.recurse):
                filename = join(p, i)
                _squize_file(filename, args.all, args.level)
                if args.rename:
                    try:
                        rename(filename, join(p, _snake_case(i)))
                    except OSError:
                        print('Can not rename {0}'.format(filename))


if __name__ == '__main__':
    _squize()
