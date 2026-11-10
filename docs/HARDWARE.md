# Hardware and recovery limitations

The historical target is the original OP-1, firmware 246, TE-BOOT 2.30. OP-1 Field and other firmware/hardware versions have not been validated. V74 is documented as owner-tested playable without physical audio; emulator sound experiments are not hardware-audio support.

Dedicated Doom replaces the normal application. USB, charging, sleep and power-switch behavior remain unresolved. The historical procedure required backing up user content, charging under stock firmware first, brief battery-only gameplay after a completed update, and restoring the exact known-good baseline before charging or normal USB use. **Never disconnect power or USB during active copying/flashing.**

Recovery historically used COM + power and the existing TE-BOOT firmware-update function. Do not use factory reset or format as a troubleshooting shortcut; they can erase user content. If TE-BOOT cannot be entered, stop and seek support. Prior recovery success on one unit does not guarantee recovery from every failure; data loss or service may be necessary.

The exact previously tested plaintext main-only recovery baseline is not automatically interchangeable with an arbitrary official download. No recovery image or install guide is supplied here. This release does not authorize firmware installation or TE-BOOT changes.

Manufacturer reference: [original OP-1 TE-BOOT guide](https://teenage.engineering/guides/op-1/original/te-boot). No procedure was executed on hardware during preparation.
