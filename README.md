# Cryptopals in pure C

This is an attempt to do the whole set of Cryptopals problems.
Partly because it's an enjoyable way to learn about crypto, and partly because it should make me a better programmer.
And since it's both fun and useful, there's no reason not to!

## Setup

You may not be very familiar with Bazel.
You may also not want to get very familiar with Bazel.
Well, that's fine.
Just download Bazelisk into PATH (doesn't have to be /usr/local/bin) and copy-paste some commands.

```bash
wget -O /usr/local/bin/bazel https://github.com/bazelbuild/bazelisk/releases/download/v1.21.0/bazelisk-linux-amd64 && \
    chmod +x /usr/local/bin/bazel
```

After you have that, you can just use it.
If you have ccache set up, make sure to prefix every command with `CC=/usr/bin/gcc`.

```bash
bazel build //...
bazel run @hedron_compile_commands//:refresh_all # from this point on, clangd should work file
bazel test //...  # run all tasks
bazel test //sXcY # run task section X challenge Y
```

## Navigation

So far, the directory structure is relatively simple.
In `lib/`, you will find code that can be used to solve challenges similar to those encountered in the problems.
In `task/`, you will find the code that uses the stuff from `lib/` to solve the challenges.
