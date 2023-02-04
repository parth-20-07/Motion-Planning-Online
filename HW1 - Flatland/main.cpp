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
#include <stdint.h>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <queue>

// #define GENERATE_GIF
#define GRID_WIDTH 128
#define GRID_HEIGHT 128
#define PIXEL_WIDTH 10

#define BLOCK_EMPTY 0
#define BLOCK_OBSTACLE 1
#define BLOCK_VISITED 2

#define BFS_SEARCH "BFS Search"
#define DFS_SEARCH "DFS Search"
#define DIJKSTRA_SEARCH "Dijkstra Search"

std::uint8_t grid_array[GRID_WIDTH][GRID_HEIGHT];   // Grid to store info
std::array<std::uint8_t, 2> entry_point = {50, 50}; // y,x
std::array<std::uint8_t, 2> exit_point = {124, 22}; // y,x

#define BG_COLOR sf::Color::White
#define OBSTACLE_COLOR sf::Color::Black
#define MAPPING_COLOR sf::Color::Yellow
#define PLOTTING_COLOR sf::Color::Cyan
#define START_POINT_COLOR sf::Color::Green
#define END_POINT_COLOR sf::Color::Red

class Setup_Grid
{
public:
    Setup_Grid(std::uint8_t grid_width, std::uint8_t grid_height, std::uint8_t coverage_percentage)
    {
        this->grid_width = grid_width;
        this->grid_height = grid_height;
        this->coverage_percentage = coverage_percentage;
    }

    /**
     * @brief Initiaize the 2D Grid for motion planning algorithm
     *
     * @param show_grid_lines set to true to visualize the pixel bounding boxes.
     * @param show_setup_animation set to true to visualize the grid setup
     * @return sf::RenderWindow* Pointer to the grid window
     */
    void initialize_grid(void)
    {
        // Do Basic Calculation on how much coverage is needed
        uint64_t total_pixels = this->grid_width * this->grid_height;
        uint64_t target_coverage_pixels = (coverage_percentage * total_pixels) / 100;
        uint16_t coverage_blocks = target_coverage_pixels / 4;

        for (size_t i = 0; i < coverage_blocks; i++)
            update_grid_array(get_block_type(), get_block_placement_position());

        while (calculate_coverage() < target_coverage_pixels)
            update_grid_array(get_block_type(), get_block_placement_position());

        std::cout << "Grid Initialization Complete" << std::endl;

        grid_array[entry_point[0]][entry_point[1]] = BLOCK_EMPTY;
        grid_array[exit_point[0]][exit_point[1]] = BLOCK_EMPTY;
    }

    sf::RenderWindow *vizualize_grid(bool show_grid_lines, bool show_setup_animation, std::string window_name)
    {
        // sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Obstacle Field"); // Each Pixel is considered 10 px wide for better visuals
        sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(GRID_WIDTH * PIXEL_WIDTH, GRID_HEIGHT * PIXEL_WIDTH), window_name); // Each Pixel is considered PIXEL_WIDTH px wide for better visuals
        window->setVerticalSyncEnabled(true);                                                                                             // Enabling VSync for FrameRate Control.
        window->setFramerateLimit(30);                                                                                                    // Set 60FPS Max
        window->clear(BG_COLOR);                                                                                                          // Set BG Color

        if (show_grid_lines)
            draw_grid_lines(window);
        window->display(); // Update display every iteration

        if (window->isOpen())
        {
            sf::RectangleShape obstacle_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
            obstacle_marker.setFillColor(OBSTACLE_COLOR);
            for (size_t y = 0; y < this->grid_height; y++)
            {
                for (size_t x = 0; x < this->grid_width; x++)
                {
                    if (grid_array[y][x] == BLOCK_OBSTACLE)
                    {
                        sf::Vector2f obstacle_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
                        obstacle_marker.setPosition(obstacle_pos);
                        window->draw(obstacle_marker);
                    }
                }
                if (show_setup_animation)
                    window->display(); // Update display every iteration
            }
            std::cout << "Grid Visualization Complete" << std::endl;
            if (!show_setup_animation)
                window->display(); // Update display every iteration
        }

        sf::CircleShape start_point_marker(sf::CircleShape(PIXEL_WIDTH / 2, 30));
        sf::Vector2f start_point_pos = sf::Vector2f((entry_point[1]) * PIXEL_WIDTH, (entry_point[0]) * PIXEL_WIDTH);
        start_point_marker.setPosition(start_point_pos);
        start_point_marker.setFillColor(START_POINT_COLOR);
        window->draw(start_point_marker);

        sf::RectangleShape end_point(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
        sf::Vector2f ep_pos = sf::Vector2f((exit_point[1]) * PIXEL_WIDTH, (exit_point[0]) * PIXEL_WIDTH);
        end_point.setPosition(ep_pos);
        end_point.setFillColor(END_POINT_COLOR);
        window->draw(end_point);
        window->display(); // Update display every iteration

        return window;
    }

    void draw_grid_lines(sf::RenderWindow *window)
    { // Plot the Horizontal Lines
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

    sf::RenderWindow *clear_grid(bool show_grid_lines, bool show_setup_animation, std::string window_name)
    {
        for (size_t y = 0; y < GRID_HEIGHT; y++)
            for (size_t x = 0; x < GRID_WIDTH; x++)
                if (grid_array[y][x] == BLOCK_VISITED)
                    grid_array[y][x] = BLOCK_EMPTY;

        return vizualize_grid(show_grid_lines, show_setup_animation, window_name);
    }

private:
    std::uint8_t grid_width;
    std::uint8_t grid_height;
    std::uint8_t coverage_percentage;

    /**
     * @brief Get the Position where the Block can be placed
     *
     * @return std::array<uint8_t, 2>  (x,y) position
     */
    std::array<uint8_t, 2> get_block_placement_position(void)
    {
        std::uint8_t y = rand() % GRID_HEIGHT;
        std::uint8_t x = rand() % GRID_WIDTH;
        std::array<uint8_t, 2> position = {y, x};
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
    std::uint8_t get_block_type(void)
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
    void update_grid_array(uint8_t block_type, std::array<uint8_t, 2> position)
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
    std::uint64_t calculate_coverage()
    {
        std::uint64_t covered_pixels = 0;
        for (size_t i = 0; i < this->grid_width; i++)
            for (size_t j = 0; j < this->grid_height; j++)
                if (grid_array[i][j] == BLOCK_OBSTACLE)
                    covered_pixels += 1;
        return covered_pixels;
    }
};

class StartSearch
{
private:
    std::string search_type;
    sf::RenderWindow *window;
    std::vector<std::vector<std::uint8_t>> position_list;
    std::array<std::uint8_t, 2> start_pos, end_pos;
    std::uint16_t cell_count;

    /* -------------------------------------------------------------------------- */
    /*                             AUXILARY FUNCTIONS                             */
    /* -------------------------------------------------------------------------- */

    /* ---------------------------- NEIGHBOUR SEARCH ---------------------------- */

    bool is_right_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
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

    bool is_down_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
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

    bool is_left_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
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

    bool is_up_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
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

    bool is_up_left_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
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

    bool is_up_right_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
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

    bool is_down_left_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
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

    bool is_down_right_empty(std::uint8_t x, std::uint8_t y, bool mark_location)
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

    void itemize_path(std::string path)
    {
        std::uint8_t y = start_pos[0];
        std::uint8_t x = start_pos[1];

        std::vector<std::uint8_t> position;
        position.push_back(y);
        position.push_back(x);
        this->position_list.push_back(position);

        for (std::uint16_t pos = 0; pos < path.length(); pos++)
        {
            if (path[pos] == 'D')
                y = y + 1;
            else if (path[pos] == 'R')
                x = x + 1;
            else if (path[pos] == 'U')
                y = y - 1;
            else if (path[pos] == 'L')
                x = x - 1;
            std::vector<std::uint8_t> position;
            position.push_back(y);
            position.push_back(x);
            this->position_list.push_back(position);
        }
        this->cell_count = this->position_list.size();
        display_path();
        return;
    }

    void display_path(void)
    {
        std::vector<std::uint8_t> cell = position_list[0];
        std::uint8_t y = cell[0];
        std::uint8_t x = cell[1];

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

        sf::CircleShape start_point_marker(sf::CircleShape(PIXEL_WIDTH / 2, 30));
        sf::Vector2f start_point_pos = sf::Vector2f((entry_point[1]) * PIXEL_WIDTH, (entry_point[0]) * PIXEL_WIDTH);
        start_point_marker.setPosition(start_point_pos);
        start_point_marker.setFillColor(START_POINT_COLOR);
        window->draw(start_point_marker);

        sf::RectangleShape end_point_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
        sf::Vector2f end_point_pos = sf::Vector2f((exit_point[1]) * PIXEL_WIDTH, (exit_point[0]) * PIXEL_WIDTH);
        end_point_marker.setPosition(end_point_pos);
        end_point_marker.setFillColor(END_POINT_COLOR);
        window->draw(end_point_marker);
        window->display(); // Update display every iteration

        std::string file_name = "Images/" + this->search_type + "Start" + std::to_string(start_pos[0]) + "," + std::to_string(start_pos[1]) + "End" + std::to_string(end_pos[0]) + "," + std::to_string(end_pos[1]) + ".png";
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

    /* ------------------------- BREADTH - FIRST SEARCH ------------------------- */

    void bfs_search(sf::RenderWindow *display_window, bool show_search_animation)
    {
        this->window = display_window;
        std::queue<std::uint8_t> x_que, y_que;
        std::queue<std::string> move_que;
        bool path_found = false;

        y_que.push(start_pos[0]);
        x_que.push(start_pos[1]);
        move_que.push("");

        grid_array[start_pos[0]][start_pos[1]] = BLOCK_VISITED;

        sf::RectangleShape mapping_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
        mapping_marker.setFillColor(MAPPING_COLOR);

        std::uint8_t iterations = 0;
        std::uint8_t count = 1;
        std::string files;

        while (true && (!x_que.empty()))
        {
            std::uint8_t x = x_que.front();
            std::uint8_t y = y_que.front();
            std::string path = move_que.front();

            x_que.pop();
            y_que.pop();
            move_que.pop();

            sf::Vector2f mapped_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
            mapping_marker.setPosition(mapped_pos);
            window->draw(mapping_marker);

            if (show_search_animation)
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

            if ((y == end_pos[0]) && (x == end_pos[1]))
            {
                path_found = true;
                move_que.push(path);
                break;
            }

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

        if (path_found)
        {
            std::cout << "Search Complete: " << move_que.back() << std::endl;
            itemize_path(move_que.back());
#ifdef GENERATE_GIF
            std::string file_search = "Images/bfs*.png";
            std::string command = "convert -delay 10 -loop 0 " + files + " Images/gif.gif " + " Images/bfs.gif ";
            std::cout << command + "\n";
            system(command.c_str());
#endif // GENERATE_GIF
        }
        else
            std::cout << "Search Failed!" << std::endl;
        system("rm Images/bfs*.png Images/gif.gif");
        return;
    }

    /* -------------------------- DEPTH - FIRST SEARCH -------------------------- */

    void dfs_search(sf::RenderWindow *display_window, bool show_search_animation)
    {
        this->window = display_window;
        std::vector<std::uint8_t> x_stack, y_stack;
        std::vector<std::string> move_stack;
        bool path_found = false;

        y_stack.push_back(start_pos[0]);
        x_stack.push_back(start_pos[1]);
        move_stack.push_back("");

        grid_array[start_pos[0]][start_pos[1]] = BLOCK_VISITED;

        sf::RectangleShape mapping_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
        mapping_marker.setFillColor(MAPPING_COLOR);

        std::uint8_t iterations = 0;
        std::uint8_t count = 1;
        std::string files;

        while (!x_stack.empty())
        {
            std::uint8_t x = x_stack.back();
            std::uint8_t y = y_stack.back();
            std::string path = move_stack.back();

            x_stack.pop_back();
            y_stack.pop_back();
            move_stack.pop_back();

            sf::Vector2f mapped_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
            mapping_marker.setPosition(mapped_pos);
            window->draw(mapping_marker);

            if (show_search_animation)
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

            if ((y == end_pos[0]) && (x == end_pos[1]))
            {
                path_found = true;
                move_stack.push_back(path);
                break;
            }

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

        if (path_found)
        {
            std::cout << "Search Complete: " << move_stack.back() << std::endl;
            std::cout << "Path Length: " << std::to_string(move_stack.back().length()) << "\n";
            itemize_path(move_stack.back());
#ifdef GENERATE_GIF
            std::string file_search = "Images/dfs*.png";
            std::string command = "convert -delay 10 -loop 0 " + files + " Images/gif.gif " + " Images/dfs.gif ";
            std::cout << command + "\n";
            system(command.c_str());
#endif // GENERATE_GIF
        }
        else
            std::cout << "Search Failed!" << std::endl;
        system("rm Images/dfs*.png Images/gif.gif");
        return;
    }

    /* ----------------------------- DIJKSTRA SEARCH ---------------------------- */

    void dijkstra_search(sf::RenderWindow *display_windows, bool show_search_animation)
    {
        this->window = display_windows;
        bool path_found = false;

        std::uint16_t grid_array_data[GRID_WIDTH][GRID_HEIGHT][3]; // Grid to store parent and distance
        for (size_t y = 0; y < GRID_HEIGHT; y++)
            for (size_t x = 0; x < GRID_WIDTH; x++)
                grid_array_data[y][x][2] = UINT16_MAX;

        std::queue<std::uint8_t> x_stack, y_stack;
        y_stack.push(start_pos[0]);
        x_stack.push(start_pos[1]);

        grid_array_data[start_pos[0]][start_pos[1]][0] = start_pos[0];
        grid_array_data[start_pos[0]][start_pos[1]][1] = start_pos[1];
        grid_array_data[start_pos[0]][start_pos[1]][2] = 0;

        sf::RectangleShape mapping_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
        mapping_marker.setFillColor(MAPPING_COLOR);

        std::uint8_t iterations = 0;
        std::uint8_t count = 1;
        std::string files;

        while (!x_stack.empty())
        {
            std::uint8_t x = x_stack.front();
            std::uint8_t y = y_stack.front();
            std::uint16_t distance = grid_array_data[y][x][2];

            x_stack.pop();
            y_stack.pop();
            if (grid_array[y][x] == BLOCK_EMPTY)
            {
                grid_array[y][x] = BLOCK_VISITED;
                sf::Vector2f mapped_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
                mapping_marker.setPosition(mapped_pos);
                window->draw(mapping_marker);

                if (show_search_animation)
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

                if ((y == end_pos[0]) && (x == end_pos[1]))
                {
                    path_found = true;
                    break;
                }

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

        if (path_found)
        {
            std::uint8_t y, x;
            y = end_pos[0];
            x = end_pos[1];

            std::vector<std::uint8_t> pos;
            pos.push_back(y);
            pos.push_back(x);
            this->position_list.push_back(pos);

            while (true)
            {
                std::uint8_t ny, nx;
                ny = grid_array_data[y][x][0];
                nx = grid_array_data[y][x][1];
                y = ny;
                x = nx;
                std::vector<std::uint8_t> pos;
                pos.push_back(y);
                pos.push_back(x);
                this->position_list.push_back(pos);

                if ((y == start_pos[0]) && (x == start_pos[1]))
                {
                    std::vector<std::vector<std::uint8_t>> inverted_list;
                    while (!this->position_list.empty())
                    {
                        inverted_list.push_back(this->position_list.back());
                        this->position_list.pop_back();
                    }
                    this->position_list.clear();
                    for (size_t i = 0; i < inverted_list.size(); i++)
                        this->position_list.push_back(inverted_list[i]);
                    inverted_list.clear();
                    break;
                }
            }

            std::cout << "Path Length: " << std::to_string(this->position_list.size()) << "\n";
            this->cell_count = this->position_list.size();
            display_path();
#ifdef GENERATE_GIF
            std::string file_search = "Images/dij*.png";
            std::string command = "convert -delay 10 -loop 0 " + files + " Images/gif.gif " + " Images/dij.gif ";
            std::cout << command + "\n";
            system(command.c_str());
#endif // GENERATE_GIF
        }
        else
            std::cout << "Search Failed!" << std::endl;
        system("rm Images/dij*.png Images/gif.gif");
        return;
    }

public:
    /* ------------------------------- CONSTRUCTOR ------------------------------ */

    StartSearch(std::uint8_t start_position_y, std::uint8_t start_position_x, std::uint8_t end_position_y, std::uint8_t end_position_x)
    {
        this->start_pos[0] = start_position_y;
        this->start_pos[1] = start_position_x;
        this->end_pos[0] = end_position_y;
        this->end_pos[1] = end_position_x;
    }

    /* ----------------------------- SEARCH FUNCTION ---------------------------- */
    /**
     * @brief Search the grid
     *
     * @param search_type
     * @param window
     * @param start_pos
     * @param end_pos
     * @param show_search_animation
     * @param quick_solve
     */
    void
    initiate_search(std::string search_type, sf::RenderWindow *window, bool show_search_animation)
    {
        this->position_list.clear();
        if (search_type == BFS_SEARCH)
        {
            this->search_type = BFS_SEARCH;
            bfs_search(window, show_search_animation);
        }
        else if (search_type == DFS_SEARCH)
        {
            this->search_type = DFS_SEARCH;
            dfs_search(window, show_search_animation);
        }
        else if (search_type == DIJKSTRA_SEARCH)
        {
            this->search_type = DIJKSTRA_SEARCH;
            dijkstra_search(window, show_search_animation);
        }
    }
};

int main(int argc, char *argv[])
{
    std::srand(std::time(0));
    // Take User Input for Coverage
    std::cout << "Enter The Area Coverage Percentage: ";
    int coverage_percentage;
    std::cin >> coverage_percentage;

    entry_point[0] = entry_point[0] - 1;
    entry_point[1] = entry_point[1] - 1;
    exit_point[0] = exit_point[0] - 1;
    exit_point[1] = exit_point[1] - 1;

    // Setup Grid
    Setup_Grid *grid = new Setup_Grid(GRID_WIDTH, GRID_HEIGHT, coverage_percentage);
    grid->initialize_grid();

    // Initialize Path Planner
    StartSearch *plan_path = new StartSearch(entry_point[0], entry_point[1], exit_point[0], exit_point[1]);

    // Breadth-First Search
    sf::RenderWindow *search1 = grid->clear_grid(false, false, BFS_SEARCH);
    plan_path->initiate_search(BFS_SEARCH, search1, true);

    // Depth-First Search
    sf::RenderWindow *search2 = grid->clear_grid(false, false, DFS_SEARCH);
    plan_path->initiate_search(DFS_SEARCH, search2, true);

    // Dijkstra Search
    sf::RenderWindow *search3 = grid->clear_grid(false, false, DIJKSTRA_SEARCH);
    plan_path->initiate_search(DIJKSTRA_SEARCH, search3, true);

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
    return EXIT_SUCCESS;
}