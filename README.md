**Table of Contents**

<!-- TOC -->

- [Introduction](#introduction)
- [Setup Basic Tools](#setup-basic-tools)
- [Assignments](#assignments)
    - [Assignment 0: Setup Environment](#assignment-0-setup-environment)
    - [Assignment 1: Flatland](#assignment-1-flatland)
    - [Assignment 2: Valet](#assignment-2-valet)
- [Final Project](#final-project)
- [Design Details](#design-details)
- [License](#license)
- [Update Logs](#update-logs)

<!-- /TOC -->

# Introduction

The aim of the course is to learn about Motion Planning and Path Planning.

Each Assignment shown here has a detailed explaination in its own directory. Please refer the individual directories for in-depth explaination of how the code works.

⚠*Warning: The complete system and assignments are built on Ubuntu 20.04 and not tested on other systems.*


🗒*Note: But if you understand the codes enough and are smart enough, it will be super simple to port it on the system that you like. I understand coding enough to build all these things but still am not too expert to build cross-platform products yet!*

# Setup Basic Tools

Click the [Link](./Docs/Basic%20Setup.md) to see the complete guide on how to setup the essential tools for the project.


# Assignments

## Assignment 0: Setup Environment

The aim of the [Assignment 0](./HW0%20-%20Turtles/) is to setup the environment and try a basic run through programming. Here we try to develop a rudimentary and random obstacle field, tryout turtle sim in python and initiate a *Victor Sierra* Search pattern to find lost objects.

## Assignment 1: Flatland

The aim of the [Assignment 1](./HW1%20-%20Flatland/) is to implement Breadth First Search, Depth First Search and Dijkstra Search to find a path between Start Point and Goal Point.

## Assignment 2: Valet

The aim of the [Assignment 2](./HW2%20-%20Valet/) is to implement search and path planning algorithms to park a car with non-holonomic constraints.

# Final Project
Project Topic: [**Multi-Tier UAV Search and Retrieve**](https://github.com/parth-20-07/Multi-Tier-UAV-Search-and-Retrieve)

The project focuses on 3 drones with different abilities:
- **Planner Drone**: Slow Speed, High Battery Capacity, Moderate Payload Capacity
- **Search Drone**: High Speed, Low Battery Capacity, Low Payload Capacity
- **Retrieval Drone**: Pay Speed, Low Battery Capacity, High Payload Capacity

The Combination of three drones are used to perform Search and Retrieve operation.
1. The Planner Drone achieves high altitude to design a general search algorithm to find lost object.
2. The Search Drone has high speed and a local obstacle avoidance and path planning algorithm which avoids obstacles that are not possible to map from heights. This drone is used to map the environment while looking for the lost object
3. The Retrieval Drone get the final coordinates of lost object from Search Drone along with mapping of small obstacle detected by Search Drone. This drone finally goes to the location of lost object and retrieves it.

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

# Update Logs

All the major updates in the repository can be found in [Update Logs.md](./Docs/Update%20Logs.md).