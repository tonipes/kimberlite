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
      sources: ["source/amalgamated.cpp"],
      cxxSettings: [
        .headerSearchPath("include"),
        .headerSearchPath("libraries/fmt/include"),
      ]
    ),
  ],
  cxxLanguageStandard: CXXLanguageStandard.cxx14

)
