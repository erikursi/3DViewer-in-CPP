# 3DViewer-in-CPP

## Overview
Implementation of an application for viewing 3D wireframe models in C++ and performing various transformations and rendering operations on them.

<div align="center">
<img src="./misc/00.png" alt="Neon cow" width="80%">
</div>


  <h3 align=center>3D model with custom colors</h3>

<div align="center">
<img src="./misc/03.png" alt="Paper bear" width="80%">
</div>

  <h3 align=center>3D model with texture</h3>
<div align="center">
<img src="./misc/02.png" alt="Paper bear" width="80%">
</div>

  <h3 align=center>3D model with material</h3>
<div align="center">
<img src="./misc/01.png" alt="Yellow car" width="80%">
</div>


## Installation 
Run `make all` or `make install` to install the program.

## Usage
Run `make run` to launch the GUI-based application.

### Use files and directories

<img src="./misc/10.png" alt="Yellow car" width="80%"> 

<img src="./misc/04.png" alt="Yellow car" width="80%">

Via GUI you can: 
- load 3D models with OBJ files 
- load materials with MTL files
- load textures with image files
- save rendered model as image
- save rendered model as image
- configure default load and save directories


### Customize drawing settings

<img src="./misc/07.png" alt="Yellow car" width="80%"> 

Via GUI you can customize:
- background color
- type, size and color of vertices
- type, size and color of edges
- visibility and color of faces
- type of projection
- visibility, size and color of normal vectors
- mode, color and intensity of light
- texture and material usage

### Perform model transformations
<img src="./misc/11.png" alt="Yellow car" width="80%"> 

Via GUI you can: 
- translate model
- rotate model
- scale model

### Save settings
The settings are saved between program restarts.

### Additional information
The application can generate texture coordinates and vertex normals if that information is not present in OBJ file.

By default the program generates flat normals. In order to generate smooth normals you should modify file *3DViewer/sources/shared/consts.h* and set *default_smooth_normals* to *true*.

<div align="center">
  <h4 align=center>Flat</h4>
  <img src="./misc/15.png" alt="Scientific" width="70%">
    <img src="./misc/19.png" alt="Scientific" width="70%">


  <h4 align=center>Smooth</h4>
  <img src="./misc/16.png" alt="Scientific" width="70%">
  <img src="./misc/18.png" alt="Scientific" width="70%">
</div>
  
## Dependencies
This program depends on the following libraries:
- Qt 6 (for GUI)
- GTest (for tests)
