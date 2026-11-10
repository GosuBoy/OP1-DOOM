# Historical V74 controls

The private source maps the following physical scan bits to Doom keys. This table is source-derived; keycap labels still need verification against the original V73 mapping. It is not an installation or hardware-test result.

| Physical scan bit | Action |
| --- | --- |
| 10 | Turn left |
| 3 | Move forward |
| 11 | Move backward |
| 12 | Turn right |
| 26 | Fire |
| 27 | Use/open |
| 44 | Open/close menu |
| 43 | Confirm menu selection |
| 20 | Next weapon |
| 21 | Run modifier |
| 19 | Strafe modifier |

The original V74 owner checklist says this eleven-control mapping does not expose automap. The port retains finite session/input/frame budgets; their timer ticks are not calibrated wall-clock durations. Persistent saves, unlimited sessions and a return to the stock music UI are not promised.

The input implementation files without explicit retained license notices are withheld from this initial source subset. Their facts are documented here; the absence of those source files is one reason this is not a complete build kit.
