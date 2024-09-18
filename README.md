# Cryptopals in pure C

This is an attempt to do the whole set of Cryptopals problems.

## Setup

In this project, I use Bazel not for reproducibility, but for caching and simple build files for a project this small. 
Just download Bazelisk into PATH (doesn't have to be /usr/local/bin) and copy-paste some commands.

```bash
wget -O /usr/local/bin/bazel https://github.com/bazelbuild/bazelisk/releases/download/v1.21.0/bazelisk-linux-amd64 && \
    chmod +x /usr/local/bin/bazel
```

After you have that, you can just use it.
If you have ccache set up, make sure to prefix every command with `CC=/usr/bin/gcc`, or just do an export.

```bash
bazel build //...
bazel run @hedron_compile_commands//:refresh_all # from this point on, clangd should work file
bazel test //...  # run all tasks
bazel test //sXcY # run task section X challenge Y
```

## Navigation

In `lib/`, you will find code that can be used to solve challenges similar to those encountered in the problems.
In `task/`, you will find the code that uses the stuff from `lib/` to solve the challenges.
