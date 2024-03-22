import ast
import subprocess
import os
import argparse
import shutil

def arg_as_list(s):
    v = ast.literal_eval(s)
    if type(v) is not list:
        raise argparse.ArgumentTypeError("Argument \"%s\" is not a list" % (s))
    return v

parser = argparse.ArgumentParser()
parser.add_argument('-s', '--server_path', default='', action='store', dest='server_path', help='server project directory path')
parser.add_argument('-c', '--client_path', default='', action='store', dest='client_path', help='client project directory path')
parser.add_argument('-n', '--namespace', default='gen', action='store', dest='namespace', help='namespace name')
args = parser.parse_args()

subprocess.call(['python', 'generate.py', '-l', 'cpp', '-p', args.server_path+'message/', '-n', args.namespace], shell=True)
# # subprocess.call(['python', 'generate.py', '-l', 'csharp'], shell=True)

if args.server_path != '':
    dest = args.server_path + f'generated/{args.namespace}'
    if os.path.exists(dest):
        shutil.rmtree(dest)
        
    shutil.copytree(f'generated/{args.namespace}', dest)
    os.remove(args.server_path + f'generated/{args.namespace}/ClientPacketHandler.gen.hpp')
for clientPath in arg_as_list(args.client_path):
    dest = clientPath + f'generated/{args.namespace}'
    if os.path.exists(dest):
        shutil.rmtree(dest)
        
    shutil.copytree(f'generated/{args.namespace}', dest)
    os.remove(clientPath + f'generated/{args.namespace}/ServerPacketHandler.gen.hpp')