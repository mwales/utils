#!/bin/bash

echo "never" | sudo tee /sys/kernel/mm/transparent_hugepage/defrag
echo "0" | sudo tee /sys/kernel/mm/transparent_hugepage/khugepaged/defrag

