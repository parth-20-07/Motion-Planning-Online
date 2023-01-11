<!-- TOC -->

- [Introduction](#introduction)
- [How to run the code](#how-to-run-the-code)
- [Code Explaination](#code-explaination)

<!-- /TOC -->

# Introduction
The aim of the assignment is to learn Probabilistic Robotics. 

The Problem statement asks for us to predict whethter the door is open or closed by updating the prediction belief from the sensor measurement.

This is done using **Bayes Filter**. It runs of the following set of formulas:

**_Algorithm Bayes Filter_ $(bel(x_{t-1},u_{t},z_{t})):$**

    for all $x_{t}$ do

        $\overline{bel}(x_{t}) = \int{p(x_{t}|u_{t},x_{t-1})bel(x_{t-1})dx}$

        $bel(x_{t}) = \eta p(z_{t}|x_{t})bel(x_{t})$

    endfor

    return $bel(x_{t})$
        

In our case, These are the possible values for the system:
- State: State of the Door
    - Open
    - Closed
- Action: Robot using its arms
    - Trying to open the door
    - Do Nothing

Robot has no idea what is the initial state of the robot. Therefore:

$bel(X_{0} = open) = 0.5$

$bel(X_{0} = closed) = 0.5$

where, $bel$ stands for belief or probability

The Robot measurement are noisy in such a way that:

$p(Z_{t}=sense-open|X_{t}=is-open)=0.6$

$p(Z_{t}=sense-closed|X_{t}=is-open)=0.4$


$p(Z_{t}=sense-open|X_{t}=is-closed)=0.6$

$p(Z_{t}=sense-closed|X_{t}=is-closed)=0.6$

State Transition Probability: $p(x_{t}|x_{t-1},u_{t})$
Measurement Probability: $p(z_{t}|x_{t})$

**Action 1**: Robot tries to open the door.
But the Action is not perfect either

$p(X_{t}=is-open|U_{t}=pull,X_{t-1}=is-open)=1$

$p(X_{t}=is-closed|U_{t}=pull,X_{t-1}=is-open)=0$

$p(X_{t}=is-open|U_{t}=pull,X_{t-1}=is-closed)=0.8$

$p(X_{t}=is-closed|U_{t}=pull,X_{t-1}=is-closed)=0.2$

**Action 2**: Robot does nothing.

$p(X_{t}=is-open|U_{t}=do_nothing,X_{t-1}=is-open)=1$

$p(X_{t}=is-closed|U_{t}=do_nothing,X_{t-1}=is-open)=0$

$p(X_{t}=is-open|U_{t}=do_nothing,X_{t-1}=is-closed)=0$

$p(X_{t}=is-closed|U_{t}=do_nothing,X_{t-1}=is-closed)=1$

**Prediction**:
This step take a set of belief values of the states of the robot and uses it to predict what the joint states are would be. In our example robot tries to **DO NOTHING**

$\overline{bel}(x_{t}) = \int{p(x_{t}|u_{t},x_{t-1})bel(x_{t-1})dx}$

$\overline{bel}(x_{t})=\sum p(x_{1}|u_{1},x_{0})bel(x_{0})$

$\overline{bel}(x_{t}) = p(x_{1}|U_{1}=do-nothing,X_{0}=is-open)bel(X_{0}=is-open)+p(x_{1}|U_{1}=do-nothing,X_{0}=is-closed)bel(X_{0}=is-closed)$

Calculate for $\overline{bel}(x_{t}=is-open)$ which can be used to calculate 

$\overline{bel}(x_{t}=is-closed) = 1 - \overline{bel}(x_{t}=is-open)$

**MEASUREMENT**
This set takes the prediction calculated in last step and used it to feed through sensor's accuracy probability function to predict how close is the sensor value to actual value.

$bel(x_{t}) = \eta p(z_{t}|x_{t})bel(x_{t})$

$bel(x_{1}=is-open) = \eta p(z_{1}=sense-open|x_{1}=is-open)bel(x_{1}=is-open)$

$bel(x_{1}=is-closed) = \eta p(z_{1}=sense-open|x_{1}=is-closed)bel(x_{1}=is-closed)$

When $bel(x_{1}=is-closed)$ and $bel(x_{1}=is-open)$ calculates, they are in form of $\eta * (number)$. Also,

$bel(x_{1}=is-closed) + bel(x_{1}=is-closed) = 1$ 

Thus, 
    $\eta = \frac{1}{bel(x_{1}=is-closed) + bel(x_{1}=is-closed)}$

Placing the value of $\eta$ back into the variables, provide us with new beliefs which can be iterated through with time to improve the accuracy of our prediction.

# How to run the code
cd to the Directory in terminal
```
cd HW\ 8/Solution/Code/
```
Run the program by typing
```
./main.exe
```
You should recieve output similar to as shown below
```
$ ./main.exe
-----------------------------------------------------------
Action: Do Nothing
Sensor Reading: Door Close
New Door Open Belief: 0.333333
New Door Close Belief: 0.666667
New Normalizer: 1.66667
-----------------------------------------------------------
Action: Open Door
Sensor Reading: Door Close
New Door Open Belief: 0.764706
New Door Close Belief: 0.235294
New Normalizer: 2.20588
-----------------------------------------------------------
Action: Do Nothing
Sensor Reading: Door Close
New Door Open Belief: 0.619048
New Door Close Belief: 0.380952
New Normalizer: 2.02381
-----------------------------------------------------------
Action: Open Door
Sensor Reading: Door Open
New Door Open Belief: 0.973244
New Door Close Belief: 0.0267559
New Normalizer: 1.75585
-----------------------------------------------------------
Action: Do Nothing
Sensor Reading: Door Open
New Door Open Belief: 0.990919
New Door Close Belief: 0.00908059
New Normalizer: 1.69694
```

# Code Explaination
Complete Code: 

```
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <math.h>

/* ------------------------- Measurement Probability ------------------------ */
#define p_m_open_x_open 0.6
#define p_m_close_x_open 0.4
#define p_m_open_x_close 0.2
#define p_m_close_x_close 0.8

/* --------------------------- Action Probability --------------------------- */
// Action Pull
#define p_x_open_u_pull_x0_open 1
#define p_x_close_u_pull_x0_open 0
#define p_x_open_u_pull_x0_close 0.8
#define p_x_close_u_pull_x0_close 0.2
// Action Do Nothing
#define p_x_open_u_nothing_x0_open 1
#define p_x_close_u_nothing_x0_open 0
#define p_x_open_u_nothing_x0_close 0
#define p_x_close_u_nothing_x0_close 1

/* ----------------------------- Defining Model ----------------------------- */
enum actions // Possible Door Actions
{
    open_door,
    do_nothing_to_door
};

enum door_states // Possible Door States
{
    door_is_open,
    door_is_closed
};

/* --------------------------- Initial Assumptions -------------------------- */
std::double_t belief_door_is_open = 0.5;
std::double_t belief_door_is_closed = 0.5;
std::double_t normalizer;

/**
 * @brief Bayers Filter creates a probablistic model of the environment by predicting a value of environment state
 * and updating it against sensor measurement values
 *
 * @param iteration Struct of action and sensor measurement values
 */
int bayers_filter(std::double_t action, std::double_t door_state)
{
    std::cout << "-----------------------------------------------------------" << std::endl;
    // std::cout << "Old Door Open Belief: " << belief_door_is_open << std::endl;
    // std::cout << "Old Door Close Belief: " << belief_door_is_closed << std::endl;
    std::double_t p_xt_i_ut_xt_1_i, p_xt_i_ut_xt_1_not_i;
    // Determining Action Type
    if (action == do_nothing_to_door)
    {
        std::cout << "Action: Do Nothing" << std::endl;
        p_xt_i_ut_xt_1_i = p_x_open_u_nothing_x0_open;
        p_xt_i_ut_xt_1_not_i = p_x_open_u_nothing_x0_close;
    }
    else if (action == open_door)
    {
        std::cout << "Action: Open Door" << std::endl;
        p_xt_i_ut_xt_1_i = p_x_open_u_pull_x0_open;
        p_xt_i_ut_xt_1_not_i = p_x_open_u_pull_x0_close;
    }
    else
    {
        std::cout << "Action Not Recognised" << std::endl;
        return EXIT_FAILURE;
    }
    // std::cout << "p_xt_i_ut_xt_1_i: " << p_xt_i_ut_xt_1_i << std::endl;
    // std::cout << "p_xt_i_ut_xt_1_not_i: " << p_xt_i_ut_xt_1_not_i << std::endl;

    // Calculating Prediction
    std::double_t door_open_prediction = (p_xt_i_ut_xt_1_i * belief_door_is_open) + (p_xt_i_ut_xt_1_not_i * belief_door_is_closed);
    std::double_t door_closed_prediction = 1 - door_open_prediction;
    // std::cout << "Prediction Door Open: " << door_open_prediction << std::endl;
    // std::cout << "Prediction Door Close: " << 1 - door_open_prediction << std::endl;

    // Determining Sensor Measurement
    std::double_t p_m_sense_x_belief, p_m_sense_x_not_belief;
    if (door_state == door_is_open)
    {
        std::cout << "Sensor Reading: Door Open" << std::endl;
        p_m_sense_x_belief = p_m_open_x_open;
        p_m_sense_x_not_belief = p_m_open_x_close;
    }
    else if (door_state == door_is_closed)
    {
        std::cout << "Sensor Reading: Door Close" << std::endl;
        p_m_sense_x_belief = p_m_close_x_open;
        p_m_sense_x_not_belief = p_m_close_x_close;
    }
    else
    {
        std::cout << "Measurement Not Recognised" << std::endl;
        return EXIT_FAILURE;
    }

    // std::cout << "p_m_sense_x_belief: " << p_m_sense_x_belief << std::endl;
    // std::cout << "p_m_sense_x_not_belief: " << p_m_sense_x_not_belief << std::endl;
    // Calculating Sensor Belief
    std::double_t bel_x_1 = (p_m_sense_x_belief * door_open_prediction);
    std::double_t bel_x_not_1 = (p_m_sense_x_not_belief * door_closed_prediction);
    // std::cout << "bel_x_1: " << bel_x_1 << std::endl;
    // std::cout << "bel_x_not_1: " << bel_x_not_1 << std::endl;
    normalizer = 1 / (bel_x_1 + bel_x_not_1);
    // Updating Beliefs
    belief_door_is_open = normalizer * bel_x_1;
    belief_door_is_closed = normalizer * bel_x_not_1;
    std::cout << "New Door Open Belief: " << belief_door_is_open << std::endl;
    std::cout << "New Door Close Belief: " << belief_door_is_closed << std::endl;
    std::cout << "New Normalizer: " << normalizer << std::endl;
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    // for (size_t i = 0; i < 100; i++) // Training System
    // bayers_filter(rand() % 2, rand() % 2);
    // system("clear");
    std::double_t iteration[5][2] =
        {
            {do_nothing_to_door, door_is_closed},
            {open_door, door_is_closed},
            {do_nothing_to_door, door_is_closed},
            {open_door, door_is_open},
            {do_nothing_to_door, door_is_open},
        };

    for (size_t i = 0; i < 5; i++) // Testing System
        bayers_filter(iteration[i][0], iteration[i][1]);

    return EXIT_SUCCESS;
}
```
Defining the accuracy of sensors measurement
```
/* ------------------------- Measurement Probability ------------------------ */
#define p_m_open_x_open 0.6
#define p_m_close_x_open 0.4
#define p_m_open_x_close 0.2
#define p_m_close_x_close 0.8
```

Defining the Action Accuracy
```
/* --------------------------- Action Probability --------------------------- */
// Action Pull
#define p_x_open_u_pull_x0_open 1
#define p_x_close_u_pull_x0_open 0
#define p_x_open_u_pull_x0_close 0.8
#define p_x_close_u_pull_x0_close 0.2
// Action Do Nothing
#define p_x_open_u_nothing_x0_open 1
#define p_x_close_u_nothing_x0_open 0
#define p_x_open_u_nothing_x0_close 0
#define p_x_close_u_nothing_x0_close 1
```

Defining possible Actions and Door States
```
/* ----------------------------- Defining Model ----------------------------- */
enum actions // Possible Door Actions
{
    open_door,
    do_nothing_to_door
};

enum door_states // Possible Door States
{
    door_is_open,
    door_is_closed
};
```

Initial Assumptions for system
```
/* --------------------------- Initial Assumptions -------------------------- */
std::double_t belief_door_is_open = 0.5;
std::double_t belief_door_is_closed = 0.5;
std::double_t normalizer;
```

This function takes the action performed by robot and the state value measured by the sensor as input parameters
```
int bayers_filter(std::double_t action, std::double_t door_state)
```

The variable names here are bad but that was all I was able to think at the moment.

`p_xt_i_ut_xt_1_i` : probability of state x(t) being same as x(t-1) with effort is applied

`p_xt_i_ut_xt_1_not_i` : probability of state x(t) not being same as x(t-1) with effort is applied.This takes the action input and allots the values of success based on the action to the variables.
```
std::double_t p_xt_i_ut_xt_1_i, p_xt_i_ut_xt_1_not_i;
// Determining Action Type
if (action == do_nothing_to_door)
{
    std::cout << "Action: Do Nothing" << std::endl;
    p_xt_i_ut_xt_1_i = p_x_open_u_nothing_x0_open;
    p_xt_i_ut_xt_1_not_i = p_x_open_u_nothing_x0_close;
}
else if (action == open_door)
{
    std::cout << "Action: Open Door" << std::endl;
    p_xt_i_ut_xt_1_i = p_x_open_u_pull_x0_open;
    p_xt_i_ut_xt_1_not_i = p_x_open_u_pull_x0_close;
}
else
{
    std::cout << "Action Not Recognised" << std::endl;
    return EXIT_FAILURE;
}
```

Calculating Prediction as defined mathematically above
```
// Calculating Prediction
std::double_t door_open_prediction = (p_xt_i_ut_xt_1_i * belief_door_is_open) + (p_xt_i_ut_xt_1_not_i * belief_door_is_closed);
std::double_t door_closed_prediction = 1 - door_open_prediction;
```

The variable names here are bad but that was all I was able to think at the moment.

`p_m_sense_x_belief` : probability of sensor measuring the same as states

`p_m_sense_x_not_belief` : probability of sensor measure different from states
Alloting the probability values as per the measured state
```
// Determining Sensor Measurement
std::double_t p_m_sense_x_belief, p_m_sense_x_not_belief;
if (door_state == door_is_open)
{
    std::cout << "Sensor Reading: Door Open" << std::endl;
    p_m_sense_x_belief = p_m_open_x_open;
    p_m_sense_x_not_belief = p_m_open_x_close;
}
else if (door_state == door_is_closed)
{
    std::cout << "Sensor Reading: Door Close" << std::endl;
    p_m_sense_x_belief = p_m_close_x_open;
    p_m_sense_x_not_belief = p_m_close_x_close;
}
else
{
    std::cout << "Measurement Not Recognised" << std::endl;
    return EXIT_FAILURE;
}
```

Calculating belief values of states
```
// Calculating Sensor Belief
std::double_t bel_x_1 = (p_m_sense_x_belief * door_open_prediction);
std::double_t bel_x_not_1 = (p_m_sense_x_not_belief * door_closed_prediction);
```
Calculating normalizer value
```
normalizer = 1 / (bel_x_1 + bel_x_not_1);
```
Updating the Global Belief Values
```
// Updating Beliefs
belief_door_is_open = normalizer * bel_x_1;
belief_door_is_closed = normalizer * bel_x_not_1;
```

This is a training set where the model is forced through 100 random action states and measurement values
```
// for (size_t i = 0; i < 100; i++) // Training System
// bayers_filter(rand() % 2, rand() % 2);
// system("clear");
```

The five action measurement pairs asked to iterate through in the assignment
```
std::double_t iteration[5][2] =
    {
        {do_nothing_to_door, door_is_closed},
        {open_door, door_is_closed},
        {do_nothing_to_door, door_is_closed},
        {open_door, door_is_open},
        {do_nothing_to_door, door_is_open},
    };

for (size_t i = 0; i < 5; i++) // Testing System
    bayers_filter(iteration[i][0], iteration[i][1]);
```