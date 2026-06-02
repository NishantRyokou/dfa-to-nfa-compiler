# Setup Guide for Windows

## Quick Start

Your project is ready but you need to install compiler tools. Choose one:

### ✅ RECOMMENDED: MSYS2 (Easiest)

1. **Download MSYS2**: https://www.msys2.org/
   - Download the installer (x86_64)
   
2. **Install**:
   - Run the installer
   - Choose installation path (default: `C:\msys64`)
   - Follow the prompts

3. **First Run - Update packages**:
   - Open "MSYS2 MinGW 64-bit" from Start Menu
   - Run these commands:
     ```bash
     pacman -Syuu
     ```
   - Close and reopen the terminal

4. **Install Build Tools**:
   ```bash
   pacman -S base-devel flex bison mingw-w64-x86_64-gcc mingw-w64-x86_64-make
   ```

5. **Navigate to Project**:
   ```bash
   cd /c/Users/legam/Documents/btech/assignment/toc
   ```

6. **Build**:
   ```bash
   make
   ```

7. **Run**:
   ```bash
   ./dfa_converter < tests/test_input.dfa
   ```

---

### Alternative: Ubuntu WSL (Windows Subsystem for Linux)

If you have Windows 10/11:

1. **Enable WSL2**:
   - Open PowerShell as Administrator
   - Run: `wsl --install`
   - Restart computer

2. **Install Ubuntu**:
   - Open Microsoft Store
   - Search for "Ubuntu"
   - Click "Get" to install

3. **Open Ubuntu Terminal and run**:
   ```bash
   sudo apt update
   sudo apt install flex bison gcc make
   cd /mnt/c/Users/legam/Documents/btech/assignment/toc
   make
   ./dfa_converter < tests/test_input.dfa
   ```

---

### Alternative: Windows Package Manager (Chocolatey)

If you already have Chocolatey:

```powershell
choco install make mingw
# Then install flex and bison via MSYS2
```

---

## Verify Installation

After installing, open a terminal and verify:

```bash
gcc --version
flex --version
bison --version
make --version
```

All should output version information.

---

## Manual Build (Without Make)

If you can't install make, use the batch script:

```cmd
cd C:\Users\legam\Documents\btech\assignment\toc
build.bat
```

Then run:
```cmd
dfa_converter.exe < tests\test_input.dfa
```

**Note**: Requires flex and bison (install via MSYS2)

---

## Troubleshooting

### "Command not found"
- Add MSYS2 to PATH: `C:\msys64\mingw64\bin`
- Or use full MSYS2 terminal

### "flex: command not found"
- In MSYS2: `pacman -S flex`

### "bison: command not found"  
- In MSYS2: `pacman -S bison`

### Compilation errors
- Ensure all tools are installed
- Try rebuilding: `make clean && make`

---

## Project Status

✅ All source files created
✅ Ready to build
⏳ Waiting for toolchain setup

Once you install Flex, Bison, and Make, run: `make`
