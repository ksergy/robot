#!/bin/bash

egrep --exclude-dir .git --exclude-dir temp --exclude *.diff --exclude $(basename "$0") --colour -Rin "(TODO|FIXME|HACK|XXX)"

