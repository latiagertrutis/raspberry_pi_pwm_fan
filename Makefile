######################################################################
#Filename: Makefile
#Description:
#Author: Mateo Rodriguez Ripolles (teorodrip@posteo.net)
#Maintainer:
#Created: Fri May 22 19:02:20 2020 (+0200)
######################################################################

.PHONY: all build debug install test clean re

CC = gcc

TARGET = pwm_fan

INCLUDES = -I.

LIBRARIES = -l bcm2835

SRC = ./pwm_fan.c

INSTALL_DIR = /usr/bin

SERVICE_DIR = /etc/systemd/system

all: build

build:
	@$(CC) -o $(TARGET) $(SRC) $(LIBRARIES) $(INCLUDES)

debug:
	@$(CC) -o $(TARGET) $(SRC) $(LIBRARIES) -D TGT_DBG $(INCLUDES)

clean:
	@rm -f $(TARGET)

install: clean build
	@cp $(TARGET) $(INSTALL_DIR)
	@cp pwm_fan.service $(SERVICE_DIR)

re: clean build
