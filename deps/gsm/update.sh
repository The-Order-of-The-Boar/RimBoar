#!/usr/bin/env bash

# forward errors
# set -e

INPUT_FILE="subtrees.csv"
VERSION_REGEX='((0|[1-9][0-9]*)(\.(0|[1-9][0-9]*)(\.(0|[1-9][0-9]*))?)?)'
VERSION_REMOTE_REGEX='refs\/(tags|heads)\/v((0|[1-9][0-9]*)(\.(0|[1-9][0-9]*)(\.(0|[1-9][0-9]*))?)?)'

function parse_version {

    if ! [[ $1 =~ $VERSION_REGEX ]]; then
        echo "ERROR: $1 is not a valid version"
        exit 1
    fi

    echo "${BASH_REMATCH[1]}"
}

function parse_remote_ref {

    if ! [[ $1 =~ $VERSION_REMOTE_REGEX ]]; then
        return 1
    fi

    echo "${BASH_REMATCH[2]}"
}

function get_version_field_list {

    fields=($(echo "$1" | tr '.' ' '))
    while [[ ${#fields[@]} -lt 3 ]]; do
        fields+=('0')
    done
    echo "${fields[@]}"
}

function check_version_compat {

    v1=($(get_version_field_list "$1"))
    v2=($(get_version_field_list "$2"))

    if ! [[ "${v1[0]}" -eq "${v2[0]}" ]]; then
        return 1
    fi

    if ! [[ "${v1[1]}" -ge "${v2[1]}" ]]; then
        return 1
    fi

    if [[ "${v1[1]}" -eq "${v2[1]}" ]] && [[ "${v1[2]}" -lt "${v2[2]}" ]]; then
        return 1
    fi

    return 0
}

function version_gt {

    v1=($(get_version_field_list "$1"))
    v2=($(get_version_field_list "$2"))

    for i in {0..2}; do
        if [[ ${v1[$i]} -gt ${v2[$i]} ]]; then
            return 0
        fi

        if [[ ${v1[$i]} -lt ${v2[$i]} ]]; then
            return 1
        fi
    done

    return 1
}


# check if the CSV file exists
if ! [[ -f $INPUT_FILE ]]; then
    echo "ERROR: The file $INPUT_FILE does not exist."
    exit 1
fi

# loop through the lines of the CSV file
while IFS=';' read -r path repository mode remote_ref || [ -n "$path" ]; do
    
    # check if the directory in the first column exists
    if ! [[ -d "$path" ]]; then
        echo "$path was not found"
        exit 1
    fi

    echo "checking the $path subtree"

    PULL_REMOTE_REF=0
    
    if [[ "$mode" == "semver" ]]; then

        PULL_REMOTE_REF="__placeholder__"
        PULL_REMOTE_REF_VERSION="__placeholder__"
        VERSION=$( parse_version $remote_ref )

        refs=($(git ls-remote "$repository" | awk '{print $2}'))
        for ref in ${refs[@]}; do
            ref_version=$(parse_remote_ref $ref)
            if [[ $ref_version ]] && check_version_compat $ref_version $VERSION; then
                if version_gt $ref_version $PULL_REMOTE_REF_VERSION || [[ "$PULL_REMOTE_REF_VERSION" == "__placeholder__" ]]; then
                    PULL_REMOTE_REF=$ref
                    PULL_REMOTE_REF_VERSION=$ref_version
                fi
            fi
        done

        if [[ "$PULL_REMOTE_REF" == "__placeholder__" ]]; then
            echo "could not find an compatible version"
            continue
        fi
    
    elif [[ "$mode" == "raw" ]]; then
        PULL_REMOTE_REF=$remote_ref
    else
        echo "invalid mode"
        exit 1
    fi

    echo "trying to update subtree $path with remote ref $PULL_REMOTE_REF"
    git subtree -P $path pull $repository $PULL_REMOTE_REF --squash

done < "$INPUT_FILE"