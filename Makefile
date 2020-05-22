######################################################################
#Filename: Makefile
#Description:
#Author: Mateo Rodriguez Ripolles (teorodrip@posteo.net)
#Maintainer:
#Created: Fri May 22 19:02:20 2020 (+0200)
######################################################################

.PHONY: all build debug test clean re

CC = gcc

TARGET = pwm_fan

INCLUDES = -I.

LIBRARIES = -lbcm2835

SRC = ./pwm_fan.c

all: build

build:
	@$(CC) -o $(TARGET) $(LIBRARIES) $(INCLUDES) $(SRC)

debug:
	@$(CC) -o $(TARGET) $(LIBRARIES) $(INCLUDES) -D TGT_DBG $(SRC)

clean:
	@rm $(TARGET)

re: clean build
