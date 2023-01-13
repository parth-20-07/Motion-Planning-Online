import os
import turtle
from PIL import Image
import random
"""The Aim of the Code is to Generate Victor Sierra Pattern and to find a lost turtle if required
Designed by: Parth Patel
"""


def perform_search(rescuer_turtle, lost_turtle, search_radius, active_search):
    """
    Move the turtle in Vierra Search Pattern.
    If the rescuer is in:
    -> active rescue state: it move a unit in forward direction and looks for the lost turtle.
        -> If it fails to find it, it continues.
        -> If it finds the lost turtle, it returns the current location of rescuer turtle and the error of position
    -> Not Rescue State: Creates Infinite Vierra Search Pattern

    Args:
        rescuer_turtle (turtle.Turtle()): The Turtle Object Looking for the Lost Turtle
        lost_turtle (turtle.Turtle()): The Turtle Object that is lost
        search_radius (int): Search Area Radius in px
        active_search (bool): True -> Active Search, False -> Pattern Building

    Returns:
        _type_: _description_
    """
    for i in range(9):
        if not active_search:
            # If it is not an active search, we move the rescuer at full speed to make the pattern as quick as possible.
            rescuer_turtle.forward(search_radius)
        else:
            for rescuer_pos in range(int(search_radius/4)):
                # In active Search, we move 4 steps each time before looking for the lost turtle, this increases the search speed by reducing polling time.
                rescuer_turtle.forward(4)

                if (round(rescuer_turtle.distance(lost_turtle.pos())) < 5):
                    # Data Output in case the turtle is found.
                    rescuer_turtle.color('red')
                    rescuer_turtle.write(
                        str(rescuer_turtle.pos()), align="right", font=("Arial", 12, "normal"))
                    print(f"Rescuer Turtle Location: {rescuer_turtle.pos()}")
                    print(
                        f"Error: {abs(lost_turtle.pos()-rescuer_turtle.pos())}")
                    return True

        if (round(abs(rescuer_turtle.pos()), 2) != 0):
            rescuer_turtle.right(120)

    return False


def victor_sierra_search(rescuer_turtle, lost_turtle, search_radius, active_search):
    """ Initiate a Victor Sierra Search Pattern.

    Args:
        rescuer_turtle (turtle.Turtle()): The Turtle Object Looking for the Lost Turtle
        lost_turtle (turtle.Turtle()): The Turtle Object that is lost
        search_radius (int): Search Area Radius in px
        active_search (bool): True -> Active Search, False -> Pattern Building
    """
    rescuer_turtle.color('cyan')
    lost_turtle_found = False
    while not lost_turtle_found:
        lost_turtle_found = perform_search(
            rescuer_turtle=rescuer_turtle, lost_turtle=lost_turtle, search_radius=search_radius, active_search=active_search)
        if not active_search:
            # If this is not an active search, then the rescuer will rotate 30 degrees to initiate the next pattern of search
            rescuer_turtle.right(30)
        else:
            # If this is an active search, then the rescuer will rotate 29 degrees to initiate the next pattern of search
            # This number is used to avoid it overlaping any previous search pattern as 360 is not divisible by 29
            rescuer_turtle.right(29)
    # We just want to export the canvas which contains the information about the rescued turtle.
    canvas = turtle.Turtle().getscreen()
    canvas.getcanvas().postscript(file="Turtle Graphics Output.eps")
    img = Image.open('Turtle Graphics Output.eps')
    img.save('Turtle Graphics Output.jpg')
    turtle.exitonclick()


def place_lost_turtle(lost_turtle, lost_radius, active_search):
    """Randomly select the position to set the location of lost turtle. Turtle is hidden if it is not active search

    Args:
        lost_turtle (turtle.Turtle()): The Turtle Object that is lost
        search_radius (int): Search Area Radius in px
        active_search (bool): True -> Active Search, False -> Pattern Building
    """
    if active_search:
        lost_turtle.color('blue')
        lost_turtle.pencolor('white')
        x, y = [random.randrange(0, lost_radius, 1),
                random.randrange(0, lost_radius, 1)]
        lost_turtle.goto(x, y)
        print(f"Lost Turtle Location: {lost_turtle.pos()}")
    else:
        lost_turtle.hideturtle()


def main():
    search_spread = 50  # Radius of search
    active_search = False  # Whether it is an active search or simple pattern build

    os.system('clear')

    turtle.Screen().setup(width=6*search_spread, height=6*search_spread)
    # Initializing two turtles
    rescuer = turtle.Turtle()
    lost_turtle = turtle.Turtle()

    place_lost_turtle(lost_turtle=lost_turtle,
                      lost_radius=search_spread, active_search=active_search)  # Placing Lost Turtle
    victor_sierra_search(rescuer_turtle=rescuer, lost_turtle=lost_turtle,
                         search_radius=2*search_spread, active_search=active_search)  # Starting Search


if __name__ == '__main__':
    main()
