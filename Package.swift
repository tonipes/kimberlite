// swift-tools-version:5.3

import PackageDescription

let package = Package(
  name: "Kimberlite",
  products: [
    .library(
      name: "Kimberlite",
      type: .static,
      targets: ["Kimberlite", "fmt"]
    )
  ],
  dependencies: [  
  ],
  targets: [
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
        .headerSearchPath("libraries/fmt/include"),
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
