#!/bin/bash

kill -9 $(ps aux | grep node | grep start | awk '{print $2}' | tail -1 )