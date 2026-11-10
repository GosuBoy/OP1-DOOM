# SPDX-License-Identifier: GPL-2.0-or-later
"""Offline source identity, privacy and fresh-history audit."""
from pathlib import Path
import ast, hashlib, json, re, subprocess, sys

root=Path(__file__).resolve().parents[1]
manifest=json.loads((root/'provenance/imports.json').read_text())
errors=[]
for item in manifest['files']:
    p=root/item['path']
    if not p.is_file() or hashlib.sha256(p.read_bytes()).hexdigest()!=item['sha256']:
        errors.append('source identity mismatch: '+item['path'])
allowed={'.c','.h','.S','.py','.md','.json','.txt'}
def inspect(name,data):
    if Path(name).suffix not in allowed and name not in {'.gitignore','LICENSE'}:
        errors.append('unexpected file type: '+name)
    try: text=data.decode('utf-8')
    except UnicodeDecodeError:
        errors.append('binary content: '+name);return
    if '\0' in text:errors.append('NUL payload: '+name)
    if name.endswith('.py'):
        try:ast.parse(text,filename=name)
        except SyntaxError:errors.append('Python syntax: '+name)
    if name=='tools/check_sources.py':return
    if re.search(r'/Users/|/workspace/|libfile_|-----BEGIN (?:RSA |EC |OPENSSH )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}|AKIA[0-9A-Z]{16}',text):
        errors.append('private path or credential-shaped content: '+name)
    if name.startswith('src/') and re.search(r'\.incbin|\.byte\b|fromhex\(',text):
        errors.append('embedded data requires review: '+name)
for p in root.rglob('*'):
    if p.is_file() and not any(x in p.relative_to(root).parts for x in ('.git','.build','__pycache__')):
        inspect(str(p.relative_to(root)),p.read_bytes())
commits=[];blobs=set()
if (root/'.git').exists():
    commits=subprocess.check_output(['git','rev-list','--all'],cwd=root,text=True).splitlines()
    for commit in commits:
        rows=subprocess.check_output(['git','ls-tree','-r',commit],cwd=root,text=True).splitlines()
        for row in rows:
            meta,name=row.split('\t',1);mode,typ,sha=meta.split()
            if typ!='blob' or mode not in {'100644','100755'}:
                errors.append('unexpected history entry: '+name);continue
            if (name,sha) not in blobs:
                blobs.add((name,sha));inspect(name,subprocess.check_output(['git','cat-file','blob',sha],cwd=root))
result={'status':'PASS' if not errors else 'FAIL','source_files':len(manifest['files']),'commits_checked':len(commits),'unique_history_blobs':len(blobs),'errors':errors,'scope':'Mechanical source/privacy checks; not legal clearance or firmware validation'}
out=root/'.build';out.mkdir(exist_ok=True)
(out/'source-audit.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2));sys.exit(bool(errors))
