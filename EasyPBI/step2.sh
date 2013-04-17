#!/bin/sh

# Second step for packaging up the EasyPBI sources into a distfile

VERSION=2.0

#Package up the distfile
tar -czf easypbi-${VERSION}.tar.gz EasyPBI-source
#Clean up the source directory
rm -r EasyPBI-source

echo "EasyPBI version ${VERSION} has been packaged for distribution"
