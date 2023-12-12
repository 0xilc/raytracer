# Ray Tracer
![image](https://github.com/0xilc/raytracer/assets/28317283/0b68a298-352f-4566-b061-7adf2f1a73d3)

## Introduction

This repository contains the source code for a basic ray tracer implemented as part of the CENG 477 - Introduction to Computer Graphics course at Middle East Technical University.

## Table of Contents

- [Objectives](#objectives)
- [Specifications](#specifications)
- [Getting Started](#getting-started)
- [Sample Scenes](#sample-scenes)

  
## Objectives

Ray tracing is a fundamental rendering algorithm commonly used for animations and architectural simulations. The objective of this project is to implement a basic ray tracer that simulates the propagation of light in the real world.

## Specifications

- The executable is named "raytracer".
- Accepts an XML scene file as an argument.
- Saves resulting images in PPM format.
- Implements Blinn-Phong shading model for specular shading computations.
- Supports point and ambient light sources.
- Uses a parser for scene file and a PPM writer provided in the project template.

## Getting Started

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/0xilc/raytracer.git
   cd raytracer

2. **Build the Project**

   ```bash
   make
3. **Run the Raytracer:**

   ```bash
   ./raytracer scene.xml
  
## Sample Scenes

Sample scenes can be found on scenes folder in the main directory.

### Usage

To use a sample scene, you can specify the path to the XML scene file when running the ray tracer. For example:

```bash
./raytracer scenes/simple.xml
