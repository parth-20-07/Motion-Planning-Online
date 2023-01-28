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
#include <cmath>
#include <SFML/Graphics.hpp>
#include <queue>

#define GRID_WIDTH 128
#define GRID_HEIGHT 128
#define PIXEL_WIDTH 10

#define BLOCK_EMPTY 0
#define BLOCK_OBSTACLE 1
#define BLOCK_VISITED 2
#define FINAL_PATH 3

#define BFS_SEARCH "bfs"
#define DFS_SEARCH "dfs"

std::uint8_t grid_array[GRID_WIDTH][GRID_HEIGHT];                   // Grid to store info
std::array<std::uint8_t, 2> entry_point = {1, 1};                   // y,x
std::array<std::uint8_t, 2> exit_point = {GRID_HEIGHT, GRID_WIDTH}; // y,x

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

    sf::RenderWindow *vizualize_grid(bool show_grid_lines, bool show_setup_animation)
    {
        // sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Obstacle Field"); // Each Pixel is considered 10 px wide for better visuals
        sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(GRID_WIDTH * PIXEL_WIDTH, GRID_HEIGHT * PIXEL_WIDTH), "Obstacle Field"); // Each Pixel is considered PIXEL_WIDTH px wide for better visuals
        window->setVerticalSyncEnabled(true);                                                                                                  // Enabling VSync for FrameRate Control.
        window->setFramerateLimit(30);                                                                                                         // Set 60FPS Max
        window->clear(sf::Color::White);                                                                                                       // Set BG Color

        if (show_grid_lines)
            draw_grid_lines(window);
        window->display(); // Update display every iteration

        if (window->isOpen())
        {
            sf::RectangleShape marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
            marker.setFillColor(sf::Color::Black);
            for (size_t y = 0; y < this->grid_height; y++)
            {
                for (size_t x = 0; x < this->grid_width; x++)
                {
                    if (grid_array[y][x] == BLOCK_OBSTACLE)
                    {
                        sf::Vector2f marker_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
                        marker.setPosition(marker_pos);
                        window->draw(marker);
                    }
                }
                if (show_setup_animation)
                    window->display(); // Update display every iteration
            }
            std::cout << "Grid Visualization Complete" << std::endl;
            if (!show_setup_animation)
                window->display(); // Update display every iteration
        }

        sf::CircleShape bot(sf::CircleShape(PIXEL_WIDTH / 2, 30));
        sf::Vector2f bot_pos = sf::Vector2f((entry_point[1]) * PIXEL_WIDTH, (entry_point[0]) * PIXEL_WIDTH);
        bot.setPosition(bot_pos);
        bot.setFillColor(sf::Color::Green);
        window->draw(bot);

        sf::RectangleShape end_point(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
        sf::Vector2f ep_pos = sf::Vector2f((exit_point[1]) * PIXEL_WIDTH, (exit_point[0]) * PIXEL_WIDTH);
        end_point.setPosition(ep_pos);
        end_point.setFillColor(sf::Color::Red);
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
                    sf::Vertex(sf::Vector2f(0, x * PIXEL_WIDTH), sf::Color::Black),
                    sf::Vertex(sf::Vector2f(GRID_WIDTH * PIXEL_WIDTH, x * PIXEL_WIDTH), sf::Color::Black),
                };
            window->draw(line, 2, sf::Lines);
        }

        // Plot the Vertical Lines
        for (size_t y = 0; y <= GRID_WIDTH; y++)
        {
            sf::Vertex line[] =
                {
                    sf::Vertex(sf::Vector2f(y * PIXEL_WIDTH, 0), sf::Color::Black),
                    sf::Vertex(sf::Vector2f(y * PIXEL_WIDTH, GRID_HEIGHT * PIXEL_WIDTH), sf::Color::Black),
                };
            window->draw(line, 2, sf::Lines);
        }
    }

    sf::RenderWindow *clear_grid(bool show_grid_lines, bool show_setup_animation)
    {
        for (size_t y = 0; y < GRID_HEIGHT; y++)
            for (size_t x = 0; x < GRID_WIDTH; x++)
                if (grid_array[y][x] != BLOCK_OBSTACLE)
                    grid_array[y][x] = BLOCK_EMPTY;

        return vizualize_grid(show_grid_lines, show_setup_animation);
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

    bool is_left_empty(std::uint8_t x, std::uint8_t y)
    {
        if (x == 0)
            return false;
        if (grid_array[y][x - 1] == BLOCK_EMPTY)
            return true;
        return false;
    }

    bool is_right_empty(std::uint8_t x, std::uint8_t y)
    {
        if (x == GRID_WIDTH - 1)
            return false;
        if (grid_array[y][x + 1] == BLOCK_EMPTY)
            return true;
        return false;
    }

    bool is_up_empty(std::uint8_t x, std::uint8_t y)
    {
        if (y == 0)
            return false;
        if (grid_array[y - 1][x] == BLOCK_EMPTY)
            return true;
        return false;
    }

    bool is_down_empty(std::uint8_t x, std::uint8_t y)
    {
        if (y == GRID_HEIGHT - 1)
            return false;
        if (grid_array[y + 1][x] == BLOCK_EMPTY)
            return true;
        return false;
    }

    void bfs_search(sf::RenderWindow *window, std::array<std::uint8_t, 2> &start_pos, std::array<std::uint8_t, 2> &end_pos, bool show_search_animation, bool quick_solve)
    {
        std::queue<std::uint8_t> x_que, y_que;
        std::queue<std::string> move_que;
        move_que.push("");

        y_que.push(start_pos[0]);
        x_que.push(start_pos[1]);

        std::uint8_t x = x_que.front();
        std::uint8_t y = y_que.front();
        grid_array[y][x] = BLOCK_VISITED;

        sf::RectangleShape mapping_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));

        while (true && (!x_que.empty()))
        {
            x = x_que.front();
            y = y_que.front();
            std::string path = move_que.front();

            sf::Vector2f mapped_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
            mapping_marker.setPosition(mapped_pos);
            mapping_marker.setFillColor(sf::Color::Yellow);
            window->draw(mapping_marker);
            if (show_search_animation)
                window->display(); // Update display every iteration

            if ((y == end_pos[0]) && (x == end_pos[1]))
                break;

            grid_array[y][x] = BLOCK_VISITED;
            if (is_down_empty(x, y))
            {
                x_que.push(x);
                y_que.push(y + 1);
                if (quick_solve)
                    grid_array[y + 1][x] = BLOCK_VISITED;
                std::string possible_path = path + 'D';
                move_que.push(possible_path);
            }
            if (is_right_empty(x, y))
            {
                x_que.push(x + 1);
                y_que.push(y);
                if (quick_solve)
                    grid_array[y][x + 1] = BLOCK_VISITED;
                std::string possible_path = path + 'R';
                move_que.push(possible_path);
            }
            if (is_up_empty(x, y))
            {
                x_que.push(x);
                y_que.push(y - 1);
                if (quick_solve)
                    grid_array[y - 1][x] = BLOCK_VISITED;
                std::string possible_path = path + 'U';
                move_que.push(possible_path);
            }
            if (is_left_empty(x, y))
            {
                x_que.push(x - 1);
                y_que.push(y);
                if (quick_solve)
                    grid_array[y][x - 1] = BLOCK_VISITED;
                std::string possible_path = path + 'L';
                move_que.push(possible_path);
            }
            move_que.pop();
            x_que.pop();
            y_que.pop();

            // for (size_t y = 0; y < GRID_HEIGHT; y++)
            // {
            //     for (size_t x = 0; x < GRID_WIDTH; x++)
            //         std::cout << std::to_string(grid_array[y][x]) << "\t";
            //     std::cout << "\n";
            // }
            // std::cout << "\n\n\n\n";
        }

        while (move_que.front().length() < (GRID_WIDTH + GRID_HEIGHT - 3))
            move_que.pop();

        if (move_que.front().length() > 0)
        {
            std::cout << "Search Complete: " << move_que.front() << std::endl;
            display_path(window, start_pos, end_pos, move_que.front());
        }
        else
            std::cout << "Search Failed!" << std::endl;
    }

    void display_path(sf::RenderWindow *window, std::array<std::uint8_t, 2> &start_pos, std::array<std::uint8_t, 2> &end_pos, std::string path)
    {
        std::uint8_t y = start_pos[0];
        std::uint8_t x = start_pos[1];
        sf::RectangleShape mapping_marker(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
        sf::Vector2f mapped_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
        mapping_marker.setPosition(mapped_pos);
        mapping_marker.setFillColor(sf::Color::Blue);
        window->draw(mapping_marker);
        window->display(); // Update display every iteration

        sf::CircleShape bot(sf::CircleShape(PIXEL_WIDTH / 2, 30));
        sf::Vector2f bot_pos = sf::Vector2f((entry_point[1]) * PIXEL_WIDTH, (entry_point[0]) * PIXEL_WIDTH);
        bot.setPosition(bot_pos);
        bot.setFillColor(sf::Color::Green);
        window->draw(bot);

        for (std::uint8_t pos = 0; pos < path.length(); pos++)
        {
            if (path[pos] == 'D')
                y++;
            else if (path[pos] == 'R')
                x++;
            else if (path[pos] == 'U')
                y--;
            else if (path[pos] == 'L')
                x--;
            grid_array[y][x] == FINAL_PATH;
            sf::Vector2f mapped_pos = sf::Vector2f(x * PIXEL_WIDTH, y * PIXEL_WIDTH);
            mapping_marker.setPosition(mapped_pos);
            mapping_marker.setFillColor(sf::Color::Blue);
            window->draw(mapping_marker);
            window->display(); // Update display every iteration
        }

        sf::RectangleShape end_point(sf::Vector2f(PIXEL_WIDTH, PIXEL_WIDTH));
        sf::Vector2f ep_pos = sf::Vector2f((exit_point[1]) * PIXEL_WIDTH, (exit_point[0]) * PIXEL_WIDTH);
        end_point.setPosition(ep_pos);
        end_point.setFillColor(sf::Color::Red);
        window->draw(end_point);
        window->display(); // Update display every iteration

        std::string file_name = this->search_type + ".png";
        sf::Texture texture;
        texture.create(window->getSize().x, window->getSize().y);
        texture.update(*window);
        if (texture.copyToImage().saveToFile(file_name))
            std::cout << "Screenshot Saved as " << file_name << std::endl;
    }

public:
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
    void initiate_search(std::string search_type, sf::RenderWindow *window, std::array<std::uint8_t, 2> &start_pos, std::array<std::uint8_t, 2> &end_pos, bool show_search_animation, bool quick_solve)
    {
        if (search_type == BFS_SEARCH)
        {
            this->search_type = BFS_SEARCH;
            bfs_search(window, start_pos, end_pos, show_search_animation, quick_solve);
        }
    }
};

int main(int argc, char *argv[])
{
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
    StartSearch *plan_path = new StartSearch;

    grid->initialize_grid();

    // Breadth-First Search
    sf::RenderWindow *search1 = grid->vizualize_grid(false, false);
    plan_path->initiate_search(BFS_SEARCH, search1, entry_point, exit_point, false, true);

    // Depth-First Search
    sf::RenderWindow *search2 = grid->clear_grid(false, false);
    plan_path->initiate_search(DFS_SEARCH, search2, entry_point, exit_point, false, true);

    while (search1->isOpen())
    {
        sf::Event event;
        while (search1->pollEvent(event))
            if (event.type == sf::Event::Closed) // "close requested" event: we close the window
                search1->close();
    }
    return EXIT_SUCCESS;
}