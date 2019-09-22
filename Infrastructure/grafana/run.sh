#!/bin/bash

GRAFANA_PATH='/root/raindrop/grafana/'

docker kill grafana 2> /dev/null

docker run \
    -d \
    --rm \
    -p 8080:3000 \
    -e "GF_SECURITY_ADMIN_PASSWORD=FILLMEIN" \
    -v $GRAFANA_PATH/var:/var/lib/grafana \
    --name grafana \
    grafana/grafana
