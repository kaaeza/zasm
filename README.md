# Zilog Z80 Emulator
**ZASM** is an emulator of the Zilog Z80 CPU. It's written in C and usable from the terminal.
## Current status
### Pre-Alpha
This program is still in **Pre-Alpha**, expect major bugs and missing implementations.
### Warranty
**This program comes with absolutely NO WARRANTY.**

I do not take any responsibility for what may happen if you modify the code, even slightly.
## How to install
To install this program you firstly need to clone this repository.
You can do this using git, paste this command in the terminal:
```bash
git clone https://github.com/kaaeza/zasm
```
This command will create a directory called ***zasm***.

Then enter the directory with:
```bash
cd zasm
```


## Making the executable

To compile we will use **Make**
### Install Make

On Ubuntu and Debian:
```bash
sudo apt update
sudo apt upgrade -y
sudo apt install build-essential
```

On Arch based distro:
```bash
sudo pacman -S base-devel
```

### Compiling
After installing Make we can compile the project in an executable using this command:
```bash
make
```
**Make sure you are in the directory 'zasm'**

## Using ZASM
To use ZASM you can either run it straight from the directory using:

```bash
path-to-directory/bin/zasm -f file.asm
```

Or you can export the path so you can use it everywhere.

### Exporting the path
To export the path you need to open with a text editor the ***.bashrc*** file in your user directory.

Then you will add this line to the bottom of the file:

```bash
export PATH="$PATH:$HOME/zasm/bin"
```

After saving the file reload it with:
```bash
source ~/.bashrc
```

### Run ZASM

After exporting the path you can use ZASM from everywhere on your computer using:
```bash
zasm -f file.asm
```
For more info you can use the help flag:
```bash
zasm -h | zasm --help
```

## Supported Instructions

| Instruction | Description |
|-------------|-------------|
| `LD`        | Load a value into a register or memory |
| `ADD`       | Add a value to register A |
| `SUB`       | Subtract a value from register A |
| `AND`       | Logical AND with register A |
| `OR`        | Logical OR with register A |
| `XOR`       | Logical XOR with register A |
| `CP`        | Compare a value with register A |
| `INC`       | Increment a register or memory value by 1 |
| `DEC`       | Decrement a register or memory value by 1 |
| `ADC`       | Add value and Carry flag to register A |
| `SBC`       | Subtract value and Carry flag from register A |
| `JP`        | Jump to an absolute address (can be conditional) |
| `JR`        | Jump relative to the current PC (can be conditional) |
| `DJNZ`      | Decrement register B and jump if not zero |
| `HALT`      | Stop program execution |
| `NOP`       | No operation (does nothing) |
| `CCF`       | Complement the Carry flag (invert it) |
| `SCF`       | Set the Carry flag (CY = 1) |
| `NEG`       | Negate the value in register A (A = 0 - A) |
| `ERR`       | Represents an unrecognized instruction or syntax error |
