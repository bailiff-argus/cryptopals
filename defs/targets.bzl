load("@rules_cc//cc:defs.bzl", "cc_test")

# create_targets_for_tasks_with_no_inputs
def create_targets_for_tasks_with_no_inputs(target_names):
  for target in target_names:
    cc_test(
      name = target,
      srcs = ["task/{}.c".format(target)],
      size = "small",
      deps = [":crypto"],
    )

def create_targets_for_tasks_with_single_inputs(target_names):
  for target in target_names:
    native.filegroup(
      name = "{}_file".format(target),
      srcs = ["files/{}".format(target)],
    )

    cc_test(
      name = target,
      srcs = ["task/{}.c".format(target)],
      size = "small",
      deps = [":crypto"],
      data = [":{}_file".format(target)],
    )
