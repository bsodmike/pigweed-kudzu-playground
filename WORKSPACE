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

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

workspace(name = "kudzu")

http_archive(
    name = "rules_cc",
    integrity = "sha256-NddP6xi6LzsIHT8bMSVJ2NtoURbN+l3xpjvmIgB6aSg=",
    strip_prefix = "rules_cc-1acf5213b6170f1f0133e273cb85ede0e732048f",
    urls = [
        "https://github.com/bazelbuild/rules_cc/archive/1acf5213b6170f1f0133e273cb85ede0e732048f.zip",
    ],
)

# Load Pigweed's own dependencies that we'll need.
#
# TODO: b/274148833 - Don't require users to spell these out.
http_archive(
    name = "platforms",
    sha256 = "5308fc1d8865406a49427ba24a9ab53087f17f5266a7aabbfc28823f3916e1ca",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/platforms/releases/download/0.0.6/platforms-0.0.6.tar.gz",
        "https://github.com/bazelbuild/platforms/releases/download/0.0.6/platforms-0.0.6.tar.gz",
    ],
)

http_archive(
    name = "bazel_skylib",
    sha256 = "aede1b60709ac12b3461ee0bb3fa097b58a86fbfdb88ef7e9f90424a69043167",
    strip_prefix = "bazel-skylib-1.6.1",  # 2024-04-24
    urls = ["https://github.com/bazelbuild/bazel-skylib/archive/refs/tags/1.6.1.tar.gz"],
)

http_archive(
    name = "rules_proto",
    sha256 = "dc3fb206a2cb3441b485eb1e423165b231235a1ea9b031b4433cf7bc1fa460dd",
    strip_prefix = "rules_proto-5.3.0-21.7",
    urls = [
        "https://github.com/bazelbuild/rules_proto/archive/refs/tags/5.3.0-21.7.tar.gz",
    ],
)

http_archive(
    name = "rules_python",
    sha256 = "9acc0944c94adb23fba1c9988b48768b1bacc6583b52a2586895c5b7491e2e31",
    strip_prefix = "rules_python-0.27.0",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.27.0/rules_python-0.27.0.tar.gz",
)

load("@rules_python//python:repositories.bzl", "py_repositories", "python_register_toolchains")

py_repositories()

http_archive(
    name = "com_google_protobuf",
    sha256 = "616bb3536ac1fff3fb1a141450fa28b875e985712170ea7f1bfe5e5fc41e2cd8",
    strip_prefix = "protobuf-24.4",
    url = "https://github.com/protocolbuffers/protobuf/releases/download/v24.4/protobuf-24.4.tar.gz",
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()

http_archive(
    name = "rules_fuzzing",
    sha256 = "d9002dd3cd6437017f08593124fdd1b13b3473c7b929ceb0e60d317cb9346118",
    strip_prefix = "rules_fuzzing-0.3.2",
    urls = ["https://github.com/bazelbuild/rules_fuzzing/archive/v0.3.2.zip"],
)

load("@rules_fuzzing//fuzzing:repositories.bzl", "rules_fuzzing_dependencies")

rules_fuzzing_dependencies()

# Add Pigweed itself, as a submodule.
#
# TODO: b/300695111 - Support depending on Pigweed as a git_repository, even if
# you use pw_toolchain.
local_repository(
    name = "pigweed",
    path = "third_party/pigweed",
)

local_repository(
    name = "pigweed-experimental",
    path = "third_party/pigweed-experimental",
)

git_repository(
    name = "thirtytwo-blit-sdk",
    build_file = "//lib/32blit:32blit.BUILD",
    commit = "7048b93680b72421fae7f93651f1802dcff18566",
    remote = "https://github.com/32blit/32blit-sdk",
)

local_repository(
    name = "pw_toolchain",
    path = "third_party/pigweed/pw_toolchain_bazel",
)

# Get ready to grab CIPD dependencies. For this minimal example, the only
# dependencies will be the toolchains and OpenOCD (used for flashing).
load(
    "@pigweed//pw_env_setup/bazel/cipd_setup:cipd_rules.bzl",
    "cipd_client_repository",
    "cipd_repository",
)

cipd_client_repository()

load("@pigweed//pw_toolchain:register_toolchains.bzl", "register_pigweed_cxx_toolchains")

register_pigweed_cxx_toolchains()

# Get the OpenOCD binary (we'll use it for flashing).
cipd_repository(
    name = "openocd",
    path = "infra/3pp/tools/openocd/${os}-${arch}",
    tag = "version:2@0.11.0-3",
)

# Set up the Python interpreter we'll need.
python_register_toolchains(
    name = "python3",
    python_version = "3.11",
)

load("@python3//:defs.bzl", "interpreter")
load("@rules_python//python:pip.bzl", "pip_parse")

pip_parse(
    name = "python_packages",
    python_interpreter_target = interpreter,
    requirements_darwin = "@pigweed//pw_env_setup/py/pw_env_setup/virtualenv_setup:upstream_requirements_darwin_lock.txt",
    requirements_linux = "@pigweed//pw_env_setup/py/pw_env_setup/virtualenv_setup:upstream_requirements_linux_lock.txt",
    requirements_windows = "@pigweed//pw_env_setup/py/pw_env_setup/virtualenv_setup:upstream_requirements_windows_lock.txt",
)

load("@python_packages//:requirements.bzl", "install_deps")

install_deps()

# Setup Nanopb protoc plugin.
# Required by: Pigweed.
# Used in modules: pw_protobuf.
http_archive(
    name = "com_github_nanopb_nanopb",
    sha256 = "3f78bf63722a810edb6da5ab5f0e76c7db13a961c2aad4ab49296e3095d0d830",
    strip_prefix = "nanopb-0.4.8",
    url = "https://github.com/nanopb/nanopb/archive/refs/tags/0.4.8.tar.gz",
)

load("@com_github_nanopb_nanopb//extra/bazel:nanopb_deps.bzl", "nanopb_deps")

nanopb_deps()

load("@com_github_nanopb_nanopb//extra/bazel:python_deps.bzl", "nanopb_python_deps")

nanopb_python_deps(interpreter)

load("@com_github_nanopb_nanopb//extra/bazel:nanopb_workspace.bzl", "nanopb_workspace")

nanopb_workspace()

http_archive(
    name = "freertos",
    build_file = "@pigweed//third_party/freertos:freertos.BUILD.bazel",
    sha256 = "89af32b7568c504624f712c21fe97f7311c55fccb7ae6163cda7adde1cde7f62",
    strip_prefix = "FreeRTOS-Kernel-10.5.1",
    urls = ["https://github.com/FreeRTOS/FreeRTOS-Kernel/archive/refs/tags/V10.5.1.tar.gz"],
)

http_archive(
    name = "hal_driver",
    build_file = "@pigweed//third_party/stm32cube:stm32_hal_driver.BUILD.bazel",
    sha256 = "c8741e184555abcd153f7bdddc65e4b0103b51470d39ee0056ce2f8296b4e835",
    strip_prefix = "stm32f4xx_hal_driver-1.8.0",
    urls = ["https://github.com/STMicroelectronics/stm32f4xx_hal_driver/archive/refs/tags/v1.8.0.tar.gz"],
)

http_archive(
    name = "cmsis_device",
    build_file = "@pigweed//third_party/stm32cube:cmsis_device.BUILD.bazel",
    sha256 = "6390baf3ea44aff09d0327a3c112c6ca44418806bfdfe1c5c2803941c391fdce",
    strip_prefix = "cmsis_device_f4-2.6.8",
    urls = ["https://github.com/STMicroelectronics/cmsis_device_f4/archive/refs/tags/v2.6.8.tar.gz"],
)

http_archive(
    name = "cmsis_core",
    build_file = "@pigweed//third_party/stm32cube:cmsis_core.BUILD.bazel",
    sha256 = "f711074a546bce04426c35e681446d69bc177435cd8f2f1395a52db64f52d100",
    strip_prefix = "cmsis_core-5.4.0_cm4",
    urls = ["https://github.com/STMicroelectronics/cmsis_core/archive/refs/tags/v5.4.0_cm4.tar.gz"],
)

git_repository(
    name = "pico-sdk",
    # commit = "f469db237d4d7330088600e412be6bdfe8cb2223",
    # remote = "https://pigweed.googlesource.com/third_party/github/raspberrypi/pico-sdk",
    commit = "7a2578294366fec12c605189d67e6b57fffad9b7",
    remote = "https://github.com/armandomontanez/pico-sdk",
)

# TODO: Provide tinyusb as a proper Bazel module.
http_archive(
    name = "tinyusb",
    build_file = "@pico-sdk//src/rp2_common/tinyusb:tinyusb.BUILD",
    sha256 = "ac57109bba00d26ffa33312d5f334990ec9a9a4d82bf890ed8b825b4610d1da2",
    strip_prefix = "tinyusb-86c416d4c0fb38432460b3e11b08b9de76941bf5",
    url = "https://github.com/hathach/tinyusb/archive/86c416d4c0fb38432460b3e11b08b9de76941bf5.zip",
)

# TODO(339893258): These are required because objcopy is hard-coded to the
# toolchain tools set up by the pico-sdk.
http_archive(
    name = "arm_gcc_linux-x86_64",
    build_file = "@pw_toolchain//build_external:gcc_arm_none_eabi.BUILD",
    sha256 = "6cd1bbc1d9ae57312bcd169ae283153a9572bd6a8e4eeae2fedfbc33b115fdbb",
    strip_prefix = "arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi",
    url = "https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz",
)

http_archive(
    name = "arm_gcc_win-x86_64",
    build_file = "@pw_toolchain//build_external:gcc_arm_none_eabi.BUILD",
    sha256 = "51d933f00578aa28016c5e3c84f94403274ea7915539f8e56c13e2196437d18f",
    strip_prefix = "arm-gnu-toolchain-13.2.Rel1-mingw-w64-i686-arm-none-eabi",
    url = "https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-mingw-w64-i686-arm-none-eabi.zip",
)

http_archive(
    name = "arm_gcc_mac-x86_64",
    build_file = "@pw_toolchain//build_external:gcc_arm_none_eabi.BUILD",
    sha256 = "075faa4f3e8eb45e59144858202351a28706f54a6ec17eedd88c9fb9412372cc",
    strip_prefix = "arm-gnu-toolchain-13.2.Rel1-darwin-x86_64-arm-none-eabi",
    url = "https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-darwin-x86_64-arm-none-eabi.tar.xz",
)

http_archive(
    name = "arm_gcc_mac-aarch64",
    build_file = "@pw_toolchain//build_external:gcc_arm_none_eabi.BUILD",
    sha256 = "39c44f8af42695b7b871df42e346c09fee670ea8dfc11f17083e296ea2b0d279",
    strip_prefix = "arm-gnu-toolchain-13.2.Rel1-darwin-arm64-arm-none-eabi",
    url = "https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-darwin-arm64-arm-none-eabi.tar.xz",
)
