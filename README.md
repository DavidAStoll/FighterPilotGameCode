# FighterPilotGameCode
Code for Android game project

Contains all the code (C++ and Java) used for the Android game Fighter Pilot: The Pacific War (See https://play.google.com/store/apps/details?id=fighterPilot.ThePacificWar for more info)

The project uses a third party library called FreeType in order to read ttf fonts and the Android NDK to run and build C++ Android, thus the cpp files. Most of the project is in C++ (99%), the Java files mostly server as a wrapper to kickoff the application than call the C++ subroutine that starts the actual game. It only returns to Java from time to time to do some OS operations such as writing to a file, socket or do some android specific calls otherwise it is completly independent of the OS and runs in C++. With that in mind, the C++ code was also structered to be completly oblivious to what OS it is running on and thus can easily be ported to any other OS that subports C++ native code such as Windows OS or iOS.

With that in mind the game was roughly split into three core categories:

native, these are OS dependent files, the headers only specify the abstract method the game needs to know in order run (compile), the actual implementation on each particular OS thus contains the actual OS name, for example, AndroidFileStream is the implemtation for the FileStream class that game needs to run on Android. Thus, on iOS it would link in the iOSFileStream class instead (not actual implemented)

core, these are core classes that contain basic code that is not specific to any type of game and thus can be shared between them. These classes are completly oblivious on what OS they ran on but still can call OS functions specified in the native interface for the engine. For example, it could call any function specified in the FileStream class.

game, these are classes specific to the game and can probably not be reused for another game. Again, this code is completly independent of the OS would not need any rewriting if ported to another platform.
