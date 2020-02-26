#!/bin/bash

set -e
set -x

echo -n | openssl s_client -connect https://scan.coverity.com:443 | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' | sudo tee -a /etc/ssl/certs/ca-

wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ bionic main"
sudo apt-get -y update

sudo apt-get -y install cmake gcovr
