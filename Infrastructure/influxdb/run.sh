#!/bin/bash

INFLUX_PATH='/root/raindrop/influxdb/'

docker kill influxdb 2> /dev/null

docker run \
    -d \
    --rm \
    --network container:grafana \
    -v $INFLUX_PATH/data:/var/lib/influxdb \
    -v $INFLUX_PATH/influxdb.conf:/etc/influxdb/influxdb.conf:ro \
    --name influxdb \
    influxdb
