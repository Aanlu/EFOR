## What changed

<!-- Describe the change in a few short bullets. -->

## Why

<!-- Explain the reason for the change and link the issue. -->

## Verification

- [ ] `cmake --build build --config Debug`
- [ ] `ctest --test-dir build -C Debug` — all tests pass
- [ ] Manual check in a real terminal when user-facing behavior changes

## Issue linkage

<!-- Use `Closes #N` only when this PR fully resolves the issue. -->
<!-- Use `Refs #N` when the work is related but the issue stays open. -->

- Closes #N

## Checklist

- [ ] The scope matches one issue
- [ ] Commits follow the Conventional Commits format
- [ ] No unrelated files or generated artifacts are included
- [ ] Documentation is updated when behavior changes
- [ ] The PR was a draft until verification passed
