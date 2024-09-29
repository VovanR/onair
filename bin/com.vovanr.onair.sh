#!/bin/bash

fn () {
	status=$(lsmod | grep uvcvideo --max-count=1 | awk '{print $3}')
	if [ ${status} == "1" ]; then
		curl -d '{"status":true}' -X POST http://192.168.1.1:3000/api/status -H "Content-Type: application/json" 
	fi
}

fn

exit 0
