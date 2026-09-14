# Changelog

Notable changes to `deki-tween`. Engine and editor changes are in the
[engine changelog](https://github.com/dekiengine/deki-engine/blob/master/CHANGELOG.md).

A package's `minEngine` names the engine version it needs. Before 1.0 a
breaking change bumps the minor across the editor, the engine and every
package together, so a package with no changes of its own is still released
alongside one that has them.

## 0.15.0

### Added
- Unit tests over all 31 easing curves: endpoints, finiteness across the unit
  interval, In/Out mirroring, InOut symmetry, and monotonicity for the curves
  that are not meant to overshoot.
