#!/bin/sh
cd $1 ; git add -A ; git commit -m 'shell commit' ; git status ; git push origin master ; cd -



