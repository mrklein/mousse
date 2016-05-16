#!/usr/bin/env python
# -*- coding: utf-8 -*-


from __future__ import print_function, division
from argparse import ArgumentParser
import os


def _rewrite_file(path, guard):
    if not os.path.exists(path):
        return
    probable_old_guard = os.path.basename(path)
    fn, fext = os.path.splitext(probable_old_guard)
    probable_old_guard = fn + "_" + fext[1:] + "_"
    src = ""
    with open(path, "r") as f:
        src = f.read()
    os.rename(path, path + "~")
    src = src.replace(probable_old_guard, guard)
    if src.find("#ifndef {0}\n".format(guard)) > 0:
        src = src.replace("#ifndef {0}\n".format(guard), "")
        src = src.replace("#define {0}\n".format(guard), "")
        src = "#ifndef {0}\n#define {0}\n\n".format(guard) + src
    with open(path, "w") as f:
        f.write(src)
    """
    res = []
    res.append("#ifndef {0}".format(guard))
    res.append("#define {0}".format(guard))
    res.append("")
    with open(path, "r") as f:
        for l in f:
            if l.startswith("#ifndef") \
                    or l.startswith("#define {0}".format(probable_old_guard)):
                continue
            else:
                res.append(l.replace("\n", ""))
    # Create backup
    os.rename(path, path + "~")
    with open(path, "w") as f:
        f.write(os.linesep.join(res))
    """

def _process_files(paths, base=None):
    local_base = None
    for path in paths:
        print('-> {0}'.format(path))
        if base is None:
            local_base = path
        else:
            local_base = base
        for root, dirs, files in os.walk(path, topdown=False,
                                         followlinks=False):
            for name in files:
                fn, fext = os.path.splitext(name)
                if os.path.basename(root) == "_lninclude":
                    continue
                if fext == '.hpp' or fext == '.cpp':
                    folder = root.replace(local_base, "")
                    if fn.startswith("_"):
                        rfn = "T" + fn[1:]
                    else:
                        rfn = fn
                    t = folder.split("/") + [rfn, fext[1:]]
                    tt = []
                    for s in t:
                        if s.startswith("_"):
                            tt.append("T" + s[1:])
                        else:
                            tt.append(s)
                    guard = "_".join([s.upper().replace("-", "_") for s in tt]) + "_"
                    _rewrite_file(os.path.join(root, name), guard)


def _fix():
    parser = ArgumentParser()
    parser.add_argument('-b', '--base', action='store', type=str)
    parser.add_argument('paths', action='store', nargs='*')

    args = parser.parse_args()
    base = args.base
    paths = args.paths
    if len(paths) == 0:
        paths = [os.getcwd()]
    _process_files(paths, base)


if __name__ == '__main__':
    _fix()
