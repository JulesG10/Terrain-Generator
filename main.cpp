#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>

void export_as_file(int **map, int width, int height, int id)
{
    std::fstream file;
    file.open(std::to_string(id) + "_" + std::to_string(width) + "x" + std::to_string(height) + ".txt", std::ios::out);

    if (file.is_open())
    {
        std::string line;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                line += std::to_string(map[i][j]) + " ";
            }
            file << line << "\n";
            line = "";
        }
        file.close();
    }
}

void generate_montain(int ***map, int width, int height, int x, int y)
{
    int base = (*map)[x][y];
    for (int i = base - 1; i >= 0; i--)
    {
        for (int k = x - i / 2; k < x + i / 2; k++)
        {
            for (int j = y - i / 2; j < y + i / 2; j++)
            {
                int tempJ = j;
                int tempK = k;
                if (std::rand() % 1 == 0)
                {
                    tempJ += std::rand()%10 + 1;
                    tempK -= std::rand()%10 + 1;
                }else
                {
                    tempJ -= std::rand()%10 + 1;
                    tempK += std::rand()%10 + 1;
                }

                if (tempJ < height && tempJ > 0 && tempK < width && tempK > 0)
                {
                    (*map)[tempK][tempJ] = std::abs(i - base);
                }
            }
        }
    }
}

void generate_terrain(int width, int height)
{
    std::srand(std::time(nullptr));

    unsigned int MAX_POINTS = 80; //std::rand() % ((width / 4) - 10 + 1) + 10;

    std::cout << "Points: " << MAX_POINTS << std::endl;
    std::cout << "Size: " << width << "x" << height << std::endl;

    int **map = new int *[width];
    for (int i = 0; i < width; i++)
    {
        map[i] = new int[height];
    }
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            map[i][j] = 0;
        }
    }

    for (size_t i = 0; i < MAX_POINTS; i++)
    {
        int x = std::rand() % width;
        int y = std::rand() % height;
        map[x][y] = (std::rand() % (width - MAX_POINTS));

        std::thread action(generate_montain, &map, width, height, x, y);
        action.detach();
    }

    export_as_file(map, width, height, 1);//MAX_POINTS);

    delete[] map;
}

int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    generate_terrain(100, 100);
    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << "Duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl;

    std::cin.get();
    return 0;
}