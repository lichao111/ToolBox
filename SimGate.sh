#!/bin/bash
nc -ll -p 9988  < fifo1  | nc -ll -p 4713 > fifo1 &
