# How to Build Kilobdat

Follow these steps to build and run the Kilobdat program:

## Prerequisites
- A C++ compiler (e.g., `g++` for Linux/MacOS, MinGW for Windows, or Visual Studio).

## Build Instructions

1. **Clone the repository**:
   ```bash
   git clone https://github.com/TimofeyLednev/kilobdat.git

2. **Navigate to the project directory**:
  cd kilobdat
3. **Compile the program**:
  - On Linux/MacOS (using g++):
  g++ main.cpp GameFunctions.cpp -o kilobdat
  - On Windows (using MinGW):
  g++ main.cpp GameFunctions.cpp -o kilobdat.exe
  - Alternatively, if you're using Visual Studio:

    Open the project in Visual Studio.
    Build the solution (usually F5 or via the "Build" menu).
    The executable will be generated in the Debug or Release folder (e.g., kilobdat.exe).
4. **Run the program**:

-  On Linux/MacOS: 
    ./kilobdat

-  On Windows:   
    kilobdat.exe
