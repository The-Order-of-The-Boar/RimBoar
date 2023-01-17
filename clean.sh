#!/usr/bin/bash

ls -A | git check-ignore --stdin | xargs rm -rf
