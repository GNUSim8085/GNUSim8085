#!/bin/bash

cd "${0%/*}"
XDG_DATA_DIRS=$XDG_DATA_DIRS:../Resources ./myapp
