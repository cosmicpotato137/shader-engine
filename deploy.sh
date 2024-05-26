#!/bin/bash

# Accept the first argument as the zip name
# this should be a required argument
zipname=$1

if [ -z "$zipname" ]; then
  echo "Error: No argument provided for zipname."
  echo "Usage: deploy.sh [zipname]"
  exit 1
fi

# Make deploy folder if it doesn't exist
mkdir -p deploy

# Remove contents of deploy folder
rm -rf deploy/*

# Copy bin/debug to deploy folder
cp -r bin/* deploy/

# Copy res/ to deploy folder
cp -r res/ deploy/res/

# Clean the data folder
rm -rf deploy/data

# Remove old zip file if it exists
rm -f $zipname.zip

# Check if zip is installed
if ! [ -x "$(command -v zip)" ]; then
  # Install zip with sudo
  echo "zip is not installed. Installing zip..."
  sudo apt-get install zip
fi


# Change to deploy directory
cd deploy

# zip contents of deploy folder
zip -r ../$zipname.zip .