#!/bin/bash

includeStr="#include <stdint.h>\n\
#include <stdlib.h>\n\
#include <time.h>\n\
#include <string.h>\n\
#include <assert.h>\n\
#include <stdio.h>\n\
#include <ctype.h>\n\
#include <stdarg.h>\n\n"

sed -i "1s|^|${includeStr}|" $@
astyle $@
rm -f "$@.orig"
