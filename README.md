# Cub3D - 3D Raycaster Engine
> A high-performance 3D raycasting engine inspired by classic games like Wolfenstein 3D.

## Introduction
Welcome to Cub3D – a 3D raycasting engine that brings retro gaming to life. This project implements a complete first-person perspective renderer using the raycasting technique, creating a realistic 3D environment from 2D map data. Built as part of the rigorous 42 École curriculum, Cub3D demonstrates advanced graphics programming concepts, real-time rendering optimization, and immersive gameplay mechanics.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Map Format](#map-format)
- [Detailed Functionality and Implementation](#detailed-functionality-and-implementation)
- [Technical Architecture](#technical-architecture)
- [Performance Optimizations](#performance-optimizations)
- [Requirements](#requirements)
- [Acknowledgments](#acknowledgments)

## Features
- **3D Raycasting Engine:** Real-time 3D rendering using the classic raycasting algorithm with DDA (Digital Differential Analysis).
- **Textured Wall Rendering:** Support for directional wall textures (North, South, East, West) with proper perspective correction.
- **Customizable Floor & Ceiling:** Configurable solid colors for floor and ceiling surfaces.
- **Smooth Movement Controls:** WASD movement with collision detection and smooth camera rotation (A/D keys).
- **Optimized Rendering:** Level-of-Detail (LOD) system that reduces computational load for distant objects.
- **Comprehensive Map Parsing:** Robust parser for `.cub` map files with error handling and validation.
- **Cross-Platform Graphics:** Built with MiniLibX for reliable graphics rendering across Unix systems.

### Bonus Features
- **Interactive Door System:** Open/close doors with spacebar when near them.
- **Mouse Look Controls:** Smooth mouse-based camera rotation for enhanced immersion.
- **Animated Weapon Display:** First-person gun animation with firing effects (left mouse click).
- **Dynamic Lighting System:** Toggle between light/dark modes affecting floor and ceiling textures.
- **Real-Time Minimap:** Top-down view showing player position, walls, doors, and field of view.
- **FPS Counter:** Real-time frame rate display for performance monitoring.
- **Advanced Collision System:** Enhanced collision detection supporting doors and complex geometry.

## Installation
Follow these steps to set up Cub3D:

1. **Clone the Repository:**
    ```bash
    git clone https://github.com/denuen/cub3d.git
    cd cub3d
    ```

2. **Compile the Mandatory Version:**
    ```bash
    make
    ```

3. **Compile the Bonus Version:**
    ```bash
    make bonus
    ```

4. **Run Cub3D:**
    ```bash
    ./cub3d mandatory/cub.cub        # Mandatory version
    ./cub3d_bonus bonus/cub.cub  # Bonus version
    ```

*Note:* Ensure you have `gcc`, GNU Make, and X11 development libraries installed.

## Usage
### Basic Controls (Mandatory)
- **W/S:** Move forward/backward
- **A/D:** Rotate camera left/right
- **ESC:** Exit the program
- **Red X:** Close window to exit

### Enhanced Controls (Bonus)
- **W/A/S/D:** Move forward/left/backward/right
- **Mouse Movement:** Look around (camera rotation)
- **Left Mouse Click:** Fire weapon animation
- **Spacebar:** Open/close doors when nearby
- **L:** Toggle lighting mode
- **ESC:** Exit the program

## Map Format
Cub3D uses `.cub` files for map configuration. Here's the format:

### Mandatory Format
```
NO ./path/to/north_texture.xpm
SO ./path/to/south_texture.xpm
WE ./path/to/west_texture.xpm
EA ./path/to/east_texture.xpm

F 220,100,0    # Floor color (RGB)
C 225,30,0     # Ceiling color (RGB)

        1111111111111111111111111
        1000000000110000000000001
        1011000001110000000000001
        1001000000000000000000001
111111111011000001110000000000001
100000000011000001110111111111111
11110111111111011100000010001
11110111111111011101010010001
11000000110101011100000010001
10000000000000001100000010001
10000000000000001101010010001
11000001110101011111011110N0111
11110111 1110101 101111010001
11111111 1111111 111111111111
```

### Bonus Format
The bonus version supports additional texture definitions:
```
WA ./textures/Wall/wall.xpm           # Wall texture
G1 ./textures/Gun/gun_fire.xpm        # Gun firing texture
G2 ./textures/Gun/gun_nofire.xpm      # Gun idle texture
DO ./textures/Door/door.xpm           # Door texture
C1 ./textures/Ceiling/ceiling_light.xpm
C2 ./textures/Ceiling/ceiling_nolight.xpm
F1 ./textures/Floor/floor_light.xpm
F2 ./textures/Floor/floor_nolight.xpm
# ... more texture definitions
```

### Map Rules
- **1:** Wall
- **0:** Empty space (floor)
- **3:** Door (bonus only)
- **N/S/E/W:** Player starting position and orientation
- Map must be surrounded by walls
- Only one player position allowed

## Detailed Functionality and Implementation

### 1. Raycasting Algorithm
The core rendering engine implements the classic raycasting technique:
- **Ray Generation:** For each screen column, a ray is cast from the player position
- **DDA Algorithm:** Digital Differential Analysis efficiently determines wall intersections
- **Distance Calculation:** Perpendicular wall distance prevents fish-eye distortion
- **Wall Height Projection:** 3D wall height calculated based on distance and screen resolution

### 2. Texture Mapping System
- **Directional Textures:** Different textures for each wall face (North, South, East, West)
- **Texture Coordinates:** Precise mapping from wall intersection points to texture coordinates
- **Perspective Correction:** Proper texture scaling maintains visual consistency
- **XPM Format Support:** Efficient loading and manipulation of XPM texture files

### 3. Movement and Camera System
- **Collision Detection:** Prevents walking through walls with configurable collision margins
- **Smooth Rotation:** Camera direction and field-of-view plane rotation using trigonometric functions
- **Movement Vectors:** Forward/backward movement along camera direction vector
- **Input Handling:** Real-time key press/release detection with smooth movement

### 4. Map Parsing and Validation
- **Robust Parser:** Comprehensive parsing of .cub files with detailed error reporting
- **Map Validation:** Ensures map integrity, proper wall boundaries, and single player spawn
- **Memory Management:** Efficient allocation and cleanup of map data structures
- **Error Handling:** Clear error messages for malformed maps or missing resources

### 5. Bonus Features Implementation

#### Interactive Door System
- **Proximity Detection:** Calculates player distance to doors for interaction
- **State Management:** Tracks open/closed state for each door in the map
- **Collision Integration:** Doors affect movement and raycasting when closed

#### Mouse Look Controls
- **Sensitivity Control:** Configurable mouse sensitivity for comfortable gameplay
- **Smooth Rotation:** Mouse movement translated to camera rotation with acceleration control
- **Boundary Management:** Proper handling of mouse cursor positioning

#### Minimap System
- **Real-Time Rendering:** Live top-down view of the game world
- **Field of View Visualization:** Shows player's current viewing cone
- **Multi-Element Display:** Walls, doors, player position, and movement direction

#### Lighting System
- **Texture Switching:** Dynamic switching between light and dark texture variants
- **Global State Management:** Consistent lighting state across all surfaces
- **Visual Feedback:** Immediate visual response to lighting changes

## Technical Architecture

### Core Data Structures
- **t_cub:** Main game state container
- **t_raycaster:** Ray calculation and player positioning
- **t_map:** Map data, dimensions, and validation
- **t_textures:** Texture management and loading
- **t_ray:** Individual ray calculation data

### Memory Management
- **Dynamic Allocation:** Efficient memory allocation for rays and map data
- **Resource Cleanup:** Comprehensive cleanup on exit to prevent memory leaks
- **Error Recovery:** Graceful handling of allocation failures

### Graphics Pipeline
1. **Input Processing:** Handle keyboard/mouse input
2. **Ray Casting:** Calculate ray intersections for each screen column
3. **Texture Sampling:** Sample appropriate textures based on wall orientation
4. **Screen Rendering:** Draw textured walls, floor, and ceiling
5. **Overlay Rendering:** Draw minimap, FPS counter, and weapon (bonus)
6. **Frame Presentation:** Present completed frame to screen

## Performance Optimizations

### Level of Detail (LOD) System
- **Distance-Based Optimization:** Reduces rendering quality for distant objects
- **Configurable Thresholds:** Adjustable LOD distance threshold
- **Render Scale Adjustment:** Variable column rendering based on distance

### Efficient Algorithms
- **DDA Implementation:** Optimized Digital Differential Analysis for fast ray-wall intersection
- **Memory Access Patterns:** Cache-friendly data structure layout
- **Trigonometric Optimization:** Pre-calculated values where possible

### Frame Rate Management
- **Target FPS Control:** Maintains consistent 60 FPS (bonus version)
- **Frame Time Monitoring:** Real-time performance metrics
- **Adaptive Rendering:** Dynamic quality adjustment based on performance

## Requirements
- **System:** Unix or Linux environment with X11
- **Compiler:** GCC (GNU Compiler Collection)
- **Build Tool:** GNU Make
- **Graphics Library:** MiniLibX (included)
- **Dependencies:** X11 development libraries (libX11-dev, libXext-dev)
- **Standards Compliance:** Code adheres to the 42 Norme standards

### Installation Dependencies (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install gcc make libx11-dev libxext-dev libbsd-dev
```

## Acknowledgments
- **Development Team:** This project was developed collaboratively by **denuen** and **ahmeee-dev**, combining expertise in graphics programming and optimization techniques
- **Wolfenstein 3D:** The classic game that pioneered raycasting technology and inspired countless developers
- **MiniLibX Community:** For the graphics library, documentation, and ongoing support
- **42 School Community:** Special thanks to fellow students and peers who provided valuable feedback and shared experiences throughout the development process

---

*Experience the thrill of classic 3D gaming with modern optimization and educational value!*
