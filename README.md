# Audacity

> [!NOTE]
> このリポジトリ（ブランチ `claude/release-3.7.9-import-drop`）は Audacity 3.7.9 をベースにした非公式の改造版です。
> 追加した機能の説明は [.github/CUSTOM_BUILD_NOTES.md](.github/CUSTOM_BUILD_NOTES.md) を、
> Windows 版インストーラーは [Releases](https://github.com/cartooh/audacity/releases) を参照してください。
>
> - `.txt` / `.lbl` / `.srt` のドラッグ＆ドロップでラベルトラックを作成
> - `.raw` / `.pcm` のドラッグ＆ドロップで Raw データ取り込みダイアログを表示
> - 新規オーディオトラックの表示モード（波形 / スペクトログラム / 両方）と高さのデフォルトを環境設定で変更・リセット
> - Windows 64bit 版で ASIO に対応


[**Audacity**](https://www.audacityteam.org) is an easy-to-use, multi-track audio editor and recorder for Windows, macOS, GNU/Linux and other operating systems.

- **Recording** from any real or virtual audio device that is available to the host system.
- **Export / Import** a wide range of audio formats, extensible with FFmpeg.
- **High quality** using 32-bit float audio processing.
- **Plugin Support** for multiple audio plugin formats, including VST, LV2, and AU.
- **Macros** for chaining commands and batch processing.
- **Scripting** in Python, Perl, or any other language that supports named pipes.
- **Nyquist** a powerful built-in scripting language that may also be used to create plugins.
- **Editing** multi-track editing with sample accuracy and arbitrary sample rates.
- **Accessibility** for VI users.
- **Analysis and visualization** tools to analyze audio or other signal data.

## Users

For end users, the latest Windows and macOS release version of Audacity is available from the [Audacity website](https://www.audacityteam.org/download/).
Help with using Audacity is available [here](https://audacityteam.org/help/).

## Developers
Build instructions are available [here](https://github.com/audacity/audacity/blob/master/BUILDING.md).

Additional development resources may be found [here](https://audacity.gitbook.io/dev/).

## License

Audacity is open source software licensed GPLv3. Most code files are GPLv2-or-later, with the notable exceptions being /lib-src (which contains third party libraries), as well as VST3-related code. Documentation is licensed CC-by 3.0 unless otherwise noted. Details can be found in the [license file](LICENSE.txt).
