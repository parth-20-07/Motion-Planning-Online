#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>

#define GRID_WIDTH 128
#define GRID_HEIGHT 128
std::uint8_t grid_array[GRID_WIDTH][GRID_HEIGHT]; // Grid to store info

/**
 * @brief Set the up the Graphical Window to display the Matrix
 * Set the Frame Rate
 * Draw the Grid Lines
 * @param window
 */
void setup_grid(sf::RenderWindow *window)
{
    window->setVerticalSyncEnabled(true); // Enabling VSync for FrameRate Control.
    window->setFramerateLimit(30);        // Set 60FPS Max
    window->clear(sf::Color::White);      // Set BG Color

    // Plot the Horizontal Lines
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

/**
 * @brief Get the Position where the Block can be placed
 *
 * @return std::array<uint8_t, 2>  (x,y) position
 */
std::array<uint8_t, 2> get_block_placement_position()
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
uint8_t get_block_type()
{
    return (rand() % 4);
}

/**
 * @brief Set the block on grid window
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
 * @param window
 */
void update_grid_window(uint8_t block_type, std::array<uint8_t, 2> location, sf::RenderWindow *window)
{
    // Calculate Position of four markers to make the design
    sf::Vector2f marker1_pos, marker2_pos, marker3_pos, marker4_pos;
    switch (block_type)
    {
    case 0: // Line
        marker1_pos = sf::Vector2f((location[0]) * 10.f, (location[1]) * 10.f);
        marker2_pos = sf::Vector2f((location[0]) * 10.f, (location[1] + 1) * 10.f);
        marker3_pos = sf::Vector2f((location[0]) * 10.f, (location[1] + 2) * 10.f);
        marker4_pos = sf::Vector2f((location[0]) * 10.f, (location[1] + 3) * 10.f);
        break;

    case 1: // L
        marker1_pos = sf::Vector2f((location[0]) * 10.f, (location[1]) * 10.f);
        marker2_pos = sf::Vector2f((location[0] + 1) * 10.f, (location[1]) * 10.f);
        marker3_pos = sf::Vector2f((location[0] + 1) * 10.f, (location[1] + 1) * 10.f);
        marker4_pos = sf::Vector2f((location[0] + 1) * 10.f, (location[1] + 2) * 10.f);
        break;

    case 2: // S
        marker1_pos = sf::Vector2f((location[0]) * 10.f, (location[1]) * 10.f);
        marker2_pos = sf::Vector2f((location[0]) * 10.f, (location[1] + 1) * 10.f);
        marker3_pos = sf::Vector2f((location[0] + 1) * 10.f, (location[1] + 1) * 10.f);
        marker4_pos = sf::Vector2f((location[0] + 1) * 10.f, (location[1] + 2) * 10.f);
        break;

    case 3: // T
        marker1_pos = sf::Vector2f((location[0] + 1) * 10.f, (location[1]) * 10.f);
        marker2_pos = sf::Vector2f((location[0]) * 10.f, (location[1] + 1) * 10.f);
        marker3_pos = sf::Vector2f((location[0] + 1) * 10.f, (location[1] + 1) * 10.f);
        marker4_pos = sf::Vector2f((location[0] + 1) * 10.f, (location[1] + 2) * 10.f);
        break;

    default:
        break;
    }

    // Plot Markers
    sf::RectangleShape marker1(sf::Vector2f(10, 10));
    marker1.setPosition(marker1_pos);
    marker1.setFillColor(sf::Color::Black);
    window->draw(marker1);

    sf::RectangleShape marker2(sf::Vector2f(10, 10));
    marker2.setPosition(marker2_pos);
    marker2.setFillColor(sf::Color::Black);
    window->draw(marker2);

    sf::RectangleShape marker3(sf::Vector2f(10, 10));
    marker3.setPosition(marker3_pos);
    marker3.setFillColor(sf::Color::Black);
    window->draw(marker3);

    sf::RectangleShape marker4(sf::Vector2f(10, 10));
    marker4.setPosition(marker4_pos);
    marker4.setFillColor(sf::Color::Black);
    window->draw(marker4);
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
    for (size_t i = 0; i < GRID_WIDTH; i++)
        for (size_t j = 0; j < GRID_HEIGHT; j++)
            if (grid_array[i][j] == 1)
                covered_pixels += 1;
    std::cout << "Coverage Pixels: " << covered_pixels << "\n";
    return covered_pixels;
}

/**
 * @brief Sequence to plot the object and update the grid matrix
 * @param window
 */
void plot_object(sf::RenderWindow *window)
{
    std::array<uint8_t, 2> block_position;
    block_position = get_block_placement_position();
    uint8_t block_type = get_block_type();
    update_grid_array(block_type, block_position);
    update_grid_window(block_type, block_position, window);
    window->display(); // Update display every iteration
}

int main(int argc, char *argv[])
{
    // Take User Input for Coverage
    std::cout << "Enter The Area Coverage Percentage: ";
    int coverage_percentage;
    std::cin >> coverage_percentage;

    // Do Basic Calculation on how much coverage is needed
    uint64_t total_pixels = GRID_HEIGHT * GRID_HEIGHT;
    uint64_t target_coverage_pixels = (coverage_percentage * total_pixels) / 100;
    uint16_t coverage_blocks = target_coverage_pixels / 4;

    // Display Basic information for User
    std::cout << "\nGrid Size: " << std::to_string(GRID_HEIGHT) << " X " << std::to_string(GRID_WIDTH) << "\n";
    std::cout << "Total Pixels: " << total_pixels << "\n";
    std::cout << "Coverage Percentage: " << coverage_percentage << "\n";
    std::cout << "Covered Pixels: " << target_coverage_pixels << "\n";

    sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * 10, GRID_HEIGHT * 10), "Obstacle Field"); // Each Pixel is considered 10 px wide for better visuals
    setup_grid(&window);                                                                         // Setup Display

    bool first_run = true;

    while (window.isOpen())
    {
        if (first_run)
        {
            for (size_t i = 0; i < coverage_blocks; i++)
                plot_object(&window);
            first_run = false;
            calculate_coverage();
        }

        if (calculate_coverage() < target_coverage_pixels)
            plot_object(&window);
        else
        {
            std::string file_name = "Coverage " + std::to_string(coverage_percentage) + " percent.png";
            sf::Texture texture;
            texture.create(window.getSize().x, window.getSize().y);
            texture.update(window);
            if (texture.copyToImage().saveToFile(file_name))
                std::cout << "Screenshot Saved as " << file_name << std::endl;
            break;
        }

        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed) // "close requested" event: we close the window
                window.close();
    }
    return EXIT_SUCCESS;
}