#!/bin/bash

set -e

docker buildx build \
      -t vinjn/cloudy:latest \
      .
