// swift-tools-version:5.3

import PackageDescription

let package = Package(
  name: "Kimberlite",
  products: [
    .library(
      name: "Kimberlite",
      type: .static,
      targets: ["Kimberlite", "fmt"]
    ),
    .executable(name: "kb_geomc", targets: ["kb_geomc"]),
    .executable(name: "kb_bin2c", targets: ["kb_bin2c"]),
    .executable(name: "kb_fontc", targets: ["kb_fontc"]),
    .executable(name: "kb_texturec", targets: ["kb_texturec"]),
  ],
  dependencies: [  
  ],
  targets: [
    .target(
      name: "kb_texturec",
      dependencies: ["fmt"],
      path: "tools/texturec",
      sources: [
        "main.cpp",
      ],
      cxxSettings: [
        .headerSearchPath(".."),
        .headerSearchPath("../../include"),
        .headerSearchPath("../../source"),
        .headerSearchPath("../../libraries"),
      ]
    ),
    .target(
      name: "kb_fontc",
      dependencies: ["fmt"],
      path: "tools/fontc",
      sources: [
        "main.cpp",
      ],
      cxxSettings: [
        .headerSearchPath(".."),
        .headerSearchPath("../../include"),
        .headerSearchPath("../../source"),
        .headerSearchPath("../../libraries"),
      ]
    ),
    .target(
      name: "kb_geomc",
      dependencies: ["fmt"],
      path: "tools/geomc",
      sources: [
        "main.cpp",
      ],
      cxxSettings: [
        .headerSearchPath(".."),
        .headerSearchPath("../../include"),
        .headerSearchPath("../../source"),
        .headerSearchPath("../../libraries"),
      ]
    ),
    .target(
      name: "kb_bin2c",
      dependencies: ["fmt"],
      path: "tools/bin2c",
      sources: [
        "main.cpp",
      ],
      cxxSettings: [
        .headerSearchPath(".."),
        .headerSearchPath("../../include"),
        .headerSearchPath("../../source"),
        .headerSearchPath("../../libraries"),
      ]
    ),
    .target(
      name: "Kimberlite",
      dependencies: ["fmt"],
      path: ".",
      exclude: [
        "LICENSE",
        "README.md",
        "file_header",
        "libraries",
        "build",
        "assets",
        "test",
        "examples",
        "tools",
        "source",
        "meson.build",
        "meson_options.txt",
      ],
      sources: [
        "amalgamated.cpp",
      ],
      cxxSettings: [
        .headerSearchPath("include"),
        .headerSearchPath("libraries"),
      ]
    ),
    
    .target(
      name: "fmt",
      dependencies: [],
      path: "libraries/fmt",
      exclude: [
        "meson.build",
        "LICENSE"
      ],
      sources: [
        "src/os.cc",
        "src/format.cc",
        "src/format.os",
      ],
      cxxSettings: [
        .headerSearchPath("include"),
      ]
    ),
  ],
  cxxLanguageStandard: CXXLanguageStandard.cxx14

)
