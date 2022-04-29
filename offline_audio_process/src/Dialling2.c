// open pcm file, pass pcm file to glbal buffer, playback pcm file

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "portaudio.h"
#include "lpcnet.h"

/* Select sample format. */
#define SAMPLE_RATE  (16000)
#define FRAMES_PER_BUFFER (0)
#define NUM_SECONDS     (2)
#define NUM_CHANNELS    (1)

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

pthread_mutex_t mutex;
pthread_cond_t cond_new_wanted, cond_new_ready;
SAMPLE* global_buffer;
int global_buffer_ready_to_fetch = 0;


void* upload_data(void* arg){
    size_t ret;
    int i;
    SAMPLE local_buffer[sizeof(SAMPLE) * LPCNET_PACKET_SAMPLES];
    FILE *fin = fopen("input.pcm", "rb");
    
    while(1){

        pthread_mutex_lock(&mutex);
        while(global_buffer_ready_to_fetch){
            pthread_cond_wait(&cond_new_wanted, &mutex);
        }

        ret = fread(local_buffer, sizeof(SAMPLE), LPCNET_PACKET_SAMPLES, fin);
        if(feof(fin) || ret != LPCNET_PACKET_SAMPLES){
            memcpy(global_buffer, local_buffer, sizeof(SAMPLE) * ret);
            
            global_buffer_ready_to_fetch = ret + 2;
            pthread_cond_signal(&cond_new_ready);
            pthread_mutex_unlock(&mutex);
            break;
        }
        

        // copy content of producer local buffer to global buffer
        memcpy(global_buffer, local_buffer, sizeof(SAMPLE) * LPCNET_PACKET_SAMPLES);

        global_buffer_ready_to_fetch = 1;
        pthread_cond_signal(&cond_new_ready);
        pthread_mutex_unlock(&mutex);
    }


    return 0;
}

void* fetch_data(void* arg){
    SAMPLE* local_buffer = malloc(sizeof(SAMPLE) * LPCNET_PACKET_SAMPLES);
    int j = 0;
    FILE* fout = fopen("output.pcm","wb");

    
    while(1){

        pthread_mutex_lock(&mutex);
        while(!global_buffer_ready_to_fetch){
            pthread_cond_wait(&cond_new_ready, &mutex);
        }
        if(global_buffer_ready_to_fetch > 1){
            memcpy(local_buffer, global_buffer, sizeof(SAMPLE) * (global_buffer_ready_to_fetch-2));
            fwrite(local_buffer, sizeof(SAMPLE), (global_buffer_ready_to_fetch-2), fout);
            break;
        }

        memcpy(local_buffer, global_buffer, sizeof(SAMPLE) * LPCNET_PACKET_SAMPLES);
        fwrite(local_buffer, sizeof(SAMPLE), LPCNET_PACKET_SAMPLES, fout);

        global_buffer_ready_to_fetch = 0;
        pthread_cond_signal(&cond_new_wanted);
        pthread_mutex_unlock(&mutex);
    }

    pthread_cond_signal(&cond_new_wanted);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int main(int argc, char* argv[]) {
    pthread_t th[2];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_new_wanted, NULL);
    pthread_cond_init(&cond_new_ready, NULL);
    // srand(time(0));

    global_buffer = malloc(sizeof(SAMPLE) * LPCNET_PACKET_SAMPLES);
    int i;

    for(i = 0; i < 2; i++){
        if(i == 0){
            pthread_create(&th[i], NULL, &upload_data, NULL);
        }
        else{
            pthread_create(&th[i], NULL, &fetch_data, NULL);
        }
    }

    for(i = 0; i < 2; i++){
        pthread_join(th[i], NULL);
    }



    PaStreamParameters  outputParameters;
    PaStream*           stream;
    PaError             err = paNoError;
    paTestData          data;

    // int                 i;
    int                 totalFrames;
    int                 numSamples;
    int                 numBytes;

    // set the total number of frames to be recorded
    data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE;
    data.frameIndex = 0;
    numSamples = totalFrames * NUM_CHANNELS;
    numBytes = numSamples * sizeof(SAMPLE);
    data.recordedSamples = (SAMPLE *) malloc( numBytes );

    FILE *fid;
    fid = fopen("input.pcm", "rb");
    fread(data.recordedSamples, NUM_CHANNELS * sizeof(SAMPLE), totalFrames, fid);
    fclose(fid);

    err = Pa_Initialize();
    if( err != paNoError ) goto done;

    /* Playback recorded data.  -------------------------------------------- */
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto done;
    }
    outputParameters.channelCount = NUM_CHANNELS;
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


    fid = fopen("output.pcm", "rb");
    fread(data.recordedSamples, NUM_CHANNELS * sizeof(SAMPLE), totalFrames, fid);
    fclose(fid);
    data.frameIndex = 0;


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


    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_new_wanted);
    pthread_cond_destroy(&cond_new_ready);

    return err;
}