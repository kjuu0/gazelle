#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"

// #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE  (16000)
#define FRAMES_PER_BUFFER (0)
#define NUM_SECONDS     (2.4)
#define NUM_CHANNELS    (1)
/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG     (0) /**/
/** Set to 1 if you want to capture the recording to a file. */
#define WRITE_TO_FILE   (1)
#define PLAYBACK (0)

/* Select sample format. */

#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"

typedef struct
{
    int          frameIndex;  /* Index into sample array. */
    int          maxFrameIndex;
    SAMPLE      *recordedSamples; // float recorded samples
}
paTestData;

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    paTestData *data = (paTestData*)userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) outputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo; 
    (void) statusFlags;
    (void) userData;

    // the number of frames to be recorded
    if( framesLeft < framesPerBuffer )
    {
        framesToCalc = framesLeft;
        finished = paComplete;
    }
    else
    {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if( inputBuffer == NULL )
    {
        for( i=0; i<framesToCalc; i++ )
        {
            *wptr++ = SAMPLE_SILENCE;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
        }
    }
    else
    {   // recording
        for( i=0; i<framesToCalc; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
    }
    data->frameIndex += framesToCalc;
    return finished;
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int playCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    paTestData *data = (paTestData*)userData;
    SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    SAMPLE *wptr = (SAMPLE*)outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) inputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if( framesLeft < framesPerBuffer )
    {
        /* final buffer... */
        for( i=0; i<framesLeft; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
        for( ; i<framesPerBuffer; i++ )
        {
            *wptr++ = 0;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = 0;  /* right */
        }
        data->frameIndex += framesLeft;
        finished = paComplete;
    }
    else
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}

/*******************************************************************/
int main(void);
int main(void)
{
    // PaStreamParameters:
    // int device
    // int channelCount
    // unsigned long sampleFormat (buffer provided to the stream callback)
    // double suggestedLatency (the desired latency in seconds)
    // void *hostApiSpecificStreamInfo ?

    PaStreamParameters  inputParameters,
                        outputParameters;
    // PaStream* = void*, this is a function pointer (https://www.javatpoint.com/function-pointer-as-argument-in-c)
    PaStream*           stream;
    // int err, see PaErrorCode in portaudio.h file 
    PaError             err = paNoError;
    paTestData          data;

    int                 i;
    int                 totalFrames;
    int                 numSamples;
    int                 numBytes;
    SAMPLE              max, val; // float
    double              average;

    printf("patest_record.c\n"); fflush(stdout);


    // set the total number of frames to be recorded
    data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
    // the beginning
    data.frameIndex = 0;
    // total number of Samples(float)
    numSamples = totalFrames * NUM_CHANNELS; // if two channels, 2*totalFrames sampls
    // total number of bytes
    numBytes = numSamples * sizeof(SAMPLE);
    // allocate space for recordedSamples(SAMPLE) in paTestData data
    data.recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
    if( data.recordedSamples == NULL )
    {
        printf("Could not allocate record array.\n");
        goto done;
    } // No record space error
    
    // set default voice to be 0
    for( i=0; i<numSamples; i++ ) data.recordedSamples[i] = 0;

    // always do this to initialze Portaudio
    err = Pa_Initialize();
    if( err != paNoError ) goto done; // typical way to check for errors

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        goto done;
    } // No input device error

    inputParameters.channelCount = 2;                    /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    // configure Latency
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,                  /* &outputParameters, */
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              recordCallback,
              &data );
    if( err != paNoError ) goto done;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto done;
    printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

    while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
    {
        Pa_Sleep(1000);
        printf("index = %d\n", data.frameIndex ); fflush(stdout);
    }
    if( err < 0 ) goto done;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto done;

    /* Measure maximum peak amplitude. */
    max = 0;
    average = 0.0;
    for( i=0; i<numSamples; i++ )
    {
        val = data.recordedSamples[i];
        if( val < 0 ) val = -val; /* ABS */
        if( val > max )
        {
            max = val;
        }
        average += val;
    }

    average = average / (double)numSamples;

    printf("sample max amplitude = "PRINTF_S_FORMAT"\n", max );
    printf("sample average = %lf\n", average );

    /* Write recorded data to a file. */
#if WRITE_TO_FILE
    {
        FILE  *fid;
        fid = fopen("input.pcm", "wb");
        if( fid == NULL )
        {
            printf("Could not open file.");
        }
        else
        {
            fwrite( data.recordedSamples, NUM_CHANNELS * sizeof(SAMPLE), totalFrames, fid );
            fclose( fid );
            printf("Wrote data to 'input.pcm'\n");
        }
    }
#endif

#if PLAYBACK
    {
        /* Playback recorded data.  -------------------------------------------- */
        data.frameIndex = 0;

        outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
        if (outputParameters.device == paNoDevice) {
            fprintf(stderr,"Error: No default output device.\n");
            goto done;
        }
        outputParameters.channelCount = 2;                     /* stereo output */
        outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = NULL;

        printf("\n=== Now playing back. ===\n"); fflush(stdout);
        err = Pa_OpenStream(
                &stream,
                NULL, /* no input */
                &outputParameters,
                SAMPLE_RATE,
                FRAMES_PER_BUFFER,
                paClipOff,      /* we won't output out of range samples so don't bother clipping them */
                playCallback,
                &data );
        if( err != paNoError ) goto done;

        if( stream )
        {
            err = Pa_StartStream( stream );
            if( err != paNoError ) goto done;

            printf("Waiting for playback to finish.\n"); fflush(stdout);

            while( ( err = Pa_IsStreamActive( stream ) ) == 1 ) Pa_Sleep(100);
            if( err < 0 ) goto done;

            err = Pa_CloseStream( stream );
            if( err != paNoError ) goto done;
        }
    }
#endif

done:
    printf("Done.\n"); fflush(stdout);
    Pa_Terminate();
    if( data.recordedSamples )       /* Sure it is NULL or valid. */
        free( data.recordedSamples );
    if( err != paNoError )
    {
        fprintf( stderr, "An error occurred while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        err = 1;          /* Always return 0 or 1, but no other return codes. */
    }
    return err;
}
