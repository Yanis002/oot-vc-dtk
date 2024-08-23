#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object NotLinked status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

### Script's arguments

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "--non-matching",
    action="store_true",
    help="create non-matching build for modding",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
    default=True,
)
parser.add_argument(
    "--no-asm",
    action="store_true",
    help="don't incorporate .s files from asm directory",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--progress-version",
    metavar="VERSION",
    help="version to print progress for",
)

args = parser.parse_args()

### Project configuration

config = ProjectConfig()
config.versions = [
    "vc-j",
]
config.default_version = "vc-j"
config.warn_missing_config = True
config.warn_missing_source = False
config.progress_all = False

config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.sjiswrap_path = args.sjiswrap
config.non_matching = args.non_matching

if not is_windows():
    config.wrapper = args.wrapper

if args.no_asm:
    config.asm_dir = None

### Tool versions

config.binutils_tag = "2.42-1"
config.compilers_tag = "20231018"
config.dtk_tag = "v0.9.2"
config.sjiswrap_tag = "v1.1.1"
config.wibo_tag = "0.6.11"
config.linker_version = "GC/3.0a5"

### Flags

config.asflags = [
    "-mgekko",
    "-I include",
    "-I libc",
]

config.ldflags = [
    "-fp hardware",
    "-nodefaults",
    "-warn off",
]

cflags_base = [
    "-Cpp_exceptions off",
    "-proc gekko",
    "-fp hardware",
    "-fp_contract on",
    "-enum int",
    "-align powerpc",
    "-nosyspath",
    "-RTTI off",
    "-str reuse",
    "-enc SJIS",
    "-O4,p",
    "-inline auto",
    "-nodefaults",
    "-msgstyle gcc",
    "-sym on",
    "-i include",
    "-i libc",
]

if config.non_matching:
    cflags_base.append("-DNON_MATCHING")

### Helper functions

def EmulatorLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/3.0a5",
        "cflags": [*cflags_base, "-inline auto"],
        "host": False,
        "objects": objects,
    }

def RevolutionLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/3.0a5", # from strings, note: dvd is using a different version
        "cflags": cflags_base,
        "host": False,
        "objects": objects,
    }

def GenericLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/3.0a3",
        "cflags": [*cflags_base, "-rostr", "-use_lmw_stmw on", "-lang c"],
        "host": False,
        "objects": objects,
    }

### Link order

# Not matching for any version
NotLinked = {}

# Matching for all versions
Linked = config.versions

# Matching for specific versions
def LinkedFor(*versions):
    return versions

config.libs = [
    EmulatorLib(
        "emulator",
        [
            Object(LinkedFor("vc-j"), "emulator/vc64_RVL.c"),
            Object(NotLinked, "emulator/system.c"),
            Object(LinkedFor("vc-j"), "emulator/ai.c"),
            Object(LinkedFor("vc-j"), "emulator/vi.c"),
            Object(LinkedFor("vc-j"), "emulator/si.c"),
            Object(LinkedFor("vc-j"), "emulator/pi.c"),
            Object(LinkedFor("vc-j"), "emulator/mi.c"),
            Object(LinkedFor("vc-j"), "emulator/disk.c"),
            Object(NotLinked, "emulator/cpu.c", asm_processor=True),
            Object(NotLinked, "emulator/pif.c"),
            Object(LinkedFor("vc-j"), "emulator/ram.c"),
            Object(LinkedFor("vc-j"), "emulator/rom.c"),
            Object(LinkedFor("vc-j"), "emulator/rdb.c"),
            Object(LinkedFor("vc-j"), "emulator/eeprom.c"),
            Object(LinkedFor("vc-j"), "emulator/sram.c"),
            Object(LinkedFor("vc-j"), "emulator/flash.c"),
            Object(NotLinked, "emulator/_frameGCNcc.c"),
            Object(NotLinked, "emulator/_buildtev.c"),
            Object(NotLinked, "emulator/frame.c"),
            Object(NotLinked, "emulator/library.c"),
            Object(LinkedFor("vc-j"), "emulator/codeGCN.c"),
            Object(NotLinked, "emulator/helpRVL.c"),
            Object(NotLinked, "emulator/soundGCN.c"),
            Object(LinkedFor("vc-j"), "emulator/video.c"),
            Object(NotLinked, "emulator/store.c"),
            Object(NotLinked, "emulator/controller.c"),
            Object(NotLinked, "emulator/rsp.c"),
            Object(NotLinked, "emulator/rdp.c"),
            Object(LinkedFor("vc-j"), "emulator/xlCoreRVL.c"),
            Object(LinkedFor("vc-j"), "emulator/xlPostRVL.c"),
            Object(LinkedFor("vc-j"), "emulator/xlFileRVL.c"),
            Object(LinkedFor("vc-j"), "emulator/xlText.c"),
            Object(LinkedFor("vc-j"), "emulator/xlList.c"),
            Object(NotLinked, "emulator/xlHeap.c"),
            Object(LinkedFor("vc-j"), "emulator/xlFile.c"),
            Object(LinkedFor("vc-j"), "emulator/xlObject.c", asm_processor=True),
        ]
    ),
    RevolutionLib(
        "base",
        [
            Object(NotLinked, "revolution/base/PPCArch.c"),
        ]
    ),
    RevolutionLib(
        "os",
        [
            Object(NotLinked, "revolution/os/OS.c"),
            Object(NotLinked, "revolution/os/OSAlarm.c"),
            Object(NotLinked, "revolution/os/OSArena.c"),
            Object(NotLinked, "revolution/os/OSAudioSystem.c"),
            Object(NotLinked, "revolution/os/OSCache.c"),
            Object(NotLinked, "revolution/os/OSContext.c"),
            Object(NotLinked, "revolution/os/OSError.c"),
            Object(NotLinked, "revolution/os/OSExec.c"),
            Object(NotLinked, "revolution/os/OSFatal.c"),
            Object(NotLinked, "revolution/os/OSFont.c"),
            Object(NotLinked, "revolution/os/OSInterrupt.c"),
            Object(NotLinked, "revolution/os/OSLink.c"),
            Object(NotLinked, "revolution/os/OSMessage.c"),
            Object(NotLinked, "revolution/os/OSMemory.c"),
            Object(NotLinked, "revolution/os/OSMutex.c"),
            Object(NotLinked, "revolution/os/OSReset.c"),
            Object(NotLinked, "revolution/os/OSRtc.c"),
            Object(NotLinked, "revolution/os/OSSync.c"),
            Object(NotLinked, "revolution/os/OSThread.c"),
            Object(NotLinked, "revolution/os/OSTime.c"),
            Object(NotLinked, "revolution/os/OSUtf.c"),
            Object(NotLinked, "revolution/os/OSIpc.c"),
            Object(NotLinked, "revolution/os/OSStateTM.c"),
            Object(NotLinked, "revolution/os/code_800945CC.c"), # TODO: figure this out
            Object(NotLinked, "revolution/os/OSPlayRecord.c"),
            Object(NotLinked, "revolution/os/__start.c"),
            Object(NotLinked, "revolution/os/__ppc_eabi_init.c"),
        ]
    ),
    RevolutionLib(
        "exi",
        [
            Object(NotLinked, "revolution/exi/EXIBios.c"),
            Object(NotLinked, "revolution/exi/EXIUart.c"),
            Object(NotLinked, "revolution/exi/EXICommon.c"),
        ]
    ),
    RevolutionLib(
        "si",
        [
            Object(NotLinked, "revolution/si/SIBios.c"),
            Object(NotLinked, "revolution/si/SISamplingRate.c"),
        ]
    ),
    RevolutionLib(
        "db",
        [
            Object(NotLinked, "revolution/db/db.c"),
        ]
    ),
    RevolutionLib(
        "vi",
        [
            Object(NotLinked, "revolution/vi/vi.c"),
            Object(NotLinked, "revolution/vi/i2c.c"),
            Object(NotLinked, "revolution/vi/vi3in1.c"),
        ]
    ),
    RevolutionLib(
        "mtx",
        [
            Object(NotLinked, "revolution/mtx/mtx.c"),
            Object(NotLinked, "revolution/mtx/mtxvec.c"),
            Object(NotLinked, "revolution/mtx/mtx44.c"),
        ]
    ),
    RevolutionLib(
        "gx",
        [
            Object(NotLinked, "revolution/gx/GXInit.c"),
            Object(NotLinked, "revolution/gx/GXFifo.c"),
            Object(NotLinked, "revolution/gx/GXAttr.c"),
            Object(NotLinked, "revolution/gx/GXMisc.c"),
            Object(NotLinked, "revolution/gx/GXGeometry.c"),
            Object(NotLinked, "revolution/gx/GXFrameBuf.c"),
            Object(NotLinked, "revolution/gx/GXLight.c"),
            Object(NotLinked, "revolution/gx/GXTexture.c"),
            Object(NotLinked, "revolution/gx/GXBump.c"),
            Object(NotLinked, "revolution/gx/GXTev.c"),
            Object(NotLinked, "revolution/gx/GXPixel.c"),
            Object(NotLinked, "revolution/gx/GXTransform.c"),
            Object(NotLinked, "revolution/gx/GXPerf.c"),
        ]
    ),
    RevolutionLib(
        "dvd",
        [    
            Object(NotLinked, "revolution/dvd/dvdfs.c"),
            Object(NotLinked, "revolution/dvd/dvd.c"),
            Object(NotLinked, "revolution/dvd/dvdqueue.c"),
            Object(NotLinked, "revolution/dvd/dvderror.c"),
            Object(NotLinked, "revolution/dvd/dvdidutils.c"),
            Object(NotLinked, "revolution/dvd/dvdFatal.c"),
            Object(NotLinked, "revolution/dvd/dvd_broadway.c"),
        ]
    ),
    RevolutionLib(
        "demo",
        [
            Object(NotLinked, "revolution/demo/DEMOPuts.c"),
        ]
    ),
    RevolutionLib(
        "ai",
        [
            Object(NotLinked, "revolution/ai/ai.c"),
        ]
    ),
    RevolutionLib(
        "ax",
        [
            Object(NotLinked, "revolution/ax/AX.c"),
            Object(NotLinked, "revolution/ax/AXAlloc.c"),
            Object(NotLinked, "revolution/ax/AXAux.c"),
            Object(NotLinked, "revolution/ax/AXCL.c"),
            Object(NotLinked, "revolution/ax/AXOut.c"),
            Object(NotLinked, "revolution/ax/AXSPB.c"),
            Object(NotLinked, "revolution/ax/AXVPB.c"),
            Object(NotLinked, "revolution/ax/AXComp.c"),
            Object(NotLinked, "revolution/ax/DSPCode.c"),
        ]
    ),
    GenericLib(
        "runtime",
        [
            Object(NotLinked, "runtime/__va_arg.c"),
            Object(LinkedFor("vc-j"), "runtime/global_destructor_chain.c"),
            Object(NotLinked, "runtime/code_8015263C.c"),
            Object(NotLinked, "runtime/ptmf.c"),
            Object(NotLinked, "runtime/runtime.c"),
            Object(NotLinked, "runtime/__init_cpp_exceptions.cpp"), # TODO: matched but does not build OK
            Object(NotLinked, "runtime/Gecko_setjmp.c"),
            Object(NotLinked, "runtime/Gecko_ExceptionPPC.c"),
        ]
    )
]

### Execute mode

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    calculate_progress(config, args.progress_version)
else:
    sys.exit("Unknown mode: " + args.mode)
