import subprocess

subprocess.call(['python', 'generate.py', '-l', 'cpp'], shell=True)
subprocess.call(['python', 'generate.py', '-l', 'csharp'], shell=True)