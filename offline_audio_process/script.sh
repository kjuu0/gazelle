
./build/bin/paex_record && 
./LPCNet/lpcnet_demo -encode input.pcm compressed.bin && 
./LPCNet/lpcnet_demo -decode compressed.bin output.pcm && 
./build/bin/playback_record
