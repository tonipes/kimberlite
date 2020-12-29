// swift-tools-version:5.3

import PackageDescription

let package = Package(
  name: "Kimberlite",
  products: [
    .library(
      name: "Kimberlite",
      type: .static,
      targets: ["Kimberlite"]
    )
  ],
  dependencies: [  
  ],
  targets: [
    .target(
      name: "Kimberlite",
      dependencies: [],
      path: ".",
      sources: [
        "source/amalgamated.cpp",
        "source/platform/platform_noop.cpp",
        "libraries/fmt/src/format.cc",
        "libraries/fmt/src/format.os",
      ],
      cxxSettings: [
        .headerSearchPath("include"),
        .headerSearchPath("libraries/fmt/include"),
      ]
    ),
  ],
  cxxLanguageStandard: CXXLanguageStandard.cxx14

)
