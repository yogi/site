#!/bin/sh

jekyll build -d ../424

git ci -am "$@"
