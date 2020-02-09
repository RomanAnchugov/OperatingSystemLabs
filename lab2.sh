#!/bin/sh
# Anchugov Roman

counter=0
filesCount=0

printFilesQueContent() {
  result=$(lpq | tail -n +3 | awk '{print $4}')
  for f in $result; do
    filesCount=$((filesCount + 1))
    cat "$f"
    echo ''
  done
}

# SIGINT handler
sigintHandler() {
  echo ''
  # increase interrupt counter
  counter=$((counter + 1))

  # every third interruption
  if [ $counter -eq 3 ]; then
    # prints all non-binary files in current directory
    counter=0
    echo "Files in que $filesCount"
  fi
}

trap 'sigintHandler' 2
printFilesQueContent

while true; do
  echo 'Enter "q" to stop ot Ctrl+C * 3 to show files count in lpq'
  read -r command
  if [ "$command" = "q" ]; then
    exit 0
  fi
done
