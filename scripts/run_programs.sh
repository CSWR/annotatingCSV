#!/bin/bash

# Run experiments in the dataset

shopt -s globstar
shopt -s nullglob

DATALOG_BIN=../bin/datalog_bin
NAVIGATIONAL_BIN=../bin/navigational_bin
DATASET_DIR=../datasets/
MAXSIZE=$((240*1024*1024))

for experiment in "$DATASET_DIR"/**/*.{nav,dat}; do
  current_dir=$(dirname "$experiment")
  experiment_name=$(basename "$experiment")

  echo -e "Running experiment: ${experiment##*/}..."

  for dataset in "$current_dir"/*.{csv,log*,ttl}; do
    size=$(stat -c '%s' "$dataset")
    dataset_name=$(basename "$dataset")
    outfile="$dataset_name"
    outfile="${outfile/%.csv/.out}"
    outfile="${outfile/%.ttl/.out}"
    outfile="${outfile/.log/.out}"
    outfile="$current_dir"/"$outfile"

    echo -e "\ton ${dataset}."
    
    # Continue if file too big
    if [ "$size" -ge "$MAXSIZE" ]; then
      continue
    fi

    # Or if experiment already ran
    if [ -f "$outfile" ] && grep -q "$experiment_name" "$outfile"; then
      continue
    fi

    echo -e "# # # ${experiment_name} # # #" >> "$outfile"
    for i in {1..3}; do
      echo -e "* run $i *" >> "$outfile"
      case "${experiment##*.}" in
        nav)
          $NAVIGATIONAL_BIN -b "$dataset" < "$experiment" >> "$outfile"
        ;;
        dat)
          $DATALOG_BIN -b "$dataset" "$experiment" >> "$outfile"
        ;;
      esac
      echo -e "* * *" >> "$outfile"
    done
    echo -e "# # # # # #" >> "$outfile"
  done
done
