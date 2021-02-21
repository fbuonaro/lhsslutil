#!python3.7

import argparse
import os
import subprocess
import sys

DFLT_LHCLUSTER_IMAGE_NAME = "lhsslutil:test-env"

def getParser():
    parser = argparse.ArgumentParser(
        description='start lhsslutil development container')
    cwd = os.getcwd()

    parser.add_argument(
        '-i', '--image',
        help='lhsslutil image name',
        default=DFLT_LHCLUSTER_IMAGE_NAME )
    parser.add_argument(
        '-p', '--project_root',
        help='root of lhsslutil project, default to $PWD',
        default=cwd )

    return parser

def main():
    parser = getParser()
    args = parser.parse_args()
    dockerParams = \
    [
        'docker',
        'run',
        '--rm',
        '-it',
        '-v',
        '{0}:/lhsslutil-dev'.format( args.project_root ),
        '--cap-add=SYS_PTRACE', 
        '--entrypoint',
        '/bin/bash',
        args.image
    ]
    proc = subprocess.Popen( dockerParams )

    proc.wait()

    ret = proc.returncode

    return ret


if __name__ == '__main__':
    ret = main()
    sys.exit( ret )
