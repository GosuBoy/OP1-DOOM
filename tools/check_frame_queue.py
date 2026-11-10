# SPDX-License-Identifier: GPL-2.0-or-later
"""Pure ownership/refusal checks only; no interrupt or peripheral model."""
from pathlib import Path
import os, subprocess, tempfile, json

root=Path(__file__).resolve().parents[1]
code=r'''
#include "frame_queue.h"
#include <assert.h>
int main(void) {
    struct frame_queue q;
    assert(!fq_init(0,0));
    assert(!fq_init(&q,2));
    for (unsigned a=0;a<2;a++) {
        assert(fq_init(&q,a));
        assert(fq_irq_choice(&q,a,0)==FQ_NONE);
        unsigned slot=fq_reserve(&q);
        assert(slot==1-a);
        assert(fq_reserve(&q)==FQ_NONE);
        assert(fq_cancel(&q,slot));
        assert(fq_reserve(&q)==slot);
        assert(fq_commit(&q,slot));
        assert(!fq_commit(&q,slot));
        assert(fq_reserve(&q)==FQ_NONE);
        assert(fq_irq_choice(&q,a,1)==FQ_NONE);
        assert(fq_irq_choice(&q,a,0)==slot);
        assert(!fq_irq_complete(&q,a));
        assert(fq_irq_complete(&q,slot));
        assert(q.active==slot && q.pending==FQ_NONE);
        assert(q.submitted==1 && q.published==1);
        assert(fq_reserve(&q)==a);
        assert(fq_cancel(&q,a));
        assert(fq_irq_choice(&q,a,0)==FQ_NONE && q.fault);
        assert(fq_reserve(&q)==FQ_NONE);
    }
    return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='op1-doom-queue-') as tmp:
    p=Path(tmp);(p/'test.c').write_text(code)
    subprocess.run([os.environ.get('CC','cc'),'-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-I'+str(root/'src/native'),str(p/'test.c'),str(root/'src/native/frame_queue.c'),'-o',str(p/'test')],check=True,timeout=30)
    subprocess.run([str(p/'test')],check=True,timeout=10)
result={'status':'PASS','scope':'Both active slots; queue ownership, cancellation, publication, DMA-running refusal and source mismatch fault; UBSan','hardware_test':False,'firmware_build':False}
out=root/'.build';out.mkdir(exist_ok=True)
(out/'frame-queue.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
