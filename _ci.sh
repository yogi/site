#!/bin/sh

jekyll build -d out

git ci -am "$@"
