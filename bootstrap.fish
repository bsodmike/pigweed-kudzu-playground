# Copyright 2024 The Pigweed Authors
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

# This script must be tested on fish 3.6.0

# Get the absolute path of the bootstrap script.
set _PW_BOOTSTRAP_PATH (path resolve (status current-filename))

# Check if this file is being executed or sourced.
set _pw_sourced 0
if string match --quiet '*from sourcing file*' (status)
    set _pw_sourced 1
end

# Downstream projects need to set something other than PW_ROOT here, like
# YOUR_PROJECT_ROOT. Please also set PW_PROJECT_ROOT and PW_ROOT before
# invoking pw_bootstrap or pw_activate.
######### BEGIN PROJECT-SPECIFIC CODE #########
set -x KUDZU_ROOT (path resolve (dirname $_PW_BOOTSTRAP_PATH))
set -x PIGWEED_EXPERIMENTAL_ROOT "$KUDZU_ROOT"
set -x PW_PROJECT_ROOT "$KUDZU_ROOT"
set -x PW_ROOT "$KUDZU_ROOT/third_party/pigweed"

# Set your project's banner and color.
set -x PW_BRANDING_BANNER "$KUDZU_ROOT/banner.txt"

function KUDZU_banner
  cat "$PW_BRANDING_BANNER"
  echo
end

set -x PW_BANNER_FUNC "KUDZU_banner"
########## END PROJECT-SPECIFIC CODE ##########

set _util_fish "$PW_ROOT/pw_env_setup/util.fish"

# Double-check that the Pigweed submodule has been checked out.
if not test -e "$_util_fish"
    echo "Updating git submodules ..."
    # Init without recursion.
    git submodule update --init
end
if not test -f "$PW_PROJECT_ROOT/third_party/pico-sdk/lib/tinyusb/LICENSE"
    echo "Updating git submodules for 'third_party/pico-sdk' ..."
    # Init tinyusb only with no recursion.
    pushd third_party/pico-sdk/
    git submodule update --init lib/tinyusb
    popd
end

source "$_util_fish"

# Check environment properties
pw_deactivate
pw_eval_sourced $_pw_sourced $_PW_BOOTSTRAP_PATH
if not pw_check_root $PW_ROOT
    return
end

set --export _PW_ACTUAL_ENVIRONMENT_ROOT (pw_get_env_root)

set SETUP_SH "$_PW_ACTUAL_ENVIRONMENT_ROOT/activate.fish"

# Run full bootstrap when invoked as bootstrap, or env file is missing/empty.
if test (status basename) = "bootstrap.fish"
    or not test -e $SETUP_SH
    or not test -s $SETUP_SH
    ######### BEGIN PROJECT-SPECIFIC CODE #########
    pw_bootstrap --shell-file "$SETUP_SH" --install-dir "$_PW_ACTUAL_ENVIRONMENT_ROOT" --config-file "$PW_PROJECT_ROOT/pigweed.json"
    ########## END PROJECT-SPECIFIC CODE ##########
    set finalize_mode bootstrap
else
    pw_activate_message
    set finalize_mode activate
end
# NOTE: Sourced scripts in fish cannot be sourced within a fuction if
# variables should be exported to the calling shell. So activate.fish
# must be sourced here instead of within pw_finalize or another
# function.
pw_finalize_pre_check $finalize_mode "$SETUP_SH"
source $SETUP_SH
pw_finalize_post_check $finalize_mode "$SETUP_SH"

if set --query _PW_ENV_SETUP_STATUS; and test "$_PW_ENV_SETUP_STATUS" -eq 0
    # This is where any additional checks about the environment should go.
    ######### BEGIN PROJECT-SPECIFIC CODE #########
    echo -n
    ########## END PROJECT-SPECIFIC CODE ##########
end

set -e _pw_sourced
set -e _PW_BOOTSTRAP_PATH
set -e SETUP_SH

source $PW_ROOT/pw_cli/py/pw_cli/shell_completion/pw.fish

pw_cleanup

git -C "$PW_ROOT" config blame.ignoreRevsFile .git-blame-ignore-revs
