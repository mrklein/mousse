#!/usr/bin/env python
# -*- coding: utf-8 -*-


def get_files(path, recurse=False):
    from os import listdir, walk
    from os.path import exists, join
    if not exists(path):
        raise OSError
    if not recurse:
        for itm in listdir(path):
            yield join(path, itm)
    else:
        for root, _, files in walk(path, topdown=False, followlinks=False):
            for itm in files:
                yield join(root, itm)

if __name__ == '__main__':
    for p in [".", ".."]:
        for f in get_files(p):
            print(f)
        print
