# Deki Tween

Documentation: https://dekiengine.github.io/deki-tween/ (components and properties, generated from the code)

Tweening and easing animation system for the Deki Engine with 30+ easing functions.

Part of the [Deki Engine](https://github.com/dekiengine/deki-engine) package ecosystem.

## Namespace

This package's types live in `DekiTween`. Scene files store the qualified
name, so a component is `DekiTween::SomeComponent` there, and code naming one
needs the namespace:

```cpp
using namespace DekiTween;
obj->AddComponent<SomeComponent>();
```

Scenes saved before 0.16.0 used bare names and still load: every component
records what it used to be called, and a save writes the current name.

## Installation

Install via the Package Manager inside the Deki Editor.

## Dependencies

None.

## License

Licensed under the Apache License, Version 2.0. See [LICENSE](LICENSE) for details.
