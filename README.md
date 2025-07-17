# Multi-Monitor Mouse Finder

* **Name Change:** This project, formerly known as "Cursor-track-find," has been renamed to **Multi-Monitor Mouse Finder**. This new name more accurately reflects the app's primary purpose. The name change will be fully implemented with the next release.

A minimal Windows utility to help you **find your mouse cursor quickly and easily—especially in multi-monitor setups**.

> **Note:**  
> Windows has a built-in way to find your mouse (like “Show location of pointer when pressing the CTRL key” in Mouse Settings).  
> If you don’t mind higher system usage and want more visual effects, you can also try [Microsoft PowerToys – Mouse Utilities](https://docs.microsoft.com/powertoys/mouse-utilities) or [PowerToys on GitHub](https://github.com/microsoft/PowerToys).

---

## Why use this?

After revising the code with Claude 4.0, the apps memory usage (according to Windows Task Manager) has been reduced from about 1.2 MB to as little as 0.5 MB—making it even more efficient and lightweight!

Most built-in and third-party solutions either:

- Aren’t effective when using multiple screens
- Or use more system resources than necessary

**Cursor Find and Track** offers an ultra-lightweight, always-on approach:  
Subtle overlay arrows at the screen edges always point to your cursor’s position—across all your monitors—so you can find it instantly and continuously.

![Demo](demo/Cursor_Find_and_Track_demo.gif)

---

## Features

- **Designed for multiple monitors:** Works seamlessly across all your screens
- **Native C code:** No frameworks or dependencies—just pure Win32 API
- **Extremely lightweight:** Minimal memory and CPU usage
- **No extra resources:** Uses Windows defaults, no bundled graphics
- **Background process:** No visible window, just a tray icon (standard Windows icon)
- **Open source:** Code available for review or improvement
- **Installer included:** Simple installation and autostart

---

## Known Limitations

- In some cases, such as when using full-screen applications, the overlay may not remain on top.
- Because this app is new and unsigned, Windows may warn you when installing. You can safely choose ‘More info’ and then ‘Run anyway.’

---

## Download

- [Installer (EXE)](https://github.com/inspiringsource/Cursor-track-find/releases/download/v1.0.0/Cursor.Track.and.find-Setup.exe)
- [Source Code (GitHub)](https://github.com/inspiringsource/Cursor-track-find/archive/refs/tags/v1.0.0.zip)

---

## License

MIT License

---

