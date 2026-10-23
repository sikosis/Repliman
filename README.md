<p align="center">
  <img src="resources/icon.png" alt="icon" width="256">
</p>

<h1 align="center">Repliman</h1>

<p align="center">
  <img src="https://img.shields.io/badge/version-v0.1-f5a623" alt="Version v0.1">
  <img src="https://img.shields.io/badge/platform-Haiku-ffcc00" alt="Platform: Haiku">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599c" alt="C++17">
</p>

Repliman is a small legacy Haiku/BeOS utility for managing desktop and Deskbar replicants—those little app fragments that can get stuck and refuse to go away cleanly. It is a native C++ application built against the classic Be API. The interface follows Haiku's configured system colours, including dark appearances, and automatically sizes itself to the loaded replicants.

Select a replicant and choose **Remove** to permanently remove it after a Yes/No confirmation. Choose **Disable** to archive it in `~/config/settings/Repliman/disabled_replicants` and remove it from its shelf. Disabled entries remain available in the list under **Disabled**; select one and choose **Enable** to restore it to its original Desktop or Deskbar location.

This repository is a maintained continuation of the original Repliman source. The untouched upstream snapshot is retained in `Repliman-Original`; active development lives in `src` and `resources`.

## Building

Repliman must be built on Haiku with the platform development tools installed:

```sh
make
```

The resulting `Repliman` application is created in the repository root. Run the repository checks with:

```sh
./scripts/validate.sh
```

On Haiku, validation also performs a clean application build. On other operating systems it validates the repository structure and reports that the platform-specific build was skipped.

## Licence

The imported source retains its original file-level notices and distribution terms. It should not be represented or redistributed as MIT-licensed unless those rights are separately established.
