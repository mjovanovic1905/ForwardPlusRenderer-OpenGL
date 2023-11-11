# Forward Plus Renderer with CSM and FXAA

This repository contains a Forward Plus Renderer implemented in C++ using OpenGL, featuring Cascaded Shadow Maps (CSM) and Fast Approximate Anti-Aliasing (FXAA).

## Overview

The Forward Plus Renderer optimizes dynamic lighting using a tiled rendering technique, making it suitable for scenes with numerous dynamic lights.

### Features

- **Forward Plus Renderer:** Efficient handling of dynamic lights through tiled rendering.
- **Cascaded Shadow Maps (CSM):** Improves shadow quality by utilizing multiple shadow maps for various scene distances.
- **Fast Approximate Anti-Aliasing (FXAA):** Enhances image quality through post-processing anti-aliasing.

## Requirements

- **Visual Studio 2022:** The project is built using Visual Studio 2022.
- **OpenGL:** Required for rendering and graphical operations.

### Dependencies

The project relies on the following libraries:

- **GLFW:** A multi-platform library for window and input handling.
- **GLEW:** OpenGL Extension Wrangler Library for accessing modern OpenGL functionality.
- **Assimp:** Open Asset Import Library for model loading and scene processing.
- **GLM:** OpenGL Mathematics for matrix and vector operations.
