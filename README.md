**Table of Contents**

<!-- TOC -->

- [IntroductionThe course aims to learn about Motion Planning and Path Planning.](#introductionthe-course-aims-to-learn-about-motion-planning-and-path-planning)
- [Setup Basic Tools](#setup-basic-tools)
- [Assignments](#assignments)
    - [Assignment 0: Setup Environment](#assignment-0-setup-environment)
    - [Assignment 1: Flatland](#assignment-1-flatland)
    - [Assignment 2: Valet](#assignment-2-valet)
    - [Assignment 3: WildFire](#assignment-3-wildfire)
    - [Assignment 4](#assignment-4)
- [Final Project](#final-project)
- [Design Details](#design-details)
- [License](#license)

<!-- /TOC -->

# IntroductionThe course aims to learn about Motion Planning and Path Planning.

Each Assignment shown here has a detailed explanation in its directory. Please refer to the individual directories for an in-depth explanation of how the code works.

🗒*Note: If you understand the codes enough and are smart enough, it will be super simple to port it to the system that you like. I understand coding enough to build all these things but still am not too expert to build cross-platform products yet!*

# Setup Basic Tools

Click the [Link](./Docs/Basic%20Setup.md) to see the complete guide on how to set up the essential tools for the project.


# Assignments

## [Assignment 0: Setup Environment](https://github.com/parth-20-07/Victor-Sierra-Pattern-Design)

The aim of the [Assignment 0](https://github.com/parth-20-07/Victor-Sierra-Pattern-Design) is to set up the environment and try a basic run of programming. Here we try to develop a rudimentary and random obstacle field, try out turtle sim in Python and initiate a *Victor Sierra* Search pattern to find lost objects.

## [Assignment 1: Flatland](https://github.com/parth-20-07/BFS-DFS-and-Djikstra-Search-using-CPP)

The aim of [Assignment 1](https://github.com/parth-20-07/BFS-DFS-and-Djikstra-Search-using-CPP) is to implement Breadth First Search, Depth First Search and Dijkstra Search to find a path between Start Point and Goal Point.

## [Assignment 2: Valet](https://github.com/parth-20-07/Motion-Planning-Using-Hybrid-AStar-for-Non-Holonomic-Constraint-Vehicles)

The aim of [Assignment 2](https://github.com/parth-20-07/Motion-Planning-Using-Hybrid-AStar-for-Non-Holonomic-Constraint-Vehicles) is to implement search and path planning algorithms to park a car with non-holonomic constraints.

## [Assignment 3: WildFire](https://github.com/parth-20-07/Combinational-And-Sampling-Based-Motion-Planning-for-MultiAgents)

The aim of [Assignment 3](https://github.com/parth-20-07/Combinational-And-Sampling-Based-Motion-Planning-for-MultiAgents) is to implement sampling-based (Probabilistic RoadMap) and Combinational based (PRM + AStar) to simulate a scenario where an Arsonist lights the obstacles using PRM + AStar with no constraints to move around the environment and light the obstacles. On other hand, a FireTruck with Ackermann Steering (Non-Holonomic Constraints) has to move around the environment using combinational Motion Planning to diffuse the fire.

## [Assignment 4: Transmission](https://github.com/parth-20-07/Gearbox-Disassembly-using-RRT-Algorithm-in-3D-Space)

The aim of [Assignment 4](https://github.com/parth-20-07/Gearbox-Disassembly-using-RRT-Algorithm-in-3D-Space) is to design a motion planner to disassemble a mainshaft from SM-465 Transmission Box using RRT Planner in 3D Space without any collisions

# [Final Project]((https://github.com/parth-20-07/Multi-Tier-UAV-Search-and-Retrieve))
Project Topic: [**Multi-Tier UAV Search and Retrieve**](https://github.com/parth-20-07/Multi-Tier-UAV-Search-and-Retrieve)

The project focuses on 3 drones with different abilities:
- **Planner Drone**: Slow Speed, High Battery Capacity, Moderate Payload Capacity
- **Search Drone**: High Speed, Low Battery Capacity, Low Payload Capacity
- **Retrieval Drone**: Pay Speed, Low Battery Capacity, High Payload Capacity.

The combination of three drones is used to perform the Search and Retrieve operation.
1. The Planner Drone achieves high altitude to design a general search algorithm to find lost objects.
2. The Search Drone has high speed and a local obstacle avoidance and path planning algorithm which avoids obstacles that are not possible to map from heights. This drone is used to map the environment while looking for the lost object
3. The Retrieval Drone gets the final coordinates of a lost object from Search Drone along with mapping the small obstacle detected by the Search Drone. This drone finally goes to the location of the lost object and retrieves it.

# Design Details

- Designed for:
  - Worcester Polytechnic Institute
  - RBE 550-S23-S01: Motion Planning
- Designed by:
  - [Parth Patel](mailto:parth.pmech@gmail.com)

# License

This project is licensed under [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html) (see [LICENSE.md](LICENSE.md)).

Copyright 2023 Parth Patel

Licensed under the GNU General Public License, Version 3.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at

_https://www.gnu.org/licenses/gpl-3.0.en.html_

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.