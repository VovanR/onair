#!/bin/bash

SERVER_ADDRESS="http://192.168.1.1:3000"

fn () {
    cameraActive=$(lsmod | grep uvcvideo --max-count=1 | awk '{print $3}' | grep -qi "0" && echo "false" || echo "true")
    microphoneActive=$(pactl list sources | grep -qi "Mute: yes" && echo "false" || echo "true")
    if [ ${cameraActive} == "true" ] || [ ${microphoneActive} == "true" ]; then
        curl -d $(echo "{\"camera\":${cameraActive},\"microphone\":${microphoneActive}}") -X POST ${SERVER_ADDRESS}/api/status -H "Content-Type: application/json"
    fi
}

fn

exit 0
