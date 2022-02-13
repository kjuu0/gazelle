#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (1)
#define NUM_CHANNELS    (2)
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"

typedef struct
{
    // int          frameIndex;  /* Index into sample array. */
    // int          maxFrameIndex;
    SAMPLE      *recordedSamples; // float recorded samples
}
paTestData;

static int EchoCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    paTestData *data = (paTestData*)userData;
    const SAMPLE *mic_rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *mic_wptr = &data->recordedSamples[0];    
    
    SAMPLE *speaker_rptr = &data->recordedSamples[0];
    SAMPLE *speaker_wptr = (SAMPLE*)outputBuffer;
    
    unsigned int i;
    int finished;
    (void) timeInfo;
    (void) statusFlags;
    
    for(i=0; i<framesPerBuffer; i++)
    {
        *speaker_wptr++ = *mic_rptr++;  /* left */
        if( NUM_CHANNELS == 2 ) *speaker_wptr++ = *mic_rptr++;  /* right */
    }

    // for(i=0; i<framesPerBuffer; i++)
    // {
    //     *mic_wptr++ = *mic_rptr++;  /* left */
    //     if( NUM_CHANNELS == 2 ) *mic_wptr++ = *mic_rptr++;  /* right */
    // }

    // for(i=0; i<framesPerBuffer; i++)
    // {
    //     *speaker_wptr++ = *speaker_rptr++;  /* left */
    //     if( NUM_CHANNELS == 2 ) *speaker_wptr++ = *speaker_rptr++;  /* right */
    // }

    return 0;
}


int main(void)
{
    PaStream *stream;
    PaError err = paNoError;
    paTestData data;
    int                 i;
    int                 totalFrames;
    int                 numSamples;
    int                 numBytes;
    PaStreamParameters  inputParameters,
                        outputParameters;

    // // totalFrames = 44100, in this example, we record for 1 second
    // data.maxFrameIndex = 
    totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for one second. */
    // the beginning
    // data.frameIndex = 0;
    // total number of Samples(float)
    numSamples = totalFrames * NUM_CHANNELS; // if two channels, 2*totalFrames sampls
    // total number of bytes
    numBytes = numSamples * sizeof(SAMPLE);
    // allocate space for recordedSamples(SAMPLE) in paTestData data
    data.recordedSamples = (SAMPLE *) malloc( numBytes ); 
    if( data.recordedSamples == NULL )
    {
        printf("Could not allocate record array.\n");
        goto done;
    } // No record space error
    // set default voices to 0
    for( i=0; i<numSamples; i++ ) data.recordedSamples[i] = 0;
    

    err = Pa_Initialize();
    if( err != paNoError ) goto done;
    

    // set up input device
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        goto done;
    } // No input device error
    inputParameters.channelCount = NUM_CHANNELS; /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;


    // set up output device
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto done;
    }
    outputParameters.channelCount = NUM_CHANNELS; /* stereo output */
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    

    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,                  /* &outputParameters, */
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              EchoCallback,
              &data );
    if( err != paNoError ) goto done; 


    err = Pa_StartStream( stream );
    if( err != paNoError ) goto done;
    printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

    while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
    {
        Pa_Sleep(1000);
    }
    if( err < 0 ) goto done;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto done;


done:
    printf("Done.\n"); fflush(stdout);
    Pa_Terminate();
    // if( data.recordedSamples )       /* Sure it is NULL or valid. */
    //     free( data.recordedSamples );
    if( err != paNoError )
    {
        fprintf( stderr, "An error occurred while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        err = 1;          /* Always return 0 or 1, but no other return codes. */
    }
    return err;
}