import subprocess
import os
import argparse
from distutils.dir_util import copy_tree

parser = argparse.ArgumentParser()
parser.add_argument('-s', '--server_path', default='', action='store', dest='server_path', help='server project directory path')
parser.add_argument('-c', '--client_path', default='', action='store', dest='client_path', help='client project directory path')
parser.add_argument('-n', '--namespace', default='gen', action='store', dest='namespace', help='namespace name')
args = parser.parse_args()

subprocess.call(['python', 'generate.py', '-l', 'cpp', '-p', args.server_path+'message/', '-n', args.namespace], shell=True)
#subprocess.call(['python', 'generate.py', '-l', 'csharp'], shell=True)

if args.server_path != '':
    copy_tree('generated', args.server_path + 'generated')
    os.remove(args.server_path + 'generated/ClientPacketHandler.gen.hpp')
if args.client_path != '':
    copy_tree('generated', args.client_path + 'generated')
    os.remove(args.client_path + 'generated/ServerPacketHandler.gen.hpp')