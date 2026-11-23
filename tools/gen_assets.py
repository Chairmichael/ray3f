#!/usr/bin/env python3

# TODO: Make script differentiate between images, audio, etc and create an enum-indexed array for each

"""Usage: gen_assets.py <asset_dir> <output_header>

Embeds every file in <asset_dir> as a const byte array, e.g.
apple.png -> apple_png / apple_png_len.

The header declares the arrays everywhere, and defines them only in the
file that defines <NAME>_ASSETS_IMPLEMENTATION before including it.
"""
import re
import sys
from pathlib import Path

BYTES_PER_LINE = 12


def c_name(filename: str) -> str:
    name = re.sub(r"\W", "_", filename)  # apple.png -> apple_png
    return "_" + name if name[0].isdigit() else name


def main() -> int:
    if len(sys.argv) != 3:
        print(__doc__)
        return 1

    asset_dir, out_path = Path(sys.argv[1]), Path(sys.argv[2])
    guard = re.sub(r"\W", "_", out_path.name).upper()
    impl = guard.removesuffix("_H") + "_IMPLEMENTATION"
    files = sorted(p for p in asset_dir.iterdir() if p.is_file())

    lines = [f"#ifndef {guard}", f"#define {guard}", ""]
    for f in files:
        name = c_name(f.name)
        lines.append(f"extern const unsigned char {name}[];")
        lines.append(f"extern const unsigned int {name}_len;")
    lines += ["", f"#ifdef {impl}", ""]
    for f in files:
        data = f.read_bytes()
        name = c_name(f.name)
        lines.append(f"const unsigned char {name}[] = {{")
        for i in range(0, len(data), BYTES_PER_LINE):
            chunk = data[i : i + BYTES_PER_LINE]
            lines.append("  " + ", ".join(f"0x{b:02x}" for b in chunk) + ",")
        lines.append("};")
        lines.append(f"const unsigned int {name}_len = {len(data)};")
        lines.append("")
    lines += [f"#endif /* {impl} */", "", f"#endif /* {guard} */"]

    text = "\n".join(lines) + "\n"
    # Don't touch the file if nothing changed, so make doesn't recompile for nothing.
    if not out_path.exists() or out_path.read_text() != text:
        out_path.write_text(text)
    return 0


if __name__ == "__main__":
    sys.exit(main())