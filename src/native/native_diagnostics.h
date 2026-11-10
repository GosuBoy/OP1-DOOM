#pragma once
struct title_diagnostics {
    unsigned entered, stage, raw, pool, result, frames, frame, guard_ok;
    unsigned trace_count, sleep_calls, capture_frame;
    unsigned char trace[1024];
    unsigned stack_bottom, stack_top, stack_sp, stack_guard_ok;
    unsigned capture_tic, copied_frames, presented_frames;
};
extern volatile struct title_diagnostics title_diag;
