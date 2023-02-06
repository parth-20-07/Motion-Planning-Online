/**
 * @file main.cpp
 * @author Parth Patel (parth.pmech@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-01-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <queue>
#include <chrono>

/* -------------------------------------------------------------------------- */
/*                               BASIC VARIABLES                              */
/* -------------------------------------------------------------------------- */

/* -------------------------- VISUALIZATION MACROS -------------------------- */
// #define GENERATE_GIF // Uncomment this if you want to generate GIF of the path mapping and planning
// #define PERFORMANCE_TESTING // Perform Performance Testing

/* ------------------------------ VISUAL MACROS ----------------------------- */
/**
 * @brief Window Configuration.
 */
#define GRID_WIDTH 128
#define GRID_HEIGHT 128
#define PIXEL_WIDTH 10

/**
 * @brief Colors for Grid
 */
#define BG_COLOR sf::Color::White
#define OBSTACLE_COLOR sf::Color::Black
#define MAPPING_COLOR sf::Color::Yellow
#define PLOTTING_COLOR sf::Color::Cyan
#define START_POINT_COLOR sf::Color::Green
#define END_POINT_COLOR sf::Color::Red

/* --------------------------- SEARCH TYPE MACROS --------------------------- */
/**
 * @brief Types of Searches
 */
#define RANDOM_SEARCH "Random Search"
#define BFS_SEARCH "BFS Search"
#define DFS_SEARCH "DFS Search"
#define DIJKSTRA_SEARCH "Dijkstra Search"

/* --------------------- SECONDARY VARIABLES AND MACROS --------------------- */
/**
 * @brief Value to mark each block
 */
#define BLOCK_EMPTY 0
#define BLOCK_OBSTACLE 1
#define BLOCK_VISITED 2

/**
 * @brief Global Variables for usage
 */
std::uint8_t grid_array[GRID_WIDTH][GRID_HEIGHT];                   // Grid to store info
std::array<std::uint8_t, 2> entry_point = {1, 1};                   // y,x
std::array<std::uint8_t, 2> exit_point = {GRID_HEIGHT, GRID_WIDTH}; // y,x

/* -------------------------------------------------------------------------- */
/*                              CLASS DEFINITIONS                             */
/* -------------------------------------------------------------------------- */

/* ---------------------------- GRID SETUP CLASS ---------------------------- */
/**
 * @brief Initialize the Grid to setup the Grid to create the map for path planning
 */
class Setup_Grid
{
private:
    std::uint8_t grid_width;
    std::uint8_t grid_height;
    std::uint8_t coverage_percentage;
    std::array<uint8_t, 2> get_block_placement_position(void);
    std::uint8_t get_block_type(void);
    void update_grid_array(uint8_t block_type, std::array<uint8_t, 2> position);
    std::uint64_t calculate_coverage();
    sf::RenderWindow *visualize_grid(bool show_grid_lines, bool show_setup_animation, std::string window_name);
    void draw_grid_lines(sf::RenderWindow *window);

public:
    Setup_Grid(std::uint8_t grid_width, std::uint8_t grid_height, std::uint8_t coverage_percentage);
    void initialize_grid(void);
    sf::RenderWindow *clear_grid(bool show_grid_lines, bool show_setup_animation, std::string window_name);
};

/* ------------------------- SEARCH ALGORITHM CLASS ------------------------- */
/**
 * @brief Choose Path Planning algorithm for Grid Path Planning
 *
 */
class StartSearch
{
private:
    std::string search_type;                              // Tag to determine search type
    sf::RenderWindow *window;                             // Pointer to the grid window
    std::vector<std::vector<std::uint8_t>> position_list; // List of all the cells to travel
    std::array<std::uint8_t, 2> start_pos, end_pos;       // Vector to store starting and end position
    std::uint16_t cell_count;                             // Step count

    bool is_right_empty(std::uint8_t x, std::uint8_t y, bool mark_location);
    bool is_down_empty(std::uint8_t x, std::uint8_t y, bool mark_location);
    bool is_left_empty(std::uint8_t x, std::uint8_t y, bool mark_location);
    bool is_up_empty(std::uint8_t x, std::uint8_t y, bool mark_location);
    bool is_up_left_empty(std::uint8_t x, std::uint8_t y, bool mark_location);
    bool is_up_right_empty(std::uint8_t x, std::uint8_t y, bool mark_location);
    bool is_down_left_empty(std::uint8_t x, std::uint8_t y, bool mark_location);
    bool is_down_right_empty(std::uint8_t x, std::uint8_t y, bool mark_location);
    std::uint16_t itemize_path(std::string path);
    void display_path(void);
    std::uint16_t random_search(sf::RenderWindow *display_window, bool show_search_animation);
    std::uint16_t bfs_search(sf::RenderWindow *display_window, bool show_search_animation);
    std::uint16_t dfs_search(sf::RenderWindow *display_window, bool show_search_animation);
    std::uint16_t dijkstra_search(sf::RenderWindow *display_windows, bool show_search_animation);

public:
    StartSearch(std::uint8_t start_position_y, std::uint8_t start_position_x, std::uint8_t end_position_y, std::uint8_t end_position_x);
    std::uint16_t initiate_search(std::string search_type, sf::RenderWindow *window, bool show_search_animation);
};

/* -------------------------------------------------------------------------- */
/*                               MAIN PROCEDURE                               */
/* -------------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
#ifdef PERFORMANCE_TESTING
    std::uint16_t counts = 0;
    while (counts < 1000)
    {
        entry_point[0] = rand() % GRID_HEIGHT;
        entry_point[1] = rand() % GRID_WIDTH;
        exit_point[0] = rand() % GRID_HEIGHT;
        exit_point[1] = rand() % GRID_WIDTH;

        for (size_t y = 0; y < GRID_HEIGHT; y++)
            for (size_t x = 0; x < GRID_WIDTH; x++)
                grid_array[y][x] = BLOCK_EMPTY;
#endif // PERFROMANCE_TESTING

        // Take User Input for Coverage
        std::cout << "Enter The Area Coverage Percentage: ";
        int coverage_percentage;
#ifndef PERFORMANCE_TESTING
        std::cin >> coverage_percentage;
#else
    coverage_percentage = (rand() % 20) + 10;
    std::cout << std::to_string(coverage_percentage) << "\n";
#endif
        // Setup Grid
        Setup_Grid *grid = new Setup_Grid(GRID_WIDTH, GRID_HEIGHT, coverage_percentage);
        grid->initialize_grid();

        // Initialize Path Planner
        StartSearch *plan_path = new StartSearch(entry_point[0], entry_point[1], exit_point[0], exit_point[1]);

        // Random Search
        sf::RenderWindow *search0 = grid->clear_grid(false, false, RANDOM_SEARCH);
        std::uint16_t random_steps = plan_path->initiate_search(RANDOM_SEARCH, search0, true);

        // Breadth-First Search
        sf::RenderWindow *search1 = grid->clear_grid(false, false, BFS_SEARCH);
        std::uint16_t bfs_steps = plan_path->initiate_search(BFS_SEARCH, search1, false);

        // Depth-First Search
        sf::RenderWindow *search2 = grid->clear_grid(false, false, DFS_SEARCH);
        std::uint16_t dfs_steps = plan_path->initiate_search(DFS_SEARCH, search2, false);

        // Dijkstra Search
        sf::RenderWindow *search3 = grid->clear_grid(false, false, DIJKSTRA_SEARCH);
        std::uint16_t dij_steps = plan_path->initiate_search(DIJKSTRA_SEARCH, search3, false);

        // Stitch Images Together Breadth-wise
        std::string file_search = "Images/*Start" + std::to_string(entry_point[0]) + "," + std::to_string(entry_point[1]) + "End" + std::to_string(exit_point[0]) + "," + std::to_string(exit_point[1]) + ".png";
        std::string command = "convert " + file_search + " +append Images/CombinedStart" + std::to_string(entry_point[0]) + "," + std::to_string(entry_point[1]) + "End" + std::to_string(exit_point[0]) + "," + std::to_string(exit_point[1]) + ".png ";
        std::cout << command + "\n";
        system(command.c_str());

        // while (search1->isOpen())
        // {
        //     sf::Event event;
        //     while (search1->pollEvent(event))
        //         if (event.type == sf::Event::Closed) // "close requested" event: we close the window
        //             search1->close();
        // }

        std::cout << "BFS Steps: " << std::to_string(bfs_steps) << "\n";
        std::cout << "DFS Steps: " << std::to_string(dfs_steps) << "\n";
        std::cout << "Dij Steps: " << std::to_string(dij_steps) << "\n";

        std::ofstream out;
        out.open("results.csv", std::ios_base::app);
        out << coverage_percentage << ",";
        out << bfs_steps << ",";
        out << dfs_steps << ",";
        out << dij_steps << "\n";
        out.close();
#ifdef PERFORMANCE_TESTING
        counts++;
        std::cout << std::to_string(counts) << "\n";
        std::cout << "--------------------------------------------------------------------------\n";
    }
#endif // PERFROMANCE_TESTING
    return EXIT_SUCCESS;
}

/* -------------------------------------------------------------------------- */
/*                         SETUP_GRID CLASS DEFINITION                        */
/* -------------------------------------------------------------------------- */

/**
 * @brief Get the Position where the Block can be placed
 *
 * @return std::array<uint8_t, 2>  (x,y) position
 */
std::array<uint8_t, 2> Setup_Grid::get_block_placement_position(void)
{
    std::uint8_t y{rand() % GRID_HEIGHT};
    std::uint8_t x{rand() % GRID_WIDTH};
    std::array<uint8_t, 2> position{y, x};
    return position;
}

/**
 * @brief Get the block type to place
 * @return uint8_t Block Type
 * 0 -> Line
 * 1 -> Inverted L
 * 2 -> S
 * 3 -> T
 */
std::uint8_t Setup_Grid::get_block_type(void)
{
    return (rand() % 4);
}

/**
 * @brief Set the bit on grid array
 * @param block_type
 * 0 -> Line
 * |* 0|
 * |* 0|
 * |* 0|
 * |* 0|
 * 1 -> Inverted L
 * |* *|
 * |0 *|
 * |0 *|
 * 2 -> S
 * |* 0|
 * |* *|
 * |0 *|
 * 3 -> T
 * |0 *|
 * |* *|
 * |0 *|
 * @param location
 * (x,y) position to place the block
 */
void Setup_Grid::update_grid_array(uint8_t block_type, std::array<uint8_t, 2> position)
{
    uint8_t y = position[0];
    uint8_t x = position[1];

    switch (block_type)
    {
    case 0: // Line
        grid_array[y][x] = BLOCK_OBSTACLE;
        grid_array[y + 1][x] = BLOCK_OBSTACLE;
        grid_array[y + 2][x] = BLOCK_OBSTACLE;
        grid_array[y + 3][x] = BLOCK_OBSTACLE;
        break;

    case 1: // Inverted L
        grid_array[y][x] = BLOCK_OBSTACLE;
        grid_array[y][x + 1] = BLOCK_OBSTACLE;
        grid_array[y + 1][x + 1] = BLOCK_OBSTACLE;
        grid_array[y + 2][x + 1] = BLOCK_OBSTACLE;
        break;

    case 2: // S
        grid_array[y][x] = BLOCK_OBSTACLE;
        grid_array[y + 1][x] = BLOCK_OBSTACLE;
        grid_array[y + 1][x + 1] = BLOCK_OBSTACLE;
        grid_array[y + 2][x + 1] = BLOCK_OBSTACLE;
        break;

    case 3: // Inverted T
        grid_array[y][x + 1] = BLOCK_OBSTACLE;
        grid_array[y + 1][x] = BLOCK_OBSTACLE;
        grid_array[y + 1][x + 1] = BLOCK_OBSTACLE;
        grid_array[y + 2][x + 1] = BLOCK_OBSTACLE;
        break;
    default:
        std::cout << "Update Type Incorrect!\n";
        break;
    }
}

/**
 * @brief Sum the whole matrix looking for the '1' bit and totaling it.
 *
 * @return std::uint64_t The covered pixels
 */
std::uint64_t Setup_Grid::calculate_coverage()
{
    std::uint64_t covered_pixels = 0;
    for (size_t i = 0; i < this->grid_width; i++)
        for (size_t j = 0; j < this->grid_height; j++)
            if (grid_array[i][j] == BLOCK_OBSTACLE)
                covered_pixels += 1;
    return covered_pixels;
}

/**
 * @brief Generate the visualization of the grid
 *
 * @param show_grid_lines True -> Show Grid Lines
 *                          False -> Hide Grid Lines
 * @param show_setup_animation True -> Show Grid Setup Animation
 *                              False -> Hide Grid Setup Animation
 * @param window_name Pointer to the Grid Window
 * @return sf::RenderWindow*
 */
sf::RenderWindow *Setup_Grid::visualize_grid(bool show_grid_lines, bool show_setup_animation, std::string window_name)
{
    // sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Obstacle Field"); // Each Pixel is considered 10 px wide for better visuals
#ifdef PERFORMANCE_TESTING
    sf::RenderWindow *window_perf;
    return window_perf;
#endif // PERFORMANCE_TESTING

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(GRID_WIDTH * PIXEL_WIDTH, GRID_HEIGHT * PIXEL_WIDTH), window_name); // Each Pixel is considered PIXEL_WIDTH px wide for better visuals
    window->setVerticalSyncEnabled(true);                                                                                             // Enabling VSync for FrameRate Control.
    window->setFramerateLimit(30);                                                                                                    // Set 60FPS Max
    window->clear(BG_COLOR);                                                                                                          // Set BG Color

    if (show_grid_lines)
        draw_grid_lines(window); // Draw Grid Lines if True
    window->display();           // Update display every iteration

    sf::RectangleShape obstacle_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH)); // Set Size of Obstacle Block
    obstacle_marker.setFillColor(OBSTACLE_COLOR);                               // Set Color of Obstacle Block

    if (window->isOpen())
    {

        for (size_t y = 0; y < this->grid_height; y++)
        {
            for (size_t x = 0; x < this->grid_width; x++)
            {
                if (grid_array[y][x] == BLOCK_OBSTACLE)
                {
                    sf::Vector2f obstacle_pos{sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH)}; // Set the Position of the Block on Grid
                    obstacle_marker.setPosition(obstacle_pos);                                 // Place the Block on the Grid
                    window->draw(obstacle_marker);                                             // Update the Grid
                }
            }
            if (show_setup_animation)
                window->display(); // Update display every iteration
        }

        std::cout << "Grid Visualization Complete" << std::endl;

        if (!show_setup_animation)
            window->display(); // Update the whole window at once
    }
    return window;
}

void Setup_Grid::draw_grid_lines(sf::RenderWindow *window)
{
    // Plot the Horizontal Lines
    for (size_t x = 0; x <= GRID_HEIGHT; x++)
    {
        sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(0, x * PIXEL_WIDTH), OBSTACLE_COLOR),
                sf::Vertex(sf::Vector2f(GRID_WIDTH * PIXEL_WIDTH, x * PIXEL_WIDTH), OBSTACLE_COLOR),
            };
        window->draw(line, 2, sf::Lines);
    }

    // Plot the Vertical Lines
    for (size_t y = 0; y <= GRID_WIDTH; y++)
    {
        sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(y * PIXEL_WIDTH, 0), OBSTACLE_COLOR),
                sf::Vertex(sf::Vector2f(y * PIXEL_WIDTH, GRID_HEIGHT * PIXEL_WIDTH), OBSTACLE_COLOR),
            };
        window->draw(line, 2, sf::Lines);
    }
}

/**
 * @brief Construct a new Setup_Grid object
 *
 * @param grid_width
 * @param grid_height
 * @param coverage_percentage range (0,100)
 */
Setup_Grid::Setup_Grid(std::uint8_t grid_width, std::uint8_t grid_height, std::uint8_t coverage_percentage)
{
    this->grid_width = grid_width;
    this->grid_height = grid_height;
    this->coverage_percentage = coverage_percentage;
}

/**
 * @brief Initialize the Grid for motion planning algorithm
 */
void Setup_Grid::initialize_grid(void)
{
    // Do Basic Calculation on how much coverage is needed
    uint64_t total_pixels = this->grid_width * this->grid_height;
    uint64_t target_coverage_pixels = (coverage_percentage * total_pixels) / 100;
    uint16_t coverage_blocks = target_coverage_pixels / 4;

    std::srand(std::time(0)); // Initialize Time Base Randomizer for grid

    // Place the minimum number of block required to meet the coverage
    for (size_t i = 0; i < coverage_blocks; i++)
        update_grid_array(get_block_type(), get_block_placement_position());

    // Place more block until the exact coverage is met
    while (calculate_coverage() < target_coverage_pixels)
        update_grid_array(get_block_type(), get_block_placement_position());

    std::cout << "Grid Initialization Complete" << std::endl;
}

/**
 * @brief Clear the value of grid cells from BLOCK_VISITED to BLOCK_CLEAR
 *
 * @param show_grid_lines True -> Show Grid Lines
 *                          False -> Hide Grid Lines
 * @param show_setup_animation True -> Show Grid Setup Animation
 *                              False -> Hide Grid Setup Animation
 * @param window_name Pointer to the Grid Window
 * @return sf::RenderWindow*
 */
sf::RenderWindow *Setup_Grid::clear_grid(bool show_grid_lines, bool show_setup_animation, std::string window_name)
{
    for (size_t y = 0; y < GRID_HEIGHT; y++)
        for (size_t x = 0; x < GRID_WIDTH; x++)
            if (grid_array[y][x] == BLOCK_VISITED)
                grid_array[y][x] = BLOCK_EMPTY;

    return visualize_grid(show_grid_lines, show_setup_animation, window_name);
}

/* -------------------------------------------------------------------------- */
/*                        STARTSEARCH CLASS DEFINITION                        */
/* -------------------------------------------------------------------------- */

/* -------------------------- AUXILIARY DEFINITION -------------------------- */
/**
 * @brief Search for all the locations around the current cell
 *
 * @param x
 * @param y
 * @param mark_location True -> Mark cell as BLOCK_VISITED if empty
 *                          False -> Don't Mark Cell as BLOCK_VISITED if empty or the edge cells are detected which cannot be visited.
 * @return true
 * @return false
 */
bool StartSearch::is_right_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
{
    if (x == GRID_WIDTH - 1)
        return false;

    if (grid_array[y][x + 1] == BLOCK_EMPTY)
    {
        if (mark_location)
            grid_array[y][x + 1] = BLOCK_VISITED;
        return true;
    }
    return false;
}

bool StartSearch::is_down_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
{
    if (y == GRID_HEIGHT - 1)
        return false;
    if (grid_array[y + 1][x] == BLOCK_EMPTY)
    {
        if (mark_location)
            grid_array[y + 1][x] = BLOCK_VISITED;
        return true;
    }
    return false;
}

bool StartSearch::is_left_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
{
    if (x == 0)
        return false;
    if (grid_array[y][x - 1] == BLOCK_EMPTY)
    {
        if (mark_location)
            grid_array[y][x - 1] = BLOCK_VISITED;
        return true;
    }
    return false;
}

bool StartSearch::is_up_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
{
    if (y == 0)
        return false;
    if (grid_array[y - 1][x] == BLOCK_EMPTY)
    {
        if (mark_location)
            grid_array[y - 1][x] = BLOCK_VISITED;
        return true;
    }
    return false;
}

bool StartSearch::is_up_left_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
{
    if (y == 0 or x == 0)
        return false;
    if (grid_array[y - 1][x - 1] == BLOCK_EMPTY)
    {
        if (mark_location)
            grid_array[y - 1][x - 1] == BLOCK_VISITED;
        return true;
    }
    return false;
}

bool StartSearch::is_up_right_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
{
    if (y == 0 or x == GRID_WIDTH - 1)
        return false;
    if (grid_array[y - 1][x + 1] == BLOCK_EMPTY)
    {
        if (mark_location)
            grid_array[y - 1][x + 1] == BLOCK_VISITED;
        return true;
    }
    return false;
}

bool StartSearch::is_down_left_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
{
    if (y == GRID_HEIGHT - 1 or x == 0)
        return false;
    if (grid_array[y + 1][x - 1] == BLOCK_EMPTY)
    {
        if (mark_location)
            grid_array[y + 1][x - 1] == BLOCK_VISITED;
        return true;
    }
    return false;
}

bool StartSearch::is_down_right_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
{
    if (y == GRID_HEIGHT - 1 or x == GRID_WIDTH - 1)
        return false;
    if (grid_array[y + 1][x + 1] == BLOCK_EMPTY)
    {
        if (mark_location)
            grid_array[y + 1][x + 1] == BLOCK_VISITED;
        return true;
    }
    return false;
}

/* ------------------------- PATH PLOTTING FUNCTION ------------------------- */
/**
 * @brief Break the String of path into the cell positions
 *
 * @param path String of path
 */
std::uint16_t StartSearch::itemize_path(std::string path)
{
    std::uint8_t y = start_pos[0];
    std::uint8_t x = start_pos[1];

    std::vector<std::uint8_t> position;
    position.push_back(y);
    position.push_back(x);

    this->position_list.push_back(position); // Store the first position into the array

    for (auto move : path)
    {
        // Element-wise selection from path string and move around the cells are requested
        if (move == 'D')
            y = y + 1;
        else if (move == 'R')
            x = x + 1;
        else if (move == 'U')
            y = y - 1;
        else if (move == 'L')
            x = x - 1;
        else if (move == 'Q')
        {
            x = x - 1;
            y = y - 1;
        }
        else if (move == 'E')
        {
            x = x + 1;
            y = y - 1;
        }
        else if (move == 'Z')
        {
            x = x - 1;
            y = y + 1;
        }
        else if (move == 'C')
        {
            x = x + 1;
            y = y + 1;
        }

        std::vector<std::uint8_t> position;
        position.push_back(y);
        position.push_back(x);
        this->position_list.push_back(position); // Store the new position in the array
    }

    this->cell_count = this->position_list.size(); // Update the list count
    display_path();                                // Visualize the path
    return this->cell_count;
}

/**
 * @brief Visualize the Cells separated into the grid
 *
 */
void StartSearch::display_path(void)
{
#ifdef PERFORMANCE_TESTING
    return;
#endif // PERFORMANCE_TESTING

    // Select the first position
    std::vector<std::uint8_t> cell = position_list[0];
    std::uint8_t y = cell[0];
    std::uint8_t x = cell[1];

    // Setup marker for plotting path
    sf::RectangleShape plotting_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
    plotting_marker.setFillColor(PLOTTING_COLOR);
    std::uint8_t iteration, count = 0;
    std::string files;

    for (std::uint16_t pos = 0; pos < cell_count; pos++)
    {
        y = this->position_list[pos][0];
        x = this->position_list[pos][1];

        sf::Vector2f plotting_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
        plotting_marker.setPosition(plotting_pos);
        window->draw(plotting_marker);
        window->display(); // Update display every iteration
        iteration++;

#ifdef GENERATE_GIF
        if ((iteration == 10) || (pos == cell_count - 1))
        {
            std::string file_name = "Images/gif" + std::to_string(count) + ".png";
            files += file_name + " ";
            sf::Texture texture;
            texture.create(window->getSize().x, window->getSize().y);
            texture.update(*window);
            if (texture.copyToImage().saveToFile(file_name))
                std::cout << "Screenshot Saved as " << file_name << std::endl;
            count++;
            iteration = 0;
        }
#endif // GENERATE_GIF
    }

    // Draw the start position
    sf::CircleShape start_point_marker(sf::CircleShape(PIXEL_WIDTH / 2, 30));
    sf::Vector2f start_point_pos = sf::Vector2f((this->start_pos[1]) * PIXEL_WIDTH, (this->start_pos[0]) * PIXEL_WIDTH);
    start_point_marker.setPosition(start_point_pos);
    start_point_marker.setFillColor(START_POINT_COLOR);
    window->draw(start_point_marker);

    // Draw the end position
    sf::RectangleShape end_point_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
    sf::Vector2f end_point_pos = sf::Vector2f((this->end_pos[1]) * PIXEL_WIDTH, (this->end_pos[0]) * PIXEL_WIDTH);
    end_point_marker.setPosition(end_point_pos);
    end_point_marker.setFillColor(END_POINT_COLOR);
    window->draw(end_point_marker);
    window->display(); // Update display every iteration

    // Save the Grid as image
    std::string file_name = "Images/" + this->search_type + "Start" + std::to_string(start_pos[0] + 1) + "," + std::to_string(start_pos[1] + 1) + "End" + std::to_string(end_pos[0] + 1) + "," + std::to_string(end_pos[1] + 1) + ".png";
    sf::Texture texture;
    texture.create(window->getSize().x, window->getSize().y);
    texture.update(*window);
    if (texture.copyToImage().saveToFile(file_name))
        std::cout << "Screenshot Saved as " << file_name << std::endl;

#ifdef GENERATE_GIF
    std::string command = "convert -delay 10 -loop 0 " + files + " Images/gif.gif ";
    std::cout << command + "\n";
    system(command.c_str());
    system("rm Images/gif*.png");
#endif // GENERATE_GIF
}

/* -------------------------------------------------------------------------- */
/*                           PATH PLANNING FUNCTIONS                          */
/* -------------------------------------------------------------------------- */

/* ------------------------------ RANDOM SEARCH ----------------------------- */

/**
 * @brief Perform Random Search
 *
 * @param display_window Pointer to the display window
 * @param show_search_animation True -> Show Search Animation
 *                                  False -> Hide Search Animation
 * @return std::uint16_t Number of Steps
 */
std::uint16_t StartSearch::random_search(sf::RenderWindow *display_window, bool show_search_animation)
{
    this->window = display_window; // Update the pointer to the window

    // Initialize LIFO Stack to store Neighbour Information
    std::vector<std::uint8_t> x_stack, y_stack;
    std::vector<std::string> move_stack;

    bool path_found = false; // Boolean to determine whether the path is found or not

    // Push the Start Node to Stack
    y_stack.push_back(start_pos[0]);
    x_stack.push_back(start_pos[1]);
    move_stack.push_back("");

    grid_array[start_pos[0]][start_pos[1]] = BLOCK_VISITED; // Mark the Start as visited

    // Setup marker to mark the visited blocks
    sf::RectangleShape mapping_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
    mapping_marker.setFillColor(MAPPING_COLOR);

    std::uint8_t count = 1; // To Name the Image Uniquely
    std::string files;

    // Pop the last element from the Stack
    std::uint8_t x = start_pos[1];
    std::uint8_t y = start_pos[0];
    std::string path;

    while (!x_stack.empty())
    {
        x_stack.clear();
        y_stack.clear();
        move_stack.clear();

        // Mark the Current node as visited
        sf::Vector2f mapped_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
        mapping_marker.setPosition(mapped_pos);
        window->draw(mapping_marker);

        if (show_search_animation) // Update the Grid for Visualization
            window->display();     // Update display every iteration

#ifdef GENERATE_GIF
        std::string file_name = "Images/rand" + std::to_string(count) + ".png";
        files += file_name + " ";
        sf::Texture texture;
        texture.create(window->getSize().x, window->getSize().y);
        texture.update(*window);
        if (texture.copyToImage().saveToFile(file_name))
            std::cout << "Screenshot Saved as " << file_name << std::endl;
        count++;
#endif // GENERATE_GIF

        if ((y == end_pos[0]) && (x == end_pos[1])) // Break if end point reached
        {
            path_found = true;
            move_stack.push_back(path);
            break;
        }

        // Look for surrounding Neighbors, if empty then add them to stack and mark them as visited
        if (is_up_empty(x, y, true))
        {
            x_stack.push_back(x);
            y_stack.push_back(y - 1);
            move_stack.push_back(path + 'U');
        }
        if (is_left_empty(x, y, true))
        {
            x_stack.push_back(x - 1);
            y_stack.push_back(y);
            move_stack.push_back(path + 'L');
        }
        if (is_down_empty(x, y, true))
        {
            x_stack.push_back(x);
            y_stack.push_back(y + 1);
            move_stack.push_back(path + 'D');
        }
        if (is_right_empty(x, y, true))
        {
            x_stack.push_back(x + 1);
            y_stack.push_back(y);
            move_stack.push_back(path + 'R');
        }
        if (!x_stack.empty())
        {
            std::uint8_t pos = rand() % x_stack.size();
            x = x_stack[pos];
            y = y_stack[pos];
            path = move_stack[pos];
        }
        else
            break;
    }

    std::uint16_t steps = 0;
    if (path_found)
    {
        // std::cout << "Search Complete: " << move_stack.back() << std::endl;
        steps = itemize_path(move_stack.back()); // Break the Path Strings into coordinated and display them

#ifdef GENERATE_GIF
        std::string file_search = "Images/rand*.png";
        std::string command = "convert -delay 10 -loop 0 " + files + " Images/gif.gif " + " Images/random.gif ";
        std::cout << command + "\n";
        system(command.c_str());
        system("rm Images/rand*.png Images/gif.gif");
#endif // GENERATE_GIF
    }
    else
        std::cout << "Search Failed!" << std::endl;
    return steps;
}

/* ------------------------- BREADTH - FIRST SEARCH ------------------------- */

/**
 * @brief Perform BFS Search
 *
 * @param display_window Pointer to the display window
 * @param show_search_animation True -> Show Search Animation
 *                                  False -> Hide Search Animation
 * @return std::uint16_t Number of Steps
 */
std::uint16_t StartSearch::bfs_search(sf::RenderWindow *display_window, bool show_search_animation)
{
    // Update the pointer to the window
    this->window = display_window;

    // Initialize FIFO Queue to Store Neighbour Information
    std::queue<std::uint8_t> x_que, y_que;
    std::queue<std::string> move_que;

    bool path_found = false; // Boolean to determine whether the path is found or not.

    // Push the Start to Queue
    y_que.push(start_pos[0]);
    x_que.push(start_pos[1]);
    move_que.push("");

    grid_array[start_pos[0]][start_pos[1]] = BLOCK_VISITED; // Mark Start as Visited

    // Setup marker to mark the visited blocks
    sf::RectangleShape mapping_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
    mapping_marker.setFillColor(MAPPING_COLOR);

    std::uint8_t iterations = 0; // Update Iterations for grid
    std::uint8_t count = 1;      // To Name the Image Uniquely
    std::string files;

    while (true && (!x_que.empty())) // Continue till the queue is empty or end position is reached
    {
        // Pop the First Element from Queue
        std::uint8_t x = x_que.front();
        std::uint8_t y = y_que.front();
        std::string path = move_que.front();
        x_que.pop();
        y_que.pop();
        move_que.pop();

        // Mark the Current Node as Visited
        sf::Vector2f mapped_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
        mapping_marker.setPosition(mapped_pos);
#ifndef PERFORMANCE_TESTING
        window->draw(mapping_marker);
#endif // PERFORMANCE_TESTING;

        if (show_search_animation) // Update the Grid for Visualization
            if (iterations == UINT8_MAX)
            {
                window->display(); // Update display every iteration
                iterations = 0;
#ifdef GENERATE_GIF
                std::string file_name = "Images/bfs" + std::to_string(count) + ".png";
                files += file_name + " ";
                sf::Texture texture;
                texture.create(window->getSize().x, window->getSize().y);
                texture.update(*window);
                if (texture.copyToImage().saveToFile(file_name))
                    std::cout << "Screenshot Saved as " << file_name << std::endl;
                count++;
#endif // GENERATE_GIF
            }
            else
                iterations++;

        if ((y == end_pos[0]) && (x == end_pos[1])) // Break if End Point Reached
        {
            path_found = true;
            move_que.push(path);
            break;
        }

        // Look for surrounding Neighbors, if empty then add them to queue and mark them as visited
        if (is_right_empty(x, y, true))
        {
            x_que.push(x + 1);
            y_que.push(y);
            move_que.push(path + 'R');
        }
        if (is_down_empty(x, y, true))
        {
            x_que.push(x);
            y_que.push(y + 1);
            move_que.push(path + 'D');
        }
        if (is_left_empty(x, y, true))
        {
            x_que.push(x - 1);
            y_que.push(y);
            move_que.push(path + 'L');
        }
        if (is_up_empty(x, y, true))
        {
            x_que.push(x);
            y_que.push(y - 1);
            move_que.push(path + 'U');
        }
    }

    std::uint16_t steps = 0;
    if (path_found)
    {
        // std::cout << "Search Complete: " << move_que.back() << std::endl;
        steps = itemize_path(move_que.back()); // Break the Path Strings into coordinated and display them
#ifdef GENERATE_GIF
        std::string file_search = "Images/bfs*.png";
        std::string command = "convert -delay 10 -loop 0 " + files + " Images/gif.gif " + " Images/bfs.gif ";
        std::cout << command + "\n";
        system(command.c_str());
        system("rm Images/bfs*.png Images/gif.gif");
#endif // GENERATE_GIF
    }
    else
        std::cout << "Search Failed!" << std::endl;
    return steps;
}

/* -------------------------- DEPTH - FIRST SEARCH -------------------------- */
/**
 * @brief Perform DFS Search
 *
 * @param display_window Pointer to the display window
 * @param show_search_animation True -> Show Search Animation
 *                                  False -> Hide Search Animation
 * @return std::uint16_t Number of Steps
 */
std::uint16_t StartSearch::dfs_search(sf::RenderWindow *display_window, bool show_search_animation)
{
    this->window = display_window; // Update the pointer to the window

    // Initialize LIFO Stack to store Neighbour Information
    std::vector<std::uint8_t> x_stack, y_stack;
    std::vector<std::string> move_stack;

    bool path_found = false; // Boolean to determine whether the path is found or not

    // Push the Start Node to Stack
    y_stack.push_back(start_pos[0]);
    x_stack.push_back(start_pos[1]);
    move_stack.push_back("");

    grid_array[start_pos[0]][start_pos[1]] = BLOCK_VISITED; // Mark the Start as visited

    // Setup marker to mark the visited blocks
    sf::RectangleShape mapping_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
    mapping_marker.setFillColor(MAPPING_COLOR);

    std::uint8_t iterations = 0; // Update Iterations for grid
    std::uint8_t count = 1;      // To Name the Image Uniquely
    std::string files;

    while (!x_stack.empty())
    {
        // Pop the last element from the Stack
        std::uint8_t x = x_stack.back();
        std::uint8_t y = y_stack.back();
        std::string path = move_stack.back();
        x_stack.pop_back();
        y_stack.pop_back();
        move_stack.pop_back();

        // Mark the Current node as visited
        sf::Vector2f mapped_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
        mapping_marker.setPosition(mapped_pos);
#ifndef PERFORMANCE_TESTING
        window->draw(mapping_marker);
#endif // PERFORMANCE_TESTING;

        if (show_search_animation) // Update the Grid for Visualization
            if (iterations == UINT8_MAX)
            {
                window->display(); // Update display every iteration
                iterations = 0;
#ifdef GENERATE_GIF
                std::string file_name = "Images/dfs" + std::to_string(count) + ".png";
                files += file_name + " ";
                sf::Texture texture;
                texture.create(window->getSize().x, window->getSize().y);
                texture.update(*window);
                if (texture.copyToImage().saveToFile(file_name))
                    std::cout << "Screenshot Saved as " << file_name << std::endl;
                count++;
#endif // GENERATE_GIF
            }
            else
                iterations++;

        if ((y == end_pos[0]) && (x == end_pos[1])) // Break if end point reached
        {
            path_found = true;
            move_stack.push_back(path);
            break;
        }

        // Look for surrounding Neighbors, if empty then add them to stack and mark them as visited
        if (is_up_empty(x, y, true))
        {
            x_stack.push_back(x);
            y_stack.push_back(y - 1);
            move_stack.push_back(path + 'U');
        }
        if (is_left_empty(x, y, true))
        {
            x_stack.push_back(x - 1);
            y_stack.push_back(y);
            move_stack.push_back(path + 'L');
        }
        if (is_down_empty(x, y, true))
        {
            x_stack.push_back(x);
            y_stack.push_back(y + 1);
            move_stack.push_back(path + 'D');
        }
        if (is_right_empty(x, y, true))
        {
            x_stack.push_back(x + 1);
            y_stack.push_back(y);
            move_stack.push_back(path + 'R');
        }
    }

    std::uint16_t steps = 0;
    if (path_found)
    {
        // std::cout << "Search Complete: " << move_stack.back() << std::endl;
        steps = itemize_path(move_stack.back()); // Break the Path Strings into coordinated and display them

#ifdef GENERATE_GIF
        std::string file_search = "Images/dfs*.png";
        std::string command = "convert -delay 10 -loop 0 " + files + " Images/gif.gif " + " Images/dfs.gif ";
        std::cout << command + "\n";
        system(command.c_str());
        system("rm Images/dfs*.png Images/gif.gif");
#endif // GENERATE_GIF
    }
    else
        std::cout << "Search Failed!" << std::endl;
    return steps;
}

/* ----------------------------- DIJKSTRA SEARCH ---------------------------- */
/**
 * @brief Perform Dijkstra Search
 *
 * @param display_window Pointer to the display window
 * @param show_search_animation True -> Show Search Animation
 *                                  False -> Hide Search Animation
 * @return std::uint16_t Number of Steps
 */
std::uint16_t StartSearch::dijkstra_search(sf::RenderWindow *display_windows, bool show_search_animation)
{
    this->window = display_windows; // Update the pointer to the window

    bool path_found = false; // Boolean to determine whether the path is found or not

    std::uint16_t grid_array_data[GRID_WIDTH][GRID_HEIGHT][3]; // Grid to store parent and distance
    for (size_t y = 0; y < GRID_HEIGHT; y++)
        for (size_t x = 0; x < GRID_WIDTH; x++)
            grid_array_data[y][x][2] = UINT16_MAX; // Mark all the nodes at INFINITY distance

    std::queue<std::uint8_t> x_stack, y_stack; // Initialize the FIFO Queue  to store neighbour information

    // Push the Start Node into the queue
    y_stack.push(start_pos[0]);
    x_stack.push(start_pos[1]);

    // Set the Parent of Start Node as itself
    grid_array_data[start_pos[0]][start_pos[1]][0] = start_pos[0];
    grid_array_data[start_pos[0]][start_pos[1]][1] = start_pos[1];
    grid_array_data[start_pos[0]][start_pos[1]][2] = 0; // Set the distance of start node as 'zero'

    // Setup marker to mark the visited blocks
    sf::RectangleShape mapping_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
    mapping_marker.setFillColor(MAPPING_COLOR);

    std::uint8_t iterations = 0; // Update Iterations for grid
    std::uint8_t count = 1;      // To Name the Image Uniquely
    std::string files;

    while (!x_stack.empty())
    {
        // Pop the node information from the queue
        std::uint8_t x = x_stack.front();
        std::uint8_t y = y_stack.front();
        std::uint16_t distance = grid_array_data[y][x][2]; // Retrieve the distance from the grid of the current node

        x_stack.pop();
        y_stack.pop();

        if (grid_array[y][x] == BLOCK_EMPTY) // Visit the node only if it is empty
        {
            grid_array[y][x] = BLOCK_VISITED; // Mark the Node as visited

            sf::Vector2f mapped_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
            mapping_marker.setPosition(mapped_pos);
#ifndef PERFORMANCE_TESTING
            window->draw(mapping_marker);
#endif // PERFORMANCE_TESTING;

            if (show_search_animation) // Update the Grid for Visualization
                if (iterations == UINT8_MAX)
                {
                    window->display(); // Update display every iteration
                    iterations = 0;
#ifdef GENERATE_GIF
                    std::string file_name = "Images/dij" + std::to_string(count) + ".png";
                    files += file_name + " ";
                    sf::Texture texture;
                    texture.create(window->getSize().x, window->getSize().y);
                    texture.update(*window);
                    if (texture.copyToImage().saveToFile(file_name))
                        std::cout << "Screenshot Saved as " << file_name << std::endl;
                    count++;
#endif // GENERATE_GIF
                }
                else
                    iterations++;

            if ((y == end_pos[0]) && (x == end_pos[1])) // Break if end point is reached
            {
                path_found = true;
                break;
            }

            /**
             * Look for Surrounding Empty Cells
             * Push the location of new node in queue
             * If their current distance is less the the distance of the currently active nodes (distance+1),
             * then mark the current node as parent node for the neighbour and update their distance
             */
            if (is_down_empty(x, y, false))
            {
                std::uint8_t ny, nx;
                ny = y + 1;
                nx = x;
                x_stack.push(nx);
                y_stack.push(ny);
                if (grid_array_data[ny][nx][2] > distance + 1)
                {
                    grid_array_data[ny][nx][0] = y;
                    grid_array_data[ny][nx][1] = x;
                    grid_array_data[ny][nx][2] = distance + 1;
                }
            }
            if (is_down_left_empty(x, y, false))
            {
                std::uint8_t ny, nx;
                ny = y + 1;
                nx = x - 1;
                x_stack.push(nx);
                y_stack.push(ny);
                if (grid_array_data[ny][nx][2] > distance + 1)
                {
                    grid_array_data[ny][nx][0] = y;
                    grid_array_data[ny][nx][1] = x;
                    grid_array_data[ny][nx][2] = distance + 1;
                }
            }
            if (is_left_empty(x, y, false))
            {
                std::uint8_t ny, nx;
                ny = y;
                nx = x - 1;
                x_stack.push(nx);
                y_stack.push(ny);
                if (grid_array_data[ny][nx][2] > distance + 1)
                {
                    grid_array_data[ny][nx][0] = y;
                    grid_array_data[ny][nx][1] = x;
                    grid_array_data[ny][nx][2] = distance + 1;
                }
            }
            if (is_up_left_empty(x, y, false))
            {
                std::uint8_t ny, nx;
                ny = y - 1;
                nx = x - 1;
                x_stack.push(nx);
                y_stack.push(ny);
                if (grid_array_data[ny][nx][2] > distance + 1)
                {
                    grid_array_data[ny][nx][0] = y;
                    grid_array_data[ny][nx][1] = x;
                    grid_array_data[ny][nx][2] = distance + 1;
                }
            }
            if (is_up_empty(x, y, false))
            {
                std::uint8_t ny, nx;
                ny = y - 1;
                nx = x;
                x_stack.push(nx);
                y_stack.push(ny);
                if (grid_array_data[ny][nx][2] > distance + 1)
                {
                    grid_array_data[ny][nx][0] = y;
                    grid_array_data[ny][nx][1] = x;
                    grid_array_data[ny][nx][2] = distance + 1;
                }
            }
            if (is_up_right_empty(x, y, false))
            {
                std::uint8_t ny, nx;
                ny = y - 1;
                nx = x + 1;
                x_stack.push(nx);
                y_stack.push(ny);
                if (grid_array_data[ny][nx][2] > distance + 1)
                {
                    grid_array_data[ny][nx][0] = y;
                    grid_array_data[ny][nx][1] = x;
                    grid_array_data[ny][nx][2] = distance + 1;
                }
            }
            if (is_right_empty(x, y, false))
            {
                std::uint8_t ny, nx;
                ny = y;
                nx = x + 1;
                x_stack.push(nx);
                y_stack.push(ny);
                if (grid_array_data[ny][nx][2] > distance + 1)
                {
                    grid_array_data[ny][nx][0] = y;
                    grid_array_data[ny][nx][1] = x;
                    grid_array_data[ny][nx][2] = distance + 1;
                }
            }
            if (is_down_right_empty(x, y, false))
            {
                std::uint8_t ny, nx;
                ny = y + 1;
                nx = x + 1;
                x_stack.push(nx);
                y_stack.push(ny);
                if (grid_array_data[ny][nx][2] > distance + 1)
                {
                    grid_array_data[ny][nx][0] = y;
                    grid_array_data[ny][nx][1] = x;
                    grid_array_data[ny][nx][2] = distance + 1;
                }
            }
        }
    }

    std::uint16_t steps = 0;

    if (path_found)
    {
        // Start from the end position
        std::uint8_t y, x;
        y = end_pos[0];
        x = end_pos[1];

        std::vector<std::uint8_t> pos;
        pos.push_back(y);
        pos.push_back(x);
        this->position_list.push_back(pos); // Push the location of end position into the coordinate list

        while (true)
        {
            std::uint8_t ny, nx;
            ny = grid_array_data[y][x][0]; // Look for the parents of each cell visited
            nx = grid_array_data[y][x][1];
            y = ny;
            x = nx;
            std::vector<std::uint8_t> pos;
            pos.push_back(y);
            pos.push_back(x);
            this->position_list.push_back(pos); // Push the location of parent into the Coordinate List

            if ((y == start_pos[0]) && (x == start_pos[1])) // Break the Loop when Start position is reached
            {
                std::vector<std::vector<std::uint8_t>> inverted_list;

                while (!this->position_list.empty()) // Invert the whole position list as the current list goes from end position to starting position
                {
                    inverted_list.push_back(this->position_list.back());
                    this->position_list.pop_back();
                }

                this->position_list.clear();
                for (size_t i = 0; i < inverted_list.size(); i++) // Update the inverted List into Position List
                    this->position_list.push_back(inverted_list[i]);
                inverted_list.clear();
                break;
            }
        }

        this->cell_count = this->position_list.size(); // Calculate Length
        steps = this->cell_count;
        display_path(); // Display the Path
#ifdef GENERATE_GIF
        std::string file_search = "Images/dij*.png";
        std::string command = "convert -delay 10 -loop 0 " + files + " Images/gif.gif " + " Images/dij.gif ";
        std::cout << command + "\n";
        system(command.c_str());
        system("rm Images/dij*.png Images/gif.gif");
#endif // GENERATE_GIF
    }
    else
        std::cout << "Search Failed!" << std::endl;
    return steps;
}

/* ------------------------------- CONSTRUCTOR ------------------------------ */
/**
 * @brief Construct a new Start Search object
 *
 * @param start_position_y
 * @param start_position_x
 * @param end_position_y
 * @param end_position_x
 */
StartSearch::StartSearch(std::uint8_t start_position_y, std::uint8_t start_position_x, std::uint8_t end_position_y, std::uint8_t end_position_x)
{
    this->start_pos[0] = start_position_y - 1;
    this->start_pos[1] = start_position_x - 1;
    this->end_pos[0] = end_position_y - 1;
    this->end_pos[1] = end_position_x - 1;
    grid_array[this->start_pos[0]][this->start_pos[1]] = BLOCK_EMPTY; // Mark the Start Position as Empty
    grid_array[this->end_pos[0]][this->end_pos[1]] = BLOCK_EMPTY;     // Mark the End Position as Empty
}

/* ----------------------------- SEARCH FUNCTION ---------------------------- */
/**
 * @brief Search the Grid as requested
 *
 * @param search_type Search Tag Type
 * @param window Pointer to Grid Window for Visualization
 * @param show_search_animation True -> Show Search Animation
 *                                  False -> Hide Search Animation
 */
std::uint16_t StartSearch::initiate_search(std::string search_type, sf::RenderWindow *window, bool show_search_animation)
{
    this->position_list.clear();
    if (search_type == RANDOM_SEARCH)
    {
        this->search_type = RANDOM_SEARCH;
        return random_search(window, show_search_animation);
    }
    else if (search_type == BFS_SEARCH)
    {
        this->search_type = BFS_SEARCH;
        return bfs_search(window, show_search_animation);
    }
    else if (search_type == DFS_SEARCH)
    {
        this->search_type = DFS_SEARCH;
        return dfs_search(window, show_search_animation);
    }
    else if (search_type == DIJKSTRA_SEARCH)
    {
        this->search_type = DIJKSTRA_SEARCH;
        return dijkstra_search(window, show_search_animation);
    }
    return 0;
}