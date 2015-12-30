#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function


def _backup_file(f):
    from shutil import copy
    copy(f, '{0}~'.format(f))


def _align(f, length=79):
    res = []
    for line in f:
        l = line.rstrip(' \n')
        if len(l) >= length:
            res.append(l)
            continue
        if l.endswith('\\'):
            slash_position = l.index('\\')
            shift = length - slash_position - 1
            t = '{0}{1}\\'.format(l.replace('\\', ''), ' '*shift)
            res.append(t)
        else:
            res.append(l)
    return res


def _do_align():
    from argparse import ArgumentParser

    parser = ArgumentParser()
    parser.add_argument('-i', '--no-backup', action='store_false',
                        dest='backup',
                        help='Edit file in place without creating backup.')
    parser.add_argument('-l', '--length', action='store', type=int,
                        dest='length', help='Target line length', default=79)
    parser.add_argument('file', nargs='+', help='File to edit')
    args = parser.parse_args()

    if args.file is not None:
        for f in args.file:
            if args.backup:
                _backup_file(f)
            with open(f, 'r') as fin:
                src = _align(fin, args.length)
            with open(f, 'w') as fout:
                for l in src:
                    fout.write(l + '\n')


if __name__ == '__main__':
    _do_align()
