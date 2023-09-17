
function die
{
    echo "$@"
    exit 1
}

function file_or_die
{
    if [[ ! -f "$1" ]] ; then
        die "Couldn't find file '$1' from '$(pwd)'"
    fi
}

# vairable setup
# https://docs.gitlab.com/ee/ci/variables/predefined_variables.html

function detect_workspace
{
	if [[ ! -d "$WORKSPACE" ]] ; then
		# not jenkins, maybe gitlab ?
		WORKSPACE="$CI_PROJECT_DIR"
	fi

	if [[ ! -d "$WORKSPACE" ]] ; then
		# hmm maybe local build, need to autodetect
		# https://stackoverflow.com/questions/59895/how-do-i-get-the-directory-where-a-bash-script-is-located-from-within-the-script
		WORKSPACE="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
		WORKSPACE="$( realpath "$WORKSPACE" )"
	fi

	if [[ ! -f "$WORKSPACE/source-common.sh" ]] ; then
		die "WORKSPACE detection failed (${WORKSPACE})"
	fi
}

detect_workspace
DIR_FIRMWARE="$WORKSPACE/firmware"
echo "WORKSPACE is '$WORKSPACE',  DIR_FIRMWARE is '$DIR_FIRMWARE'"


function detect_commit
{
	if [[ "$CI_COMMIT_SHA" == "" ]] ; then
		if [[ "$GIT_COMMIT" != "" ]] ; then
			CI_COMMIT_SHA=$GIT_COMMIT
		else
			# try to autodetect
			local owd="$(pwd)"
			cd "$WORKSPACE"
			CI_COMMIT_SHA=$(git rev-parse HEAD)
			cd "$owd"
		fi
	fi

	if [[ "$CI_COMMIT_SHA" == "" ]] ; then
		die "Was not able to figure out the CI_COMMIT_SHA"
	fi

	if [[ "$CI_COMMIT_SHORT_SHA" == "" ]] ; then
		CI_COMMIT_SHORT_SHA=$(echo $CI_COMMIT_SHA | head -c 8)
	fi

	# maybe autodetect CI_COMMIT_TAG
}

detect_commit
echo "CI_COMMIT_SHA is $CI_COMMIT_SHA / CI_COMMIT_SHORT_SHA is $CI_COMMIT_SHORT_SHA / CI_COMMIT_TAG is '$CI_COMMIT_TAG'"


V_MAJOR=""
V_MINOR=""
V_PATCH=""
V_PRE=""
SEMVER=""

function detect_version
{
    # e.g. "v1.0.0"
    if [[ "$CI_COMMIT_TAG" == "v"* ]] ; then
        local clean=${CI_COMMIT_TAG:1}
        V_MAJOR=$(echo $clean | cut -d . -f 1)
        V_MINOR=$(echo $clean | cut -d . -f 2)
        V_PATCH=$(echo $clean | cut -d . -f 3)
        V_PRE=""
    fi

    if [[ "$V_MAJOR" == "" ]] || [[ "$V_MAJOR" == "" ]] || [[ "$V_MAJOR" == "" ]] ; then 
        V_MAJOR=0
        V_MINOR=0
        V_PATCH=0
        V_PRE="c${CI_COMMIT_SHORT_SHA}"
    fi

	SEMVER="${V_MAJOR}.${V_MINOR}.${V_PATCH}"

	if [[ "$V_PRE" != "" ]] ; then
		SEMVER="${SEMVER}-$V_PRE"
	fi
}

detect_version
echo "Detected semver '$SEMVER'"

function get_cpu_cores
{
	local cores=1

	if [[ -e /proc/cpuinfo ]] ; then
		cores=$(cat /proc/cpuinfo | grep processor | wc -l)
		if [[ "$cores" == "" ]] ; then
			cores=1
		fi
	fi

	echo -n $cores
}

function get_compile_threads
{
	local threads=$(( $(get_cpu_cores) * 15))
	threads=$(( $threads / 10 ))
	echo -n $threads
}

function cp_or_die
{
	cp "$1" "$2" || die "cp '$1' -> '$2' failed"
}

function mv_or_die
{
	mv "$1" "$2" || die "mv '$1' -> '$2' failed"
}
