#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#define GRID_WIDTH 128
#define GRID_HEIGHT 128
std::uint8_t grid_array[GRID_WIDTH][GRID_HEIGHT]; // Grid to store info

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
    sf::RenderWindow *initialize_grid(bool show_grid_lines, bool show_setup_animation)
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
        return vizualize_grid(show_grid_lines, show_setup_animation);
    }

    sf::RenderWindow *vizualize_grid(bool show_grid_lines, bool show_setup_animation)
    {
        sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(GRID_WIDTH * 10, GRID_HEIGHT * 10), "Obstacle Field"); // Each Pixel is considered 10 px wide for better visuals
        window->setVerticalSyncEnabled(true);                                                                                // Enabling VSync for FrameRate Control.
        window->setFramerateLimit(30);                                                                                       // Set 60FPS Max
        window->clear(sf::Color::White);                                                                                     // Set BG Color

        if (show_grid_lines)
        { // Plot the Horizontal Lines
            for (size_t i = 0; i <= GRID_WIDTH; i++)
            {
                sf::Vertex line[] =
                    {
                        sf::Vertex(sf::Vector2f(0, i * 10), sf::Color::Black),
                        sf::Vertex(sf::Vector2f(GRID_HEIGHT * 10, i * 10), sf::Color::Black),
                    };
                window->draw(line, 2, sf::Lines);
            }

            // Plot the Vertical Lines
            for (size_t i = 0; i <= GRID_HEIGHT; i++)
            {
                sf::Vertex line[] =
                    {
                        sf::Vertex(sf::Vector2f(i * 10, 0), sf::Color::Black),
                        sf::Vertex(sf::Vector2f(i * 10, GRID_WIDTH * 10), sf::Color::Black),
                    };
                window->draw(line, 2, sf::Lines);
            }
        }
        window->display(); // Update display every iteration

        if (window->isOpen())
        {
            sf::RectangleShape marker(sf::Vector2f(10, 10));
            for (size_t i = 0; i < this->grid_height; i++)
            {
                for (size_t j = 0; j < this->grid_width; j++)
                {
                    if (grid_array[i][j] == 1)
                    {
                        sf::Vector2f marker_pos = sf::Vector2f(i * 10.f, j * 10.f);
                        marker.setPosition(marker_pos);
                        marker.setFillColor(sf::Color::Black);
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
        return window;
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
        std::array<uint8_t, 2> position = {rand() % GRID_WIDTH, rand() % GRID_HEIGHT};
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
        uint8_t x = position[0];
        uint8_t y = position[1];

        switch (block_type)
        {
        case 0: // Line
            grid_array[x][y] = 1;
            grid_array[x][y + 1] = 1;
            grid_array[x][y + 2] = 1;
            grid_array[x][y + 3] = 1;
            break;

        case 1: // Inverted L
            grid_array[x][y] = 1;
            grid_array[x + 1][y] = 1;
            grid_array[x + 1][y + 1] = 1;
            grid_array[x + 1][y + 2] = 1;
            break;

        case 2: // S
            grid_array[x][y] = 1;
            grid_array[x][y + 1] = 1;
            grid_array[x + 1][y + 1] = 1;
            grid_array[x + 1][y + 2] = 1;
            break;

        case 3: // Inverted T
            grid_array[x + 1][y] = 1;
            grid_array[x][y + 1] = 1;
            grid_array[x + 1][y + 1] = 1;
            grid_array[x + 1][y + 2] = 1;
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
                if (grid_array[i][j] == 1)
                    covered_pixels += 1;
        return covered_pixels;
    }
};

int main(int argc, char *argv[])
{
    // Take User Input for Coverage
    std::cout << "Enter The Area Coverage Percentage: ";
    int coverage_percentage;
    std::cin >> coverage_percentage;

    Setup_Grid *new_grid = new Setup_Grid(GRID_WIDTH, GRID_HEIGHT, coverage_percentage);
    sf::RenderWindow *layout = new_grid->initialize_grid(false, true);

    sf::CircleShape bot(sf::CircleShape(5.f, 30));
    sf::Vector2f bot_pos = sf::Vector2f(0.f, 0.f);
    bot.setPosition(bot_pos);
    bot.setFillColor(sf::Color::Green);
    layout->draw(bot);

    sf::RectangleShape end_point(sf::Vector2f(10.f, 10.f));
    sf::Vector2f ep_pos = sf::Vector2f((GRID_WIDTH - 1) * 10, (GRID_HEIGHT - 1) * 10);
    end_point.setPosition(ep_pos);
    end_point.setFillColor(sf::Color::Red);
    layout->draw(end_point);
    layout->display(); // Update display every iteration

    while (layout->isOpen())
    {
        sf::Event event;
        while (layout->pollEvent(event))
            if (event.type == sf::Event::Closed) // "close requested" event: we close the window
                layout->close();
    }
    return EXIT_SUCCESS;
}