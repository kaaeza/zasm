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