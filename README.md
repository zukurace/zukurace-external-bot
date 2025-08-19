# ZukuRace-external-bot-SDK
This is IPC external bot controll for ZukuRace.

To enable external control press F5 once you launched a game. It will create all necessay handles for IPC.

<br>

WHAT DOES THIS CODE DO:
- Receives position, rotation, ans speed magnitude of the car from the game (10 times/s).
- Sends input data (throttle, braking, steering) to the game back.
Data transmission is implemented with shared memory and semaphores IPC.

<br>

DEDPENDENCIES:
- Download or clone GLM library https://github.com/g-truc/glm
- Properties -> VC++ Directories -> \path\to\glm;$(IncludePath)

<br>

AT THIS MOMENT:
- It works only on Windows.
- The game sends the player's car data only. Will send nearest cars position soon.
- The bot connection works only with RR01 and Night City track.
- RR01 car is unbalanced: slow speed understeer, high speed oversteer.
All this will be fixed later.

<br>

SUPPORT:
- To support this project, you can buy the ZukuRace game for yourself or as a gift for your friends.
- You can try ZukuRace External Bot in the game Demo version for free.

<br>

Feel free make the bot better and compete with other developers!
