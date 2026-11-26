# Contributing to EFOR

Thanks for helping improve EFOR. This project is small on purpose, so keeping the
process lightweight is part of the design.

## Ground rules

- Assume good intent and follow the [Code of Conduct](./CODE_OF_CONDUCT.md).
- Discuss non-trivial changes in an issue before writing code.
- One issue, one branch, one focused pull request.
- No direct pushes to `main`.
- Keep user-facing text, comments, tests and documentation in English.

## Labels

Use labels consistently so anyone can filter the backlog.

- **Type:** `bug`, `feature`, `refactor`, `tech-debt`, `testing`,
  `documentation`, `question`. Use one primary type per issue.
- **Platform:** `windows`, `linux`, `cross-platform`. Use `cross-platform` for
  shared abstractions that affect both backends.
- **Area:** `ui`, `build`, `ci`, `release`.
- **Community:** `good first issue`, `help wanted`.
- **Lifecycle:** `duplicate`, `invalid`, `wontfix`.

Every issue should have a type label. Add a platform or area label whenever the
work is specific to one of them.

## Reporting issues

Use one of the issue templates. A good issue contains:

- A short title with a prefix: `fix(scope):`, `feat(scope):`, `docs:` or `refactor:`.
- What happens and why it matters.
- Steps to reproduce, when applicable.
- Acceptance criteria written as `- [ ]` checkboxes.
- The correct labels and the v0.3.0 or v0.4.0 milestone.

An issue is closed only when all its acceptance criteria are checked. Tracking
issues stay open until their whole checklist is done.

## Branches

Create a branch from `main` for each issue:

```text
feat/issue-12-english-strings
fix/issue-10-footer-clear
docs/repo-foundation
```

Keep the branch focused. If the work changes direction, close or update the
issue before continuing.

## Commits

Use Conventional Commits:

```text
type(scope): short summary

Optional body explaining why the change exists.
```

Common types:

- `feat` — new user-facing capability
- `fix` — bug fix
- `docs` — documentation only
- `refactor` — internal change with no behavior change
- `test` — tests only
- `chore` — build or repository maintenance

Write the body for the reader who will investigate this in three months. Explain
the reason, not just the diff.

## Pull requests

- Open the PR as a **draft** early, then push commits to it.
- Mark it ready for review only when the checklist passes.
- Link the issue with `Closes #N` when the PR fully resolves it, or `Refs #N`
  when it only relates to the issue.
- Fill in the PR template and keep the diff focused on the linked issue.
- A PR is merged with a merge commit so individual commits remain traceable.

## Testing

Before marking a PR ready:

```sh
cmake --build build --config Debug
ctest --test-dir build -C Debug
```

All 7 tests must pass. Add or update tests when behavior changes in core logic.
User-facing changes also need a manual check in a real terminal.

## Coding style

- C99, compiled with warnings as errors.
- Four spaces, no tabs; final newline required.
- Headers expose declarations only; implementation stays in `src/`.
- Prefer pure helpers for rules that can be tested without Win32.
- Keep platform-specific code under `src/platform/`.
- Do not add external library dependencies without an issue and approval.

## Releases

Only the maintainer creates tags and release notes. A version is published when
its tracking issue is fully checked, including CI, changelog and version bump.
