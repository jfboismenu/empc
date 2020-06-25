
from pathlib import Path
import os

src = Path("/opt/sourceryg++-2016.11/bin")
dst = Path("/usr/local/bin")
assert src.exists()
assert dst.exists()

for executable in os.listdir(src):
    os.symlink(src / executable, dst / executable.replace("ia16-elf-", ""))