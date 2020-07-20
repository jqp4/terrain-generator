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
#include <ctime>

const int width  = 2100;
const int height = 1400;
const int xLen = 90;
const int yLen = 90;

int main(){
    //srand(static_cast<unsigned int>(time(nullptr)));
    sf::RenderWindow window(sf::VideoMode(width, height), "Terrain Generator");
    label_Start:
    srand(static_cast<unsigned int>(time(nullptr)));
    unsigned int seed = rand();
    
    
    
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
