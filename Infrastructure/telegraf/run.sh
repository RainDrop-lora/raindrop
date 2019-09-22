#!/bin/bash

TELEGRAF_PATH='/root/raindrop/telegraf/'

docker kill telegraf 2> /dev/null

docker run \
    -d \
    --rm \
    --network container:grafana \
    -v $TELEGRAF_PATH/telegraf.conf:/etc/telegraf/telegraf.conf:ro \
    --name telegraf \
    telegraf
