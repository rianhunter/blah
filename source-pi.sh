
source source-common.sh

function _pico_get_repo
{
	# adapted from https://raw.githubusercontent.com/raspberrypi/pico-setup/master/pico_setup.sh

	local REPO="$1"
	local BRANCH=$2

	if [[ "$BRANCH" == "" ]] ; then
		BRANCH=master
	fi

	local DEST="$DIR_FIRMWARE/pico-$REPO"

	if [[ -d "$DEST" ]] ; then
		echo "$DEST already exists so skipping"
	else
		local REPO_URL="https://github.com/raspberrypi/pico-${REPO}.git"
		echo "Cloning $REPO_URL"
		git clone --depth 1 -b $BRANCH $REPO_URL "$DEST" || die "git clone failed"
	fi

	# Any submodules
	local owd="$(pwd)"
	cd "$DEST" || die "can't cd to '$DEST'"
	git submodule update --init || die "git submodule update failed"
	cd "$owd"

	# Define PICO_XXX_PATH
	local VARNAME="PICO_${REPO^^}_PATH"
	eval "${VARNAME}"="$DEST"
	echo "${VARNAME}=$DEST"
}

function pico_get_sdk
{
	_pico_get_repo sdk $1
}

function pico_get_extras
{
	_pico_get_repo extras $1
}

function pico_get_examples
{
	_pico_get_repo examples $1
}

function pico_get_playground
{
	_pico_get_repo playground $1
}
