#!/bin/bash

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

cd $DIR

cd ../../../addons
git clone https://github.com/n1ckfg/ofxSpout
git clone https://github.com/n1ckfg/ofxSyphon
git clone https://github.com/n1ckfg/ofxXmlSettings
git clone https://github.com/n1ckfg/ofxCrypto
git clone https://github.com/n1ckfg/ofxLibwebsockets
