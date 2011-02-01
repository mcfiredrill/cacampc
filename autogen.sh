#!/bin/bash

aclocal
automake --add-missing --copy
autoreconf --install
