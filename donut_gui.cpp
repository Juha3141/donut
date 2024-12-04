#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <cstring>
#include <unistd.h>

#include <SFML/Graphics.hpp>

#define FOV_ANGLE 100

using namespace std;

typedef struct {
    double x,y,z;
}coord , vect;

coord rotate_by_x(coord c , double A) { return {c.x , c.y*cos(A)-c.z*sin(A) , c.y*sin(A)+c.z*cos(A)}; }
coord rotate_by_y(coord c , double A) { return {c.x*cos(A)-c.z*sin(A) , c.y , c.x*sin(A)+c.z*cos(A)}; }
coord rotate_by_z(coord c , double A) { return {c.x*cos(A)-c.y*sin(A) , c.x*sin(A)+c.y*cos(A) , c.z}; }

double transform_3d(double x_y , double z) {
    return x_y/(z*tan((FOV_ANGLE*(M_PI/180)/2)));
}

double dot_product(vect v1 , vect v2) {
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

const int width = 1024;
const int height = 1024;

const int cycle_ms = 10;
const double scale = 0.05f;

void change_w_a(double &ax , double &ay , double &wx , double &wy);

int main(void) {
    // constants
    double r1 = 2 , r2 = 4;
    double spin_angle_x = 0 , spin_angle_y = 0;
    double ox = 0 , oy = 0 , oz = 10;
    int dot_count = (int)(2*M_PI/scale)*(int)(2*M_PI/scale);
    sf::Color line_color[dot_count*2];
    double wx = 0.0f;
    double ax = 0.0f;
    double wy = 0.0f;
    double ay = 0.0f;
    
    int line_index = 0;
    sf::Font default_font;
    default_font.loadFromFile("font/Consolas.ttf");
    sf::RenderWindow window(sf::VideoMode(width , height) , "flying donut");
    sf::Clock clock;
    clock.restart();
    srand(time(0x00));
    for(int i = 0; i < dot_count; i++) {
        line_color[i] = sf::Color(random()%256 , random()%256 , random()%256);
    }

    sf::VertexArray lines(sf::LineStrip);
    while(window.isOpen()) {
        lines.clear();
        line_index = 0;
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) window.close();
        }
        const double add_a = 0.00001;
        const double add_w = 0.001;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ay -= add_a;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ay += add_a;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ax -= add_a;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ax += add_a;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) wy -= add_w;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) wy += add_w;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) wx -= add_w;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) wx += add_w;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
            change_w_a(ax , ay , wx , wy);
        }
        window.clear(sf::Color::Black);
        sf::Text text1;
        wchar_t str[128];
        swprintf(str , 128 , L"ax:%0.08f°/s²  ay:%0.08f°/s²  wx=%0.08f°/s  wy=%0.08f°/s" , ax , ay , wx , wy);
        text1.setFont(default_font);
        text1.setString(str);
        text1.setCharacterSize(15);
        text1.setPosition(0 , 0);
        text1.setFillColor(sf::Color::White);
        window.draw(text1);

        sf::Text text2;
        swprintf(str , 128 , L"dots:%d" , (int)(2*M_PI/scale)*(int)(2*M_PI/scale));
        text2.setFont(default_font);
        text2.setString(str);
        text2.setCharacterSize(15);
        text2.setPosition(0 , 17);
        text2.setFillColor(sf::Color::White);
        window.draw(text2);
        
        if(clock.getElapsedTime().asMilliseconds() < cycle_ms) {
            continue;
        }
        clock.restart();
        wx += ax;
        wy += ay;

        spin_angle_x += (wx*(M_PI/180)/(1000/cycle_ms));
        spin_angle_y += (wy*(M_PI/180)/(1000/cycle_ms));
        for(double angle1 = 0; angle1 <= 2*M_PI; angle1 += scale) {    
            coord base_circle = {r2+r1*cos(angle1) , 0 , r1*sin(angle1)};
            double z = r1*sin(angle1);
            for(double angle2 = 0; angle2 <= 2*M_PI; angle2 += scale) {
                coord donut; // completely good donut
                double xaxis = spin_angle_x;
                double yaxis = spin_angle_y;

                donut = rotate_by_z(base_circle , angle2);
                
                donut = rotate_by_x(donut , xaxis);
                donut = rotate_by_y(donut , yaxis);
                donut = {donut.x+ox , donut.y+oy , donut.z+oz};

                double scrx = transform_3d(donut.x , donut.z);
                double scry = transform_3d(donut.y , donut.z);
                
                int fx = (int)(scrx*(width/2)+(width/2));
                int fy = (int)(scry*(height/2)+(height/2));
                
                    sf::Vertex v;
                    v.color = line_color[line_index];
                    v.position = sf::Vector2f(fx , fy);
                    lines.append(v);
                    line_index++;
            }
        }
        window.draw(lines);
        window.display();
    }
    return 0;
}

void change_w_a(double &ax , double &ay , double &wx , double &wy) {
    cout<<"====== setting new acceleration & velocity ======\n";
    cout<<"enter new ax value : ";
    string str;
    getline(cin , str);
    if(!str.empty()) {
        ax = stod(str);
    }
    cout<<"enter new ay value : ";
    getline(cin , str);
    if(!str.empty()) {
        ay = stod(str);
    }
    cout<<"enter new wx value : ";
    getline(cin , str);
    if(!str.empty()) {
        wx = stod(str);
    }
    cout<<"enter new wy value : ";
    getline(cin , str);
    if(!str.empty()) {
        wy = stod(str);
    }
    sleep(1);
}
