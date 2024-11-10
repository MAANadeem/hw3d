# Graphics Simulation Engine

A high-performance graphics simulation engine built with **C++**, **DirectX11**, **Win32 APIs**, and **ImGui**. This engine simulates realistic, dynamic 3D scenes with real-time lighting, object manipulation, and smooth first-person navigation.

## Key Features

- **Real-Time First-Person Navigation**: Explore the scene with smooth, responsive first-person controls.
- **Parameter Control GUI**: Easily adjust lighting, camera, object properties, and more in real-time using an intuitive ImGui-based GUI.
- **Dynamic Object Manipulation**: Interact with and manipulate objects directly within the simulation.
- **Optimized Performance**: Capable of rendering over 10,000 objects per scene with custom optimizations
- **Advanced Lighting & Shaders**: Custom HLSL shaders and advanced lighting techniques bring lifelike materials and detailed animations to the environment.

## Getting Started

### Prerequisites

- **Windows OS** (for Win32 API and DirectX11)
- **Visual Studio 2019+** (or a compatible C++ compiler)
- **DirectX11 SDK**

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/GraphicsSimulationEngine.git
   cd GraphicsSimulationEngine
2. Open the Visual Studio solution file (.sln), and ensure all dependencies are linked.
3. Build the project and run the executable.

### Controls
- First person navivation
  - WASD: Move Camera
- GUI Controls: Adjust lighting, material properties, and object parameters through the ImGui interface.
- Tab: Switch scenes

## Technical Overview

### Architecture
This engine leverages DirectX11 and the 3D rendering pipeline to deliver high-quality visuals. Custom HLSL shaders are used for complex lighting and material effects, while Win32 API handles window management. The ImGui library powers an accessible GUI for real-time parameter control.

### Performance Optimization
Designed to handle thousands of objects efficiently, this engine includes optimizations that reduce CPU/GPU load and maintain a high frame rate, achieving up to 40% performance gains in scenes with over 10,000 objects.

### Rendering Pipeline
1. Vertex Processing: Optimized vertex shaders handle transformations.
2. Rasterization: Efficient geometry culling techniques reduce rendering overhead.
3. Pixel Processing: Custom pixel shaders apply dynamic lighting and material effects.

## Roadmap
Future Enhancements:
- Additional lighting models (e.g., global illumination)
- Support for more complex animations and skeletal rigs
- Integration with modern physics engines (e.g., Bullet Physics)

## License
This project is licensed under the MIT License. You may use, distribute, and modify this software under the terms of the MIT license. See LICENSE file for more details.
