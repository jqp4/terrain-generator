//
//  main.cpp
//  terraingen
//
//  Created by  Gleb on 20.07.2020.
//  Copyright © 2020  Gleb. All rights reserved.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <cmath>

const int winWidth  = 2100;
const int winHeight = 1400;
const int xLen = 13; // ---
const int yLen = 8;  //  |
const int world_height = 10;
const int octaveAmount = (int)log2(xLen < yLen ? xLen : yLen);






int main(){
    //srand(static_cast<unsigned int>(time(nullptr)));
    sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "Terrain Generator");
    label_Start:
    srand(static_cast<unsigned int>(time(nullptr)));
    unsigned int seed = rand();
    
    //int octaves[octaveAmount][xLen][yLen];
    //int landscape[xLen][yLen];
    int octaves[octaveAmount][yLen][xLen];
    int landscape[yLen][xLen];
    
    for (int onum = 0; onum < octaveAmount; onum++){
        int tfx = 0, step = pow(2, onum);
        for (int y = 0; y < yLen; y += step){
            for (int x = 0; x < xLen; x += step){
                tfx = (int)(rand() % world_height);
                int xm = (x + step) > xLen ? xLen : (x + step);
                int ym = (y + step) > yLen ? yLen : (y + step);
                for (int j = y; j < ym; j++){
                    for (int i = x; i < xm; i++){
                        octaves[onum][j][i] = tfx;
                    }
                }
            }
        }
    }
    
    for (int onum = 0; onum < octaveAmount; onum++){
        for (int y = 0; y < yLen; y++){
            for (int x = 0; x < xLen; x++){
                printf("%2.1d ", octaves[onum][y][x]);
            }
            printf("\n");
        }
        printf("\n");
    }
    
    for (int y = 0; y < yLen; y++){
        for (int x = 0; x < xLen; x++){
            int sum = 0;
            for (int onum = 0; onum < octaveAmount; onum++){
                sum += octaves[onum][y][x];
            }
            landscape[y][x] = sum / octaveAmount;
        }
    }
    
    for (int y = 0; y < yLen; y++){
        for (int x = 0; x < xLen; x++){
            printf("%2.1d ", landscape[y][x]);
        }
        printf("\n");
    }
    
    
    
    
    
    
    
    
    
    window.close();
    
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::R:
                            goto label_Start;
                            break;
                        case sf::Keyboard::S:
                            printf("seed is %u\n", seed);
                            break;
                        case sf::Keyboard::Q:
                            window.close();
                            break;
                        case sf::Keyboard::W:
                            window.close();
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        
        window.clear();
        window.display();
        sf::sleep(sf::milliseconds(10));
    }
    return 0;
}


long fibonacci(unsigned int n){
    if (n < 2) return n;
    else return fibonacci(n - 1) + fibonacci(n - 2);
}

int main_timer_example(){
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    
    start = std::chrono::system_clock::now();
    printf("f(42) = %ld \n", fibonacci(42));
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    return 0;
}
