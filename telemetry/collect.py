#!/usr/bin/python

import math
import serial
import sys
import time

SYNC_CHAR = 0xA7
SEQUENCE_LEN = 4

BATTERY_POTENTIAL_TYPE = 0
SOLAR_CURRENT_TYPE = 1
SOLAR_POTENTIAL_TYPE = 2

BATTERY_POTENTIAL_DECIMAL_PLACES = 3
SOLAR_CURRENT_DECIMAL_PLACES = 3
SOLAR_POTENTIAL_DECIMAL_PLACES = 3

class Logger:
  column_separator = "\t"
  row_separator = "\n"
  def __init__(self, filename, append):
    self.fh = open(filename, 'a' if append else 'w')

  def append(self, *data):
    for i in range(0, len(data)):
      self.fh.write(str(data[i]))
      self.fh.write(self.column_separator if i < len(data) - 1 \
                                          else self.row_separator)
      self.fh.flush()

battery_potential_logger = Logger("battery_potential", True)
solar_current_logger = Logger("solar_current", True)
solar_potential_logger = Logger("solar_potential", True)

def read():
  return ord(stream.read())

def warn(str):
  print >> sys.stderr, str

def combine_big_endian(big_endian_data):
  val = 0
  for byte in big_endian_data:
    val = (val << 8) | byte
  return val

def write_datum(logger, datum):
  logger.append(time.time(), datum)

def processBatteryPotentialData(data):
  # Decode and interpret the big-endian fixed-point value.
  battery_potential = float(combine_big_endian(data)) / \
      math.pow(10, BATTERY_POTENTIAL_DECIMAL_PLACES)
  write_datum(battery_potential_logger, battery_potential)
  print "Battery potential: %f" % battery_potential

def processSolarCurrentData(data):
  solar_current = float(combine_big_endian(data)) / \
      math.pow(10, SOLAR_CURRENT_DECIMAL_PLACES)
  write_datum(solar_current_logger, solar_current)
  print "Solar current: %f" % solar_current

def processSolarPotentialData(data):
  solar_potential = float(combine_big_endian(data)) / \
      math.pow(10, SOLAR_POTENTIAL_DECIMAL_PLACES)
  write_datum(solar_potential_logger, solar_potential)
  print "Solar potential: %f" % solar_potential

def processPayload(data):
  if (len(data) < 1 + SEQUENCE_LEN):
    warn("Undersized datagram")

  # Extract the datagram type.
  type = data[0]

  # Extract the datagram sequence number.
  seq = combine_big_endian(data[1:5])

  # Extract the message payload content.
  payload = data[5:]

  # Process payload content based on type.
  if (type == BATTERY_POTENTIAL_TYPE):
    processBatteryPotentialData(payload)
  elif (type == SOLAR_CURRENT_TYPE):
    processSolarCurrentData(payload)
  elif (type == SOLAR_POTENTIAL_TYPE):
    processSolarPotentialData(payload)
  else:
    warn("Invalid message type %d" % type)

def deserialize():
  while True:
    # Synchronize to the start of the next datagram
    sync = read()
    while (sync != SYNC_CHAR):
      sync = read()

    # Read the datagram length.
    length = read()

    # Read the buffer.
    data = []
    for i in range(0, length):
      data.append(read())

    # Verify the checksum
    checksum = read()
    if checksum != (length + sum(data)) % 256:
      warn("Checksum failed")
      continue

    # Process payload.
    processPayload(data)

stream = serial.Serial('/dev/ttyUSB0', 57600)
deserialize()
