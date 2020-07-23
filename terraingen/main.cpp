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
const int xLen = 90; // ---
const int yLen = 65; //  |
const sf::Color bgColor = sf::Color::White;
const sf::Color frameColor = sf::Color(100, 100, 100);


class WindowField{
public:
    int pixel_size;
    int indent;
    int olt; //OutlineThickness
    
    class BackgroundPicture{
    public:
        static const int qn = 1;
        sf::RectangleShape quads[qn];
        
        BackgroundPicture(){}
        
        BackgroundPicture(int pixel_size, int indent){
            quads[0].setSize(sf::Vector2f(pixel_size * xLen, pixel_size * yLen));
            quads[0].setPosition(indent, indent);
            quads[0].setOutlineThickness(indent);
            quads[0].setOutlineColor(frameColor);
            quads[0].setFillColor(bgColor);
            
            /*quads[0].setSize(sf::Vector2f(width, height));
            quads[0].setPosition(0, 0);
            quads[0].setFillColor(frameColor);
            
            quads[1].setSize(sf::Vector2f(pixel_size * xLen, pixel_size * yLen));
            quads[1].setPosition(indent, indent);
            quads[1].setFillColor(bgColor);*/
        }
        
        void draw(sf::RenderWindow *window){
            for (int i = 0; i < qn; i++){
                window->draw(quads[i]);
            }
        }
    } bgpic;
    
    class Landscape{
    public:
        int octaveAmount = log2(xLen < yLen ? xLen : yLen);
        int octaves[10][yLen][xLen]; // xLen, yLen < 2^10 = 1024
        int height, field[yLen][xLen];
        
        void makeField(){
            for (int y = 0; y < yLen; y++){
                for (int x = 0; x < xLen; x++){
                    int sum = 0;
                    for (int onum = 0; onum < octaveAmount; onum++){
                        sum += octaves[onum][y][x];
                    }
                    field[y][x] = sum / octaveAmount;
                }
            }
        }
        
        Landscape(int height){
            this->height = height;
            for (int onum = 0; onum < octaveAmount; onum++){
                int tfx = 0, step = pow(2, onum);
                for (int y = 0; y < yLen; y += step){
                    for (int x = 0; x < xLen; x += step){
                        tfx = (int)(rand() % height);
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
            makeField();
        }
        
        void update(int procent){
            int prh = (int)((float)height * procent / 100);
            for (int onum = 0; onum < octaveAmount; onum++){
                int tfx = 0, step = pow(2, onum);
                for (int y = 0; y < yLen; y += step){
                    for (int x = 0; x < xLen; x += step){
                        tfx = (int)(rand() % prh);
                        int xm = (x + step) > xLen ? xLen : (x + step);
                        int ym = (y + step) > yLen ? yLen : (y + step);
                        for (int j = y; j < ym; j++){
                            for (int i = x; i < xm; i++){
                                octaves[onum][j][i] += tfx;
                                octaves[onum][j][i] %= height;
                            }
                        }
                    }
                }
            }
            makeField();
        }
        
        void showOctaves(){
            for (int onum = 0; onum < octaveAmount; onum++){
                for (int y = 0; y < yLen; y++){
                    for (int x = 0; x < xLen; x++){
                        printf("%2.1d ", octaves[onum][y][x]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
        }
        
        void showField(){
            for (int y = 0; y < yLen; y++){
                for (int x = 0; x < xLen; x++){
                    printf("%2.1d ", field[y][x]);
                }
                printf("\n");
            }
        }
        
    } landscape {40};
    
    class Ghost{
    public:
        sf::RectangleShape bot;
        sf::RectangleShape surface;
        sf::RectangleShape organics;
        sf::ConvexShape custom;
        
        Ghost(){}
        
        Ghost(int ps, int olt){
            bot = sf::RectangleShape(sf::Vector2f(ps - olt*2, ps - olt*2));
            bot.setOutlineColor(sf::Color(82, 82, 82));
            bot.setOutlineThickness(olt);
            
            surface = sf::RectangleShape(sf::Vector2f(ps, ps));
            
            organics = sf::RectangleShape(sf::Vector2f(ps - olt*2, ps - olt*2));
            organics.setFillColor(sf::Color(202, 202, 202));
            organics.setOutlineColor(bgColor);
            organics.setOutlineThickness(olt);
            
            custom.setPointCount(3);
            custom.setPoint(0, sf::Vector2f(ps/2, 0));
            custom.setPoint(1, sf::Vector2f(ps, ps));
            custom.setPoint(2, sf::Vector2f(0, ps));
            custom.setFillColor(sf::Color::Black);
        }
    } ghost;
    
    WindowField(int pixel_size, int outline_thickness){
        this->pixel_size = pixel_size;
        olt = outline_thickness;
        indent = pixel_size/2;
        bgpic = BackgroundPicture(pixel_size, indent);
        ghost = Ghost(pixel_size, olt);
    }
    
    void draw(sf::RenderWindow *window){
        bgpic.draw(window);
        for (int x = 0; x < xLen; x++){
            for (int y = 0; y < yLen; y++){
                //int i = world.field[x][y];
                int wx = indent + olt + pixel_size * x;
                int wy = indent + olt + pixel_size * y;
                /*switch (i){
                    case -1:
                        //pass
                        break;
                    case -2:
                        //organics
                        ghost.organics.setPosition(wx, wy);
                        window->draw(ghost.organics);
                        break;
                    default:
                        //bot
                        if (i < 0){
                            //World::Bot bbb = world.bots[i]; // debug custom bot:
                            ghost.custom.setPosition(wx, wy);
                            //ghost.custom.setFillColor(world.bots[i].color);
                            window->draw(ghost.custom);
                        } else {
                            ghost.bot.setPosition(wx, wy);
                            //ghost.bot.setFillColor(world.bots[i].color);
                            ghost.bot.setFillColor(world.bots[i].getColor());
                            window->draw(ghost.bot);
                        }
                        if (debug == 2){
                            text.setString(std::to_string(world.bots[i].energy));
                            text.setFillColor(sf::Color::Black);
                            text.setPosition(wx - 2, wy - 9);
                            window->draw(text);
                        }
                        break;
                }*/
                
                //case -1:
                //surface
                ghost.surface.setPosition(wx - olt, wy - olt);
                int tbc = (float)landscape.field[y][x] / landscape.height * 255;
                ghost.surface.setFillColor(sf::Color(0, 0, tbc));
                window->draw(ghost.surface);
            }
        }
    }
};


int main(){
    //srand(static_cast<unsigned int>(time(nullptr)));
    sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "Terrain Generator");
    label_Start:
    srand(static_cast<unsigned int>(time(nullptr)));
    unsigned int seed = rand();
    
    WindowField winfld(20, 2);
    
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
                        case sf::Keyboard::U:
                            //winfld.landscape = WindowField::Landscape();
                            winfld.landscape.update(5);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        
        window.clear();
        winfld.landscape.update(5);
        winfld.draw(&window);
        window.display();
        sf::sleep(sf::milliseconds(100));
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
