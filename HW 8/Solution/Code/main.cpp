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