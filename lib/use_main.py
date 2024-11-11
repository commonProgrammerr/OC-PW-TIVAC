#! /bin/python3

import os
import sys

try:
  target_dir = sys.argv[1]
  
  base_path = os.path.abspath(os.curdir)

  use_home=os.getenv('HOME', base_path)
  src_main=os.path.join(base_path, "src/main.cpp")

  framework_dir=os.path.join(use_home, ".platformio/packages/framework-energiativa")
  energiativa_main=os.path.join(framework_dir, "cores/tivac/main.cpp")
  old_main=os.path.join(framework_dir, "cores/tivac/main.old")

  if target_dir == 'src':
    if not os.path.exists(old_main) and os.path.exists(energiativa_main):
      os.rename(energiativa_main, old_main)
      
    # if os.path.exists(energiativa_main):
    #   exit(os.system(f"ls -s {src_main} {energiativa_main}"))
  
  elif target_dir == 'default':

    if os.path.exists(old_main):
      if os.path.exists(energiativa_main):
        os.remove(energiativa_main)
        
      os.rename(old_main, energiativa_main)

except IndexError as e:
  print(str(e)) 
  exit(1)
