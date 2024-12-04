# The Flying Donut
Developed from C++ and SFML in one day. (2024.01.24)

This project gave me a valuable experience on dealing with intermediate mathematical concepts like Rotation Matrix or Perspective Transformation. It is always fun to stumble upon a new field of programming! 

![Flying Donut](img/donut.gif)

### Requirements
You need SFML library to both run and compile the program. 

### Compiling 
Simply execute ./build.sh and it will create executable file called "donut_gui." (If it's not obvious yet, this script's for Linux not Windows.)

### Usage
Once you execute the program, you can change the angular acceleration using your **↑←↓→ keys**. Up/Down key decreases/increases the x-axis angular acceleration respectively, and Left/Right key decreases/increases the y-axis angular acceleration respectively. (The donut spins by the same direction with the key you press.) 

You can also change the angular velocity using **wasd keys**. The direction the donut moves is similar to the arrow keys. 

You can directly set of the angular velocity and angular acceleration by pressing **F1 key**! Type the new values of each variables on the console to change them. If you do not wish to change the value, leave it blank. 

Have Fun!
