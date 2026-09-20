# Deki Tween

Docs: https://dekiengine.github.io/deki-tween/ (components and properties, generated from the code)

Tweening and easing animation system for the Deki Engine with 30+ easing functions.

Part of [Deki Engine](https://github.com/dekiengine/deki-engine).

## Namespace

Types live in `DekiTween`. Scene files store the qualified name, and so does code:

```cpp
using namespace DekiTween;
obj->AddComponent<SomeComponent>();
```

Scenes saved before 0.16.0 used bare names and still load; saving writes the current one.

## Install

Package Manager in the Deki Editor, or `DekiEditor --packages-add deki-tween <project>`.

## Dependencies

None.

## License

Apache 2.0. See [LICENSE](LICENSE).
