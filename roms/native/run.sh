#!/usr/bin/env bash

cd /roms2/native

SELECTED="$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"

FILES=`ls -I "*.sh" -I "*.xml" -I "*.old"`
for f in $FILES
do
  FULLPATH="$(cd "$(dirname "$f")" && pwd)/$(basename "$f")"
  if [ ! "$SELECTED" = "$FULLPATH" ]; then
    ALL="$ALL $FULLPATH"
  fi
done

$SELECTED

