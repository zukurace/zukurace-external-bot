#pragma once

#include <type_traits>

//
// GAME INTERFACE COMMON STRUCTS
//

struct CarInput {
    float throttle, braking, steering;
};

struct CarData {
    double pos[3];
    double pitch, yaw, roll;
    float speed;
};

enum class MsgCar : uint8_t {
    Invalid,
    Update,
    Possessed,
    Unpossessed
};

enum class MsgBot : uint8_t {
    Invalid,
    Update,
    Start,
    Stop
};

struct GameData {
    CarInput carInput;
    CarData carData;
    MsgCar msgCar;
    MsgBot msgBot;
};