#!/bin/bash
exec astyle --style="k&r" --indent=tab=4 --indent-col1-comments --lineend=linux $*

