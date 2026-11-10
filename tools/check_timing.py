# SPDX-License-Identifier: GPL-2.0-or-later
"""Run the retained pure timing regression; no clock or peripheral model."""
from pathlib import Path
import json, os, subprocess, tempfile
root=Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory(prefix='op1-doom-timing-') as tmp:
    target=Path(tmp)/'test'
    subprocess.run([os.environ.get('CC','cc'),'-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-I'+str(root/'src/native'),*[str(root/'src/native'/name) for name in ['test_probe.c','timing_probe.c','timing_stats.c']],'-o',str(target)],check=True,timeout=30)
    subprocess.run([str(target)],check=True,timeout=10)
result={'status':'PASS','scope':'Retained test_probe: timestamp wraparound, invalid spans/indices, freeze and reset; UBSan','hardware_test':False,'firmware_build':False}
out=root/'.build';out.mkdir(exist_ok=True)
(out/'timing.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
