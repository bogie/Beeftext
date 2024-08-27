# Beeftext

![Beeftext](https://raw.githubusercontent.com/wiki/xmichelo/Beeftext/assets/images/Screenshot.png)

Beeftext is a text snippet management tool for Windows, inspired by [TextExpander].

> [!important]  
> I have continued development for BeefText in a limited capacity to suit mainly my needs. PRs might be accepted, feature requests might be considered.

TextExpander is a great program, packed with features. To keep the company profitable, 
[Smile] had to switch to a subscription-based model in 2016. And for people who only need the
basic features of TextExpander, the monthly price is way too high.

Beeftext aims to be the best possible alternative to TextExpander. Beeftext is an open-source project 
written in C++ using the [Qt] framework.

## Contributors

The project was originally developed and managed by [Xavier Michelon]. I have continued limited work on some missing features.

Contributors for translations are listed in the [dedicated wiki page](https://github.com/xmichelo/Beeftext/wiki/User-provided-translations).

The [beeftext.org] website uses a template provided by [HTML5 UP].

## License

This project is licensed under the [MIT License].

Beeftext uses the [Qt] framework under the [GNU Lesser General Public License v3].

Beeftext uses the [OpenSSL] library under [OpenSSL and SSLeay licenses].

Beeftext uses the [emojilib](https://github.com/muan/emojilib) library under the [MIT License].

## Documentation
### End User

The end user documentation is available as a [wiki](https://github.com/xmichelo/Beeftext/wiki) on the GitHub project page.

### Developer

Developer documentation can be extracted using the [Doxygen] documentation tool.

## Building Beeftext

In order to build Beeftext from sources you will need:

- The [Beeftext] source code. The git repository includes the [XMiLib] library as a submodule.
- [Visual Studio 2022]. The Community Edition is available for free.
- The [Qt] framework version 6.8 with msvc2022 installed. Beeftext is generally built using the latest version of the library. Beeftext uses the 64-bit version compiled for the most recent version of Visual Studio.
- The [Qt Visual Studio Tools]. This add-on can be installed from the `Extensions and Updates` tool built into Visual Studio.
- If you plan to generate the installer for Beeftext, you will also need [NSIS].

Instruction:
1. Install Qt 6.8.x MSVC2022 64bit
2. Install Microsoft Visual Studio 2022 with Windows 10 SDK and MSVC v143 x64/x86 build tools
3. Add the Qt 6.8 binary folder to your system environment PATH (usually it is installed in: C:\Qt\6.8.0\msvc2022_64\bin)
4. Start VS 2022 go to Extensions -> Manage and install Qt Visual Studio Tools for VS 2022
5. Go to Extensions -> Qt VS Tools -> QT Versions and select the installed Qt 6.8 msvc2022_64 version. Sometimes autodetect works, sometimes you need to browse to the Qt Folder (e.g. C:\Qt\6.8.0\msvc2022_64)
6. In the Qt VS tools extension options make sure IntelliSense is activated and under QML Language Server select the correct Qt Version.
7. Sequentially right click the Beeftext and XMiLib projects, go to Qt -> Qt Project Settings and select the correct Qt Installation version.

[TextExpander]: https://textexpander.com
[Smile]: https://smilesoftware.com/
[Qt]: https://www.qt.io/developers/
[Xavier Michelon]: https://michelon.ch
[beeftext.org]: https://beeftext.org
[HTML5 UP]: https://html5up.net/
[MIT License]: https://opensource.org/licenses/MIT
[GNU Lesser General Public License v3]: https://www.gnu.org/licenses/lgpl-3.0.txt
[OpenSSL]: https://www.openssl.org
[OpenSSL and SSLeay licenses]:https://www.openssl.org/source/license.html
[Doxygen]: http://www.stack.nl/~dimitri/doxygen/
[Beeftext]: https://github.com/xmichelo/Beeftext
[Visual Studio 2022]: https://www.visualstudio.com/vs/community/
[Qt Visual Studio Tools]: https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022
[XMiLib]: https://github.com/xmichelo/XMiLib
[NSIS]: http://nsis.sourceforge.net/Main_Page
