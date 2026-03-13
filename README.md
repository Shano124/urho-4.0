---

# Urho 4.0

![](https://raw.githubusercontent.com/urho3d-learn/flappy-urho/main/screen.png)

[![CI/CD](https://github.com/urho3d/Urho3D/workflows/CI/CD/badge.svg)](https://github.com/urho3d/Urho3D/actions?query=workflow%3ACI%2FCD)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/4954/badge.svg)](https://scan.coverity.com/projects/urho3d-urho3d)

**Urho 4.0** is a free lightweight, cross-platform 2D and 3D game engine implemented in C++ and released under the MIT license.

## License

Licensed under the MIT license, see [LICENSE](licenses/urho3d/LICENSE) for details.  
Third-party library licenses: [licenses](licenses).  
Credits: [licenses/urho3d/credits.md](licenses/urho3d/credits.md).

## Important Links

* [Last Official Release (1.9.0)](https://github.com/urho3d/urho3d/releases/tag/1.9.0)
* [API Documentation (1.9.0)](https://urho3d-doxygen.github.io/1_9_0_tutors/index.html)
* [Latest Documentation](https://urho3d-doxygen.github.io/latest/index.html)
* [Community Wiki Archive](https://github.com/urho3d-community/wiki-archive/wiki)
* [Community Forum](https://github.com/urho3d-community/discussions/discussions/categories/for-english-speakers)
* [Web Samples](https://urho3d-web-samples.github.io)
* [Community Tools](https://github.com/urho3d-tools)

## Project Founders & Contributors

The original founder is [Lasse Öörni](https://github.com/cadaver). Major contributions have also been made by [Yao Wei Tjong](https://github.com/weitjong), [asterj](https://github.com/aster2013), and many other developers - see the [complete contributor list](https://github.com/urho3d/Urho3D/graphs/contributors).

---

## About Urho 4.0

Urho 4.0 is a modern fork of the original Urho3D project, focusing on:

- **Performance**: Optimized for contemporary and legacy hardware (i3 4th gen, OpenGL support)
- **Portability**: Cross-platform support (Windows, Linux, macOS, and more)
- **Game Development**: Comprehensive 2D and 3D game engine features
- **Physics Integration**: Bullet Physics engine with vehicle support
- **Extensibility**: Clean C++ API with support for custom components and systems

### Key Features

- **Graphics**: OpenGL and DirectX support, advanced shader systems
- **Physics**: 3D rigid body dynamics, vehicle physics (RaycastVehicle)
- **Audio**: 3D positional sound with spatial effects
- **Scripting**: AngelScript integration for rapid prototyping
- **UI**: Full 2D UI system with layout management
- **Networking**: Built-in server/client networking framework
- **Advanced Effects**: Particle systems, decals, post-processing

### Recent Additions (Urho 4.0 Extensions)

- **Nitro Boost Racing System**: Complete power-up mechanics for racing games with smooth power curves, visual effects, and performance optimization
- **Enhanced Vehicle Physics**: Extended RaycastVehicle with advanced state management
- **Visual Effects Framework**: Particle systems, material effects, and UI feedback systems

## Getting Started

### Building from Source

**Windows with Visual Studio:**
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

**Linux with GCC:**
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

**Using Ninja (faster):**
```bash
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DURHO3D_SAMPLES=1
ninja
```

### Quick Example

See `Source/Samples/` for complete working examples. The VehicleDemo (`Source/Samples/40_VehicleDemo/`) demonstrates vehicle physics and can be extended with the nitro boost system.

## Documentation

Complete API documentation and guides:

- **Engine Concepts**: Core architecture, asset management, component system
- **Graphics API**: Rendering, materials, shaders, and effects
- **Physics & Vehicles**: Rigid body dynamics, collision detection, vehicle control
- **UI System**: Layout management, event handling, styling
- **Scripting**: AngelScript API reference
- **Advanced Topics**: Networking, custom components, performance optimization

For the latest documentation, visit [urho3d-doxygen.github.io](https://urho3d-doxygen.github.io).

## Community & Support

- **GitHub Issues**: [Report bugs and request features](https://github.com/urho3d/Urho3D/issues)
- **Discussions**: [Community Q&A and discussions](https://github.com/urho3d-community/discussions)
- **Wiki**: [Community-maintained documentation](https://github.com/urho3d-community/wiki)

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Make your improvements
4. Submit a pull request

See CONTRIBUTING.md for detailed guidelines.

## License & Acknowledgments

Urho 4.0 is licensed under the MIT License - see LICENSE file for full details.

Urho 4.0 extends the original Urho3D project with additional features for modern game development while maintaining compatibility with the Urho3D philosophy and architecture.
[Yao Wei Tjong](https://github.com/weitjong), [asterj](https://github.com/aster2013)
и многие другие разработчики, список которых можно посмотреть [тут](https://github.com/urho3d/Urho3D/graphs/contributors).
