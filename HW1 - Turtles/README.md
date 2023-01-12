<!-- TOC -->

- [Introduction](#introduction)
- [Turtle Graphics](#turtle-graphics)
    - [Requirements](#requirements)
    - [Code](#code)
        - [How to Run the code](#how-to-run-the-code)
        - [Code Basics](#code-basics)
        - [Results](#results)

<!-- /TOC -->

# Introduction
The aim of the assignment is to setup the environment (guide to [setup](../Docs/)) and try out hands on the environment to practise and test the setup.

# Turtle Graphics

## Requirements
To encourage the setup of a working Python environment, create a pattern using turtle graphics. Python has a built-in turtle graphics module. To make things more interesting, our dimwittedturtle has lost his friend, and needs to execute a *Victor Sierra* search pattern, shown below

![Victor Sierra Search Pattern](/Resources/Turtle%20Graphics%20Requirement.png)

## Code

### How to Run the code

Open a new terminal and cd into `HW1 - Turtles` directory. Run the Python file using:
```shell
python3 turtle_graphics.py
```

### Code Basics

```python
def main():
    search_spread = 50  # Radius of search
    active_search = False  # Whether it is an active search or simple pattern build
```

- `search_spread` variable is used to set the search area radius for the Vierra Pattern to build. This distance is in *pixels*.
- `active_search` is a boolean which determines whether to actively look for the lost turtle or just create a Search Pattern for visual display.

```python
def perform_search(rescuer_turtle, lost_turtle, search_radius, active_search):
```

Move the turtle in Vierra Search Pattern. If the rescuer is in:
- active rescue state: it move 4 unit in forward direction and looks for the lost turtle.
    - If it fails to find it, it continues.
    - If it finds the lost turtle, it returns the current location of rescuer turtle and the error of position
- Not Rescue State: Creates Infinite Vierra Search Pattern

Args:
- `rescuer_turtle (turtle.Turtle())`: The Turtle Object Looking for the Lost Turtle
- `lost_turtle (turtle.Turtle())`: The Turtle Object that is lost
- `search_radius (int)`: Search Area Radius in px
- `active_search (bool)`: True -> Active Search, False -> Pattern Building

```python
def victor_sierra_search(rescuer_turtle, lost_turtle, search_radius, active_search):
```

Initiate a Victor Sierra Search Pattern.

Args:
- `rescuer_turtle (turtle.Turtle())`: The Turtle Object Looking for the Lost Turtle
- `lost_turtle (turtle.Turtle())`: The Turtle Object that is lost
- `search_radius (int)`: Search Area Radius in px
- `active_search (bool)`: True -> Active Search, False -> Pattern Building

```python
def place_lost_turtle(lost_turtle, lost_radius, active_search):
```

Randomly select the position to set the location of lost turtle. Turtle is hidden if it is not active search

Args:
- `lost_turtle (turtle.Turtle())`: The Turtle Object that is lost
- `search_radius (int)`: Search Area Radius in px
- `active_search (bool)`: True -> Active Search, False -> Pattern Building

### Results
**Active Search**

Active Search Output Canvas:

![Active Search](/Resources/Turtle%20Graphics%20Active%20Search%20Output.jpg)

Click the Video below to see the simulation:

<a href="https://youtube.com/shorts/naEB1eCU-QM">
    <img src="/HW1 - Turtles/Resources/Turtle Graphics Active Search Output.jpg"/>
</a>

**Inactive Search**

Inactive Search Output Canvas:

![Inactive Seach](/Resources/Turtle%20Graphics%20InActive%20Search%20Output.png)

Click the Video below to see the simulation:

<a href="https://youtube.com/shorts/1GMheKd6lk0?feature=share">
    <img src="/Resources/Turtle Graphics InActive Search Output.png"/>
</a>
