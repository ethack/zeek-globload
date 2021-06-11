# GlobLoad: glob patterns for Zeek's `@load` directive

[![Zeek matrix tests](https://github.com/corelight/zeek-globload/actions/workflows/zeek-matrix.yml/badge.svg)](https://github.com/corelight/zeek-globload/actions/workflows/zeek-matrix.yml)
[![Zeek nightly build](https://github.com/corelight/zeek-globload/actions/workflows/zeek-nightly.yml/badge.svg)](https://github.com/corelight/zeek-globload/actions/workflows/zeek-nightly.yml)

This Zeek package extends the functionality of the `@load` directive
to support loading scripts via glob patterns. Among other things, this
allows you to construct "`.d/`"-style directories in which you
decentralize control over the script contents, for example by allowing all
members of a UNIX group to write to the directory.

## Usage

Use a glob pattern where you'd normally point Zeek at a file or
directory:

```
@load scripts.d/*.zeek
```

You can provide absolute paths, or relative paths that will be
evaluated relative to the script containing the globbed `@load`.

The set of scripts that match the glob pattern gets sorted
alphabetically and loaded in that order, as if provided to `@load`
individually. Glob matches need not be files: they can also be
symlinks, directories, etc.

## Quirks

There's a small side-effect due to the way the package hooks into
`@load`: you'll notice an "Empty Zeekygen script doc dependency"
warning for each use of a glob. That's because Zeek internally builds
up dependencies between scripts, the glob patterns themselves aren't
files, and the package cannot control the code path that triggers the
warnings. Unless you care about this aspect of Zeekygen documentation
generation, you can ignore these.
