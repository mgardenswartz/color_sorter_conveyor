#include <iostream>
using namespace std;


state = 0;

int main() {
    switch(state) {
        case 0:
            cout << "Hello, World!" << endl;
            state = 1;
            break;
        case 1:
            cout << "Goodbye, World!" << endl;
            state = 0;
            break;
    }
    return 0;
}

// Tasks
// 1. Update motor speed from RC values
// 2. Color sensor
// 3. Time-of-flight sensor
// 4. Mastermind
// 5. Change servos