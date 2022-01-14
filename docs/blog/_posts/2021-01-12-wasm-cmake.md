---
layout: post
title:  "Integrating Emscripten and CMake with VS Code"
author: Eric Morgan
date:   2021-01-12 17:35:26 -0700
categories: setup
tags: setup
---

In planning for the development of our NES emulator, we decided to target WASM for our interface and to utilize CMake to generate our build system.  As I also utilize VS Code as my IDE, I wanted to integrate these components in such a way as to ease my development process.  I'm hoping this documentation will help others in setting up their environment.  This documentation is focused on setup for a Linux environment, though I would imagine a lot of the VS Code configuration steps would translate to other environments.

#### Install Dependencies
In addition to installing VSCode and any other development tools you may need, you'll also need to install CMake and Emscripten.  I was able to install an appropriate version of CMake through my package manager:  

`apt-get install cmake`  

Installing Emscripten was a bit more difficult, though well documented.  I followed the instructions [here](https://emscripten.org/docs/getting_started/downloads.html).  In summary:  

    # Get the emsdk repo
    git clone https://github.com/emscripten-core/emsdk.git  

    # Enter that directory
    cd emsdk

    # Fetch the latest version of the emsdk (not needed the first time you clone)
    git pull

    # Download and install the latest SDK tools.
    ./emsdk install latest

    # Make the "latest" SDK "active" for the current user. (writes .emscripten file)
    ./emsdk activate latest

    # Activate PATH and other environment variables in the current terminal
    source ./emsdk_env.sh

#### Install VSCode Extensions
I'm using a number of VS Code extensions to ease my development process.  The following extensions will need to be installed:
 - C/C++ - For language support.
 - CMake Tools - Provides CMake language support and extends features into the IDE UI.
 - C++ TestMate Legacy - Adds testing to the side bar, supports Catch2.  I had issues with the latest version, but the legacy version worked with no issues.

#### Setup CMake Project
The exact structure and contents of CMake files will vary by project, but my projects is setup as follows.  The structure was based largely on the example [here](https://cmake.org/examples/):  
- root
    - CMakeLists.txt
    - include - Project Header Files
    - src
        - wasm - CMakeLists.txt and Emscripten specific files.  The CMake build statements here are wrapped in an if statement and will only execute when CMake is targeting Emscripten.
        - emulator - CMakeLists and all emulator library files.
    - tests - CMakesLists.txt and various unit test files.  The CMake build statements here are wrapped in an if statement and will only execute when CMake is not targeting Emscripten.  

Using either the demo example or pulling our [repo](https://github.com/ericcolvinmorgan/NESEmulation) directly, you should now be able to build using CMake.  Run the follow to build:  

`mkdir build && cd build && cmake ..`

If you're able to successfully build via the command line, you should also now see a CMake extension in the sidebar and your project outline should be included here.

#### Setup Emscripten Environment
Getting everything setup so IntelliSense is working, and so you can easily switch between Emscripten and another compiler is the next step.  I setup my environment so CMake provides IntelliSense directly to VS Code.  This feature is provided by the C++ extension and is enabled via the configurationProvider setting.  A full reference can be found [here](https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference).  This setting should be defined as follows in the .vscode/c_cpp_properties.json file:  

`"configurationProvider": "ms-vscode.cmake-tools"`

You may still see IntelliSense errors when you first make this change, or when compiling using different kits.  You should see the IntelliSense update appropriately when you build through the CMake extension.

Finally, while there are a number of preconfigured kits available by default, we'll need to define a CMake kit specific to Emscripten.  [This link](https://vector-of-bool.github.io/docs/vscode-cmake-tools/kits.html) does a good job walking through what Kits are and how to set them up.  This process is definitely more straight forward than it sounds, and Emscripten makes this process incredibly easy by providing a toolchain file as part of the SDK -- this file can be viewed [here](https://github.com/emscripten-core/emscripten/blob/main/cmake/Modules/Platform/Emscripten.cmake).  As mentioned directly in the file, the toolchain file "teaches CMake about the Emscripten compiler, so that CMake can generate makefiles from CMakeLists.txt that invoke emcc."  The final kit definition used is as follows, it will need to be updated for the specific locations to where you installed your emsdk in the instructions above:  

```json
[
  {
    "name": "Emscripten",
    "toolchainFile": "/usr/local/lib/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake",
    "compilers": {
      "C": "/usr/local/lib/emsdk/upstream/emscripten/emcc",
      "CXX": "/usr/local/lib/emsdk/upstream/emscripten/em++"
    }
  }
]
```

If everything is setup correctly you should now be able to toggle your active kit to Emscripten in the status bar at the bottom of VS Code.  Clicking build should then compile using the emsdk.  These steps will make it very easy to compile the project to WASM when we're ready to test in a browser, and toggle to a different compiler when testing against our test suite.  Setting up an unfamiliar environment is always a time-consuming process.  Hopefully this documentation will help save someone a few hours when building a similar environment.