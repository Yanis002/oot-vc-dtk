# oot-gc

A work-in-progress decompilation of the N64 emulator used in the GameCube releases of The Legend of Zelda: Ocarina of Time.

Supported versions:

- `vc-j`: Wii Virtual Console - Japan

Currently the decompilation mainly targets the `vc-j` version.

## Building

### Requirements

You will need the following dependencies:
* git
* ninja
* python3
* wine (for macOS or non-x86 Linux)
* clang-format (optional)

#### Ubuntu/Debian/Windows (WSL)

You can install the dependencies with the following commands:

```
sudo apt-get update
sudo apt-get install git ninja-build python3
```

#### macOS

You can install dependencies via Homebrew with the following command:

```
brew install git ninja python3
brew install --cask --no-quarantine gcenx/wine/wine-crossover
```

#### Windows (Native)

You will need the following dependencies:
- [ninja.exe](https://github.com/ninja-build/ninja/releases/latest)
- Python (make sure to add it to your PATH during the installation)
- [Git for Windows](https://www.git-scm.com/downloads)

You need to add ``C:\Program Files\Git\bin`` to your system's PATH (not the user one) in order to execute bash scripts properly.

To get objdiff to work properly you also need to add the path to the folder containing ``ninja.exe`` to the system's PATH.

### Instructions

1. Clone the repo using `git clone https://github.com/zeldaret/oot-vc`.

2. Extract the following binary containing the N64 emulator from the wad of the version you want to build:

  * `vc-j`: `content1.app`

  Then, copy the app file and place it in the repo as `orig/<version>/content1.app`.

  You can use [gzinject](https://github.com/krimtonz/gzinject) to perform both of these extraction steps

3. Run `python3 configure.py` to generate the build. (Note: on Windows you might need to run ``python configure.py``.)

4. Run `ninja` to build the `vc-j` version, or run `ninja <version>` to build another version.

## Development Tools

### Scripts

* `./dol-diff <version>` will run `dtk dol diff` to show the first differing symbol if the build does not match.
* `./dol-apply <version>` will run `dtk dol apply` to sync symbols (e.g. if a function was renamed in the repo).
* `./format` will format all source files with `clang-format`.

### objdiff

For local decompilation testing, start the objdiff GUI and open this directory as the project directory.
Currently `objdiff` may not work properly on files using asm-processor (i.e. files with `asm_processor=True` in `configure.py`).

### asm-differ (diff.py)

First, copy a matching build to the `expected/` directory to diff against:

```
mkdir expected
cp -r build expected/
```

Then run e.g. `./diff.py -mwo3 xlMain` to diff a function for `ce-j`, or e.g. `./diff.py -mwo3 xlMain -v mq-j` to diff a function for another version.

### decomp.me

Run `tools/decompme.py <c-file> <asm-file>` (e.g. `tools/decompme.py src/emulator/cpu.c asm/non_matchings/cpu/cpuExecute.s`) to create a
[decomp.me](https://decomp.me/) scratch for a function. The C file and all of its included headers will be used as the context.

### Permuter

To import a function for [decomp-permuter](https://github.com/simonlindholm/decomp-permuter), ensure `powerpc-eabi-objdump` binary
is on your `PATH` (for instance by adding `tools/binutils` from this project) and run something like

```sh
path/to/permuter/import.py src/emulator/cpu.c asm/non_matchings/cpu/cpuExecute.s
path/to/permuter/permuter.py nonmatchings/cpuExecute -j 8
```

Sometimes you may need to tweak the source in order for things to import
correctly, for example by explicitly marking auto-inlined functions as `inline`.

### Debug Info

The files in the `debug/` directory contain a dump of the DWARF debugging information in the original ELF. Functions marked as `// Erased`
were present at one time but have been stripped by the linker, because they were either unused or inlined at all call sites.
