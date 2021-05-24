 #include <jni.h>
#include <string>

#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_thread.h"

void playWav(char *fileName) {
    SoLoud::Soloud soloud;
    SoLoud::Wav sample;

    soloud.init();
    sample.load(fileName);

    int handle = soloud.play(sample);

    while (soloud.getActiveVoiceCount() > 0)
    {
        // Still going, sleep for a bit
        SoLoud::Thread::sleep(100);
    }

    soloud.deinit();
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_yourorg_soloud_MainActivity_startSoLoudWithArguments(
        JNIEnv *env,
        jobject /* this */,
        jobjectArray arguments) {
    std::string hello = "Hello from C++";

    //argc
    jsize argument_count = env->GetArrayLength(arguments);

    //Compute byte size need for all arguments in contiguous memory.
    int c_arguments_size = 0;
    for (int i = 0; i < argument_count ; i++) {
        c_arguments_size += strlen(env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arguments, i), 0));
        c_arguments_size++; // for '\0'
    }

    //Stores arguments in contiguous memory.
    char* args_buffer=(char*)calloc(c_arguments_size, sizeof(char));

    //argv to pass into node.
    char* argv[argument_count];

    //To iterate through the expected start position of each argument in args_buffer.
    char* current_args_position=args_buffer;

    //Populate the args_buffer and argv.
    for (int i = 0; i < argument_count ; i++)
    {
        const char* current_argument = env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arguments, i), 0);

        //Copy current argument to its expected position in args_buffer
        strncpy(current_args_position, current_argument, strlen(current_argument));

        //Save current argument start position in argv
        argv[i] = current_args_position;

        //Increment to the next argument's expected position.
        current_args_position += strlen(current_args_position)+1;
    }

    // node::Start(argument_count,argv)

    if(argument_count < 1)
        return env->NewStringUTF(hello.c_str());

    char * fileName = argv[0];
    playWav(fileName);

    return env->NewStringUTF(hello.c_str());
}