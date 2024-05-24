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

package(default_visibility = ["//visibility:public"])

exports_files(
    glob(["**/*"]),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "32blit",
    srcs = [
        # "32blit/audio/audio.cpp",
        # "32blit/audio/mp3-stream.cpp",
        "32blit/engine/api.cpp",
        "32blit/engine/engine.cpp",
        "32blit/engine/file.cpp",
        "32blit/engine/input.cpp",
        "32blit/engine/multiplayer.cpp",
        "32blit/engine/output.cpp",
        "32blit/engine/particle.cpp",
        "32blit/engine/profiler.cpp",
        "32blit/engine/running_average.cpp",
        "32blit/engine/save.cpp",
        "32blit/engine/timer.cpp",
        "32blit/engine/tweening.cpp",
        "32blit/engine/version.cpp",
        "32blit/graphics/blend.cpp",
        "32blit/graphics/color.cpp",
        "32blit/graphics/filter.cpp",
        "32blit/graphics/font.cpp",
        "32blit/graphics/jpeg.cpp",
        "32blit/graphics/mask.cpp",
        "32blit/graphics/mode7.cpp",
        "32blit/graphics/primitive.cpp",
        "32blit/graphics/sprite.cpp",
        "32blit/graphics/surface.cpp",
        "32blit/graphics/text.cpp",
        "32blit/graphics/tilemap.cpp",
        "32blit/math/geometry.cpp",
        "32blit/math/interpolation.cpp",
        "32blit/types/map.cpp",
        "32blit/types/mat3.cpp",
        "32blit/types/mat4.cpp",
        "32blit/types/vec2.cpp",
        "32blit/types/vec3.cpp",
    ],
    hdrs = [
        "32blit/32blit.hpp",
        "32blit/audio/audio.hpp",
        "32blit/audio/mp3-stream.hpp",
        "32blit/contrib.hpp",
        "32blit/engine/api.hpp",
        "32blit/engine/api_private.hpp",
        "32blit/engine/engine.hpp",
        "32blit/engine/fast_code.hpp",
        "32blit/engine/file.hpp",
        "32blit/engine/input.hpp",
        "32blit/engine/menu.hpp",
        "32blit/engine/multiplayer.hpp",
        "32blit/engine/output.hpp",
        "32blit/engine/particle.hpp",
        "32blit/engine/profiler.hpp",
        "32blit/engine/running_average.hpp",
        "32blit/engine/save.hpp",
        "32blit/engine/timer.hpp",
        "32blit/engine/tweening.hpp",
        "32blit/engine/version.hpp",
        "32blit/graphics/blend.hpp",
        "32blit/graphics/color.hpp",
        "32blit/graphics/font.hpp",
        "32blit/graphics/jpeg.hpp",
        "32blit/graphics/mode7.hpp",
        "32blit/graphics/surface.hpp",
        "32blit/graphics/tilemap.hpp",
        "32blit/math/constants.hpp",
        "32blit/math/geometry.hpp",
        "32blit/math/interpolation.hpp",
        "32blit/types/map.hpp",
        "32blit/types/mat3.hpp",
        "32blit/types/mat4.hpp",
        "32blit/types/point.hpp",
        "32blit/types/rect.hpp",
        "32blit/types/size.hpp",
        "32blit/types/vec2.hpp",
        "32blit/types/vec3.hpp",
    ],
    copts = ["-Wno-shadow"],
    includes = [
        "32blit",
        "3rd-party",
    ],
    defines = [
        "BLIT_BUILD_DATE=UNKNOWN",
        "BLIT_BUILD_VER=VS-DEV",
    ],
    deps = [
        "@kudzu//lib/32blit:version_defs",
    ],
)
