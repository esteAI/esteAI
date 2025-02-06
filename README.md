![Preview](https://pbs.twimg.com/profile_banners/1887503644230307840/1738851754/1500x500)


<h1 align="center">
 esté - pushing the boundaries of intelligence, automation, and human potential.</a>
</h1>


<p>
Our project redefines possibilities, pushing the boundaries of intelligence, automation, and human potential. This is more than technology—it’s a revolution. <b>(ESTÉ)</b> is an artificial life simulation tool based on a specialized 2D particle engine in CUDA for soft bodies and fluids. Each simulated body consists of a network of particles that can be upgraded with higher-level functions, ranging from pure information processing capabilities to physical equipment (such as sensors, muscles, weapons, constructors, etc.) whose executions are orchestrated by neural networks. The bodies can be thought of as agents or digital organisms operating in a common environment. Their blueprints can be stored in genomes and passed on to offspring.
</p>
<p>
The simulation code is written entirely in CUDA and optimized for large-scale real-time simulations with millions of particles.
The development is driven by the desire to better understand the conditions for (pre-)biotic evolution and the growing complexity of biological systems.
An important goal is to make the simulator user-friendly through a modern user interface, visually appealing rendering and a playful approach. 
</p>

<p>
  Please follow our X/Twitter <a href="https://x.com/esteartificial" target="_blank">X</a> as a place for discussions, new developments and feedback around ESTÉ and artificial life in general.
</p>

#  Main features
### Physics and graphics engine
- Particles for simulating soft and rigid body mechanics, fluids, heat dissipation, damage, adhesion etc.
- Real-time user interactions with running simulations
- Simulation runs entirely on GPU via CUDA
- Rendering and post-processing via OpenGL using CUDA-OpenGL interoperability


### Artificial Life engine extensions
- Multi-cellular organisms are simulated as particle networks
- Genetic system and cell by cell construction of offspring
- Neural networks for controlling higher-level functions (e.g. sensors and muscles)
- Various colors may be used to customize cell types according to own specifications
- Support for spatially varying simulation parameters


### Extensive editing tools
- Graph editor for manipulating every particle and connection
- Freehand and geometric drawing tools
- Genetic editor for designing customized organisms
- Mass-operations and (up/down) scaling functions

### Networking
- Built-in simulation browser
- Download and upload simulation files
- Upvote simulations by giving stars

###  How is this useful?
- A first attempt to answer: Feed your curiosity by watching evolution at work! As soon as self-replicating machines come into play and mutations are turned on, the simulation itself does everything.
- Perhaps the most honest answer: Fun! It is almost like a game with a pretty fast and realistic physics engine. You can make hundreds of thousands of machines accelerate and destroy with the mouse cursor. It feels like playing god in your own universe with your own rules. Different render styles and a visual editor offer fascinating insights into the events.
  
- A more academic answer: A tool to tackle fundamental questions of how complexity or life-like structure may arise from simple components. How do entire ecosystems adapt to environmental changes and find a new equilibrium? How to find conditions that allow open-ended evolution?
- A tool for generative art: Evolution is a creative force that leads to ever new forms and behaviors.

### Documentation
The latest version includes a brief documentation and user guidance in the program itself via help windows and tooltips.

### Minimal system requirements
An Nvidia graphics card with compute capability 6.0 or higher is needed. Please check [https://en.wikipedia.org/wiki/CUDA#GPUs_supported](https://en.wikipedia.org/wiki/CUDA#GPUs_supported).

In the case that the program crashes for an unknown reason, please refer to the troubleshooting section below.

### 🔨 How to build the sources
The build process is mostly automated using the cross-platform CMake build system and the vcpkg package manager, which is included as a Git submodule.

### Getting the sources
To obtain the sources, please open a command prompt in a suitable directory (which should not contain whitespace characters) and enter the following command:
```
git clone --recursive https://github.com/esteAI/esteAI.git
```
Note: The `--recursive` parameter is necessary to check out the vcpkg submodule as well. Besides that, submodules are not normally updated by the standard `git pull` command. Instead, you need to write `git pull --recurse-submodules`.

### Build instructions
Prerequisites: [CUDA Toolkit 11.2+](https://developer.nvidia.com/cuda-downloads) and a toolchain for CMake (e.g. GCC 9.x+ or [MSVC v142+](https://visualstudio.microsoft.com/vs/)).

Build steps:
```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -j8
```
If everything goes well, the  executable can be found under the build directory in `./esteAI` or `.\Release\esteai.exe` depending on the used toolchain and platform.
It is important to start ESTÉ directly from the build folder, otherwise it will not find the resource folder.

There are reported build issues with (updated: 2025-02-06)
* GCC 12+ (version 11 should work)
* Visual Studio 17.10 (version 17.9 should work)
* CUDA 12.5 (version 12.4 should work)

### ⌨️ Command-line interface

This repository also contains a CLI for ESTÉ. It can be used to run simulations without using a GUI. This is useful for performance measurements as well as for automatic execution and evaluation of simulations for different parameters.
The CLI takes the simulation file, along with its parameters and the number of time steps, as input. It then provides the resulting simulation file and the statistics (as a CSV file) as output.
For example,
```
.\cli.exe -i example.sim -o output.sim -t 1000
```
runs the simulation file `example.sim` for 1000 time steps.

### 🔎 Troubleshooting

Please make sure that:
1) You have an NVIDIA graphics card with compute capability 6.0 or higher (for example GeForce 10 series).
2) You have the latest NVIDIA graphics driver installed.
3) The name of the installation directory (including the parent directories) should not contain non-English characters. If this is not fulfilled, please re-install ESTÉ to a suitable directory. Do not move the files manually. If you use Windows, make also sure that you install ALIEN with a Windows user that contains no non-English characters. If this is not the case, a new Windows user could be created to solve this problem.
4) ESTÉ needs write access to its own directory. This should normally be the case.
5) If you have multiple graphics cards, please check that your primary monitor is connected to the CUDA-powered card. ESTÉ uses the same graphics card for computation as well as rendering and chooses the one with the highest compute capability.
6) If you possess both integrated and dedicated graphics cards, please ensure that the este-executable is configured to use your high-performance graphics card. On Windows you need to access the 'Graphics settings,' add 'esteai.exe' to the list, click 'Options,' and choose 'High performance'.

If these conditions are not met, ESTÉ may crash unexpectedly.
If the conditions are met and the error still occurs, please start ESTÉ with the command line parameter `-d`, try to reproduce the error and then create a GitHub issue on https://github.com/chrxh/esteai/issues where the log.txt is attached.

### 🌌 Screenshots
#### Different plant-like populations around a radiation source
![Screenshot1](https://user-images.githubusercontent.com/73127001/229311601-839649a6-c60c-4723-99b3-26086e3e4340.jpg)

<h1 align="center"></h1>

#### Close-up of different types of organisms so that their cell networks can be seen
![Screenshot2](https://user-images.githubusercontent.com/73127001/229311604-3ee433d4-7dd8-46e2-b3e6-489eaffbda7b.jpg)

<h1 align="center"></h1>

#### Different swarms attacking an ecosystem
![Screenshot3](https://user-images.githubusercontent.com/73127001/229311606-2f590bfb-71a8-4f71-8ff7-7013de9d7496.jpg)

<h1 align="center"></h1>

#### Genome editor
![Screenshot3b](https://user-images.githubusercontent.com/73127001/229313813-c9ce70e2-d61f-4745-b64f-ada0b6758901.jpg)

### 🧩 Contributing to the project
Contributions to the project are very welcome. The most convenient way is to communicate via [GitHub Issues](https://github.com/esteai/esteai/issues), [Pull requests](https://github.com/esteai/esteai/pulls) or the [Discussion forum](https://github.com/esteai/esteai/discussions) depending on the subject. For example, it could be
- Providing new content (simulation or genome files)
- Producing or sharing media files
- Reporting of bugs, wanted features, questions or feedback via GitHub Issues or in the Discussion forum.
- Pull requests for bug fixes, code cleanings, optimizations or minor tweaks. If you want to implement new features, refactorings or other major changes, please use the [Discussion forum](https://github.com/esteai/esteai/discussions) for consultation and coordination in advance.

### 💎 Credits and dependencies

ESTÉ has been initiated, mainly developed and maintained by [Christian Heinemann](mailto:heinemann.christian@gmail.com). Many thanks to everyone who has contributed to this project in any way. 
### 🧾 License
ESTÉ is licensed under the [BSD 3-Clause](LICENSE) license.
