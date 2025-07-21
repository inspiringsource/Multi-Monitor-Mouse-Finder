# Multi-Monitor Mouse Finder

> *Formerly known as "Cursor-track-find" — renamed to better reflect what it does.*


## What is this?

Just a little fun side project I made to help find the mouse cursor quickly, especially if you’re using multiple monitors. 

It started as a simple experiment because I couldn’t find any good lightweight tools that worked well across multiple screens without hogging resources. Since it’s my own code, I know exactly what it does under the hood. After some rounds of improvements with AI tools like Claude 4.0 and others, the code got quite a bit leaner and optimized.

### Demo

![Demo](demo/Cursor_Find_and_Track_demo.gif)

### 🛠️ Task Manager

![Task Manager](demo/TaskManager.jpg)

## Why would you use this?

* It’s super lightweight (memory usage dropped from about 1.2MB to under 1MB after optimizations!)
* It works smoothly across multiple monitors
* No flashy animations or big system resource drain—just subtle arrows on screen edges pointing to where your cursor is
* No external dependencies — pure native Windows API in C
* Runs quietly in the background with a simple tray icon
* Open source, so you can see all the code or tweak it yourself

---

## How it compares

Windows has a built-in option to locate your cursor by pressing CTRL, and Microsoft PowerToys offers fancy mouse utilities with more visual flair but also more resource use. I made this because I wanted something minimal, always-on, and reliable for multi-monitor setups without the bloat.

## Known quirks

* Sometimes might hide behind fullscreen apps
* It’s unsigned and new, so Windows might warn you when installing. Just choose "Run anyway" if you trust it.

## Download and source

* [Installer EXE](https://github.com/inspiringsource/Multi-Monitor-Mouse-Finder/releases/download/v1.2.1/Multi-Monitor-Mouse-Finder-Setup1.2.1.exe)
* [Source Code on GitHub](https://github.com/inspiringsource/Multi-Monitor-Mouse-Finder/archive/refs/tags/v1.2.1.zip)

## License

MIT License — do what you want with it!
