## 🛠️ Prerequisites & Installation

⚠️ **Important:** This project relies on the **Simple Game Graphics (SGG)** library developed at AUEB. The library files are NOT included in this repository.

### 1. Download the SGG Library
You need to clone and build the SGG library separately:
* **Official Repository:** [https://github.com/cgaueb/sgg](https://github.com/cgaueb/sgg)
* **Documentation:** [https://cgaueb.github.io/sgg/](https://cgaueb.github.io/sgg/)

### 2. Build SGG
1. Download/Clone the SGG repo from the link above.
2. Run the build script for your system (e.g., `build_sgg_x64.bat` for Windows or `.sh` for Linux) found inside the folder.
3. This will generate the `sgg.lib` (and `sggd.lib` for debug) inside the `lib/` folder.

### 3. Setup & Run My Game
1. Clone my repository.
2. Copy the `include/` folder from SGG into my project's folder (or configure your compiler's include path).
3. Link the `sgg.lib` you built in Step 2.
4. Ensure the DLLs (from SGG's `bin/` folder) are in the same folder as the executable.
5. Compile and run!
