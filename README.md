# ReHLDS-M

**English** | **[Русский](https://github.com/hun1er/rehlds-m/blob/main/README.ru.md)**

---

Rehlds-m is a modern, cross-platform launcher for the Half-Life Dedicated Server (HLDS), written from scratch in C++17. It acts as a replacement for the standard `hlds.exe` executable, providing enhanced performance, stability, and a host of new features for server administrators, while maintaining full compatibility with the original engine libraries (`swds.dll` on Windows, `engine_i486.so` on Linux) and game mods.

## Key Features & Differences from original HLDS/ReHLDS

- **Modern Codebase:** Written in C++17 using modern practices and the Standard Template Library (STL).
- **Performance:** Compiled for modern CPUs with SSE4.2 instruction set support for maximum efficiency.
- **Enhanced Console:**
  - **Advanced Input:** Full support for `HOME`, `END`, and `DELETE` keys for easier command editing.
  - **Command History:** The last 100 commands are saved to `input_history.txt` and loaded on startup. Navigation with arrow keys is fully supported.
  - **Improved Status Bar (Windows):** The server status bar is no longer tied to the first line and is always visible.
- **Full UTF-8 Support:** Ensures correct display of all characters without requiring OEM conversions on Windows.
- **New Performance Modes:**
  - Added `-pingboost` levels 4 and 5 for more aggressive performance tuning on both Windows and Linux.
  - Introduced the `-targetfps` parameter for adaptive real-time FPS management.
- **New Utility Parameters:**
  - `-conclearlog`: Clears the `qconsole.log` file on server start.
  - `-ignoresigint`: Prevents the server from shutting down on a `CTRL+C` keypress.
  - `-pidfile`: Saves the server's process ID to a specified file.
- **Stability:**
  - Fixes for several bugs present in the original ReHLDS.
  - Advanced memory dump creation on Windows for easier crash diagnostics.
  - Uses Winsock 2.2 on Windows (up from 2.0).

## Requirements

- **Operating System:**
  - Windows 7 - 11 (x86)
  - Linux with Glibc 2.18 or higher (e.g., Debian 8+, CentOS 8+, Ubuntu 14+).
- **CPU:** A modern processor with support for the **SSE4.2** instruction set.
- **Dependencies (Windows):** [Microsoft Visual C++ Redistributable 2015-2022 (x86)](https://aka.ms/vs/17/release/vc_redist.x86.exe) must be installed.

## Installation

1.  **Backup:** Make a backup of your original server executable (e.g., `hlds.exe` on Windows, or `hlds_linux`/`hlds_run` on Linux).
2.  **Replace:** Copy all files from the release archive into your server's root directory.
    -   For **Windows**, this includes `hlds.exe`.
    -   For **Linux**, this includes `hlds_linux` and several library files (`lib*.so`). Ensure the main executable has execute permissions (`chmod +x hlds_linux`).
3.  **Launch:** Start your server using the new executable as you normally would, with any desired command-line parameters.

## Command-Line Parameters

This launcher supports all standard HLDS parameters. Below are the new or significantly modified parameters introduced by rehlds-m.

### Performance Tuning

#### `-pingboost <level>`

This parameter adjusts the server's sleep policy to control CPU usage and server FPS.

-   **`0`**: (Default) Standard 1ms sleep. Minimal CPU usage.
-   **`1`**: (Linux only) Uses `setitimer` for a 1ms sleep cycle.
-   **`2`**: (Linux only) Uses `poll` instead of the legacy `select` function for a 1ms sleep.
-   **`3`**: The original engine's implementation (`NET_Sleep_Timeout`). Behavior is unchanged.
-   **`4`**: (New) A more aggressive boost. The CPU sleeps for a shorter duration (1 microsecond), resulting in higher FPS and responsiveness. Available on both Linux and Windows.
-   **`5`**: (New) Maximum performance mode. The server thread yields instead of sleeping, aiming for the highest possible FPS. This will cause one CPU core to run at 100% load.

#### `-targetfps <value>`

This is a modern alternative to `pingboost` and `sys_ticrate`. It dynamically adjusts the server's sleep time in real-time to maintain the desired FPS.

-   **Usage:** `-targetfps 1000`
-   It becomes more aggressive as the server's performance drops to meet the target.
-   For this mode to work correctly, you **must** set `sys_ticrate` to `0` in your `server.cfg` or command line (`+sys_ticrate 0`).

### Utility Parameters

#### `-conclearlog`

If present, this parameter will clear the contents of the `qconsole.log` file every time the server starts.

#### `-ignoresigint`

When used, this prevents the server from shutting down when `CTRL+C` is pressed in the console. The server can then only be shut down using the `quit` or `exit` command.

#### `-pidfile <filename>`

Writes the current process ID (PID) of the server to the specified file.
- **Usage:** `-pidfile "hlds.pid"`

## Building from Source

To compile the project yourself, you will need:

-   [Git](https://git-scm.com/)
-   [CMake](https://cmake.org/) (version 3.21 or higher)
-   A C++17 compatible compiler (e.g., Visual Studio 2022, GCC 9+, Clang 10+)

```bash
# 1. Clone the repository
git clone https://github.com/your-repo/rehlds-m.git
cd rehlds-m

# 2. Configure the project with CMake
cmake -B build

# 3. Build the project
cmake --build build --config Release
```
The compiled binary will be located in the `build/bin/` directory.

## F.A.Q.

-   **Q: UTF-8 characters are not displaying correctly in the console.**
    -   **A:** Change your console font to one that supports UTF-8 characters, such as `Consolas` or `Lucida Console`.

-   **Q: The server won't start on Windows, complaining about a missing `msvcp140.dll`.**
    -   **A:** You need to install the [Microsoft Visual C++ Redistributable 2015-2022 (x86)](https://aka.ms/vs/17/release/vc_redist.x86.exe).

-   **Q: The status bar is not visible on Windows 11.**
    -   **A:** Go to Settings -> Privacy & Security -> For developers -> Terminal, and change the default terminal application to "Windows Console Host".

## License

This project is licensed under the terms of the provided `LICENSE` file. It also utilizes components of the Half-Life 1 SDK, which is subject to the license agreement from Valve Corporation.
