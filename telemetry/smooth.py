#!/usr/bin/python

import sys

INTERVAL = int(sys.argv[1])

values = []

for line in sys.stdin:
  values.append(float(line))
  if len(values) > INTERVAL:
    values = values[1:]
  print sum(values) / len(values)
