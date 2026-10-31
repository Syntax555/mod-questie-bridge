"""Run standalone cache and condition regressions without configuring or building AzerothCore."""
import argparse
from pathlib import Path
import shutil
import subprocess
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default="g++")
    args = parser.parse_args()
    compiler = shutil.which(args.compiler)
    if not compiler:
        parser.error(f"C++20 compiler not found: {args.compiler}")
    root = Path(__file__).resolve().parents[1]
    for name in ("state_cache", "conditions"):
        with tempfile.TemporaryDirectory(prefix=f"questie-{name}-") as temporary:
            output = Path(temporary)
            # AC recursively collects .cpp files throughout a module. Compile this
            # standalone test from a temporary copy so it never enters worldserver.
            source = output / f"test_{name}.cpp"
            source.write_bytes((root / f"tests/test_{name}.cpp.in").read_bytes())
            if Path(compiler).name.lower() in {"cl", "cl.exe"}:
                binary = output / f"{name}_tests.exe"
                command = [compiler, "/nologo", "/std:c++20", "/EHsc", "/W4", "/WX", "/permissive-", "/O2",
                           f"/I{root / 'src'}", str(source), f"/Fe:{binary}", f"/Fo:{output / (name + '.obj')}"]
            else:
                binary = output / f"{name}_tests"
                command = [compiler, "-std=c++20", "-Wall", "-Wextra", "-Werror", "-pedantic", "-O2",
                           "-I", str(root / "src"), str(source), "-o", str(binary)]
            subprocess.run(command, cwd=output, check=True)
            subprocess.run([str(binary)], cwd=output, check=True)


if __name__ == "__main__":
    main()
